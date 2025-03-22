#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <LMDBWorker.h>

class MyWindow : public QWidget {
    Q_OBJECT

public:
    MyWindow(QWidget *parent = nullptr);
    ~MyWindow();
    
private slots:
    void loadData();
    void showCreateUserWindow();
    void createUser();
    void login();
    
private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QLabel *label;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    LMDBWorker *lmdbWorker;
};

#endif // MYWINDOW_H

