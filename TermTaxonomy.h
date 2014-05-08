#ifndef TERMTAXONOMY_H
#define TERMTAXONOMY_H

#include <QString>
#include <QObject>

class TermTaxonomy : public QObject
{
    Q_OBJECT
public:
    explicit TermTaxonomy(QObject *parent = 0);
    TermTaxonomy(int termId, QString taxonomy, QString description, int parentId);
    TermTaxonomy(int id, int termId, QString taxonomy, QString description, int parentId, int count);

    int id();
    int termId();
    QString taxonomy();
    QString description();
    int parentId();
    int count();

private:
    int _id;
    int _termId;
    QString _taxonomy;
    QString _description;
    int _parentId;
    int _count;

};

#endif // TERMTAXONOMY_H
