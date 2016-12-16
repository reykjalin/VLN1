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
    ui->tabAdd->setCurrentIndex(1);
    ui->tabAdd->setTabEnabled(0, false);

    ui->txtCompName->setText(c.getName());
    ui->txtType->setText(c.getType());
    ui->radioBuiltYes->setChecked(c.getBuilt());
    ui->radioBuiltNo->setChecked(!c.getBuilt());
    if(c.getBuilt())
        ui->txtYearBuilt->setText(utils::itos(c.getYearBuilt()));

    editId = c.getId();
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
