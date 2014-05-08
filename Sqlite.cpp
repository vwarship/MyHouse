#include "Sqlite.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QApplication>
#include <QFile>
#include <QVariant>
#include <QDateTime>
#include <QDebug>
#include "TableTermsUtil.h"
#include "TableTermTaxonomyUtil.h"
#include "TableTermRelationshipsUtil.h"
#include "TableImagesUtil.h"
#include "TableFavoritesUtil.h"

const QString Sqlite::DATABASE_NAME = "myhouse.db";

Sqlite::Sqlite(QObject *parent) : QObject(parent)
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(DATABASE_NAME);
}

bool Sqlite::isDBExists()
{
    return QFile::exists(DATABASE_NAME);
}

void Sqlite::Install()
{
    if (_db.open())
    {
        deleteAllTables();
        createAllTables();

        //createMyHourseTermTaxonomyInfo();

        _db.close();
    }
}

void Sqlite::deleteAllTables()
{
    TableTermsUtil::deleteTable();
    TableTermTaxonomyUtil::deleteTable();
    TableTermRelationshipsUtil::deleteTable();
    TableImagesUtil::deleteTable();
    TableFavoritesUtil::deleteTable();
}

void Sqlite::createAllTables()
{
    TableTermsUtil::createTable();
    TableTermTaxonomyUtil::createTable();
    TableTermRelationshipsUtil::createTable();
    TableImagesUtil::createTable();
    TableFavoritesUtil::createTable();
}

void Sqlite::insertTerm(Term term)
{
    QSqlQuery query;
    query.prepare("insert into terms(name) values(?)");
    query.addBindValue( trUtf8(term.name().toAscii()) );
    query.exec();
}

Term Sqlite::queryTerm(QString term)
{
    QSqlQuery query;
    query.prepare("select id, name from terms where name=?");
    query.addBindValue( trUtf8(term.toAscii()) );
    query.exec();

    if (query.next())
    {
        return Term(query.value(0).toInt(), query.value(1).toString());
    }

    return Term(-1, "");
}

void Sqlite::insertTermTaxonomy(QString termName, int parentId, QString taxonomy, QString description)
{
    insertTerm(Term(termName));
    Term term = queryTerm(termName);
    TermTaxonomy *termTaxonomy = new TermTaxonomy(term.id(), taxonomy, description, parentId);
    TableTermTaxonomyUtil::insertTermTaxonomy(termTaxonomy);
}

int Sqlite::insertImage(int termId, Image &image)
{
    int imageId = TableImagesUtil::insertImage(image);
    TableTermRelationshipsUtil::insertTermRelationship(TermRelationship(imageId, termId));

    return imageId;
}

QVector<Image*> Sqlite::queryImages(int termId, QString taxonomy)
{
    QSqlQuery query;
    query.prepare("SELECT img.id, img.title, img.description, img.thumbnail, img.create_time "
                  "FROM images as img, term_relationships as tr, terms AS t, term_taxonomy AS tt "
                  "WHERE tt.taxonomy = ? AND tt.id=? "
                  "AND t.id = tt.term_id "
                  "AND tr.object_id = img.id "
                  "AND tt.id = tr.term_taxonomy_id");
    query.addBindValue(taxonomy);
    query.addBindValue(termId);
    query.exec();

    return createImagesByQueryResult(query);
}

QVector<Image*> Sqlite::createImagesByQueryResult(QSqlQuery &query)
{
    QVector<Image*> images;

    while (query.next())
    {
        int i=0;
        int id = query.value(i++).toInt();
        QByteArray img;// = query.value(i++).toByteArray();
        QString title = query.value(i++).toString();
        QString description = query.value(i++).toString();
        QByteArray thumbnail = query.value(i++).toByteArray();
        QString createTime = query.value(i++).toString();

        Image *image = new Image(id, img, title, description, thumbnail, createTime);
        images.push_back(image);
    }

    return images;
}

Image* Sqlite::queryImage(int imageId)
{
    QSqlQuery query;
    query.prepare("SELECT id, image, title, description, thumbnail, create_time "
                  "FROM images "
                  "WHERE id = ?");
    query.addBindValue(imageId);
    query.exec();

    if (query.next())
    {
        Image *image = new Image(
                    query.value(0).toInt(),
                    query.value(1).toByteArray(),
                    query.value(2).toString(),
                    query.value(3).toString(),
                    query.value(4).toByteArray(),
                    query.value(5).toString()
                    );

        return image;
    }

    return NULL;
}

void Sqlite::deleteImageWithTag(int termId, QString taxonomy, int imageId)
{
    TermTaxonomy *termTaxonomy = TableTermTaxonomyUtil::query(termId, taxonomy);
    TableTermRelationshipsUtil::Delete(termTaxonomy->id(), imageId);
}

void Sqlite::deleteImageWithPhotoAlbum(int imageId)
{
    TableTermRelationshipsUtil::deleteObjectAllRelationships(imageId);
    TableImagesUtil::Delete(imageId);
}

void Sqlite::deleteTag(int termId)
{
    TermTaxonomy *termTaxonomy = TableTermTaxonomyUtil::query(termId, "tag");
    TableTermTaxonomyUtil::Delete(termId, "tag");
    TableTermRelationshipsUtil::deleteByTermTaxonomyId(termTaxonomy->id());
    TableTermsUtil::Delete(termId);
}

void Sqlite::deletePhotoAlbum(int termId)
{
    deleteImages(termId, "photo_album");
    TermTaxonomy *termTaxonomy = TableTermTaxonomyUtil::query(termId, "photo_album");
    TableTermTaxonomyUtil::Delete(termId, "photo_album");
    TableTermRelationshipsUtil::deleteByTermTaxonomyId(termTaxonomy->id());
    TableTermsUtil::Delete(termId);
}

void Sqlite::deleteImages(int termId, QString taxonomy)
{
    QSqlQuery query;
    query.prepare("delete from images "
                  "where id in (select object_id from term_relationships where term_taxonomy_id = "
                  "(select id from term_taxonomy where term_id=? and taxonomy=?))");
    query.addBindValue(termId);
    query.addBindValue(taxonomy);
    query.exec();
}

QVector<Image*> Sqlite::queryFavoritesImages()
{
    QSqlQuery query;
    query.prepare("SELECT img.id, img.title, img.description, img.thumbnail, img.create_time "
                  "FROM images as img, favorites as f "
                  "WHERE f.object_id = img.id ");
    query.exec();

    return createImagesByQueryResult(query);
}

void Sqlite::queryTermsByImageId(int imageId, QVector<Term*> &photoAlbums, QVector<Term*> &tags)
{
    QSqlQuery query;
    query.prepare("SELECT t.id, t.name, tt.taxonomy "
                  "FROM terms t, term_taxonomy tt, term_relationships tr "
                  "WHERE tr.object_id = ? "
                  "AND tr.term_taxonomy_id=tt.id "
                  "AND tt.term_id=t.id");
    query.addBindValue(imageId);
    query.exec();

    while (query.next())
    {
        int termId = query.value(0).toInt();
        QString termName = query.value(1).toString();
        QString taxonomy = query.value(2).toString();

        if (taxonomy == "photo_album")
            photoAlbums.push_back(new Term(termId, termName));
        else if (taxonomy == "tag")
            tags.push_back(new Term(termId, termName));
    }
}

void Sqlite::createMyHourseTermTaxonomyInfo()
{
    insertTermTaxonomy("户型", 0, "tag");
    insertTermTaxonomy("风格", 0, "tag");
    insertTermTaxonomy("区域", 0, "tag");

    //insertTermTaxonomy("一居", 1, "tag");
    insertTermTaxonomy("二居", 1, "tag");
    insertTermTaxonomy("三居", 1, "tag");
    insertTermTaxonomy("四居", 1, "tag");
    insertTermTaxonomy("复式", 1, "tag");
    insertTermTaxonomy("跃层", 1, "tag");
    insertTermTaxonomy("别墅", 1, "tag");

    insertTermTaxonomy("风格", 2, "tag");
    //insertTermTaxonomy("简约", 2, "tag");
    insertTermTaxonomy("欧美", 2, "tag");
    insertTermTaxonomy("田园", 2, "tag");
    insertTermTaxonomy("中式", 2, "tag");

    insertTermTaxonomy("区域", 3, "tag");
    insertTermTaxonomy("客厅", 3, "tag");
    insertTermTaxonomy("卧室", 3, "tag");
    //insertTermTaxonomy("卫生间", 3, "tag");
    insertTermTaxonomy("餐厅", 3, "tag");
    insertTermTaxonomy("厨房", 3, "tag");
    insertTermTaxonomy("书房", 3, "tag");
    //insertTermTaxonomy("儿童房", 3, "tag");
    insertTermTaxonomy("阳台", 3, "tag");
    //insertTermTaxonomy("衣帽间", 3, "tag");
    insertTermTaxonomy("玄关", 3, "tag");
}
