#ifndef DIALOGEDIT_H
#define DIALOGEDIT_H

#include <QDialog>
#include <QAbstractButton>
#include <QPushButton>
#include <QAbstractItemModel>
#include <QStringListModel>

#include "Service/personservice.h"

namespace Ui {
    class DialogEdit;
}

class DialogEdit : public QDialog
{
        Q_OBJECT

    public:
        explicit DialogEdit(QWidget *parent = 0);
        ~DialogEdit();

        void setData(const Computer c);
        void setData(const Person p);

    private slots:
        void on_buttonBox_clicked(QAbstractButton *button);

        void on_btnBoxComputer_clicked(QAbstractButton *button);

        void on_radioBuiltNo_clicked(bool checked);

        void on_radioBuiltYes_clicked(bool checked);

        void on_btnAddSciConn_clicked();

        void on_btnDelSciConn_clicked();

        void on_btnAddConn_clicked();

        void on_btnDelConn_clicked();

    private:
        Ui::DialogEdit *ui;
        PersonService service;
        int editId;
};

#endif // DIALOGEDIT_H
