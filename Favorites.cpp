#include "Favorites.h"

Favorites::Favorites(QObject *parent)
    : QObject(parent)
{
}

Favorites::Favorites(int id, int objectId, double createTime, QObject *parent)
    : QObject(parent), _id(id), _objectId(objectId), _createTime(createTime)
{

}
