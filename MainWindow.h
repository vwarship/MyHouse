#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QVector>
#include <QMainWindow>
#include <QModelIndex>
#include <QProgressDialog>
#include <QPoint>

class QToolBar;
class CategoriesView;
class ThumbnailsView;
class QAction;
class QActionGroup;
class SlideWidget;
class Sqlite;
class QProgressBar;
class QLabel;
class Term;
class QTreeWidgetItem;
class ImagePropertyWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static QIcon createIcon(const QString &filename);

signals:
    void addImages(QStringList selectedFiles);

public slots:
    void addPhotoAlbum();
    void deletePhotoAlbum();
    void editTerm();
    void addTag();
    void deleteTag();
    void addImages();
    void deleteImages();
    void addFavorites();
    void deleteFavorites();
    void playSlide();
    void copyImage();
    void pasteImage();
    void about();

    void currentImageChanged();
    void playSlide(QModelIndex modelIndex);
    void selectedFirstImage();
    void selectedLastImage();
    void selectedPrevImage();
    void selectedNextImage();

    void on_addImageing(QString filename);
    void on_addImaged(int termId, int imageId);

    void updateActionState();
    void updateActionState(int termId, QString taxonomy);
    void showContextMenuWithImage(const QPoint pos);
    void locationToImage(QAction* action);
    void updateImageProperyWidget();

private:
    void createCentralWidget();
    QWidget* createLeftWidget();
    QWidget* createAdvertWidget();
    void createActions();
    void createMenus();
    void createFileMenu();
    void createEditMenu();
    void createViewMenu();
    void createWindowMenu();
    void createHelpMenu();
    void createToolbar();
    void createStatusBar();

    QMenu* createTagContextMenuWithImage(const QVector<Term*> &tags);
    void visitTree(QTreeWidgetItem *item, int termId, QTreeWidgetItem *&locationItem);
    void createDockWindowWithImageProperty();

private:
    QToolBar *toolBar;
    CategoriesView *categoriesView;
    ThumbnailsView *thumbnailsView;

    SlideWidget *slideWidget;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;

    QAction *addPhotoAlbumAction;
    QAction *deletePhotoAlbumAction;
    QAction *addTagAction;
    QAction *deleteTagAction;
    QAction *editTermAction;
    QAction *addImagesAction;
    QAction *deleteImagesAction;
    QAction *addFavoritesAction;
    QAction *deleteFavoritesAction;
    QAction *playSlideAction;
    QAction *copyImageAction;
    QAction *pasteImageAction;
    QAction *aboutAction;
    QAction *exitAction;

    QActionGroup *termActionGroup;

    ImagePropertyWidget *imagePropertyWidget;

    Sqlite *_sqlite;

    QProgressBar *_addImagesProgressBar;
    QLabel *_statusBarText;

};

#endif // MAINWINDOW_H
