#ifndef FAVORITES_H
#define FAVORITES_H

#include <QObject>

class Favorites : public QObject
{
    Q_OBJECT
public:
    explicit Favorites(QObject *parent = 0);
    Favorites(int id, int objectId, double createTime, QObject *parent = 0);
    
    int id()
    {
        return _id;
    }

    int objectId()
    {
        return _objectId;
    }

    double createTime()
    {
        return _createTime;
    }

signals:
    
public slots:
    
private:
    int _id;
    int _objectId;
    double _createTime;

};

#endif // FAVORITES_H
