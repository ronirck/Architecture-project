#include "MyWindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMainWindow>

MyWindow::MyWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    label = new QLabel("Por favor, ingresa tu nombre de usuario y contraseña:", this);
    layout->addWidget(label);

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Nombre de usuario");
    layout->addWidget(usernameInput);

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Contraseña");
    passwordInput->setEchoMode(QLineEdit::Password);
    layout->addWidget(passwordInput);

    QPushButton *createUserButton = new QPushButton("Crear Usuario", this);
    layout->addWidget(createUserButton);

    QPushButton *loginButton = new QPushButton("Iniciar Sesión", this);
    layout->addWidget(loginButton);

    connect(createUserButton, &QPushButton::clicked, this, &MyWindow::createUser);
    connect(loginButton, &QPushButton::clicked, this, &MyWindow::loadData);

    setLayout(layout);
    setWindowTitle("Crear Usuario - Qt y LMDB");
    resize(400, 300);

    lmdbWorker = new LMDBWorker();
}

MyWindow::~MyWindow() {
    delete lmdbWorker;
}

void MyWindow::createUser() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        label->setText("Por favor, ingresa ambos campos.");
        return;
    }

    // Guardar el usuario en la base de datos LMDB
    lmdbWorker->saveUserData(username, password);

    // Notificar al usuario que el usuario fue creado
    label->setText("Usuario creado exitosamente.");
    usernameInput->clear();
    passwordInput->clear();
}

void MyWindow::login() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Por favor, ingrese usuario y contraseña.");
        return;
    }

    QString storedPassword = lmdbWorker->readData(username);

    if (storedPassword.isEmpty()) {
        QMessageBox::warning(this, "Error", "El usuario no existe.");
    } else if (storedPassword == password) {
        // Iniciar sesión exitoso
        label->setText("Bienvenido, " + username + "!");
        // Aquí puedes llamar a tu ventana principal después de iniciar sesión
    } else {
        QMessageBox::warning(this, "Error", "Contraseña incorrecta.");
    }
}

void MyWindow::showCreateUserWindow() {
    // Crear ventana de creación de usuario
    QWidget *createUserWindow = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(createUserWindow);

    QLineEdit *newUsernameLineEdit = new QLineEdit(createUserWindow);
    newUsernameLineEdit->setPlaceholderText("Nuevo Usuario");
    layout->addWidget(newUsernameLineEdit);

    QLineEdit *newPasswordLineEdit = new QLineEdit(createUserWindow);
    newPasswordLineEdit->setPlaceholderText("Nueva Contraseña");
    newPasswordLineEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(newPasswordLineEdit);

    QPushButton *createButton = new QPushButton("Crear Usuario", createUserWindow);
    layout->addWidget(createButton);

    connect(createButton, &QPushButton::clicked, [this, newUsernameLineEdit, newPasswordLineEdit]() {
        QString newUsername = newUsernameLineEdit->text();
        QString newPassword = newPasswordLineEdit->text();

        if (newUsername.isEmpty() || newPassword.isEmpty()) {
            QMessageBox::warning(this, "Error", "Por favor, ingrese un nombre de usuario y una contraseña.");
            return;
        }

        lmdbWorker->writeData(newUsername, newPassword);  // Almacenar en LMDB
        QMessageBox::information(this, "Éxito", "Usuario creado exitosamente.");
    });

    createUserWindow->setLayout(layout);
    createUserWindow->setWindowTitle("Crear Usuario");
    createUserWindow->resize(300, 200);
    createUserWindow->show();
}

void MyWindow::loadData() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();    

    // Lógica para verificar si el usuario existe y la contraseña es correcta
    QString storedPassword = lmdbWorker->readData(username);
    
    if (storedPassword == password) {
        label->setText("Inicio de sesión exitoso.");
    } else {
        label->setText("Nombre de usuario o contraseña incorrectos.");
    }
}

