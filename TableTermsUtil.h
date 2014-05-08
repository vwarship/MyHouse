#ifndef TABLETERMSUTIL_H
#define TABLETERMSUTIL_H

#include <QString>

class TableTermsUtil
{
public:
    TableTermsUtil();

    static void createTable();
    static void deleteTable();

    static void Delete(int termId);
    static void updateName(int id, QString name);

};

#endif // TABLETERMSUTIL_H
