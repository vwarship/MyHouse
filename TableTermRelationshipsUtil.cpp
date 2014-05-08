#include "TableTermRelationshipsUtil.h"
#include <QSqlQuery>
#include <QVariant>

TableTermRelationshipsUtil::TableTermRelationshipsUtil()
{
}

void TableTermRelationshipsUtil::createTable()
{
    QSqlQuery query;
    query.exec("create table term_relationships ("
               "object_id INTEGER NOT NULL, "
               "term_taxonomy_id INTEGER NOT NULL, "
               "UNIQUE (object_id, term_taxonomy_id)"
               ")");
}

void TableTermRelationshipsUtil::deleteTable()
{
    QSqlQuery query;
    query.exec("drop table term_relationships");
}

void TableTermRelationshipsUtil::Delete(int termTaxonomyId, int imageId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM term_relationships "
                  "WHERE term_taxonomy_id = ? and object_id = ?");
    query.addBindValue(termTaxonomyId);
    query.addBindValue(imageId);
    query.exec();
}

void TableTermRelationshipsUtil::deleteByTermTaxonomyId(int termTaxonomyId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM term_relationships "
                  "WHERE term_taxonomy_id = ?");
    query.addBindValue(termTaxonomyId);
    query.exec();
}

void TableTermRelationshipsUtil::deleteObjectAllRelationships(int imageId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM term_relationships "
                  "WHERE object_id = ?");
    query.addBindValue(imageId);
    query.exec();
}

void TableTermRelationshipsUtil::insertTermRelationship(TermRelationship termRelationship)
{
    QSqlQuery query;
    query.prepare("insert into term_relationships(object_id, term_taxonomy_id) "
                  "values(?, ?)");
    query.addBindValue(termRelationship.objectId());
    query.addBindValue(termRelationship.termTaxonomyId());
    query.exec();
}
