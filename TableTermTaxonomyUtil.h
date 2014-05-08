#ifndef TABLETERMTAXONOMYUTIL_H
#define TABLETERMTAXONOMYUTIL_H

#include <QString>

class TermTaxonomy;
class TableTermTaxonomyUtil
{
public:
    TableTermTaxonomyUtil();

    static void createTable();
    static void deleteTable();

    static TermTaxonomy* query(int termId, QString taxonomy);
    static void Delete(int termId, QString taxonomy);

    static void insertTermTaxonomy(TermTaxonomy *termTaxonomy);

};

#endif // TABLETERMTAXONOMYUTIL_H
