#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    QString getEmployeName();
    void getemployeemail(QString &email);
     int getEmail() const;




private:
    Ui::Dialog *ui;
    void connecttoapp();
    void on_loginButton_clicked();





};

#endif // DIALOG_H

