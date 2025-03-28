#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <lmdb.h>
#include <chrono>
#include <thread>
#include "json.hpp"
#include <ncurses.h>

using json = nlohmann::json;
using namespace std;
using namespace chrono;

void sesionIniciada()
{
    mvprintw(1, 10, "hi");
    refresh();
    getch();
}

void createDataBase()
{
    MDB_env *env; // Entorno de la base de datos
    MDB_dbi dbi;  // Base de datos
    MDB_txn *txn;

    // Crear el entorno de la base de datos
    mdb_env_create(&env);
    mdb_env_set_mapsize(env, 10485760);    // Establecer el tamaño máximo del entorno: 10 MB
    mdb_env_open(env, "./users", 0, 0664); // Abrir el entorno de la base de datos
    mdb_env_close(env);
}

void insertData(const string &name, const string &lastName, const string &password)
{
    static int userIdCounter = 0; // Contador para user_id
    MDB_txn *txn;
    MDB_env *env; // Entorno de la base de datos
    MDB_dbi dbi;  // Base de datos
    MDB_val key, data;

    // Crear el entorno de la base de datos
    mdb_env_create(&env);
    mdb_env_open(env, "./users", 0, 0664); // Abrir el entorno de la base de datos
    mdb_txn_begin(env, nullptr, 0, &txn);
    mdb_dbi_open(txn, nullptr, 0, &dbi);

    // Incrementar el user_id
    userIdCounter++;

    string userId = to_string(userIdCounter);

    json user;
    user["user_id"] = userId;
    user["name"] = name;
    user["lastName"] = lastName;
    user["password"] = password; // Formato: name,lastName,password
    user["user_auth"] = "user";  // Formato: name,lastName,password

    string userData = user.dump();

    key.mv_size = userId.size();
    key.mv_data = (void *)userId.data();
    data.mv_size = userData.size();
    data.mv_data = (void *)userData.data();

    mdb_put(txn, dbi, &key, &data, 0); // Insertar el registro en la base de datos
    mdb_txn_commit(txn);               // Confirmar la transacción

    mdb_dbi_close(env, dbi);
    mdb_env_close(env);

    clear();
    printw("hi");
    refresh();
    getch();
}

// Función para validar usuario y contraseña
bool requestData(const string &username, const string &password)
{
    MDB_txn *txn;
    MDB_val key, data;
    int rc;

    // Iniciar una transacción
    rc = mdb_txn_begin(env, nullptr, MDB_RDONLY, &txn);
    if (rc != MDB_SUCCESS)
    {
        cerr << "Error beginning transaction: " << mdb_strerror(rc) << endl;
        return false;
    }

    // Crear la clave para buscar el usuario
    // En este caso, asumimos que el username es el userId
    key.mv_size = username.size();
    key.mv_data = (void *)username.data();

    // Buscar el registro en la base de datos
    rc = mdb_get(txn, dbi, &key, &data);
    if (rc != MDB_SUCCESS)
    {
        cerr << "Error getting data: " << mdb_strerror(rc) << endl;
        mdb_txn_abort(txn);
        return false; // Usuario no encontrado
    }

    // Convertir el valor recuperado a una cadena
    string userData(static_cast<char *>(data.mv_data), data.mv_size);

    // Parsear el JSON
    json user = json::parse(userData);

    // Extraer la contraseña almacenada
    string storedPassword = user["password"];

    // Comparar la contraseña ingresada con la almacenada
    bool isValid = (storedPassword == password);

    // Confirmar la transacción
    mdb_txn_commit(txn);

    return isValid;
}

// Función para simular una pantalla de carga
void loadingScreen()
{
    mvprintw(1, 10, "Cargando sistema");
    for (int i = 0; i < 5; i++)
    {
        mvprintw(1, 26 + i, ".");
        refresh(); // Actualizar la pantalla
        this_thread::sleep_for(milliseconds(500));
    }
}

void shutDownScreen(int linea)
{
    mvprintw(linea + 2, 10, "Cerrando sistema");
    for (int i = 0; i < 5; i++)
    {
        mvprintw(linea + 2, 26 + i, ".");
        refresh(); // Actualizar la pantalla
        this_thread::sleep_for(milliseconds(500));
    }
}

// Función para dar la bienvenida y opciones de inicio
void mostrarBienvenida()
{
    clear(); // Limpiar la pantalla
    mvprintw(1, 10, "==============================");
    mvprintw(2, 10, "  BIENVENIDO AL SISTEMA MEDICO  ");
    mvprintw(3, 10, "==============================");
    mvprintw(5, 10, "Por favor, seleccione una opción:");
    refresh(); // Actualizar la pantalla
}

void readData(string &data, int line, int space, char ch, bool isPassword)
{

    if (ch == KEY_BACKSPACE || ch == 7)
    { // Retroceso
        if (!data.empty())
        {
            data.pop_back();                          // Eliminar el último carácter
            mvprintw(line, space + data.size(), " "); // Limpiar el último carácter
            move(line, space + data.size());          // Mover el cursor a la posición correcta
        }
    }
    else
    {
        data += ch; // Agregar el carácter a la cadena
        if (isPassword)
        {
            mvprintw(line, space + data.size() - 1, "*"); // Mostrar asterisco en lugar del carácter
        }
        else
        {
            mvprintw(line, space + data.size() - 1, "%c", ch); // Mostrar el carácter en la pantalla
        }
    }
    refresh();
}

void iniciarSesion()
{
    clear();
    mvprintw(1, 10, "==============================");
    mvprintw(2, 10, "      INICIO DE SESION          ");
    mvprintw(3, 10, "==============================");
    char ch;

    // Solicitar name de usuario
    mvprintw(6, 10, "Ingrese su name de usuario: ");
    string username; // Usar string para el name de usuario
    noecho();        // Mostrar la entrada del usuario
    while ((ch = getch()) != '\n' && ch != '\r')
    { // Leer hasta Enter
        readData(username, 6, 10 + 30, ch, false);
    }

    // Solicitar contraseña
    mvprintw(7, 10, "Ingrese su contraseña: ");
    string password; // Usar string para la contraseña
    noecho();        // Ocultar la entrada del usuario
    while ((ch = getch()) != '\n' && ch != '\r')
    { // Leer hasta Enter
        readData(password, 7, 10 + 24, ch, true);
    }

    // Verificar si los campos están vacíos
    if (username.empty() || password.empty())
    {
        mvprintw(9, 10, "Error: Todos los campos son obligatorios. Presione cualquier tecla para reiniciar.");
        refresh();
        getch();         // Esperar a que el usuario presione una tecla
        iniciarSesion(); // Reiniciar el bucle para volver a solicitar los datos
    }

    noecho(); // Ocultar la entrada del usuario nuevamente
    refresh();

    if (requestData(username1, password1))
    {
        sesionIniciada();
    }
    else
    {
        mvprintw(9, 10, "Error: usuario o contraseña incorrectos.");
        refresh();
        getch();             // Esperar a que el usuario presione una tecla
        mostrarBienvenida(); // Volver a mostrar la bienvenida
    }
}

void crearUsuario()
{
    clear();
    mvprintw(1, 10, "==============================");
    mvprintw(2, 10, "        CREAR USUARIO           ");
    mvprintw(3, 10, "==============================");
    char ch;

    // Solicitar name
    mvprintw(6, 10, "Ingrese su nombre: ");
    string name; // Usar string para el name de usuario
    noecho();    // Mostrar la entrada del usuario
    while ((ch = getch()) != '\n' && ch != '\r')
    { // Leer hasta Enter
        readData(name, 6, 10 + 19, ch, false);
    }

    // Solicitar lastName
    mvprintw(7, 10, "Ingrese su apellido: ");
    string lastName; // Usar string para el lastName de usuario
    noecho();        // Mostrar la entrada del usuario
    while ((ch = getch()) != '\n' && ch != '\r')
    { // Leer hasta Enter
        readData(lastName, 7, 10 + 21, ch, false);
    }

    // Solicitar contraseña
    mvprintw(8, 10, "Ingrese su contraseña: ");
    string password; // Usar string para la contraseña
    noecho();        // Ocultar la entrada del usuario
    while ((ch = getch()) != '\n' && ch != '\r')
    { // Leer hasta Enter
        readData(password, 8, 10 + 24, ch, true);
    }

    // Verificar si los campos están vacíos
    if (name.empty() || lastName.empty() || password.empty())
    {
        mvprintw(10, 10, "Error: Todos los campos son obligatorios. Presione cualquier tecla para reiniciar.");
        refresh();
        getch();        // Esperar a que el usuario presione una tecla
        crearUsuario(); // Reiniciar el bucle para volver a solicitar los datos
    }

    noecho(); // Ocultar la entrada del usuario nuevamente
    refresh();

    string name1 = "san", lastName1 = "san", password1 = "san";

    insertData(name1, lastName1, password1);

    iniciarSesion(); // Volver a mostrar la bienvenida
}

void mostrarMenu()
{
    const char *opciones[] = {
        "Iniciar Sesión",
        "Crear Usuario",
        "Salir"};
    int numOpciones = sizeof(opciones) / sizeof(opciones[0]);
    int seleccion = 0;
    int ch;

    while (true)
    {
        // Mostrar el menú en una posición diferente
        for (int i = 0; i < numOpciones; ++i)
        {
            if (i == seleccion)
            {
                mvprintw(7 + i, 10, "> %s <", opciones[i]); // Imprimir opción seleccionada con flechas
            }
            else
            {
                mvprintw(7 + i, 10, "  %s  ", opciones[i]); // Imprimir opción sin flechas
            }
        }

        ch = getch(); // Esperar la entrada del usuario

        switch (ch)
        {
        case KEY_UP:
            seleccion = (seleccion - 1 + numOpciones) % numOpciones; // Mover hacia arriba
            break;
        case KEY_DOWN:
            seleccion = (seleccion + 1) % numOpciones; // Mover hacia abajo
            break;
        case 10: // Enter
            if (seleccion == 0)
            { // Iniciar Sesión
                iniciarSesion();
            }
            else if (seleccion == 1)
            { // Crear Usuario
                crearUsuario();
            }
            else if (seleccion == 2)
            { // Salir
                shutDownScreen(9);
                return;
            }
            break;
        }
        refresh(); // Actualizar la pantalla después de cada entrada
    }
}

int main()
{
    initscr();            // Iniciar ncurses
    keypad(stdscr, TRUE); // Habilitar las teclas especiales
    noecho();             // No mostrar la entrada del usuario
    curs_set(0);          // Ocultar el cursor

    createDataBase();

    loadingScreen();
    mostrarBienvenida(); // Mostrar el mensaje de bienvenida
    mostrarMenu();       // Mostrar el menú

    endwin(); // Finalizar ncurses
    return 0;
}