#ifndef THUMBNAILSVIEW_H
#define THUMBNAILSVIEW_H

#include <QListWidget>

class Sqlite;
class Image;

class ThumbnailsView : public QListWidget
{
    Q_OBJECT

public:
    explicit ThumbnailsView(QWidget *parent = 0);
    void setSqlite(Sqlite *sqlite)
    {
        _sqlite = sqlite;
    }

    int currentImageId();

    void addListWidgetItem(Image *image);
    void removeCurrentImage();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *e);

signals:

public slots:
    void showImagesByTermId(int termId, QString taxonomy);

private:
    void setDragDrop();

private:
    const QSize thumbnailSize;
    Sqlite *_sqlite;

    QPoint _beginPos;

};

#endif // THUMBNAILSVIEW_H
