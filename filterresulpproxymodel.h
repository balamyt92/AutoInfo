#ifndef FILTERRESULPPROXYMODEL_H
#define FILTERRESULPPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QBrush>
#include <QVariant>
#include <QDebug>

class FilterResulpProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    FilterResulpProxyModel(QObject *parent);
    QVariant data(const QModelIndex &index, int role) const;

};

#endif // FILTERRESULPPROXYMODEL_H
