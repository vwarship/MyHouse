#include "CategoriesView.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "Sqlite.h"
#include <QDebug>
#include <QDragEnterEvent>
#include "Term.h"
#include "MainWindow.h"
#include "ThumbnailsView.h"
#include "TableTermRelationshipsUtil.h"

const QChar CategoriesView::ITEM_DATA_DELIMITER = ';';

CategoriesView::CategoriesView(QWidget *parent) : QTreeWidget(parent)
{
    setAcceptDrops(true);
    setMinimumWidth(200);
    setHeaderHidden(true);
    setFont(QFont("", 9));

    QSqlDatabase db = QSqlDatabase::database();
    if (db.open())
    {
        QTreeWidgetItem *photoAlbumRoot = createRootTreeWidgetItem(trUtf8("相册"), 0, "photo_album", "photo_albums");
        setCurrentItem(photoAlbumRoot);
        createTreeNodeChildren(photoAlbumRoot, "photo_album");

        QTreeWidgetItem *tagRoot = createRootTreeWidgetItem(trUtf8("标签"), 0, "tag", "tags");
        createTreeNodeChildren(tagRoot, "tag");

        createRootTreeWidgetItem(trUtf8("收藏"), 0, "favorites", "favorites");
    }

    expandAll();

    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionOnChanged()));
}

QTreeWidgetItem* CategoriesView::createRootTreeWidgetItem(QString name, int id, QString taxonomy, QString iconName)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList(name));
    setTreeWidgetItem(item, id, taxonomy, iconName);
    return item;
}

QTreeWidgetItem* CategoriesView::createTreeWidgetItem(QTreeWidgetItem *parent, QString name, int id, QString taxonomy)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent, QStringList(name));
    setTreeWidgetItem(item, id, taxonomy, taxonomy);
    return item;
}

void CategoriesView::setTreeWidgetItem(QTreeWidgetItem *item, int id, QString taxonomy, QString iconName)
{
    QString itemData = QString("%1%2%3").arg(id).arg(ITEM_DATA_DELIMITER).arg(taxonomy);
    item->setData(0, Qt::UserRole, qVariantFromValue(itemData));
    item->setIcon(0, MainWindow::createIcon(iconName));
}

void CategoriesView::createTreeNodeChildren(QTreeWidgetItem* parent, QString taxonomy)
{
    QSqlQuery query;
    query.exec(QString("select a.id, a.name from terms a, term_taxonomy b "
                       "where a.id=b.term_id and b.parent=%1 and b.taxonomy='%2'")
               .arg(termId(parent))
               .arg(taxonomy)
               );
    while(query.next())
    {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();

        QTreeWidgetItem *item = createTreeWidgetItem(parent, name, id, taxonomy);
        createTreeNodeChildren(item, taxonomy);
    }
}

void CategoriesView::addNewItemInCurrentItem(Term &term, QString taxonomy)
{
    QTreeWidgetItem *parent = this->currentItem();
    /*QTreeWidgetItem *item = */createTreeWidgetItem(parent, term.name(), term.id(), taxonomy);

    if (!parent->isExpanded())
        parent->setExpanded(true);

    //setCurrentItem(item);
}

void CategoriesView::deleteCurrentItem()
{
    QTreeWidgetItem *currentItem = this->currentItem();
    currentItem->parent()->removeChild(currentItem);
}

void CategoriesView::itemSelectionOnChanged()
{
    if (selectedItems().count() > 0)
    {
        QTreeWidgetItem *item = selectedItems()[0];
        emit categoryChanged(termId(item), taxonomy(item));
    }
}

QString CategoriesView::currentTermName()
{
    return currentItem()->text(0);
}

int CategoriesView::currentTermId()
{
    return termId(currentItem());
}

int CategoriesView::termId(const QTreeWidgetItem *item) const
{
    if (item)
    {
        QVariant termIdAndTaxonomy = item->data(0, Qt::UserRole);
        QStringList strList = termIdAndTaxonomy.toString().split(ITEM_DATA_DELIMITER);
        if (strList.size())
            return strList[0].toInt();
    }

    return 0;
}

QString CategoriesView::currentTaxonomy()
{
    return taxonomy(currentItem());
}

QString CategoriesView::taxonomy(const QTreeWidgetItem *item) const
{
    if (item)
    {
        QVariant termIdAndTaxonomy = item->data(0, Qt::UserRole);
        QStringList strList = termIdAndTaxonomy.toString().split(ITEM_DATA_DELIMITER);
        if (strList.size())
            return strList[1];
    }

    return "";
}

void CategoriesView::updateCurrentItem(QString text)
{
    currentItem()->setText(0, text);
}

void CategoriesView::dragEnterEvent(QDragEnterEvent *event)
{
    ThumbnailsView *source=qobject_cast<ThumbnailsView*>(event->source());
    if(source)
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
}
void CategoriesView::dragMoveEvent(QDragMoveEvent *event)
{
    ThumbnailsView *source=qobject_cast<ThumbnailsView*>(event->source());
    if(source)
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
}
void CategoriesView::dropEvent(QDropEvent *event)
{
    ThumbnailsView *source=qobject_cast<ThumbnailsView*>(event->source());
    if (source)
    {
        QModelIndex currentId=indexAt(event->pos());
        if(currentId.isValid())
        {
            QTreeWidgetItem *item = itemFromIndex(currentId);

            int dropTermId = termId(item);
            QString dropTaxonomy = taxonomy(item);

            if (dropTaxonomy == "tag")
            {
                TableTermRelationshipsUtil::insertTermRelationship(
                            TermRelationship(source->currentImageId(), dropTermId));
            }

            event->setDropAction(Qt::CopyAction);
            event->accept();
        }
    }
}
