#include "./Librerias/Custom/include/headers.hpp"

int main()
{
    loadEnvFile("./Librerias/Custom/src/.env");  // Cargar el archivo .env
    initscr();            // Iniciar ncurses
    keypad(stdscr, TRUE); // Habilitar las teclas especiales
    noecho();             // No mostrar la entrada del usuario
    curs_set(0);          // Ocultar el cursor

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