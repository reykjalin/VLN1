#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogadd.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(service.startService().isValid()) {
        exit(1);
    }

    QStringListModel *model = new QStringListModel(this);
    QStringList list;
    QVector<Person> pList = service.getPersonList();
    for(int i = 0; i < pList.size(); i++) {
        list << pList[i].getName();
    }
    model->setStringList(list);
    ui->lstPrsn->setModel(model);


    QStringListModel *cModel = new QStringListModel(this);
    list.clear();
    QVector<Computer> cList = service.getComputerList();
    for(int i = 0; i < cList.size(); i++) {
        list << cList[i].getName();
    }
    cModel->setStringList(list);
    ui->lstCmp->setModel(cModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAdd_2_triggered()
{
    DialogAdd add;
    add.exec();
}
