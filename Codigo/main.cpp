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

// FUNCIONES PARA LA BASE DE DATOS
void createDataBase();
void insertData(const string &name, const string &lastName, const string &password);
bool requestData(const string &username, const string &password);

// FUNCIONES DE EXPERIENCIA DE INTERFAZ
void loadingScreen();
void shutDownScreen();
int mostrarMenu(vector<string> opciones, int inicio);
void mostrarBienvenida();

// FUNCIONES PARA EL INICIO DE SESION
void readData(string &data, int line, int space, char ch, bool isPassword);
void iniciarSesion();
void crearUsuario();
void mostrarMenuPrincipal();

// FUNCIONES DEL USUARIO
void mostrarDatosSistema();
void mostrarDatosMedicos();

void drawBox(int y, int x, int height, int width)
{
    mvhline(y, x, ACS_HLINE, width);
    mvhline(y + height - 1, x, ACS_HLINE, width);
    mvvline(y, x, ACS_VLINE, height);
    mvvline(y, x + width - 1, ACS_VLINE, height);
    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(y, x + width - 1, ACS_URCORNER);
    mvaddch(y + height - 1, x, ACS_LLCORNER);
    mvaddch(y + height - 1, x + width - 1, ACS_LRCORNER);
}

int main()
{
    initscr();            // Iniciar ncurses
    keypad(stdscr, TRUE); // Habilitar las teclas especiales
    noecho();             // No mostrar la entrada del usuario
    curs_set(0);          // Ocultar el cursor

    // createDataBase();

    vector<string> opciones = {"Iniciar Sesion", "Crear Usuario", "Salir"};
    bool flag = true;
    loadingScreen();

    while (flag)
    {

        mostrarBienvenida(); // Mostrar el mensaje de bienvenida
        mostrarMenuPrincipal();
        int opcion = mostrarMenu(opciones, 5); // Mostrar el menú

        switch (opcion)
        {
        case 0:
            iniciarSesion();
            break;
        case 1:
            crearUsuario();
            break;
        case 2:
            flag = false;
            break;
        }
    }
    shutDownScreen();
    endwin(); // Finalizar ncurses
    return 0;
}

void sesionIniciada()
{
    clear(); // Limpiar la pantalla
    drawBox(1, 10, 5, 25);
    mvprintw(2, 15, "MENU DE USUARIO");
    mvprintw(4, 15, "Hola, Usuario!");
    refresh(); // Actualizar la pantalla

    vector<string> opciones = {"Vista de Informacion Medica", "Vista de Informacion del Sistema", "Cerrar Sesion"};

    int opcion = mostrarMenu(opciones, 7);

    switch (opcion)
    {
    case 0:
        mostrarDatosMedicos();
        sesionIniciada();
        break;
    case 1:
        mostrarDatosSistema();
        sesionIniciada();
        break;
    case 2:
        clear();
        break;
    }
}

void mostrarDatosMedicos()
{
    clear(); // Limpiar la pantalla
    drawBox(1, 10, 3, 28);
    mvprintw(2, 15, "INFORMACION MEDICA");
    refresh(); // Actualizar la pantalla

    vector<string> opciones = {
        "Pacientes",
        "Admisiones por Año",
        "Enfermedades por Año",
        "Enfermedades por Edad",
        "Medicamento más Utilizados",
        "Volver al Menu"};

    int opcion;
    bool flag = true;

    while (flag)
    {
        opcion = mostrarMenu(opciones, 4);

        switch (opcion)
        {
        case 0:
            mvprintw(12, 10, "Pacientes");
            refresh();
            getch();
            break;
        case 1:
            mvprintw(12, 10, "Admisiones por Año");
            refresh();
            getch();
            break;
        case 2:
            mvprintw(12, 10, "Enfermedades por Año");
            refresh();
            getch();
            break;
        case 3:
            mvprintw(12, 10, "Enfermedades por Edad");
            refresh();
            getch();
            break;
        case 4:
            mvprintw(12, 10, "Medicamento más Utilizados");
            refresh();
            getch();
            break;
        case 5:
            flag = false;
            clear();
            break;
        }

        move(12, 10); // Mover el cursor a la línea 2, columna 10
        clrtoeol();
    }
}

void mostrarDatosSistema()
{
    clear(); // Limpiar la pantalla
    drawBox(1, 10, 3, 28);
    mvprintw(2, 15, "INFORMACION SISTEMA");
    refresh(); // Actualizar la pantalla

    vector<string> opciones = {
        "Tiempo de Respuesta del Servidor",
        "Tiempo de Ejecución de las Funciones de Consulta",
        "Recursos Usados",
        "Memoria de la Base de Datos",
        "Volver al Menu"};

    int opcion;
    bool flag = true;

    while (flag)
    {
        opcion = mostrarMenu(opciones, 4);

        switch (opcion)
        {
        case 0:
            mvprintw(12, 10, "Tiempo de Respuesta del Servidor");
            refresh();
            getch();
            break;
        case 1:
            mvprintw(12, 10, "Tiempo de Ejecución de las Funciones de Consulta");
            refresh();
            getch();
            break;
        case 2:
            mvprintw(12, 10, "Recursos Usados");
            refresh();
            getch();
            break;
        case 3:
            mvprintw(12, 10, "Memoria de la Base de Datos");
            refresh();
            getch();
            break;
        case 4:
            flag = false;
            clear();
            break;
        }

        move(12, 10); // Mover el cursor a la línea 2, columna 10
        clrtoeol();
    }
}

// void createDataBase()
// {
//     MDB_env *env; // Entorno de la base de datos
//     MDB_dbi dbi;  // Base de datos
//     MDB_txn *txn;

//     // Crear el entorno de la base de datos
//     mdb_env_create(&env);
//     mdb_env_set_mapsize(env, 10485760);    // Establecer el tamaño máximo del entorno: 10 MB
//     mdb_env_open(env, "./users", 0, 0664); // Abrir el entorno de la base de datos
//     mdb_env_close(env);
// }

// void insertData(const string &name, const string &lastName, const string &password)
// {
//     static int userIdCounter = 0; // Contador para user_id
//     MDB_txn *txn;
//     MDB_env *env; // Entorno de la base de datos
//     MDB_dbi dbi;  // Base de datos
//     MDB_val key, data;

//     // Crear el entorno de la base de datos
//     mdb_env_create(&env);
//     mdb_env_open(env, "./users", 0, 0664); // Abrir el entorno de la base de datos
//     mdb_txn_begin(env, nullptr, 0, &txn);
//     mdb_dbi_open(txn, nullptr, 0, &dbi);

//     // Incrementar el user_id
//     userIdCounter++;

//     string userId = to_string(userIdCounter);

//     json user;
//     user["user_id"] = userId;
//     user["name"] = name;
//     user["lastName"] = lastName;
//     user["password"] = password; // Formato: name,lastName,password
//     user["user_auth"] = "user";  // Formato: name,lastName,password

//     string userData = user.dump();

//     key.mv_size = userId.size();
//     key.mv_data = (void *)userId.data();
//     data.mv_size = userData.size();
//     data.mv_data = (void *)userData.data();

//     mdb_put(txn, dbi, &key, &data, 0); // Insertar el registro en la base de datos
//     mdb_txn_commit(txn);               // Confirmar la transacción

//     mdb_dbi_close(env, dbi);
//     mdb_env_close(env);
// }

// // Función para validar usuario y contraseña
// bool requestData(const string &username, const string &password)
// {
//     MDB_txn *txn;
//     MDB_val key, data;
//     int rc;

//     // Iniciar una transacción
//     rc = mdb_txn_begin(env, nullptr, MDB_RDONLY, &txn);
//     if (rc != MDB_SUCCESS)
//     {
//         cerr << "Error beginning transaction: " << mdb_strerror(rc) << endl;
//         return false;
//     }

//     // Crear la clave para buscar el usuario
//     // En este caso, asumimos que el username es el userId
//     key.mv_size = username.size();
//     key.mv_data = (void *)username.data();

//     // Buscar el registro en la base de datos
//     rc = mdb_get(txn, dbi, &key, &data);
//     if (rc != MDB_SUCCESS)
//     {
//         cerr << "Error getting data: " << mdb_strerror(rc) << endl;
//         mdb_txn_abort(txn);
//         return false; // Usuario no encontrado
//     }

//     // Convertir el valor recuperado a una cadena
//     string userData(static_cast<char *>(data.mv_data), data.mv_size);

//     // Parsear el JSON
//     json user = json::parse(userData);

//     // Extraer la contraseña almacenada
//     string storedPassword = user["password"];

//     // Comparar la contraseña ingresada con la almacenada
//     bool isValid = (storedPassword == password);

//     // Confirmar la transacción
//     mdb_txn_commit(txn);

//     return isValid;
// }

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

void shutDownScreen()
{
    clear();
    mvprintw(1, 10, "Cerrando sistema");
    for (int i = 0; i < 5; i++)
    {
        mvprintw(1, 26 + i, ".");
        refresh(); // Actualizar la pantalla
        this_thread::sleep_for(milliseconds(500));
    }
}

// Función para dar la bienvenida y opciones de inicio
void mostrarBienvenida()
{
    clear(); // Limpiar la pantalla
    drawBox(1, 10, 27, 138);
    mvprintw(3, 36, "######    ####  ########  ##    ##  ##    ##  ########  ##    ##  ####  #####       ####  ");
    mvprintw(4, 36, "##    ##   ##   ##        ###   ##  ##    ##  ##        ###   ##   ##   ##    ##  ##    ##");
    mvprintw(5, 36, "##    ##   ##   ##        ####  ##  ##    ##  ##        ####  ##   ##   ##    ##  ##    ##");
    mvprintw(6, 36, "######     ##   ########  ## ## ##  ##    ##  ########  ## ## ##   ##   ##    ##  ##    ##");
    mvprintw(7, 36, "##    ##   ##   ##        ##  ####   ##  ##   ##        ##  ####   ##   ##    ##  ##    ##");
    mvprintw(8, 36, "##    ##   ##   ##        ##   ###    ####    ##        ##   ###   ##   ##    ##  ##    ##");
    mvprintw(9, 36, "######    ####  ########  ##    ##     ##     ########  ##    ##  ####  #####       ####  ");
    mvprintw(11, 72, "  ####    ##      ");
    mvprintw(12, 72, "##    ##  ##      ");
    mvprintw(13, 72, "##    ##  ##      ");
    mvprintw(14, 72, "########  ##      ");
    mvprintw(15, 72, "##    ##  ##      ");
    mvprintw(16, 72, "##    ##  ##      ");
    mvprintw(17, 72, "##    ##  ########");
    mvprintw(19, 15, " ######   ####   ######   ########  ########  ##      ##    ####        ##      ##  ########  #####     ####    ######    ####  ");
    mvprintw(20, 15, "##    ##   ##   ##    ##     ##     ##        ###    ###  ##    ##      ###    ###  ##        ##    ##   ##   ##        ##    ##");
    mvprintw(21, 15, "##         ##   ##           ##     ##        ## #### ##  ##    ##      ## #### ##  ##        ##    ##   ##   ##        ##    ##");
    mvprintw(22, 15, " ######    ##    ######      ##     ########  ##  ##  ##  ########      ##  ##  ##  ########  ##    ##   ##   ##        ##    ##");
    mvprintw(23, 15, "      ##   ##         ##     ##     ##        ##      ##  ##    ##      ##      ##  ##        ##    ##   ##   ##        ##    ##");
    mvprintw(24, 15, "##    ##   ##   ##    ##     ##     ##        ##      ##  ##    ##      ##      ##  ##        ##    ##   ##   ##        ##    ##");
    mvprintw(25, 15, " ######   ####   ######      ##     ########  ##      ##  ##    ##      ##      ##  ########  #####     ####    ######    ####  ");
    refresh();                          // Actualizar la pantalla
    this_thread::sleep_for(seconds(3)); // Esperar un segundo
}

void mostrarMenuPrincipal()
{
    clear(); // Limpiar la pantalla
    drawBox(1, 10, 3, 36);
    mvprintw(2, 15, "MENU PRINCIPAL DEL SISTEMA");
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
    clear(); // Limpiar la pantalla
    drawBox(1, 10, 3, 26);
    mvprintw(2, 15, "INICIO DE SESION");
    refresh(); // Actualizar la pantalla
    char ch;

    // Solicitar name de usuario
    mvprintw(5, 10, "Ingrese su name de usuario: ");
    string username; // Usar string para el name de usuario
    noecho();        // Mostrar la entrada del usuario
    while ((ch = getch()) != '\n' && ch != '\r')
    { // Leer hasta Enter
        readData(username, 5, 10 + 28, ch, false);
    }

    // Solicitar contraseña
    mvprintw(6, 10, "Ingrese su contraseña: ");
    string password; // Usar string para la contraseña
    noecho();        // Ocultar la entrada del usuario
    while ((ch = getch()) != '\n' && ch != '\r')
    { // Leer hasta Enter
        readData(password, 6, 10 + 24, ch, true);
    }

    // Verificar si los campos están vacíos
    if (username.empty() || password.empty())
    {
        mvprintw(8, 10, "Error: Todos los campos son obligatorios. Presione cualquier tecla para reiniciar.");
        refresh();
        getch();         // Esperar a que el usuario presione una tecla
        iniciarSesion(); // Reiniciar el bucle para volver a solicitar los datos
    }

    noecho(); // Ocultar la entrada del usuario nuevamente
    refresh();

    // if (requestData(username, password))
    // {
    //     sesionIniciada();
    // }
    // else
    // {
    //     mvprintw(9, 10, "Error: usuario o contraseña incorrectos.");
    //     refresh();
    //     getch();             // Esperar a que el usuario presione una tecla
    //     mostrarBienvenida(); // Volver a mostrar la bienvenida
    // }

    sesionIniciada();
}

void crearUsuario()
{
    clear(); // Limpiar la pantalla
    drawBox(1, 10, 3, 23);
    mvprintw(2, 15, "CREAR USUARIO");
    refresh(); // Actualizar la pantalla
    char ch;

    // Solicitar name
    mvprintw(5, 10, "Ingrese su nombre: ");
    string name; // Usar string para el name de usuario
    noecho();    // Mostrar la entrada del usuario
    while ((ch = getch()) != '\n' && ch != '\r')
    { // Leer hasta Enter
        readData(name, 5, 10 + 19, ch, false);
    }

    // Solicitar lastName
    mvprintw(6, 10, "Ingrese su apellido: ");
    string lastName; // Usar string para el lastName de usuario
    noecho();        // Mostrar la entrada del usuario
    while ((ch = getch()) != '\n' && ch != '\r')
    { // Leer hasta Enter
        readData(lastName, 6, 10 + 21, ch, false);
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
    if (name.empty() || lastName.empty() || password.empty())
    {
        mvprintw(9, 10, "Error: Todos los campos son obligatorios. Presione cualquier tecla para reiniciar.");
        refresh();
        getch();        // Esperar a que el usuario presione una tecla
        crearUsuario(); // Reiniciar el bucle para volver a solicitar los datos
    }

    noecho(); // Ocultar la entrada del usuario nuevamente
    refresh();

    // insertData(name, lastName, password);

    iniciarSesion(); // Volver a mostrar la bienvenida
}

int mostrarMenu(vector<string> opciones, int inicio)
{
    int seleccion = 0;
    int ch;

    while (true)
    {
        // Mostrar el menú en una posición diferente
        for (int i = 0; i < opciones.size(); ++i)
        {
            if (i == seleccion)
            {
                mvprintw(inicio + i, 10, "> %s <", opciones[i].c_str()); // Imprimir opción seleccionada con flechas
            }
            else
            {
                mvprintw(inicio + i, 10, "  %s  ", opciones[i].c_str()); // Imprimir opción sin flechas
            }
        }

        ch = getch(); // Esperar la entrada del usuario

        switch (ch)
        {
        case KEY_UP:
            seleccion = (seleccion - 1 + opciones.size()) % opciones.size(); // Mover hacia arriba
            break;
        case KEY_DOWN:
            seleccion = (seleccion + 1) % opciones.size(); // Mover hacia abajo
            break;
        case 10: // Enter
            return seleccion;
        }
        refresh(); // Actualizar la pantalla después de cada entrada
    }
}