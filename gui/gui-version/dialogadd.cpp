#include "dialogadd.h"
#include "ui_dialogadd.h"

DialogAdd::DialogAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAdd)
{
    ui->setupUi(this);
}

DialogAdd::~DialogAdd()
{
    delete ui;
}

void DialogAdd::on_buttonBox_clicked(QAbstractButton *button)
{
    // Return 1 if cancel button is pressed
    if(button == ui->buttonBox->button(QDialogButtonBox::Cancel)) {
        done(1);
        return;
    }

    Person p;
    p.setName(ui->txtName->text());
    p.setGender(ui->txtGender->text());

    bool bSuccess = false;
    bool dSuccess = false;
    int b = utils::stoi(ui->txtBirth->text(), bSuccess);
    int d = utils::stoi(ui->txtDeath->text(), dSuccess);

    // Return 2 on conversion error
    if(!bSuccess || !dSuccess) {
        done(2);
        return;
    }

    p.setBirthYear(b);
    p.setDeathYear(d);

    // Return 3 on add to DB error
    if(!service.addPerson(p)) {
        done(3);
        return;
    }

    done(0);
}

void DialogAdd::on_radioBuiltYes_clicked(bool checked)
{
    if(checked)
        ui->txtYearBuilt->setReadOnly(false);
}

void DialogAdd::on_radioBuiltNo_clicked(bool checked)
{
   if(checked) {
       ui->txtYearBuilt->setReadOnly(true);
       ui->txtYearBuilt->clear();
   }
}

void DialogAdd::on_btnBoxComputer_clicked(QAbstractButton *button)
{
    // Return 1 if cancel button is pressed
    if(button == ui->btnBoxComputer->button(QDialogButtonBox::Cancel)) {
        done(1);
        return;
    }

    Computer c;
    c.setName(ui->txtCompName->text());
    c.setType(ui->txtType->text());

    c.setBuilt(ui->radioBuiltYes->isChecked());

    if(c.getBuilt()) {
        bool success = false;
        uint y = utils::stoi(ui->txtYearBuilt->text(), success);
        // Return 2 on conversion error
        if(!success) {
            done(2);
            return;
        }
        c.setYearBuilt(y);
    }
    else
        c.setYearBuilt(-1);

    // Return 3 on add to DB error
    if(service.addComputer(c).isValid()) {
        done(3);
        return;
    }

    done(0);
}
