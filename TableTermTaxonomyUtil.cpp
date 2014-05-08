#include "TableTermTaxonomyUtil.h"
#include <QSqlQuery>
#include <QVariant>
#include "TermTaxonomy.h"

TableTermTaxonomyUtil::TableTermTaxonomyUtil()
{
}

void TableTermTaxonomyUtil::createTable()
{
    QSqlQuery query;
    query.exec("create table term_taxonomy ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "term_id INTEGER NOT NULL DEFAULT 0, "
               "taxonomy varchar(32) NOT NULL DEFAULT '', " //category tag
               "description varchar(200) NOT NULL, "
               "parent INTEGER NOT NULL DEFAULT 0, "
               "count INTEGER NOT NULL DEFAULT 0, "
               "UNIQUE (term_id, taxonomy)"
               ")");
}

void TableTermTaxonomyUtil::deleteTable()
{
    QSqlQuery query;
    query.exec("drop table term_taxonomy");
}

TermTaxonomy* TableTermTaxonomyUtil::query(int termId, QString taxonomy)
{
    QSqlQuery query;
    query.prepare("SELECT id, term_id, taxonomy, description, parent, count "
                  "FROM term_taxonomy "
                  "WHERE term_id = ? and taxonomy = ?");
    query.addBindValue(termId);
    query.addBindValue(taxonomy);
    query.exec();

    if (query.next())
    {
        TermTaxonomy *termTaxonomy = new TermTaxonomy(
                    query.value(0).toInt(),
                    query.value(1).toInt(),
                    query.value(2).toString(),
                    query.value(3).toString(),
                    query.value(4).toInt(),
                    query.value(5).toInt()
                    );

        return termTaxonomy;
    }

    return NULL;
}

void TableTermTaxonomyUtil::Delete(int termId, QString taxonomy)
{
    QSqlQuery query;
    query.prepare("DELETE FROM term_taxonomy "
                  "WHERE term_id = ? and taxonomy = ?");
    query.addBindValue(termId);
    query.addBindValue(taxonomy);
    query.exec();
}

void TableTermTaxonomyUtil::insertTermTaxonomy(TermTaxonomy *termTaxonomy)
{
    QSqlQuery query;
    query.prepare("insert into term_taxonomy(term_id, taxonomy, description, parent, count) "
                  "values(?, ?, ?, ?, ?)");
    query.addBindValue(termTaxonomy->termId());
    query.addBindValue( QObject::trUtf8(termTaxonomy->taxonomy().toAscii()) );
    query.addBindValue( QObject::trUtf8(termTaxonomy->description().toAscii()) );
    query.addBindValue(termTaxonomy->parentId());
    query.addBindValue(termTaxonomy->count());
    query.exec();
}
