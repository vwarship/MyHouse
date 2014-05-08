#ifndef TABLEIMAGESUTIL_H
#define TABLEIMAGESUTIL_H

#include <QString>

class Image;
class TableImagesUtil
{
public:
    TableImagesUtil();

    static void createTable();
    static void deleteTable();

    static void Delete(int imageId);

    static int insertImage(Image &image);
    static int query(QString imageMD5);
    static void update(int id, QString title, QString description);

};

#endif // TABLEIMAGESUTIL_H
