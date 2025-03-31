#include "./../include/interface.hpp"

std::atomic<bool> dataLoaded(false); // Bandera para indicar si la carga terminó
vector<Patient> allPatients;         // Almacenará todos los pacientes

void loadAllPatients()
{
    allPatients = getAllPatients(); // Cargar todos los pacientes desde LMDB
    dataLoaded = true;              // Marcar como completado
}

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

void logout()
{
    mvprintw(1, 10, "Cerrando sesion");
    for (int i = 0; i < 5; i++)
    {
        mvprintw(1, 26 + i, ".");
        refresh(); // Actualizar la pantalla
        this_thread::sleep_for(milliseconds(500));
    }
    mostrarMenuPrincipal();
}

void mostrarDatosMedicos()
{
    int patientsPerPage = 10;
    int currentPage = 0;
    int selected = 0;
    vector<Patient> patients;

    // Iniciar el hilo para cargar todos los pacientes
    std::thread dataThread(loadAllPatients);
    dataThread.detach(); // Desvincular el hilo para que siga en segundo plano
    vector<string> opciones = {
        "Pacientes",
        "Admisiones por Año",
        "Enfermedades por Sexo",
        "Enfermedades por Edad",
        "Medicamento más Utilizados",
        "Volver al Menu"};

    int opcion;
    bool flag = true;

    while (flag)
    {
        clear();
        drawBox(1, 10, 3, 28);
        mvprintw(2, 15, "INFORMACION MEDICA");
        refresh();
        opcion = mostrarMenu(opciones, 4);

        switch (opcion)
        {
        case 0:

            clear();                                                                        // Limpiar la pantalla antes de mostrar resultados
            mvprintw(2, 5, "Ingrese la cantidad de pacientes que desea consultar datos: "); // Muestra la instrucción inicial
            refresh();                                                                      // Refrescar la pantalla
            int ch;                                                                         // Variable para capturar la tecla presionada

            // Loop para capturar la edad y mostrarla en tiempo real
            while (true)
            {
                // Muestra la instrucción junto con la edad ingresada en tiempo real
                mvprintw(2, 65, "%d", patientsPerPage);
                refresh(); // Refrescar la pantalla

                ch = getch(); // Obtener la tecla presionada

                if (ch == 10)
                { // Si se presiona ENTER, salir del bucle
                    break;
                }
                else if (ch == KEY_BACKSPACE || ch == 127)
                {                                           // Si se presiona BACKSPACE, borrar el último dígito
                    patientsPerPage = patientsPerPage / 10; // Eliminar el último dígito
                }
                else if (ch >= '0' && ch <= '9')
                { // Si es un número, agregarlo a la edad
                    patientsPerPage = patientsPerPage * 10 + (ch - '0');
                }

                // Limpiar la pantalla para actualizar la edad ingresada
                clear();
                mvprintw(2, 5, "Ingrese la cantidad de pacientes que desea consultar datos: ");
            }
            clear();
            patients = getPatients(patientsPerPage, currentPage);

            if (patients.empty())
            {
                mvprintw(5, 5, "No hay pacientes registrados. Presione cualquier tecla para continuar.");
                refresh();
                getch();
                continue;
            }

            selected = selectPatient(currentPage, patientsPerPage);
            if (selected == -1)
            {
                continue;
            }

            showPatientDetails(patients[selected]);

            // Limpiar después de mostrar los detalles y volver al menú
            clear();
            drawBox(1, 10, 3, 28);
            mvprintw(2, 15, "INFORMACION MEDICA");
            refresh();

            break;

        case 1:
        case 2:
        case 3:
        case 4:
            if (!dataLoaded)
            {
                mvprintw(11, 10, "Cargando datos, por favor espere...");
                refresh();
                while (!dataLoaded)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }

            // Después de que los datos se carguen, proceder con las opciones
            if (opcion == 1)
                admissionsByYear(allPatients);
            else if (opcion == 2)
                diseasesBySex(allPatients);
            else if (opcion == 3)
                diseasesByAge(allPatients);
            else if (opcion == 4)
                medicationsUsage(allPatients);

            break;

        case 5:
            flag = false;
            clear();
            break;
        }

        move(12, 10);
        clrtoeol();
    }
    clear();
}

void mostrarDatosSistema()
{
    clear();                                  // Limpiar la pantalla
    refresh();                                // Actualizar la pantalla
    std::string dbFilePath = "./DB/data.mdb"; // Ruta al archivo de la base de datos LMDB

    vector<string> opciones = {
        "Tiempo de Ejecución de las Funciones de Consulta",
        "Recursos Usados",
        "Memoria de la Base de Datos",
        "Volver al Menu"};

    int opcion;
    bool flag = true;

    while (flag)
    {
        drawBox(1, 10, 3, 28);
        mvprintw(2, 15, "INFORMACION SISTEMA");
        opcion = mostrarMenu(opciones, 4);

        switch (opcion)
        {
        case 0:
            clear();
            drawBox(1, 10, 3, 58);
            mvprintw(2, 15, "TIEMPO DE EJECUCION DE LAS FUNCIONES DE CONSULTA");
            loadAndPrintExecutionTimes();
            refresh();
            break;
        case 1:
            clear();
            drawBox(1, 10, 3, 29);
            mvprintw(2, 15, "RECURSOS UTILIZADOS");
            loadAndPrintResourceUsage();
            refresh();
            break;
        case 2:
            clear();
            drawBox(1, 10, 3, 37);
            mvprintw(2, 15, "MEMORIA DE LA BASE DE DATOS");
            printDatabaseSize(dbFilePath);
            refresh();
            break;
        case 3:
            flag = false;
            clear();
            break;
        }

        move(12, 10); // Mover el cursor a la línea 2, columna 10
        clrtoeol();
    }
}

void sesionIniciada(string name)
{
    bool flag = true;

    while (flag)
    {
        clear(); // Limpiar la pantalla
        drawBox(1, 10, 5, 25);
        mvprintw(2, 15, "MENU DE USUARIO");
        mvprintw(4, 15, "Hola, %s!", name.c_str());
        refresh(); // Actualizar la pantalla

        vector<string> opciones = {"Vista de Informacion Medica", "Vista de Informacion del Sistema", "Cerrar Sesion"};

        int opcion = mostrarMenu(opciones, 7);

        switch (opcion)
        {
        case 0:
            mostrarDatosMedicos();
            break;
        case 1:
            mostrarDatosSistema();
            break;
        case 2:
            flag = false;
            clear();
            break;
        }
    }
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
    string SECRET_KEY = getEnvVar("SECRET_KEY");
    string IV = getEnvVar("IV");
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

    if (validateUser(username, password, SECRET_KEY, IV))
    {
        sesionIniciada(username);
    }
    else
    {
        refresh();
        getch();                // Esperar a que el usuario presione una tecla
        mostrarMenuPrincipal(); // Volver a mostrar la bienvenida
    }
}

void crearUsuario()
{
    string SECRET_KEY = getEnvVar("SECRET_KEY");
    string IV = getEnvVar("IV");
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

    createUser(name, lastName, password, SECRET_KEY, IV); // Crear el usuario

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
                mvprintw(inicio + i, 10, ">%s<", opciones[i].c_str()); // Imprimir opción seleccionada con flechas
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