#ifndef DIALOGADD_H
#define DIALOGADD_H

#include <QDialog>
#include <QAbstractButton>
#include <QPushButton>

#include "Service/personservice.h"

namespace Ui {
    class DialogAdd;
}

class DialogAdd : public QDialog
{
        Q_OBJECT

    public:
        explicit DialogAdd(QWidget *parent = 0);
        ~DialogAdd();

    private slots:
        void on_buttonBox_clicked(QAbstractButton *button);

        void on_radioBuiltYes_clicked(bool checked);

        void on_radioBuiltNo_clicked(bool checked);

        void on_btnBoxComputer_clicked(QAbstractButton *button);

    private:
        Ui::DialogAdd *ui;
        PersonService service;
};

#endif // DIALOGADD_H
