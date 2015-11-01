#ifndef SERVICEEDITDIALOG_H
#define SERVICEEDITDIALOG_H

#include <QDialog>

namespace Ui {
class ServiceEditDialog;
}

class ServiceEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServiceEditDialog(QWidget *parent = 0);
    ~ServiceEditDialog();

private:
    Ui::ServiceEditDialog   *ui;

};

#endif // SERVICEEDITDIALOG_H
