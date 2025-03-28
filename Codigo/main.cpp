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
        mvprintw(linea + 1, 27 + i, ".");
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

void iniciarSesion()
{
    clear();
    mvprintw(1, 10, "==============================");
    mvprintw(2, 10, "      INICIO DE SESION          ");
    mvprintw(3, 10, "==============================");

    // Solicitar nombre de usuario
    mvprintw(6, 10, "Ingrese su nombre de usuario: ");
    char username[50];
    echo(); // Mostrar la entrada del usuario
    getnstr(username, sizeof(username) - 1);

    // Solicitar contraseña
    mvprintw(7, 10, "Ingrese su contraseña: ");
    char password[50];
    getnstr(password, sizeof(password) - 1);

    noecho(); // Ocultar la entrada del usuario nuevamente
    refresh();

    mostrarBienvenida(); // Volver a mostrar la bienvenida
}

void crearUsuario()
{
    clear();
    mvprintw(1, 10, "==============================");
    mvprintw(2, 10, "        CREAR USUARIO           ");
    mvprintw(3, 10, "==============================");

    // Solicitar nombre
    mvprintw(6, 10, "Ingrese su nombre: ");
    char nombre[50];
    echo(); // Mostrar la entrada del usuario
    getnstr(nombre, sizeof(nombre) - 1);

    // Solicitar apellido
    mvprintw(7, 10, "Ingrese su apellido: ");
    char apellido[50];
    getnstr(apellido, sizeof(apellido) - 1);

    // Solicitar contraseña
    mvprintw(8, 10, "Ingrese su contraseña: ");
    char password[50];
    getnstr(password, sizeof(password) - 1);

    // Solicitar repetir contraseña
    mvprintw(9, 10, "Repita su contraseña: ");
    char passwordRepetido[50];
    getnstr(passwordRepetido, sizeof(passwordRepetido) - 1);

    noecho(); // Ocultar la entrada del usuario nuevamente
    refresh();

    mostrarBienvenida(); // Volver a mostrar la bienvenida
}

void mostrarMenu()
{
    const char *opciones[] = {
        "Iniciar Sesión",
        "Crear usuario",
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

    loadingScreen();
    mostrarBienvenida(); // Mostrar el mensaje de bienvenida
    mostrarMenu();       // Mostrar el menú

    endwin(); // Finalizar ncurses
    return 0;
}