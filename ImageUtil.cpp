#include "ImageUtil.h"
#include <QImage>
#include <QPainter>
#include <QBuffer>
#include <QIODevice>

ImageUtil::ImageUtil()
{
}

QPixmap ImageUtil::scaleImage(const QPixmap &pixmap, const QSize &thumbnailSize)
{
    QPixmap scaledPixmap = pixmap.scaled(thumbnailSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    int x = 0;
    int y = 0;
    int w = scaledPixmap.width();
    int h = scaledPixmap.height();
    if (w < thumbnailSize.width())
        x = (thumbnailSize.width()-w)/2;
    if (h < thumbnailSize.height())
        y = (thumbnailSize.height()-h)/2;

    QPixmap thumbnail(thumbnailSize);
    thumbnail.fill();
    QPainter painter(&thumbnail);
    painter.drawPixmap(x, y, scaledPixmap);

    return thumbnail;
}

QImage ImageUtil::scaleImage(const QImage &image, const QSize &thumbnailSize)
{
    QImage scaledImage = image.scaled(thumbnailSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    int x = 0;
    int y = 0;
    int w = scaledImage.width();
    int h = scaledImage.height();
    if (w < thumbnailSize.width())
        x = (thumbnailSize.width()-w)/2;
    if (h < thumbnailSize.height())
        y = (thumbnailSize.height()-h)/2;

    //QPixmap thumbnail(thumbnailSize);
    //thumbnail.fill();
    //QPainter painter(&thumbnail);
    //painter.drawPixmap(x, y, scaledPixmap);

    return scaledImage;
}

QByteArray ImageUtil::pixmapToBytes(const QPixmap &pixmap)
{
    /*QDataBuf pixBuf;
    QDataStream bufStream(pixBuf);
    pixmap>>bufStream;
    pixBuf.buffer();*/

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");

    return bytes;
}

QByteArray ImageUtil::pixmapToBytes(const QImage &image)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");

    return bytes;
}
