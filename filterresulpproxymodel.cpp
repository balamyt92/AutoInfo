#include "filterresulpproxymodel.h"

FilterResulpProxyModel::FilterResulpProxyModel(QObject *parent)
    :QSortFilterProxyModel(parent)
{

}

QVariant FilterResulpProxyModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::BackgroundRole)
        return QSortFilterProxyModel::data(index, role);

    return QSortFilterProxyModel::data(index, role);
}


