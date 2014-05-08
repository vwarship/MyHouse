#ifndef ADDIMAGESTHREAD_H
#define ADDIMAGESTHREAD_H

#include <QThread>
#include <QString>
#include <QStringList>

class Sqlite;
class QImage;

class AddImagesThread : public QThread
{
    Q_OBJECT
public:
    explicit AddImagesThread(Sqlite *sqlite, QStringList selectedFiles, int termId, QObject *parent = 0);
    static int addImage(Sqlite *sqlite, int termId, QImage &img);

signals:
    void addImageing(QString filename);
    int addImaged(int termId, int imageId);
public slots:

protected:
    void run();

private:
    Sqlite *_sqlite;
    int _termId;
    QStringList _selectedFiles;

};

#endif // ADDIMAGESTHREAD_H
