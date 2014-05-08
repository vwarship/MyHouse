#ifndef SQLITE_H
#define SQLITE_H

#include <QObject>
#include <QVector>
#include <QSqlDatabase>
#include "Term.h"
#include "TermTaxonomy.h"
#include "TermRelationship.h"
#include "Image.h"
#include "Favorites.h"

class QSqlQuery;
class Sqlite : public QObject
{
    Q_OBJECT
public:
    explicit Sqlite(QObject *parent = 0);
    static bool isDBExists();
    void Install();
    void deleteAllTables();
    void createAllTables();

    void insertTerm(Term term);

    int insertImage(int termId, Image &image);
    void insertTermTaxonomy(QString termName, int parentId, QString taxonomy, QString description="");

    Term queryTerm(QString term);
    QVector<Image*> queryImages(int termId, QString taxonomy);
    Image* queryImage(int imageId);
    QVector<Image*> queryFavoritesImages();
    void queryTermsByImageId(int imageId, QVector<Term*> &photoAlbums, QVector<Term*> &tags);

    void deleteImageWithTag(int termId, QString taxonomy, int imageId);
    void deleteImageWithPhotoAlbum(int imageId);
    void deleteTag(int termId);
    void deletePhotoAlbum(int termId);
    void deleteImages(int termId, QString taxonomy);

signals:
    
public slots:

private:
    QVector<Image*> createImagesByQueryResult(QSqlQuery &query);

    void createMyHourseTermTaxonomyInfo();

public:
    static const QString DATABASE_NAME;

private:
    QSqlDatabase _db;
};

#endif // SQLITE_H
