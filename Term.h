#ifndef TERM_H
#define TERM_H

#include <QString>

class Term
{
public:
    Term(QString name);
    Term(int id, QString name);

    int id()
    {
        return _id;
    }

    QString name()
    {
        return _name;
    }

private:
    int _id;
    QString _name;

};

#endif // TERM_H
