#ifndef LISTITEMDELEGATE_H
#define LISTITEMDELEGATE_H

#include <QAbstractItemDelegate>

class ListItemDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit ListItemDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    virtual ~ListItemDelegate();

signals:
    
public slots:
    
};

#endif // LISTITEMDELEGATE_H
