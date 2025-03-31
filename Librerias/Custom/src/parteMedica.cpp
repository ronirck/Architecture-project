#include "./../include/headers.hpp"

using json = nlohmann::json;
using namespace std;
using namespace chrono;

using namespace std;

void diseasesByAge(const std::vector<Patient> &patients);
void diseasesBySex(const std::vector<Patient> &patients);
void medicationsUsage(const vector<Patient> &patients);
void admissionsByYear(const std::vector<Patient> &patients);
vector<Patient> getPatients(int patientsPerPage, int currentPage);
vector<Patient> getAllPatients();
int selectPatient(int &currentPage, const int patientsPerPage);
void showPatientDetails(const Patient &patient);

void diseasesByAge(const std::vector<Patient> &patients)
{
    auto start = std::chrono::high_resolution_clock::now(); // Inicia la medición del tiempo

    std::map<std::string, int> diseaseCount;
    clear();                             // Limpiar la pantalla antes de mostrar resultados
    mvprintw(2, 5, "Ingrese la edad: "); // Muestra la instrucción inicial
    refresh();                           // Refrescar la pantalla

    int inputAge = 0; // Variable para almacenar la edad
    int ch;           // Variable para capturar la tecla presionada

    // Loop para capturar la edad y mostrarla en tiempo real
    while (true)
    {
        // Muestra la instrucción junto con la edad ingresada en tiempo real
        mvprintw(2, 22, "%d", inputAge);
        refresh(); // Refrescar la pantalla

        ch = getch(); // Obtener la tecla presionada

        if (ch == 10)
        { // Si se presiona ENTER, salir del bucle
            break;
        }
        else if (ch == KEY_BACKSPACE || ch == 127)
        {                             // Si se presiona BACKSPACE, borrar el último dígito
            inputAge = inputAge / 10; // Eliminar el último dígito
        }
        else if (ch >= '0' && ch <= '9')
        { // Si es un número, agregarlo a la edad
            inputAge = inputAge * 10 + (ch - '0');
        }

        // Limpiar la pantalla para actualizar la edad ingresada
        clear();
        mvprintw(2, 5, "Ingrese la edad: ");
    }

    // Buscar enfermedades de acuerdo con la edad ingresada
    bool found = false;
    for (const Patient &p : patients)
    {
        if (p.age == inputAge)
        {
            diseaseCount[p.diagnosis]++;
            found = true;
        }
    }

    int row = 6;                         // Fila inicial donde empieza a imprimir
    int maxRows = 10;                    // Número máximo de filas visibles
    int totalRows = diseaseCount.size(); // Total de enfermedades encontradas
    int visibleRows = 0;                 // Para contar cuántas filas se han mostrado
    int selected = 0;

    // Mostrar los resultados de enfermedades
    while (true)
    {
        visibleRows = 0; // Reiniciar el contador de filas visibles

        clear(); // Limpiar pantalla para mostrar los resultados
        drawBox(1, 10, 3, 55);
        mvprintw(2, 15, "ENFERMEDADES COMUNES EN PACIENTES DE %d ANIOS:", inputAge);
        refresh();

        if (!found)
        {
            mvprintw(5, 10, "No se encontraron enfermedades para esta edad.");
        }
        else
        {
            // Imprimir los resultados de las enfermedades
            for (auto it = std::next(diseaseCount.begin(), row); it != diseaseCount.end() && visibleRows < maxRows; ++it)
            {
                if (visibleRows == selected)
                {
                    mvprintw(visibleRows + 5, 10, ">%s: %d casos<", it->first.c_str(), it->second);
                }
                else
                {
                    mvprintw(visibleRows + 5, 10, "  %s: %d casos  ", it->first.c_str(), it->second);
                }
                visibleRows++;
            }
        }

        // Mostrar los controles para el usuario
        mvprintw(visibleRows + 7, 10, "Uso de teclas:");
        mvprintw(visibleRows + 8, 10, "ENTER para salir | Flecha Abajo para bajar | Flecha Arriba para subir");

        refresh(); // Refrescar la pantalla después de cada impresión

        ch = getch(); // Obtener la tecla presionada

        if (ch == 10)
        { // Si se presiona ENTER, salir
            break;
        }
        else if (ch == KEY_DOWN)
        {
            if (selected < maxRows)
            {
                selected++;
            }

            if (selected == maxRows)
            {
                if (row + maxRows < totalRows)
                {
                    selected = maxRows - 1;
                    row++; // Bajar una fila si no estamos al final
                }
            }
        }
        else if (ch == KEY_UP)
        {
            if (selected > 0)
            {
                selected--;
            }
            else
            {
                if (row > 0)
                {
                    row--; // Subir una fila si no estamos al inicio
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now(); // Finaliza la medición del tiempo
    std::chrono::duration<double> duration = end - start;

    // Guardar el tiempo de ejecución en el archivo JSON
    saveExecutionTimeToJson(10, "diseasesByAge", duration.count());
    clear(); // Limpiar la pantalla antes de salir
}

void diseasesBySex(const std::vector<Patient> &patients)
{
    auto start = std::chrono::high_resolution_clock::now(); // Inicia la medición del tiempo

    std::map<std::string, std::map<std::string, int>> diseaseBySex;
    for (const Patient &p : patients)
    {
        diseaseBySex[p.diagnosis][p.sex]++;
    }

    int row = 4;                         // Fila inicial donde empieza a imprimir
    int maxRows = 10;                    // Número máximo de filas visibles
    int totalRows = diseaseBySex.size(); // Total de enfermedades encontradas
    int visibleRows = 0;                 // Para contar cuántas filas se han mostrado
    int selected = 0;

    int ch = 0; // Variable para capturar la tecla presionada

    // Mostrar los resultados
    while (true)
    {
        visibleRows = 0; // Reiniciar el contador de filas visibles

        clear(); // Limpiar pantalla para mostrar los resultados

        drawBox(1, 10, 3, 49);
        mvprintw(2, 15, "ENFERMEDADES MAS COMUNES SEGUN EL SEXO:");

        // Imprimir los resultados de las enfermedades
        for (auto it = std::next(diseaseBySex.begin(), row); it != diseaseBySex.end() && visibleRows < maxRows; ++it)
        {
            if (selected == visibleRows)
            {
                mvprintw(visibleRows + 5, 10, ">%s:", it->first.c_str());
                visibleRows++;

                // Imprimir las enfermedades por sexo
                for (const auto &sexEntry : it->second)
                {
                    mvprintw(visibleRows + 5, 15, "  %s: %d casos", sexEntry.first.c_str(), sexEntry.second);
                    visibleRows++;
                }
            }
            else
            {
                mvprintw(visibleRows + 5, 15, "%s:", it->first.c_str());
                visibleRows++;

                // Imprimir las enfermedades por sexo
                for (const auto &sexEntry : it->second)
                {
                    mvprintw(visibleRows + 5, 20, "  %s: %d casos", sexEntry.first.c_str(), sexEntry.second);
                    visibleRows++;
                }
            }
        }

        // Mostrar los controles para el usuario
        mvprintw(visibleRows + 6, 10, "Uso de teclas:");
        mvprintw(visibleRows + 7, 10, "ENTER para salir | Flecha Abajo para bajar | Flecha Arriba para subir");
        refresh(); // Refrescar la pantalla después de cada impresión

        ch = getch(); // Obtener la tecla presionada

        if (ch == 10)
        {
            break; // Salir si el usuario presiona ENTER
        }
        else if (ch == KEY_DOWN)
        {

            if (selected < maxRows)
            {
                selected += 3;
            }

            if (selected >= maxRows)
            {
                if (row + maxRows < totalRows)
                {
                    selected = maxRows - 1;
                    row++; // Bajar una fila si no estamos al final
                }
            }
        }
        else if (ch == KEY_UP)
        {
            if (selected > 0)
            {
                selected -= 3;
            }
            else
            {
                if (row > 0)
                {
                    row--; // Subir una fila si no estamos al inicio
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now(); // Finaliza la medición del tiempo
    std::chrono::duration<double> duration = end - start;

    // Guardar el tiempo de ejecución en el archivo JSON
    saveExecutionTimeToJson(11, "diseasesBySex", duration.count());
    clear(); // Limpiar la pantalla antes de salir
}

void medicationsUsage(const vector<Patient> &patients)
{
    auto start = chrono::high_resolution_clock::now(); // Inicia la medición del tiempo

    map<string, int> medicationCount;
    for (const Patient &p : patients)
    {
        for (const string &med : p.medications)
        {
            medicationCount[med]++;
        }
    }

    int row = 4;                            // Fila inicial donde empieza a imprimir
    int maxRows = 10;                       // Número máximo de filas visibles
    int totalRows = medicationCount.size(); // Total de medicaciones encontradas
    int visibleRows = 0;                    // Para contar cuántas filas se han mostrado
    int selected = 0;

    int ch = 0; // Variable para capturar la tecla presionada

    // Mostrar los resultados
    while (true)
    {
        visibleRows = 0; // Reiniciar el contador de filas visibles

        clear(); // Limpiar pantalla para mostrar los resultados
        drawBox(1, 10, 3, 38);
        mvprintw(2, 15, "MEDICACIONES MAS UTILIZADAS:");
        refresh();

        // Imprimir los resultados de las medicaciones
        for (auto it = std::next(medicationCount.begin(), row); it != medicationCount.end() && visibleRows < maxRows; ++it)
        {
            if (selected == visibleRows)
            {
                mvprintw(visibleRows + 5, 10, ">%s: %d veces<", it->first.c_str(), it->second);
            }
            else
            {
                mvprintw(visibleRows + 5, 10, "  %s: %d veces  ", it->first.c_str(), it->second);
            }
            visibleRows++;
        }

        // Mostrar los controles para el usuario
        mvprintw(visibleRows + 6, 10, "Uso de teclas:");
        mvprintw(visibleRows + 7, 10, "ENTER para salir | Flecha Abajo para bajar | Flecha Arriba para subir");

        refresh(); // Refrescar la pantalla después de cada impresión

        ch = getch(); // Obtener la tecla presionada

        if (ch == 10)
        {
            break; // Salir si el usuario presiona ENTER
        }
        else if (ch == KEY_DOWN)
        {
            if (selected < maxRows)
            {
                selected++;
            }

            if (selected == maxRows)
            {
                if (row + maxRows < totalRows)
                {
                    selected = maxRows - 1;
                    row++; // Bajar una fila si no estamos al final
                }
            }
        }
        else if (ch == KEY_UP)
        {
            if (selected > 0)
            {
                selected--;
            }
            else
            {
                if (row > 0)
                {
                    row--; // Subir una fila si no estamos al inicio
                }
            }
        }
    }

    auto end = chrono::high_resolution_clock::now(); // Finaliza la medición del tiempo
    chrono::duration<double> duration = end - start;

    // Guardar el tiempo de ejecución en el archivo JSON
    saveExecutionTimeToJson(12, "medicationsUsage", duration.count());
    clear(); // Limpiar la pantalla antes de salir
}

void admissionsByYear(const std::vector<Patient> &patients)
{
    auto start = std::chrono::high_resolution_clock::now(); // Inicia la medición del tiempo

    std::map<int, int> admissionsPerYear;
    for (const Patient &p : patients)
    {
        int year = std::stoi(p.admission_time.substr(0, 4)); // Obtener el año de la fecha de admisión
        admissionsPerYear[year]++;
    }

    int row = 4;                              // Fila inicial donde comienza a imprimir
    int maxRows = 10;                         // Número máximo de filas visibles
    int totalRows = admissionsPerYear.size(); // Total de registros de admisiones
    int visibleRows = 0;                      // Para contar cuántas filas se han mostrado
    int selected = 0;

    int ch = 0; // Variable para capturar la tecla presionada

    clear(); // Limpiar la pantalla antes de mostrar resultados

    // Mostrar la cabecera
    if (admissionsPerYear.empty())
    {
        mvprintw(5, 10, "No hay registros de admisiones.");
        refresh();
        getch();
        return;
    }

    // Mensaje inicial
    drawBox(1, 10, 3, 29);
    mvprintw(2, 15, "ADMISIONES POR ANIO:");
    refresh();

    // Navegación a través de los datos
    while (true)
    {
        visibleRows = 0; // Reiniciar el contador de filas visibles

        // Imprimir los datos de admisiones
        for (auto it = std::next(admissionsPerYear.begin(), row); it != admissionsPerYear.end() && visibleRows < maxRows; ++it)
        {
            if (visibleRows == selected)
            {
                mvprintw(visibleRows + 5, 10, ">%d: %d admisiones<", it->first, it->second);
            }
            else
            {
                mvprintw(visibleRows + 5, 10, "  %d: %d admisiones  ", it->first, it->second);
            }
            visibleRows++;
        }

        // Mostrar los controles para el usuario
        mvprintw(visibleRows + 6, 10, "Uso de teclas:");
        mvprintw(visibleRows + 7, 10, "ENTER para salir | Flecha Abajo para bajar | Flecha Arriba para subir");

        refresh(); // Refrescar la pantalla después de cada impresión

        ch = getch(); // Obtener la tecla presionada

        if (ch == 10)
        {
            break; // Salir si el usuario presiona 10
        }
        else if (ch == KEY_DOWN)
        {
            if (selected < maxRows)
            {
                selected++;
            }

            if (selected == maxRows)
            {
                if (row + maxRows < totalRows)
                {
                    selected = maxRows - 1;
                    row++; // Bajar una fila si no estamos al final
                }
            }
        }
        else if (ch == KEY_UP)
        {
            if (selected > 0)
            {
                selected--;
            }
            else
            {
                if (row > 0)
                {
                    row--; // Subir una fila si no estamos al inicio
                }
            }
        }
    }

    // Guardar el tiempo de ejecución en el archivo JSON
    auto end = std::chrono::high_resolution_clock::now(); // Finaliza la medición del tiempo
    std::chrono::duration<double> duration = end - start;
    saveExecutionTimeToJson(13, "admissionsByYear", duration.count());

    clear(); // Limpiar la pantalla antes de salir
}

vector<Patient> getPatients(int patientsPerPage, int currentPage)
{
    auto start = chrono::high_resolution_clock::now(); // Inicia la medición del tiempo

    vector<Patient> patients;
    MDB_env *env;
    MDB_dbi dbi;
    MDB_val key, data;
    MDB_txn *txn;
    MDB_cursor *cursor;

    mdb_env_create(&env);
    mdb_env_open(env, "./DB", 0, 0664);
    mdb_txn_begin(env, nullptr, MDB_RDONLY, &txn);
    mdb_dbi_open(txn, nullptr, 0, &dbi);
    mdb_cursor_open(txn, dbi, &cursor);

    int startIdx = currentPage * patientsPerPage;
    int endIdx = startIdx + patientsPerPage;
    int index = 0;

    // Saltar hasta el inicio de la página solicitada
    while (index < startIdx && mdb_cursor_get(cursor, &key, &data, MDB_NEXT) == 0)
    {
        index++;
    }

    // Cargar solo los pacientes de la página actual
    for (int i = startIdx; i < endIdx && mdb_cursor_get(cursor, &key, &data, MDB_NEXT) == 0; i++)
    {
        json patientJson = json::parse(string((char *)data.mv_data, data.mv_size));
        Patient p;
        p.id = patientJson.value("patient_id", 0);
        p.name = patientJson.value("name", "Desconocido");
        p.diagnosis = patientJson.value("diagnosis", "No especificado");
        p.admission_time = patientJson.value("admission_time", "Desconocido");
        p.sex = patientJson.value("sex", "Desconocido");
        p.age = patientJson.value("age", 0);
        p.height = patientJson.value("height", 0);
        p.medications = patientJson.value("medications", vector<string>{});
        patients.push_back(p);
    }

    mdb_cursor_close(cursor);
    mdb_txn_abort(txn);
    mdb_dbi_close(env, dbi);
    mdb_env_close(env);

    auto end = chrono::high_resolution_clock::now(); // Finaliza la medición del tiempo
    chrono::duration<double> duration = end - start;

    // Guardar el tiempo de ejecución de la función
    saveExecutionTimeToJson(14, "getPatients", duration.count());
    printAndSaveResourceUsage("getPatients");

    return patients;
}

vector<Patient> getAllPatients()
{
    auto start = chrono::high_resolution_clock::now(); // Inicia la medición del tiempo

    vector<Patient> patients;
    MDB_env *env;
    MDB_dbi dbi;
    MDB_val key, data;
    MDB_txn *txn;
    MDB_cursor *cursor;

    mdb_env_create(&env);
    mdb_env_open(env, "./DB", 0, 0664);
    mdb_txn_begin(env, nullptr, MDB_RDONLY, &txn);
    mdb_dbi_open(txn, nullptr, 0, &dbi);
    mdb_cursor_open(txn, dbi, &cursor);

    // Iterar sobre todos los registros sin paginación
    while (mdb_cursor_get(cursor, &key, &data, MDB_NEXT) == 0)
    {
        json patientJson = json::parse(string((char *)data.mv_data, data.mv_size));
        Patient p;
        p.id = patientJson.value("patient_id", 0);
        p.name = patientJson.value("name", "Desconocido");
        p.diagnosis = patientJson.value("diagnosis", "No especificado");
        p.admission_time = patientJson.value("admission_time", "Desconocido");
        p.sex = patientJson.value("sex", "Desconocido");
        p.age = patientJson.value("age", 0);
        p.height = patientJson.value("height", 0);
        p.medications = patientJson.value("medications", vector<string>{});
        patients.push_back(p);
    }

    mdb_cursor_close(cursor);
    mdb_txn_abort(txn);
    mdb_dbi_close(env, dbi);
    mdb_env_close(env);

    auto end = chrono::high_resolution_clock::now(); // Finaliza la medición del tiempo
    chrono::duration<double> duration = end - start;

    // Guardar el tiempo de ejecución de la función
    saveExecutionTimeToJson(15, "getAllPatients", duration.count());
    printAndSaveResourceUsage("getAllPatients");
    return patients;
}

int selectPatient(int &currentPage, const int patientsPerPage)
{
    auto patients = getPatients(patientsPerPage, currentPage); // Cargar solo la página actual
    int selected = 0;

    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    while (true)
    {
        clear();
        drawBox(1, 10, 3, 28);
        mvprintw(2, 15, "LISTA DE PACIENTES");
        refresh();

        for (int i = 0; i < patients.size(); i++)
        {
            if (i == selected)
            {
                mvprintw(i + 4, 10, ">%s<", patients[i].name.c_str());
            }
            else
            {
                mvprintw(i + 4, 10, "  %s  ", patients[i].name.c_str());
            }
        }

        mvprintw(patients.size() + 4, 10, "ENTER para seleccionar | Flecha Abajo para bajar | Flecha Arriba para subir.");
        mvprintw(patients.size() + 5, 10, "Flecha Izquierda para pagina anterior | Flecha Derecha para siguiente paginae arriba | Esc para salir.");

        refresh();

        int key = getch();

        if (key == 10)
        { // Enter (seleccionar)
            endwin();
            return selected;
        }

        if (key == 27)
        { // Escape (salir sin seleccionar)
            endwin();
            return -1;
        }

        if (key == KEY_UP)
        {
            if (selected > 0)
            {
                selected--;
            }
        }

        if (key == KEY_DOWN)
        {
            if (selected < patients.size() - 1)
            {
                selected++;
            }
        }

        // Navegar entre páginas
        if (key == KEY_RIGHT && patients.size() == patientsPerPage)
        { // 'n' para siguiente página
            currentPage++;
            patients = getPatients(patientsPerPage, currentPage);
            selected = 0; // Resetear selección al inicio de la página
        }

        if (key == KEY_LEFT && currentPage > 0)
        { // 'p' para página anterior
            currentPage--;
            patients = getPatients(patientsPerPage, currentPage);
            selected = 0; // Resetear selección al inicio de la página
        }
    }
}

// Función para mostrar la información detallada de un paciente con ncurses
void showPatientDetails(const Patient &patient)
{
    static bool initialized = false;
    if (!initialized)
    {
        initscr();
        noecho();
        curs_set(0);
        keypad(stdscr, TRUE);
        initialized = true;
    }

    auto start = high_resolution_clock::now(); // Inicio de medición de tiempo

    while (true)
    {
        clear();
        drawBox(1, 10, 3, 34);
        mvprintw(2, 15, "INFORMACION DEL PACIENTE");
        mvprintw(5, 15, "ID: %d", patient.id);
        mvprintw(6, 15, "Nombre: %s", patient.name.c_str());
        mvprintw(7, 15, "Sexo: %s", patient.sex.c_str());
        mvprintw(8, 15, "Edad: %d", patient.age);
        mvprintw(9, 15, "Estatura: %d cm", patient.height);
        mvprintw(10, 15, "Diagnóstico: %s", patient.diagnosis.c_str());
        mvprintw(11, 15, "Fecha de admisión: %s", patient.admission_time.c_str());

        mvprintw(14, 15, "Medicamentos: ");
        int y = 14;
        for (const string &med : patient.medications)
        {
            mvprintw(y++, 30, "%s", med.c_str());
        }

        mvprintw(y + 2, 10, "Presione Enter para volver al menú de pacientes");
        refresh();

        int key = getch();
        if (key == 10 || key == KEY_ENTER)
            break; // Salir al presionar Enter
    }

    auto end = high_resolution_clock::now();                  // Fin de medición de tiempo
    auto duration = duration_cast<milliseconds>(end - start); // Duración en ms

    // Guardar el tiempo de ejecución en el archivo JSON
    saveExecutionTimeToJson(16, "showPatientDetails", duration.count()); // id 14 para showPatientDetails

    endwin();
}
