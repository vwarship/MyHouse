#ifndef IMAGE_H
#define IMAGE_H

#include <QByteArray>
#include <QString>
#include <QObject>

class Image
{
public:
    Image(QByteArray image, QByteArray thumbnail);
    Image(QByteArray image, QString title, QString description, QByteArray thumbnail);
    Image(int id, QByteArray image, QString title, QString description, QByteArray thumbnail, QString createTime);

    int id()
    {
        return _id;
    }

    QByteArray image()
    {
        return _image;
    }

    QString title()
    {
        return _title;
    }

    QString description()
    {
        return _description;
    }

    QByteArray thumbnail()
    {
        return _thumbnail;
    }

    QString createTime()
    {
        return _createTime;
    }

    QString md5()
    {
        return _md5;
    }

    static QString md5(QByteArray &byteArray);

private:
    int _id;
    QByteArray _image;
    QString _title;
    QString _description;
    QByteArray _thumbnail;
    QString _createTime;
    QString _md5;

};

#endif // IMAGE_H
