#include "firmproxymodel.h"

FirmProxyModel::FirmProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

QVariant FirmProxyModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::BackgroundRole)
        return QSortFilterProxyModel::data(index, role);

    QModelIndex sourceIndex = mapToSource(index);
    if(sourceIndex.model()->index(sourceIndex.row(),5).data().toString() == "0")
        return QBrush(Qt::gray);

    return QSortFilterProxyModel::data(index, role);
}

