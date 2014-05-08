#ifndef TABLETERMRELATIONSHIPSUTIL_H
#define TABLETERMRELATIONSHIPSUTIL_H

#include "TermRelationship.h"

class TableTermRelationshipsUtil
{
public:
    TableTermRelationshipsUtil();

    static void createTable();
    static void deleteTable();

    static void Delete(int termTaxonomyId, int imageId);
    static void deleteByTermTaxonomyId(int termTaxonomyId);
    static void deleteObjectAllRelationships(int imageId);

    static void insertTermRelationship(TermRelationship termRelationship);

};

#endif // TABLETERMRELATIONSHIPSUTIL_H
