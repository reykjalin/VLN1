#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    // Return immediately if an error occurred
    if(add.exec())
        return;

    updateLists();
}

void MainWindow::updateLists() {
    on_btnPrsnSrch_clicked();
    on_btnCmpSrch_clicked();
}

void MainWindow::on_btnPrsnSrch_clicked()
{
    QVector<Person> pList;
    QVector<Computer> cList;
    service.findSimilar(ui->txtPrsnSrch->text(), pList, cList);

    QStringListModel *model = new QStringListModel(this);
    QStringList list;
    for(int i = 0; i < pList.length(); i++) {
        list << pList[i].getName();
    }
    model->setStringList(list);
    ui->lstPrsn->setModel(model);
}

void MainWindow::on_lstPrsn_clicked(const QModelIndex &index)
{
    QVector<Person> pList;
    QVector<Computer> cList;
    service.findSimilar(index.data().toString(), pList, cList);

    if(pList.size()){
        ui->txtScientistDetails->clear();
        QString txt = pList[0].getName() + "\n";
        txt += "Born in the year " + utils::itos(pList[0].getBirthYear());
        if(pList[0].getDeathYear() >= 0)
            txt +=  ", died in the year " + utils::itos(pList[0].getDeathYear()) + ".\n\n";
        else
            txt += ", still alive.\n\n";
        txt += "Worked on the following computers:\n";
        for(int i = 0; i < pList[0].getConns().size(); i++) {
            Computer c;
            c.setId(pList[0].getConns()[i]);
            service.getComputer(c);
            txt += c.getName() + "\n";
        }
        ui->txtScientistDetails->setPlainText(txt);
    }
}

void MainWindow::on_btnCmpSrch_clicked()
{
    QVector<Computer> cList;
    QVector<Person> pList;
    service.findSimilar(ui->txtCmpSrch->text(), pList, cList);

    QStringListModel *model = new QStringListModel(this);
    QStringList list;
    for(int i = 0; i < cList.size(); i++) {
        list << cList[i].getName();
    }
    model->setStringList(list);
    ui->lstCmp->setModel(model);
}

void MainWindow::on_lstCmp_clicked(const QModelIndex &index)
{
    QVector<Computer> cList;
    QVector<Person> pList;
    service.findSimilar(index.data().toString(), pList, cList);

    if(cList.size()) {
        ui->txtComputerDetails->clear();
        QString txt = cList[0].getName() + "\n";
        txt += "A " + cList[0].getType() + " computer that ";
        if(cList[0].getBuilt())
            txt += "was built in the year " + utils::itos(cList[0].getYearBuilt()) + ".\n\n";
        else
            txt += "hasn't been built.\n\n";
        txt += "Worked on by the following scientists:\n";
        for(int i = 0; i < cList[0].getConns().size(); i++) {
            Person p;
            p.setId(cList[0].getConns()[i]);
            service.getPerson(p.getId(), p);
            txt += p.getName() + "\n";
        }
        ui->txtComputerDetails->setPlainText(txt);
    }
}

void MainWindow::on_btnEditScientist_clicked()
{
    if(!ui->lstPrsn->selectionModel()->selectedIndexes().size())
        return;
    Person toEdit = getSelectedPerson(ui->lstPrsn->selectionModel()->selectedIndexes().at(0));
    DialogEdit edit;
    edit.setData(toEdit);
    edit.exec();

    updateLists();
    ui->txtScientistDetails->clear();
}

Person MainWindow::getSelectedPerson(const QModelIndex &index) {
    QVector<Person> pList;
    QVector<Computer> cList;
    service.findSimilar(index.data().toString(), pList, cList);
    if(pList.size())
        return pList[0];
    return Person();
}

Computer MainWindow::getSelectedComputer(const QModelIndex &index) {
    QVector<Person> pList;
    QVector<Computer> cList;
    service.findSimilar(index.data().toString(), pList, cList);
    if(cList.size())
        return cList[0];
    return Computer();
}

void MainWindow::on_btnEditComputer_clicked()
{
    if(!ui->lstCmp->selectionModel()->selectedIndexes().size())
        return;
    Computer toEdit = getSelectedComputer(ui->lstCmp->selectionModel()->selectedIndexes().at(0));
    DialogEdit edit;
    edit.setData(toEdit);
    edit.exec();

    updateLists();
    ui->txtComputerDetails->clear();
}
