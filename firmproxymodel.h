#ifndef FIRMPROXYMODEL_H
#define FIRMPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QBrush>
#include <QVariant>


class FirmProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    FirmProxyModel(QObject *parent = 0);

protected:
    QVariant data(const QModelIndex &index, int role) const;

};

#endif // FIRMPROXYMODEL_H
