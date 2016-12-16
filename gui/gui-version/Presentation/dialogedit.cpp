#include "dialogedit.h"
#include "ui_dialogedit.h"

DialogEdit::DialogEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEdit)
{
    ui->setupUi(this);
}

DialogEdit::~DialogEdit()
{
    delete ui;
}

void DialogEdit::setData(const Person p) {
    ui->tabAdd->setCurrentIndex(0);
    ui->tabAdd->setTabEnabled(1, false);

    ui->txtName->setText(p.getName());
    ui->txtGender->setText(p.getGender());
    ui->txtBirth->setText(utils::itos(p.getBirthYear()));
    ui->txtDeath->setText(utils::itos(p.getDeathYear()));

    editId = p.getId();

    QStringList notConn;
    QStringList conn;

    QVector<Computer> cList = service.getComputerList();

    for(int i = 0; i < p.getConns().size(); i++) {
        for(int j = 0; j < cList.size(); j++) {
            if(p.getConns()[i] == cList[j].getId()) {
                cList.removeAt(j);
                j--;
            }
        }
    }

    for(int i = 0; i < p.getConns().size(); i++) {
        Computer c;
        c.setId(p.getConns()[i]);
        if(!service.getComputer(c).isValid())
            conn << c.getName();
    }

    for(int i = 0; i < cList.size(); i++)
        notConn << cList[i].getName();

    QStringListModel *notConnModel = new QStringListModel(this);
    QStringListModel *connModel    = new QStringListModel(this);

    notConnModel->setStringList(notConn);
    connModel->setStringList(conn);

    ui->lstConnComps->setModel(connModel);
    ui->lstNotConnComps->setModel(notConnModel);
}

void DialogEdit::setData(const Computer c) {
    // Update input elements
    ui->tabAdd->setCurrentIndex(1);
    ui->tabAdd->setTabEnabled(0, false);

    ui->txtCompName->setText(c.getName());
    ui->txtType->setText(c.getType());
    ui->radioBuiltYes->setChecked(c.getBuilt());
    ui->radioBuiltNo->setChecked(!c.getBuilt());
    if(c.getBuilt())
        ui->txtYearBuilt->setText(utils::itos(c.getYearBuilt()));

    // Store id of computer being edited
    editId = c.getId();

    // Prepare connection lists
    QStringList notConn;
    QStringList conn;

    QVector<Person> pList = service.getPersonList();

    // Remove connected scientists from list of all
    for(int i = 0; i < c.getConns().size(); i++) {
        for(int j = 0; j < pList.size(); j++) {
            if(c.getConns()[i] == pList[j].getId()) {
                pList.removeAt(j);
                j--;
            }
        }
    }

    // Get connected scientists
    for(int i = 0; i < c.getConns().size(); i++) {
        Person p;
        p.setId(c.getConns()[i]);
        if(service.getPerson(p.getId(), p))
            conn << p.getName();
    }

    // Get scientists that aren't connected
    for(int i = 0; i < pList.size(); i++)
        notConn << pList[i].getName();

    // Update lists
    QStringListModel *notConnModel = new QStringListModel(this);
    QStringListModel *connModel    = new QStringListModel(this);

    notConnModel->setStringList(notConn);
    connModel->setStringList(conn);

    ui->lstConnSci->setModel(connModel);
    ui->lstNotConnSci->setModel(notConnModel);
}

void DialogEdit::on_buttonBox_clicked(QAbstractButton *button)
{
    if(button == ui->buttonBox->button(QDialogButtonBox::Cancel)) {
        done(1);
        return;
    }

    Person newInfo;
    newInfo.setName(ui->txtName->text());
    newInfo.setGender(ui->txtGender->text());

    bool bSuccess = false;
    bool dSuccess = false;
    int b = utils::stoi(ui->txtBirth->text(), bSuccess);
    int d = utils::stoi(ui->txtDeath->text(), dSuccess);

    if(!bSuccess || !dSuccess) {
        done(2);
        return;
    }

    newInfo.setBirthYear(b);
    newInfo.setDeathYear(d);
    newInfo.setId(editId);

    QVector<uint> newConns;
    QAbstractItemModel *model = ui->lstConnComps->model();
    for(int i = 0; i < model->rowCount(); i++) {
        QString name = model->data(model->index(i, 0)).toString();
        QVector<Person> pList;
        QVector<Computer> cList;
        service.findSimilar(name, pList, cList);
        if(cList.size())
            newConns.append(cList[0].getId());
    }

    newInfo.setConns(newConns);

    if(service.editPerson(newInfo).isValid()) {
        done(3);
        return;
    }

    done(0);
}

void DialogEdit::on_btnBoxComputer_clicked(QAbstractButton *button)
{
    if(button == ui->btnBoxComputer->button(QDialogButtonBox::Cancel)) {
        done(1);
        return;
    }

    Computer newInfo;
    newInfo.setName(ui->txtCompName->text());
    newInfo.setType(ui->txtType->text());
    newInfo.setBuilt(ui->radioBuiltYes->isChecked());
    newInfo.setId(editId);
    if(newInfo.getBuilt()) {
        bool success = false;
        int y = utils::stoi(ui->txtYearBuilt->text(), success);
        if(!success) {
            done(2);
            return;
        }
        newInfo.setYearBuilt(y);
    }
    else
        newInfo.setYearBuilt(-1);

    QVector<uint> newConns;
    QAbstractItemModel *model = ui->lstConnSci->model();
    for(int i = 0; i < model->rowCount(); i++) {
        QString name = model->data(model->index(i, 0)).toString();
        QVector<Person> pList;
        QVector<Computer> cList;
        service.findSimilar(name, pList, cList);

        if(pList.size())
            newConns.append(pList[0].getId());
    }

    newInfo.setConns(newConns);

    if(service.editComputer(newInfo).isValid()) {
        done(3);
        return;
    }

    done(0);
}

void DialogEdit::on_radioBuiltNo_clicked(bool checked)
{
    if(checked) {
        ui->txtYearBuilt->clear();
        ui->txtYearBuilt->setReadOnly(true);
    }
}

void DialogEdit::on_radioBuiltYes_clicked(bool checked)
{
    if(checked)
        ui->txtYearBuilt->setReadOnly(false);
}

void DialogEdit::on_btnAddSciConn_clicked()
{
    if(!ui->lstNotConnComps->selectionModel()->selectedIndexes().size())
        return;

    QAbstractItemModel *delModel = ui->lstNotConnComps->model();
    QAbstractItemModel *addModel = ui->lstConnComps->model();

    // Move between lists
    int rows = addModel->rowCount();
    addModel->insertRow(rows);
    addModel->setData(addModel->index(rows, 0), ui->lstNotConnComps->selectionModel()->selectedIndexes()[0].data().toString());
    delModel->removeRow(ui->lstNotConnComps->selectionModel()->selectedIndexes()[0].row());

    // Update lists
    ui->lstNotConnComps->setModel(delModel);
    ui->lstConnComps->setModel(addModel);
}

void DialogEdit::on_btnDelSciConn_clicked()
{
    if(!ui->lstConnComps->selectionModel()->selectedIndexes().size())
        return;

    QAbstractItemModel *delModel = ui->lstConnComps->model();
    QAbstractItemModel *addModel = ui->lstNotConnComps->model();

    // Move between lists
    int rows = addModel->rowCount();
    addModel->insertRow(rows);
    addModel->setData(addModel->index(rows, 0), ui->lstConnComps->selectionModel()->selectedIndexes()[0].data().toString());
    delModel->removeRow(ui->lstConnComps->selectionModel()->selectedIndexes()[0].row());

    // Update lists
    ui->lstNotConnComps->setModel(addModel);
    ui->lstConnComps->setModel(delModel);
}

void DialogEdit::on_btnAddConn_clicked()
{
    if(!ui->lstNotConnSci->selectionModel()->selectedIndexes().size())
        return;

    QAbstractItemModel *delModel = ui->lstNotConnSci->model();
    QAbstractItemModel *addModel = ui->lstConnSci->model();

    // Move between lists
    int rows = addModel->rowCount();
    addModel->insertRow(rows);
    addModel->setData(addModel->index(rows, 0), ui->lstNotConnSci->selectionModel()->selectedIndexes()[0].data().toString());
    delModel->removeRow(ui->lstNotConnSci->selectionModel()->selectedIndexes()[0].row());

    // Update lists
    ui->lstNotConnSci->setModel(delModel);
    ui->lstConnSci->setModel(addModel);
}

void DialogEdit::on_btnDelConn_clicked()
{
    if(!ui->lstConnSci->selectionModel()->selectedIndexes().size())
        return;

    QAbstractItemModel *delModel = ui->lstConnSci->model();
    QAbstractItemModel *addModel = ui->lstNotConnSci->model();

    // Move between lists
    int rows = addModel->rowCount();
    addModel->insertRow(rows);
    addModel->setData(addModel->index(rows, 0), ui->lstConnSci->selectionModel()->selectedIndexes()[0].data().toString());
    delModel->removeRow(ui->lstConnSci->selectionModel()->selectedIndexes()[0].row());

    // Update lists
    ui->lstNotConnSci->setModel(addModel);
    ui->lstConnSci->setModel(delModel);
}
