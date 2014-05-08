#include "AddImagesThread.h"
#include <QImage>
#include <QPixmap>
#include "Sqlite.h"
#include "Image.h"
#include "TermRelationship.h"
#include "ImageUtil.h"
#include "TableTermRelationshipsUtil.h"
#include "TableImagesUtil.h"

AddImagesThread::AddImagesThread(Sqlite *sqlite, QStringList selectedFiles, int termId, QObject *parent)
    : _sqlite(sqlite), _selectedFiles(selectedFiles), _termId(termId), QThread(parent)
{
}

void AddImagesThread::run()
{
    emit started();

    foreach (QString filename, _selectedFiles)
    {
        emit addImageing(filename);
        int imageId = addImage(_sqlite, _termId, QImage(filename));
        emit addImaged(_termId, imageId);
    }

    emit finished();
}

int AddImagesThread::addImage(Sqlite *sqlite, int termId, QImage &img)
{
    if (img.isNull())
        return 0;

    QByteArray imageBytes = ImageUtil::pixmapToBytes(img);
    int imageId = TableImagesUtil::query(Image::md5(imageBytes));
    if (imageId)
        return imageId;

    Image image(imageBytes,
                ImageUtil::pixmapToBytes(ImageUtil::scaleImage(img, QSize(220, 180))));

    return sqlite->insertImage(termId, image);
}
