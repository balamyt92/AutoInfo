#ifndef COLORDELEGATE_H
#define COLORDELEGATE_H

#include <QItemDelegate>
#include <QPainter>
#include <QBrush>


class ColorDelegate : public QItemDelegate
{
public:
    ColorDelegate(QObject *parent = 0) : QItemDelegate(parent)  { }


    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        painter->fillRect(option.rect, QBrush(QColor(245, 245, 245)));
        QItemDelegate::paint(painter, option, index);
    }
};

#endif // COLORDELEGATE_H
