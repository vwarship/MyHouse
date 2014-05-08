#ifndef TERMRELATIONSHIP_H
#define TERMRELATIONSHIP_H

class TermRelationship
{
public:
    TermRelationship(int objectId, int termTaxonomyId);

    int objectId()
    {
        return _objectId;
    }

    int termTaxonomyId()
    {
        return _termTaxonomyId;
    }

private:
    int _objectId;
    int _termTaxonomyId;

};

#endif // TERMRELATIONSHIP_H
