#ifndef CATEGORIESVIEW_H
#define CATEGORIESVIEW_H

#include <QTreeWidget>

class Term;

class CategoriesView : public QTreeWidget
{
    Q_OBJECT
public:
    explicit CategoriesView(QWidget *parent = 0);
    void addNewItemInCurrentItem(Term &term, QString taxonomy);
    void deleteCurrentItem();
    QString currentTermName();
    int currentTermId();
    QString currentTaxonomy();
    void updateCurrentItem(QString text);
    int termId(const QTreeWidgetItem *item) const;

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

signals:
    void categoryChanged(int termId, QString taxonomy);

public slots:
    void itemSelectionOnChanged();

private:
    QTreeWidgetItem* createRootTreeWidgetItem(QString name, int id, QString taxonomy, QString iconName);
    QTreeWidgetItem* createTreeWidgetItem(QTreeWidgetItem *parent, QString name, int id, QString taxonomy);
    void setTreeWidgetItem(QTreeWidgetItem *item, int id, QString taxonomy, QString iconName);
    void createTreeNodeChildren(QTreeWidgetItem* parent, QString taxonomy);

    QString taxonomy(const QTreeWidgetItem *item) const;

private:
    static const QChar ITEM_DATA_DELIMITER;
};

#endif // CATEGORIESVIEW_H
