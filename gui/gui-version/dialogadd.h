#ifndef DIALOGADD_H
#define DIALOGADD_H

#include <QDialog>
#include <Service/personservice.h>

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

    private:
        Ui::DialogAdd *ui;
        PersonService service;
};

#endif // DIALOGADD_H
