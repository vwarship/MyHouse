#include "ThumbnailsView.h"
#include <QApplication>
#include <QPainter>
#include <QResizeEvent>
#include "Sqlite.h"
#include "ImageUtil.h"
#include <QDebug>
#include "ListItemDelegate.h"

ThumbnailsView::ThumbnailsView(QWidget *parent)
    : QListWidget(parent), thumbnailSize(220, 180)
{
    setItemDelegate(new ListItemDelegate(this));
    setViewMode(QListWidget::IconMode);
    setIconSize(thumbnailSize);
    setResizeMode(QListWidget::Adjust);
    setMovement(QListWidget::Static);
    setSpacing(10);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDrop();

    setContextMenuPolicy(Qt::CustomContextMenu);
}

void ThumbnailsView::setDragDrop()
{
    viewport()->setAcceptDrops(true);;
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DragDrop);
}

void ThumbnailsView::showImagesByTermId(int termId, QString taxonomy)
{
    clear();

    QVector<Image*> images;
    if (taxonomy == "favorites")
        images = _sqlite->queryFavoritesImages();
    else
        images = _sqlite->queryImages(termId, taxonomy);
    for (int i=0; i<images.size(); ++i)
    {
        Image *image = images[i];
        addListWidgetItem(image);
        delete image;
    }

    if (images.size())
    {
        this->setCurrentItem(item(0));
        item(0)->setSelected(true);
    }
}

void ThumbnailsView::addListWidgetItem(Image *image)
{
    QPixmap thumbnail = QPixmap();
    thumbnail.loadFromData(image->thumbnail());

    QListWidgetItem *item = new QListWidgetItem();
    item->setData(Qt::DecorationRole, thumbnail);
    item->setData(Qt::UserRole, image->id());
    item->setSizeHint(QSize(220, 180));
    addItem(item);
}

int ThumbnailsView::currentImageId()
{
    QListWidgetItem *curItem = currentItem();
    if (curItem)
        return curItem->data(Qt::UserRole).toInt();

    return 0;
}

void ThumbnailsView::removeCurrentImage()
{
    takeItem(indexFromItem(currentItem()).row());
}

void ThumbnailsView::mousePressEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button())
    {
        _beginPos=event->pos();
    }

    QListWidget::mousePressEvent(event);
}

void ThumbnailsView::mouseMoveEvent(QMouseEvent *e)
{
    int distance = (e->pos()-_beginPos).manhattanLength();
    if ((e->buttons() & Qt::LeftButton) &&
        distance >= QApplication::startDragDistance() &&
        currentItem())
    {
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(currentItem()->text());

        QDrag *drag=new QDrag(this);
        drag->setMimeData(mimeData);
        QImage image;
        if (true == image.load(":/Images/images/32/tag_add.png"))
            drag->setPixmap(QPixmap::fromImage(image));

        drag->exec(Qt::CopyAction);
    }
    else
        return;//防止item位置移动

    QListWidget::mouseMoveEvent(e);
}
