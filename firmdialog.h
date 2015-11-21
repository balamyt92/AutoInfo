#ifndef FIRMDIALOG_H
#define FIRMDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QKeyEvent>
#include <QDataWidgetMapper>
#include "settings.h"

namespace Ui {
class FirmDialog;
}

class FirmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FirmDialog(QWidget *parent = 0);
    ~FirmDialog();
    void setModel(QAbstractItemModel *model);
    void setCurrent(QModelIndex &index);
    void setCurrent(int index);
    void setReadOnly(bool flag);

private:
    Ui::FirmDialog *ui;
    QDataWidgetMapper *mapper;
    Settings *settings;
    QPushButton *canceled;
    QPushButton *okay;
};

#endif // FIRMDIALOG_H
