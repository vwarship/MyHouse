#include "ListItemDelegate.h"
#include <QtGui>

ListItemDelegate::ListItemDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{
}

void ListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect r = option.rect;

    //Color: #C4C4C4
    QPen linePen(QColor::fromRgb(211,211,211), 1, Qt::SolidLine);

    //Color: #005A83
    QPen lineMarkedPen(QColor::fromRgb(0,90,131), 1, Qt::SolidLine);

    //Color: #333
    QPen fontPen(QColor::fromRgb(51,51,51), 1, Qt::SolidLine);

    //Color: #fff
    QPen fontMarkedPen(Qt::white, 1, Qt::SolidLine);

    if(option.state & QStyle::State_Selected)
    {
        QLinearGradient gradientSelected(r.left(),r.top(),r.left(),r.height()+r.top());
        gradientSelected.setColorAt(0.0, QColor::fromRgb(119,213,247));
        gradientSelected.setColorAt(0.9, QColor::fromRgb(27,134,183));
        gradientSelected.setColorAt(1.0, QColor::fromRgb(0,120,174));
        painter->setBrush(gradientSelected);
        painter->drawRect(r);

        //BORDER
        painter->setPen(lineMarkedPen);
        painter->drawLine(r.topLeft(),r.topRight());
        painter->drawLine(r.topRight(),r.bottomRight());
        painter->drawLine(r.bottomLeft(),r.bottomRight());
        painter->drawLine(r.topLeft(),r.bottomLeft());

        painter->setPen(fontMarkedPen);

    }
    else
    {
        //BACKGROUND
        //ALTERNATING COLORS
        painter->setBrush( (index.row() % 2) ? Qt::white : QColor(252,252,252) );
        painter->drawRect(r);

        //BORDER
        painter->setPen(linePen);
        painter->drawLine(r.topLeft(),r.topRight());
        painter->drawLine(r.topRight(),r.bottomRight());
        painter->drawLine(r.bottomLeft(),r.bottomRight());
        painter->drawLine(r.topLeft(),r.bottomLeft());

        painter->setPen(fontPen);
    }

    //GET TITLE, DESCRIPTION AND ICON
    QIcon icon = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
    QString title = index.data(Qt::DisplayRole).toString();
    //QString description = index.data(Qt::UserRole + 1).toString();

    int imageSpace = 10;
    if (!icon.isNull())
    {
        //ICON
        QRect iconRect = option.rect.adjusted(4, 4, -4, -4);
        icon.paint(painter, iconRect, Qt::AlignVCenter|Qt::AlignCenter);
        imageSpace = 55;
    }

    //TITLE
    r = option.rect.adjusted(4, 0, 0, 0);
    painter->setFont( QFont( "Lucida Grande", 9, QFont::Normal ) );
    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, title, &r);

    /*QStyleOptionButton check_box_style_option;
    check_box_style_option.state |= QStyle::State_Enabled;
    check_box_style_option.rect = QRect(r.left(), r.top(), 32, 32);
    QApplication::style()->drawControl(QStyle::CE_PushButton, &check_box_style_option, painter);*/

    /*//DESCRIPTION
    r = option.rect.adjusted(imageSpace, 30, -10, 0);
    painter->setFont( QFont( "Lucida Grande", 9, QFont::Normal ) );
    painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignLeft, description, &r);*/
}

QSize ListItemDelegate::sizeHint(const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    return QSize(220, 180); // very dumb value
}

ListItemDelegate::~ListItemDelegate()
{

}
