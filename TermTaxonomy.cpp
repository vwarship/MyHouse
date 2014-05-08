#include "TermTaxonomy.h"

TermTaxonomy::TermTaxonomy(QObject *parent)
    : QObject(parent)
{
}

TermTaxonomy::TermTaxonomy(int termId, QString taxonomy, QString description, int parentId)
    : _id(0), _termId(termId), _taxonomy(taxonomy), _description(description), _parentId(parentId), _count(0)
{
}

TermTaxonomy::TermTaxonomy(int id, int termId, QString taxonomy, QString description, int parentId, int count)
    : _id(id), _termId(termId), _taxonomy(taxonomy), _description(description), _parentId(parentId), _count(count)
{
}

int TermTaxonomy::id()
{
    return _id;
}

int TermTaxonomy::termId()
{
    return _termId;
}

QString TermTaxonomy::taxonomy()
{
    return _taxonomy;
}

QString TermTaxonomy::description()
{
    return _description;
}

int TermTaxonomy::parentId()
{
    return _parentId;
}

int TermTaxonomy::count()
{
    return _count;
}
