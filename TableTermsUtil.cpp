#include "TableTermsUtil.h"
#include <QSqlQuery>
#include <QVariant>

TableTermsUtil::TableTermsUtil()
{
}

void TableTermsUtil::createTable()
{
    QSqlQuery query;
    query.exec("create table terms ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name varchar(200), "
               "UNIQUE (name)"
               ")");
}

void TableTermsUtil::deleteTable()
{
    QSqlQuery query;
    query.exec("drop table terms");
}

void TableTermsUtil::Delete(int termId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM terms "
                  "WHERE id = ?");
    query.addBindValue(termId);
    query.exec();
}

void TableTermsUtil::updateName(int id, QString name)
{
    QSqlQuery query;
    query.prepare("update terms "
                  "set name=? "
                  "WHERE id = ?");
    query.addBindValue(name);
    query.addBindValue(id);
    query.exec();
}
