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

void loadAllPatients();
void drawBox(int y, int x, int height, int width);
void logout();
void mostrarDatosMedicos();
void mostrarDatosSistema();
void sesionIniciada(string name);
void loadingScreen();
void shutDownScreen();
void mostrarBienvenida();
void mostrarMenuPrincipal();
void readData(string &data, int line, int space, char ch, bool isPassword);
void iniciarSesion();
void crearUsuario();
int mostrarMenu(vector<string> opciones, int inicio);

#endif