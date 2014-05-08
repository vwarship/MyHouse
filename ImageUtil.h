#ifndef IMAGEUTIL_H
#define IMAGEUTIL_H
#include <QPixmap>

class QImage;
class ImageUtil
{
public:
    ImageUtil();

    static QPixmap scaleImage(const QPixmap &pixmap, const QSize &thumbnailSize);
    static QImage scaleImage(const QImage &image, const QSize &thumbnailSize);
    static QByteArray pixmapToBytes(const QPixmap &pixmap);
    static QByteArray pixmapToBytes(const QImage &image);

};

#endif // IMAGEUTIL_H
