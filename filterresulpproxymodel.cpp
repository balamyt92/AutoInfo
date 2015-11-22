#include "filterresulpproxymodel.h"

FilterResulpProxyModel::FilterResulpProxyModel(QObject *parent)
    :QSortFilterProxyModel(parent)
{

}

QVariant FilterResulpProxyModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::BackgroundRole)
        return QSortFilterProxyModel::data(index, role);

    QModelIndex sourceIndex = mapToSource(index);
    if(sourceIndex.model()->index(sourceIndex.row(),0).data().toString() == "0")
        return QBrush(Qt::lightGray);

    return QSortFilterProxyModel::data(index, role);
}


