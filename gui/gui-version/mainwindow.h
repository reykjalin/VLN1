#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>

#include "Service/personservice.h"
#include "dialogadd.h"
#include "dialogedit.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_actionAdd_2_triggered();

        void on_btnPrsnSrch_clicked();

        void on_lstPrsn_clicked(const QModelIndex &index);

        void on_btnCmpSrch_clicked();

        void on_lstCmp_clicked(const QModelIndex &index);

        void on_btnEditScientist_clicked();

        void on_btnEditComputer_clicked();

        void on_dropDownSort_currentIndexChanged(int index);

    private:
        Ui::MainWindow *ui;
        PersonService service;

        Person getSelectedPerson(const QModelIndex &index);
        Computer getSelectedComputer(const QModelIndex &index);

        void updateLists();
};

#endif // MAINWINDOW_H
