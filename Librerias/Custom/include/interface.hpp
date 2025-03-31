#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <vector>
#include <ncurses.h>
#include <thread>
#include <chrono>

#include "./loginAndJoin.hpp"
#include "./parteMedica.hpp"
#include "./parteSistema.hpp"

using namespace std;
using namespace chrono;

// FUNCIONES DE EXPERIENCIA DE INTERFAZ
void loadingScreen();
void shutDownScreen();
int mostrarMenu(vector<string> opciones, int inicio);
void mostrarBienvenida();
void drawBox(int y, int x, int height, int width);

// FUNCIONES PARA EL INICIO DE SESION
void readData(string &data, int line, int space, char ch, bool isPassword);
void iniciarSesion();
void crearUsuario();
void mostrarMenuPrincipal();

// FUNCIONES DEL USUARIO
void mostrarDatosSistema();
void mostrarDatosMedicos();

#endif