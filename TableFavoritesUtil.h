#ifndef TABLEFAVORITESUTIL_H
#define TABLEFAVORITESUTIL_H

#include <QObject>

class TableFavoritesUtil : public QObject
{
    Q_OBJECT
public:
    explicit TableFavoritesUtil(QObject *parent = 0);
    
    static void createTable();
    static void deleteTable();
    static void Delete(int imageId);
    static void insert(int imageId);
    static bool isExist(int imageId);

signals:
    
public slots:
    
};

#endif // TABLEFAVORITESUTIL_H
