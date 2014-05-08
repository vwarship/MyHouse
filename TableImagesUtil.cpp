#include "TableImagesUtil.h"
#include <QSqlQuery>
#include <QVariant>
#include "Image.h"

TableImagesUtil::TableImagesUtil()
{
}

void TableImagesUtil::createTable()
{
    QSqlQuery query;
    query.exec("create table images ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "image BLOB DEFAULT NULL, "
               "title varchar(50) DEFAULT '', "
               "description TEXT DEFAULT NULL, "
               "thumbnail BLOB DEFAULT NULL, "
               "create_time timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP, "
               "md5 char(32) DEFAULT '', "
               "UNIQUE (md5)"
               ")");
}

void TableImagesUtil::deleteTable()
{
    QSqlQuery query;
    query.exec("drop table images");
}

void TableImagesUtil::Delete(int imageId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM images "
                  "WHERE id = ?");
    query.addBindValue(imageId);
    query.exec();
}

int TableImagesUtil::insertImage(Image &image)
{
    QSqlQuery query;
    query.prepare("insert into images(image, title, description, thumbnail, md5) "
                  "values(?, ?, ?, ?, ?)");
    query.addBindValue(image.image());
    query.addBindValue(image.title());
    query.addBindValue(image.description());
    query.addBindValue(image.thumbnail());
    query.addBindValue(image.md5());
    query.exec();

    return query.lastInsertId().toInt();
}

int TableImagesUtil::query(QString imageMD5)
{
    QSqlQuery query;
    query.prepare("select id from images where md5=?");
    query.addBindValue(imageMD5);
    query.exec();

    if (query.next())
        return query.value(0).toInt();

    return 0;
}

void TableImagesUtil::update(int id, QString title, QString description)
{
    QSqlQuery query;
    query.prepare("update images "
                  "set title=?, description=? "
                  "WHERE id = ?");
    query.addBindValue(title);
    query.addBindValue(description);
    query.addBindValue(id);
    query.exec();
}
