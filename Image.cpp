#include "Image.h"
#include <QCryptographicHash>

Image::Image(QByteArray image, QByteArray thumbnail)
    : _image(image), _thumbnail(thumbnail)
{
    _md5 = md5(_image);
}

Image::Image(QByteArray image, QString title, QString description, QByteArray thumbnail)
    : _image(image), _title(title), _description(description), _thumbnail(thumbnail)
{
    _md5 = md5(_image);
}

Image::Image(int id, QByteArray image, QString title, QString description, QByteArray thumbnail, QString createTime)
    : _id(id), _image(image), _title(title), _description(description), _thumbnail(thumbnail), _createTime(createTime)
{
    _md5 = md5(_image);
}

QString Image::md5(QByteArray &byteArray)
{
    return QCryptographicHash::hash(byteArray, QCryptographicHash::Md5).toHex().data();
}
