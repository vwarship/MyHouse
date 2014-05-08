#include "TableFavoritesUtil.h"
#include <QSqlQuery>
#include <QVariant>

TableFavoritesUtil::TableFavoritesUtil(QObject *parent) :
    QObject(parent)
{
}

void TableFavoritesUtil::createTable()
{
    QSqlQuery query;
    query.exec("create table favorites ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "object_id INTEGER NOT NULL, "
               "create_time timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP, "
               "UNIQUE (object_id)"
               ")");
}

void TableFavoritesUtil::deleteTable()
{
    QSqlQuery query;
    query.exec("drop table favorites");
}

void TableFavoritesUtil::Delete(int imageId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM favorites "
                  "WHERE object_id = ?");
    query.addBindValue(imageId);
    query.exec();
}

void TableFavoritesUtil::insert(int imageId)
{
    QSqlQuery query;
    query.prepare("insert into favorites(object_id) "
                  "values(?)");
    query.addBindValue(imageId);
    query.exec();
}

bool TableFavoritesUtil::isExist(int imageId)
{
    QSqlQuery query;
    query.prepare("SELECT id "
                  "FROM favorites "
                  "WHERE object_id = ?");
    query.addBindValue(imageId);
    query.exec();

    if (query.next())
        return true;

    return false;
}
