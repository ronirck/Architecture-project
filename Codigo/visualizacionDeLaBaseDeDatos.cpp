#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <lmdb.h>
#include <chrono>
#include <thread>
#include "json.hpp"
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

using json = nlohmann::json;
using namespace std;
using namespace chrono;

// Estructura para almacenar los datos del paciente
struct Patient {
    int id;
    string name;
    string diagnosis;
    string admission_time;
    string sex;
    int age;
    int height;
    vector<string> medications;
};

void clearScreenAfterInput() {
    cout << "\nPresione Enter para continuar";
    cin.ignore();
    cin.get();
    system("clear");
}

// Función para obtener enfermedades comunes según la edad
void diseasesByAge(const vector<Patient>& patients, int age) {
    auto start = chrono::high_resolution_clock::now(); // Inicia la medición del tiempo

    map<string, int> diseaseCount;
    for (const Patient& p : patients) {
        if (p.age == age) {
            diseaseCount[p.diagnosis]++;
        }
    }

    if (diseaseCount.empty()) {
        cout << "No hay pacientes con esa edad.\n";
    } else {
        cout << "Enfermedades comunes en pacientes de " << age << " años:\n";
        for (const auto& entry : diseaseCount) {
            cout << entry.first << ": " << entry.second << " casos\n";
        }
    }

    auto end = chrono::high_resolution_clock::now(); // Finaliza la medición del tiempo
    chrono::duration<double> duration = end - start;

    // Guardar el tiempo de ejecución en un archivo de texto
    ofstream outFile("diseasesByAge_time.txt", ios::app);
    outFile << "Tiempo de ejecución de diseasesByAge: " << duration.count() << " segundos\n";
    outFile.close();
}

// Función para obtener las enfermedades más comunes según el sexo
void diseasesBySex(const vector<Patient>& patients) {
    auto start = chrono::high_resolution_clock::now(); // Inicia la medición del tiempo

    map<string, map<string, int>> diseaseBySex;
    for (const Patient& p : patients) {
        diseaseBySex[p.diagnosis][p.sex]++;
    }

    cout << "Enfermedades más comunes según el sexo:\n";
    for (const auto& entry : diseaseBySex) {
        cout << entry.first << ":\n";
        for (const auto& sexEntry : entry.second) {
            cout << "  " << sexEntry.first << ": " << sexEntry.second << " casos\n";
        }
    }

    auto end = chrono::high_resolution_clock::now(); // Finaliza la medición del tiempo
    chrono::duration<double> duration = end - start;

    // Guardar el tiempo de ejecución en un archivo de texto
    ofstream outFile("diseasesBySex_time.txt", ios::app);
    outFile << "Tiempo de ejecución de diseasesBySex: " << duration.count() << " segundos\n";
    outFile.close();
}

// Función para obtener las medicaciones más utilizadas
void medicationsUsage(const vector<Patient>& patients) {
    auto start = chrono::high_resolution_clock::now(); // Inicia la medición del tiempo

    map<string, int> medicationCount;
    for (const Patient& p : patients) {
        for (const string& med : p.medications) {
            medicationCount[med]++;
        }
    }

    if (medicationCount.empty()) {
        cout << "No hay medicaciones registradas.\n";
    } else {
        cout << "Medicaciones más utilizadas:\n";
        for (const auto& entry : medicationCount) {
            cout << entry.first << ": " << entry.second << " veces\n";
        }
    }

    auto end = chrono::high_resolution_clock::now(); // Finaliza la medición del tiempo
    chrono::duration<double> duration = end - start;

    // Guardar el tiempo de ejecución en un archivo de texto
    ofstream outFile("medicationsUsage_time.txt", ios::app);
    outFile << "Tiempo de ejecución de medicationsUsage: " << duration.count() << " segundos\n";
    outFile.close();
}

// Función para contar admisiones por año
void admissionsByYear(const vector<Patient>& patients) {
    auto start = chrono::high_resolution_clock::now(); // Inicia la medición del tiempo

    map<int, int> admissionsPerYear;
    for (const Patient& p : patients) {
        int year = stoi(p.admission_time.substr(0, 4));
        admissionsPerYear[year]++;
    }

    if (admissionsPerYear.empty()) {
        cout << "No hay registros de admisiones.\n";
    } else {
        cout << "Admisiones por año:\n";
        for (const auto& entry : admissionsPerYear) {
            cout << entry.first << ": " << entry.second << " admisiones\n";
        }
    }

    auto end = chrono::high_resolution_clock::now(); // Finaliza la medición del tiempo
    chrono::duration<double> duration = end - start;

    // Guardar el tiempo de ejecución en un archivo de texto
    ofstream outFile("admissionsByYear_time.txt", ios::app);
    outFile << "Tiempo de ejecución de admissionsByYear: " << duration.count() << " segundos\n";
    outFile.close();
}

// Función para simular una pantalla de carga
void loadingScreen() {
    cout << "Cargando el sistema";
    for (int i = 0; i < 5; i++) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    cout << "\n\n";
}
void shutDownScreen(){
    cout << "Cerrando el sistema";
    for (int i = 0; i < 10; i++) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    cout << "\n\n";
}

// Función para mostrar la bienvenida
void showWelcome() {
    system("clear");
    cout << "=========================================\n";
    cout << "       BIENVENIDO AL SISTEMA MEDICO      \n";
    cout << "=========================================\n";
}

// Función para mostrar el menú principal
int mainMenu() {
    int option;
    while (true) {
        system("clear");
        showWelcome();
        cout << "1. Pacientes\n";
        cout << "2. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> option;
        
        if (option == 1 || option == 2) return option;
    }
}

// Función para mostrar el submenú de pacientes
int patientMenu() {
    int option;
    while (true) {
        system("clear");
        cout << "======== MENU PACIENTES ========\n";
        cout << "1. Lista de nombres\n";
        cout << "2. Consultas adicionales\n"; // Nueva opción de consultas
        cout << "3. Volver\n";
        cout << "Seleccione una opción: ";
        cin >> option;

        if (option >= 1 && option <= 4) return option;
    }
}

// Función para obtener los pacientes almacenados en LMDB
vector<Patient> getPatients() {
    vector<Patient> patients;

    auto start = high_resolution_clock::now();  // Inicio de medición de tiempo

    MDB_env *env;
    MDB_dbi dbi;
    MDB_val key, data;
    MDB_txn *txn;
    MDB_cursor *cursor;

    mdb_env_create(&env);
    mdb_env_open(env, "./base_de_datos_lmdb", 0, 0664);
    mdb_txn_begin(env, nullptr, MDB_RDONLY, &txn);
    mdb_dbi_open(txn, nullptr, 0, &dbi);
    mdb_cursor_open(txn, dbi, &cursor);

    while (mdb_cursor_get(cursor, &key, &data, MDB_NEXT) == 0) {
        json patientJson = json::parse(string((char *)data.mv_data, data.mv_size));

        Patient p;
        p.id = patientJson.value("patient_id", 0); // Usando `value` para manejar nulos
        p.name = patientJson.value("name", "Desconocido");  // Proporciona un valor por defecto si es null
        p.diagnosis = patientJson.value("diagnosis", "No especificado");
        p.admission_time = patientJson.value("admission_time", "Desconocido");
        p.sex = patientJson.value("sex", "Desconocido");
        p.age = patientJson.value("age", 0);  // Default a 0 si es nulo
        p.height = patientJson.value("height", 0);  // Default a 0 si es nulo
        p.medications = patientJson.value("medications", vector<string>{});

        patients.push_back(p);
    }

    mdb_cursor_close(cursor);
    mdb_txn_abort(txn);
    mdb_dbi_close(env, dbi);
    mdb_env_close(env);

    auto end = high_resolution_clock::now();  // Fin de medición de tiempo
    auto duration = duration_cast<milliseconds>(end - start);  // Duración en milisegundos

    // Guardar el tiempo de ejecución en un archivo de texto
    ofstream file("tiempos_ejecucion.txt", ios::app); // Abrir el archivo en modo de adición
    if (file.is_open()) {
        file << "Tiempo de ejecución de getPatients: " << duration.count() << " ms\n";
        file.close();
    }

    return patients;
}

int selectPatient(const vector<Patient> &patients) {
    int selected = 0;        // Índice del paciente seleccionado
    int currentPage = 0;     // Página actual
    const int patientsPerPage = 50;

    auto start = high_resolution_clock::now();  // Inicio de medición de tiempo

    while (true) {
        system("clear"); // Limpiar pantalla en Linux

        // Calcular el rango de pacientes que se mostrarán en esta página
        int startIdx = currentPage * patientsPerPage;
        size_t endIdx = std::min(static_cast<size_t>(startIdx + patientsPerPage - 1), patients.size() - 1);

        // Mostrar lista de pacientes (10 a la vez)
        cout << "======== LISTA DE PACIENTES ========\n";
        for (int i = startIdx; i <= endIdx; i++) {
            if (i == selected) {
                cout << " > " << patients[i].name << " < \n";
            } else {
                cout << "   " << patients[i].name << "\n";
            }
        }

        // Mostrar opciones de navegación
        cout << "\nPresione 'w' para mover arriba, 's' para mover abajo, '1' para seleccionar, '2' para salir\n";

        char key = getchar(); // Leer la tecla presionada

        // Procesar teclas
        if (key == '1') {
            // Seleccionar
            cout << "Seleccionaste a: " << patients[selected].name << endl;
            auto end = high_resolution_clock::now();  // Fin de medición de tiempo
            auto duration = duration_cast<milliseconds>(end - start);  // Duración en milisegundos

            // Guardar el tiempo de ejecución en un archivo de texto
            ofstream file("tiempos_ejecucion.txt", ios::app); // Abrir el archivo en modo de adición
            if (file.is_open()) {
                file << "Tiempo de ejecución de selectPatient: " << duration.count() << " ms\n";
                file.close();
            }

            return selected; // Devolver el índice del paciente seleccionado
        }
        if (key == '2') {
            // Salir
            cout << "Saliendo...\n";
            auto end = high_resolution_clock::now();  // Fin de medición de tiempo
            auto duration = duration_cast<milliseconds>(end - start);  // Duración en milisegundos

            // Guardar el tiempo de ejecución en un archivo de texto
            ofstream file("tiempos_ejecucion.txt", ios::app); // Abrir el archivo en modo de adición
            if (file.is_open()) {
                file << "Tiempo de ejecución de selectPatient (salida): " << duration.count() << " ms\n";
                file.close();
            }

            return -1; // Salir sin seleccionar
        }

        if (key == 'w') {
            // Mover hacia arriba
            if (selected > startIdx) {
                selected--;
            } else if (currentPage > 0) {
                // Si estamos en el inicio de la página y hay páginas anteriores
                currentPage--;
                selected--;
            }
        }

        if (key == 's') {
            // Mover hacia abajo
            if (selected < endIdx) {
                selected++;
            } else if (endIdx < patients.size()) {
                // Si estamos al final de la página y hay más pacientes
                currentPage++;
                selected++; // Volver al primer paciente de la nueva página
            }
        }
    }
}




// Función para mostrar la información detallada de un paciente
void showPatientDetails(const Patient &patient, const vector<Patient>& patients) {
    auto start = high_resolution_clock::now();  // Inicio de medición de tiempo

    while (true) {
        system("clear");
        cout << "======= INFORMACION DEL PACIENTE =======\n";
        cout << "ID: " << patient.id << "\n";
        cout << "Nombre: " << patient.name << "\n";
        cout << "Sexo: " << patient.sex << "\n";
        cout << "Edad: " << patient.age << "\n";
        cout << "Estatura: " << patient.height << " cm\n";
        cout << "Diagnóstico: " << patient.diagnosis << "\n";
        cout << "Fecha de admisión: " << patient.admission_time << "\n";
        cout << "Medicamentos: ";
        for (const string &med : patient.medications) cout << med << " ";
        cout << "\n\nPresione '1' para volver al menu de pacientes\n";

        char key = getchar();
        if (key == '1') {
            // Volver a la lista de pacientes
            auto end = high_resolution_clock::now();  // Fin de medición de tiempo
            auto duration = duration_cast<milliseconds>(end - start);  // Duración en milisegundos

            // Guardar el tiempo de ejecución en un archivo de texto
            ofstream file("tiempos_ejecucion.txt", ios::app); // Abrir el archivo en modo de adición
            if (file.is_open()) {
                file << "Tiempo de ejecución de showPatientDetails: " << duration.count() << " ms\n";
                file.close();
            }

            break;
        }
    }
}


// Función principal del programa
int main() {
    srand(time(0));
    loadingScreen();

    while (true) {
        int option = mainMenu();
        if (option == 2) {
            system("clear");
            shutDownScreen();
            return 0;
        }

        while (true) {
            int subOption = patientMenu();
            if (subOption == 3) break;

            vector<Patient> patients = getPatients();
            if (patients.empty()) {
                cout << "No hay pacientes registrados.\n";
                system("pause");
                continue;
            }

            if (subOption == 1) {  // Lista de nombres
                int selected = selectPatient(patients);
                if (selected == -1) continue;  // Regresar al menú anterior
                showPatientDetails(patients[selected], patients);
            }
            
            if (subOption == 2) {
                // Consultas adicionales
                int queryOption;
                while (true) {
                    system("clear");
                    cout << "======= CONSULTAS ADICIONALES =======\n";
                    cout << "1. Enfermedades por edad\n";
                    cout << "2. Enfermedades por sexo\n";
                    cout << "3. Medicaciones más utilizadas\n";
                    cout << "4. Admisiones por año\n";
                    cout << "5. Volver\n";
                    cout << "Seleccione una opción: ";
                    cin >> queryOption;

                    if (queryOption == 1) {
                        int age;
                        cout << "Ingrese la edad: ";
                        cin >> age;
                        system("clear");
                        diseasesByAge(patients, age);
                    } else if (queryOption == 2) {
                        system("clear");
                        diseasesBySex(patients);
                    } else if (queryOption == 3) {
                        system("clear");
                        medicationsUsage(patients);
                    } else if (queryOption == 4) {
                        system("clear");
                        admissionsByYear(patients);
                    } else if (queryOption == 5) {
                        break;
                    }
                    clearScreenAfterInput();
                }
            }

        }
    }

    return 0;
}


