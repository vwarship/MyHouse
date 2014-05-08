#include "MainWindow.h"
#include <QtGui>
#include <QLabel>
#include <QProgressBar>
#include <QMessageBox>
#include "CategoriesView.h"
#include "ThumbnailsView.h"
#include "SlideWidget.h"
#include "Image.h"
#include "ImageUtil.h"
#include "Sqlite.h"
#include "AddImagesThread.h"
#include "AddTermDialog.h"
#include "EditTermDialog.h"
#include "TableFavoritesUtil.h"
#include "ImagePropertyWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _sqlite(new Sqlite),
      _addImagesProgressBar(new QProgressBar(this)), _statusBarText(new QLabel(this))
{
    setWindowTitle(tr("\346\210\221\347\232\204\345\256\266"));
    createCentralWidget();

    createActions();
    createMenus();

    createToolbar();
    createStatusBar();

    createDockWindowWithImageProperty();

    slideWidget = new SlideWidget(this);
    connect(slideWidget, SIGNAL(firstSlide()), this, SLOT(selectedFirstImage()));
    connect(slideWidget, SIGNAL(lastSlide()), this, SLOT(selectedLastImage()));
    connect(slideWidget, SIGNAL(prevSlide()), this, SLOT(selectedPrevImage()));
    connect(slideWidget, SIGNAL(nextSlide()), this, SLOT(selectedNextImage()));
    connect(slideWidget, SIGNAL(copyImage()), this, SLOT(copyImage()));

    connect(thumbnailsView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(playSlide(QModelIndex)));
    connect(thumbnailsView, SIGNAL(itemSelectionChanged()), this, SLOT(currentImageChanged()));
    connect(thumbnailsView, SIGNAL(itemSelectionChanged()), this, SLOT(updateActionState()));
    connect(thumbnailsView, SIGNAL(itemSelectionChanged()), this, SLOT(updateImageProperyWidget()));
    connect(categoriesView, SIGNAL(categoryChanged(int, QString)), thumbnailsView, SLOT(showImagesByTermId(int, QString)));
    connect(categoriesView, SIGNAL(categoryChanged(int, QString)), this, SLOT(updateActionState(int, QString)));

    thumbnailsView->setSqlite(_sqlite);
    connect(thumbnailsView, SIGNAL(customContextMenuRequested(const QPoint)),
            this, SLOT(showContextMenuWithImage(const QPoint)));

    updateActionState();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createCentralWidget()
{
    QSplitter *splitter = new QSplitter(this);
    setCentralWidget(splitter);

    splitter->addWidget(createLeftWidget());

    thumbnailsView = new ThumbnailsView(this);
    splitter->addWidget(thumbnailsView);

    splitter->setStretchFactor(1, 1);
}

QWidget* MainWindow::createLeftWidget()
{
    categoriesView = new CategoriesView(this);

    QWidget *leftWidget = new QWidget(this);

    QLayout *layout = new QVBoxLayout(leftWidget);
    layout->addWidget(categoriesView);
    //layout->addWidget(createAdvertWidget());

    return leftWidget;
}

QWidget* MainWindow::createAdvertWidget()
{
    QLabel *advertWidget = new QLabel();
    advertWidget->setText(tr("<font face='黑体' size=5><p></p>"
                             "<p></p>"
                             "<p></p>"
                             "<p></p></font>"));

    return advertWidget;
}

void MainWindow::createActions()
{
    addPhotoAlbumAction = new QAction(tr("新建相册..."), this);
    addPhotoAlbumAction->setIcon(createIcon("photo_album_add"));
    connect(addPhotoAlbumAction, SIGNAL(triggered()), this, SLOT(addPhotoAlbum()));

    deletePhotoAlbumAction = new QAction(tr("删除相册..."), this);
    deletePhotoAlbumAction->setIcon(createIcon("photo_album_delete"));
    connect(deletePhotoAlbumAction, SIGNAL(triggered()), this, SLOT(deletePhotoAlbum()));

    editTermAction = new QAction(tr("编辑..."), this);
    editTermAction->setIcon(createIcon("tag_edit"));
    connect(editTermAction, SIGNAL(triggered()), this, SLOT(editTerm()));

    addTagAction = new QAction(tr("新建标签(&T)..."), this);
    addTagAction->setIcon(createIcon("tag_add"));
    addTagAction->setShortcut(Qt::CTRL | Qt::Key_T);
    connect(addTagAction, SIGNAL(triggered()), this, SLOT(addTag()));

    deleteTagAction = new QAction(tr("删除标签..."), this);
    deleteTagAction->setIcon(createIcon("tag_delete"));
    connect(deleteTagAction, SIGNAL(triggered()), this, SLOT(deleteTag()));

    addImagesAction = new QAction(tr("增加图像(&O)..."), this);
    addImagesAction->setIcon(createIcon("image_add"));
    addImagesAction->setShortcut(QKeySequence::Open);
    connect(addImagesAction, SIGNAL(triggered()), this, SLOT(addImages()));

    deleteImagesAction = new QAction(tr("删除图像..."), this);
    deleteImagesAction->setIcon(createIcon("image_delete"));
    connect(deleteImagesAction, SIGNAL(triggered()), this, SLOT(deleteImages()));

    addFavoritesAction = new QAction(tr("收藏"), this);
    addFavoritesAction->setIcon(createIcon("favorites_add"));
    connect(addFavoritesAction, SIGNAL(triggered()), this, SLOT(addFavorites()));

    deleteFavoritesAction = new QAction(tr("删除收藏"), this);
    deleteFavoritesAction->setIcon(createIcon("favorites_delete"));
    connect(deleteFavoritesAction, SIGNAL(triggered()), this, SLOT(deleteFavorites()));

    copyImageAction = new QAction(tr("复制图像"), this);
    copyImageAction->setIcon(createIcon("image_copy"));
    copyImageAction->setShortcut(QKeySequence::Copy);
    connect(copyImageAction, SIGNAL(triggered()), this, SLOT(copyImage()));

    pasteImageAction = new QAction(tr("粘贴图像"), this);
    pasteImageAction->setIcon(createIcon("image_paste"));
    pasteImageAction->setShortcut(QKeySequence::Paste);
    connect(pasteImageAction, SIGNAL(triggered()), this, SLOT(pasteImage()));

    playSlideAction = new QAction(tr("全屏显示"), this);
    playSlideAction->setIcon(createIcon("slideshow"));
    playSlideAction->setShortcut(Qt::Key_F11);
    connect(playSlideAction, SIGNAL(triggered()), this, SLOT(playSlide()));

    aboutAction = new QAction(tr("关于(&A)"), this);
    aboutAction->setIcon(createIcon("help"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    exitAction = new QAction(tr("关闭"), this);
    exitAction->setIcon(createIcon("exit"));
    exitAction->setShortcut(Qt::CTRL | Qt::Key_X);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
}

QIcon MainWindow::createIcon(const QString &filename)
{
    QIcon icon;
    icon.addFile(QString(":/Images/images/16/")+ filename + ".png", QSize(16, 16));
    icon.addFile(QString(":/Images/images/32/")+ filename + ".png", QSize(32, 32));

    return icon;
}

void MainWindow::createMenus()
{
    menuBar()->setFont(QFont("", 9));

    createFileMenu();
    createEditMenu();
    createViewMenu();
    createWindowMenu();
    createHelpMenu();
}

void MainWindow::createFileMenu()
{
    fileMenu = new QMenu(tr("文件(&F)"), this);
    fileMenu->setFont(QFont("", 9));
    fileMenu->addAction(addPhotoAlbumAction);
    fileMenu->addAction(deletePhotoAlbumAction);
    fileMenu->addAction(editTermAction);
    fileMenu->addAction(addTagAction);
    fileMenu->addAction(deleteTagAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    menuBar()->addMenu(fileMenu);
}

void MainWindow::createEditMenu()
{
    editMenu = new QMenu(tr("编辑(&E)"), this);
    editMenu->setFont(QFont("", 9));
    editMenu->addAction(addImagesAction);
    editMenu->addAction(deleteImagesAction);
    editMenu->addSeparator();
    editMenu->addAction(addFavoritesAction);
    editMenu->addAction(deleteFavoritesAction);
    editMenu->addSeparator();
    editMenu->addAction(copyImageAction);
    editMenu->addAction(pasteImageAction);

    menuBar()->addMenu(editMenu);
}

void MainWindow::createViewMenu()
{
    viewMenu = new QMenu(tr("查看(&V)"), this);
    viewMenu->setFont(QFont("", 9));
    viewMenu->addAction(playSlideAction);
    menuBar()->addMenu(viewMenu);
}

void MainWindow::createWindowMenu()
{
    windowMenu = new QMenu(tr("窗口(&W)"), this);
    windowMenu->setFont(QFont("", 9));
    menuBar()->addMenu(windowMenu);
}

void MainWindow::createHelpMenu()
{
    helpMenu = new QMenu(tr("帮助(&H)"), this);
    helpMenu->setFont(QFont("", 9));
    helpMenu->addAction(aboutAction);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::createToolbar()
{
    toolBar = addToolBar(tr("My House ToolBar"));
    toolBar->setMovable(false);
    toolBar->setContextMenuPolicy(Qt::CustomContextMenu);

    toolBar->addAction(addPhotoAlbumAction);
    toolBar->addAction(deletePhotoAlbumAction);
    toolBar->addAction(editTermAction);
    toolBar->addAction(addTagAction);
    toolBar->addAction(deleteTagAction);
    toolBar->addSeparator();
    toolBar->addAction(addImagesAction);
    toolBar->addAction(deleteImagesAction);
    toolBar->addSeparator();
    toolBar->addAction(addFavoritesAction);
    toolBar->addAction(deleteFavoritesAction);
    toolBar->addSeparator();
    toolBar->addAction(copyImageAction);
    toolBar->addAction(pasteImageAction);
    toolBar->addAction(playSlideAction);
    toolBar->addSeparator();
    toolBar->addAction(aboutAction);
    toolBar->addAction(exitAction);
}

void MainWindow::updateActionState()
{
    updateActionState(categoriesView->currentTermId(), categoriesView->currentTaxonomy());
}

void MainWindow::updateActionState(int termId, QString taxonomy)
{
    addPhotoAlbumAction->setEnabled(termId == 0 && taxonomy == "photo_album");
    deletePhotoAlbumAction->setEnabled(termId != 0 && taxonomy == "photo_album");
    editTermAction->setEnabled(termId != 0);
    addTagAction->setEnabled(taxonomy == "tag");
    deleteTagAction->setEnabled(termId != 0 && taxonomy == "tag");
    addImagesAction->setEnabled(termId != 0 && taxonomy == "photo_album");

    bool selectedImage = thumbnailsView->selectedItems().size();
    deleteImagesAction->setEnabled(selectedImage && (taxonomy=="tag"||taxonomy=="photo_album"));
    playSlideAction->setEnabled(selectedImage);
    copyImageAction->setEnabled(selectedImage);

    bool favoritesed = TableFavoritesUtil::isExist(thumbnailsView->currentImageId());
    addFavoritesAction->setEnabled(selectedImage & !favoritesed);
    deleteFavoritesAction->setEnabled(selectedImage & favoritesed);
}

void MainWindow::createStatusBar()
{
    _addImagesProgressBar->setMinimumWidth(275);
    _addImagesProgressBar->setValue(0);
    statusBar()->addWidget(_addImagesProgressBar);
    statusBar()->addWidget(_statusBarText, 1);
}

void MainWindow::createDockWindowWithImageProperty()
{
    QDockWidget *dock = new QDockWidget(tr("\345\233\276\345\203\217\345\261\236\346\200\247"), this);
    dock->setAllowedAreas(Qt::RightDockWidgetArea);
    imagePropertyWidget = new ImagePropertyWidget(dock);
    dock->setWidget(imagePropertyWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    windowMenu->addAction(dock->toggleViewAction());
}

void MainWindow::addImages()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter("All Files (*.jpg *.jpeg *.jpe *.jfif *.tif *.tiff *.png *.gif);;JPEG (*.jpg *.jpeg *.jpe *.jfif);;TIFF (*.tif *.tiff);;PNG (*.png);;GIF (*.gif)");
    if (dialog.exec())
    {
        QStringList selectedFiles = dialog.selectedFiles();
        int termId = categoriesView->currentTermId();

        AddImagesThread *addImagesThread = new AddImagesThread(_sqlite, selectedFiles, termId, this);
        connect(addImagesThread, SIGNAL(addImageing(QString)), this, SLOT(on_addImageing(QString)));
        connect(addImagesThread, SIGNAL(addImaged(int,int)), this, SLOT(on_addImaged(int,int)));

        _addImagesProgressBar->setRange(0, selectedFiles.size());

        addImagesThread->start();
    }
}

void MainWindow::on_addImageing(QString filename)
{
    _statusBarText->setText(filename);
}

void MainWindow::on_addImaged(int termId, int imageId)
{
    _addImagesProgressBar->setValue(_addImagesProgressBar->value()+1);

    if (termId == categoriesView->currentTermId())
    {
        Image *image = _sqlite->queryImage(imageId);
        if (image)
        {
            thumbnailsView->addListWidgetItem(image);
            delete image;
        }
    }
}

void MainWindow::addPhotoAlbum()
{
    int termId = categoriesView->currentTermId();
    AddTermDialog addTermDialog(_sqlite, termId, "photo_album", this);
    if (QDialog::Accepted == addTermDialog.exec())
    {
        Term term = _sqlite->queryTerm(addTermDialog.termName());
        categoriesView->addNewItemInCurrentItem(term, "photo_album");
    }
}

void MainWindow::deletePhotoAlbum()
{
    if (QMessageBox::Ok == QMessageBox::question(this, tr("删除相册"), tr("您确认要删除相册?"),
                              QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Ok))
    {
        _sqlite->deletePhotoAlbum(categoriesView->currentTermId());
        categoriesView->deleteCurrentItem();
    }
}

void MainWindow::editTerm()
{
    int termId = categoriesView->currentTermId();
    QString termName = categoriesView->currentTermName();
    EditTermDialog editTermDialog(termId, termName, this);
    if (QDialog::Accepted == editTermDialog.exec())
    {
        categoriesView->updateCurrentItem(editTermDialog.termName());
    }
}

void MainWindow::addTag()
{
    int termId = categoriesView->currentTermId();
    AddTermDialog addTermDialog(_sqlite, termId, "tag", this);
    if (QDialog::Accepted == addTermDialog.exec())
    {
        Term term = _sqlite->queryTerm(addTermDialog.termName());
        categoriesView->addNewItemInCurrentItem(term, "tag");
    }
}

void MainWindow::currentImageChanged()
{
    if (!slideWidget->isHidden() &&
        thumbnailsView->selectedItems().count() > 0)
    {
        Image *image = _sqlite->queryImage(thumbnailsView->currentImageId());
        if (image)
        {
            QPixmap pixmap;
            pixmap.loadFromData(image->image());

            slideWidget->setCurrentSlide(pixmap.toImage());

            delete image;
        }
    }
}

void MainWindow::playSlide(QModelIndex /*modelIndex*/)
{
    slideWidget->setWindowFlags(Qt::Dialog);
    slideWidget->showFullScreen();

    currentImageChanged();
}

void MainWindow::selectedFirstImage()
{
    int firstIndex = 0;
    thumbnailsView->setCurrentRow(firstIndex);
}

void MainWindow::selectedLastImage()
{
    int lastIndex = thumbnailsView->count()-1;
    thumbnailsView->setCurrentRow(lastIndex);
}

void MainWindow::selectedPrevImage()
{
    int firstIndex = 0;
    int lastIndex = thumbnailsView->count()-1;
    int currentIndex = thumbnailsView->currentRow();
    --currentIndex;

    if (currentIndex < firstIndex)
        currentIndex = lastIndex;

    thumbnailsView->setCurrentRow(currentIndex);
}

void MainWindow::selectedNextImage()
{
    int firstIndex = 0;
    int lastIndex = thumbnailsView->count()-1;
    int currentIndex = thumbnailsView->currentRow();
    ++currentIndex;

    if (currentIndex > lastIndex)
        currentIndex = firstIndex;

    thumbnailsView->setCurrentRow(currentIndex);
}

void MainWindow::deleteTag()
{
    if (QMessageBox::Ok == QMessageBox::question(this, tr("删除标签"), tr("您确认要删除标签?"),
                              QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Ok))
    {
        _sqlite->deleteTag(categoriesView->currentTermId());
        categoriesView->deleteCurrentItem();
    }
}

void MainWindow::deleteImages()
{
    int termId = categoriesView->currentTermId();
    QString taxonomy = categoriesView->currentTaxonomy();
    int imageId = thumbnailsView->currentImageId();

    if (taxonomy == "photo_album")
    {
        _sqlite->deleteImageWithPhotoAlbum(imageId);
        thumbnailsView->removeCurrentImage();
    }
    else if (taxonomy == "tag")
    {
        _sqlite->deleteImageWithTag(termId, taxonomy, imageId);
        thumbnailsView->removeCurrentImage();
    }
}

void MainWindow::playSlide()
{
    playSlide(QModelIndex());
}

void MainWindow::copyImage()
{
    Image *image = _sqlite->queryImage(thumbnailsView->currentImageId());
    if (image)
    {
        QImage imageObj;
        imageObj.loadFromData(image->image());
        QApplication::clipboard()->setImage(imageObj, QClipboard::Clipboard);
    }
}

void MainWindow::pasteImage()
{
    if (categoriesView->currentTermId()!=0 &&
        categoriesView->currentTaxonomy() == "photo_album")
    {
        QImage image = QApplication::clipboard()->image();
        if (!image.isNull())
        {
            QApplication::clipboard()->clear();

            int imageId = AddImagesThread::addImage(_sqlite,
                                      categoriesView->currentTermId(), image);

            Image *image = _sqlite->queryImage(imageId);
            if (image)
            {
                thumbnailsView->addListWidgetItem(image);
                delete image;
            }
        }
    }
}

void MainWindow::addFavorites()
{
    int imageId = thumbnailsView->currentImageId();
    TableFavoritesUtil::insert(imageId);
    updateActionState();
}

void MainWindow::deleteFavorites()
{
    int imageId = thumbnailsView->currentImageId();
    TableFavoritesUtil::Delete(imageId);
    updateActionState();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("My House"),
                       tr("%1"
                          "<p>Copyright @ 2013 Junjian Wang.</p>"
                          "<p>All Rights Reserved.</p>").arg("<p><b>My House 1.0</b></p><p><a href='https://github.com/vwarship/myhouse/'>https://github.com/vwarship/myhouse/</a></p><p>Email: <a href='mailto:vwarship@163.com'>vwarship@163.com</a></p>"));
}

void MainWindow::showContextMenuWithImage(const QPoint /*pos*/)
{
    int imageId = thumbnailsView->currentImageId();
    if (imageId)
    {
        QVector<Term*> photoAlbums;
        QVector<Term*> tags;
        _sqlite->queryTermsByImageId(imageId, photoAlbums, tags);

        QMenu *menu = new QMenu(this);
        menu->setFont(QFont("", 9));

        termActionGroup = new QActionGroup(this);
        connect(termActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(locationToImage(QAction*)));
        foreach (Term *term, photoAlbums)
        {
            QAction *photoAlbumAction = new QAction(createIcon("photo_album"), term->name(), this);
            photoAlbumAction->setData(term->id());
            menu->addAction(photoAlbumAction);

            termActionGroup->addAction(photoAlbumAction);
        }

        if (tags.size())
            menu->addMenu(createTagContextMenuWithImage(tags));

        menu->addSeparator();
        menu->addAction(addFavoritesAction);
        menu->addAction(deleteFavoritesAction);
        menu->addSeparator();
        menu->addAction(deleteImagesAction);
        menu->addAction(copyImageAction);
        menu->addAction(playSlideAction);

        menu->exec(QCursor::pos());
    }
}

void MainWindow::locationToImage(QAction* action)
{
    int termId = action->data().toInt();
    //int imageId = thumbnailsView->currentImageId();

    QTreeWidgetItem *locationItem = NULL;
    for (int i=0; i<categoriesView->topLevelItemCount(); ++i)
    {
        visitTree(categoriesView->topLevelItem(i), termId, locationItem);
        if (locationItem)
        {
            categoriesView->setCurrentItem(locationItem, 0);
            locationItem->setSelected(true);
            break;
        }
    }
}

void MainWindow::visitTree(QTreeWidgetItem *item, int termId, QTreeWidgetItem *&locationItem)
{
    if (categoriesView->termId(item) == termId)
    {
        locationItem = item;
        return;
    }

    for (int i=0; i<item->childCount(); ++i)
        visitTree(item->child(i), termId, locationItem);
}

QMenu* MainWindow::createTagContextMenuWithImage(const QVector<Term*> &tags)
{
    QMenu *tagMenu = new QMenu(tr("标签"), this);
    tagMenu->setIcon(createIcon("tags"));
    tagMenu->setFont(QFont("", 9));
    foreach (Term *term, tags)
    {
        QAction *tagAction = new QAction(createIcon("tag"), term->name(), this);
        tagAction->setData(term->id());
        tagMenu->addAction(tagAction);

        termActionGroup->addAction(tagAction);
    }

    return tagMenu;
}

void MainWindow::updateImageProperyWidget()
{
    int imageId = thumbnailsView->currentImageId();
    Image* image = _sqlite->queryImage(imageId);

    if (!image)
        return;

    imagePropertyWidget->setImage(image);

    delete image;
}
