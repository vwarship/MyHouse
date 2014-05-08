#include "Term.h"

Term::Term(QString name)
    : _name(name)
{
}

Term::Term(int id, QString name)
    : _id(id), _name(name)
{
}
