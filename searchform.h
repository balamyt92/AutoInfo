#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <QWidget>
#include "settings.h"
#include <QKeyEvent>

namespace Ui {
class searchForm;
}

class SearchForm : public QWidget
{
    Q_OBJECT

public:
    explicit SearchForm(QWidget *parent = 0);
    ~SearchForm();

private slots:
    void on_pushButton_clicked();

private:
    Ui::searchForm *ui;
    Settings        *settings;

protected:
    void keyPressEvent(QKeyEvent *e);
};

#endif // SEARCHFORM_H
