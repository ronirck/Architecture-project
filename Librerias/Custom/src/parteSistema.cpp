#include "./../include/headers.hpp"

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;
using json = nlohmann::json;

void printDatabaseSize(const std::string &fileName);
void printAndSaveResourceUsage(const string &functionName);
void loadAndPrintResourceUsage();
void loadAndPrintExecutionTimes();
void saveExecutionTimeToJson(int id, const string &functionName, double duration);

void printDatabaseSize(const std::string &fileName)
{
    try
    {
        fs::path filePath(fileName);
        if (fs::exists(filePath))
        {
            long size = fs::file_size(filePath);                                          // Obtiene el tamaño del archivo en bytes
            double sizeInMB = static_cast<double>(size) / (1024 * 1024);                  // Convierte el tamaño a MB
            mvprintw(5, 15, "El tamaño de la base de datos LMDB es: %.2f MB.", sizeInMB); // Imprime en MB
            mvprintw(6, 15, "Presione ENTER para continuar");
        }
        else
        {
            mvprintw(5, 15, "El archivo no existe: %s", fileName.c_str());
        }
    }
    catch (const fs::filesystem_error &e)
    {
        mvprintw(14, 15, "Error al acceder al archivo: %s", e.what());
    }
    refresh();
    getch();
    clear();
}

void printAndSaveResourceUsage(const string &functionName)
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    // Crear un objeto JSON para esta función
    json resourceData;
    resourceData["function_name"] = functionName;
    resourceData["user_time_seconds"] = usage.ru_utime.tv_sec;
    resourceData["user_time_microseconds"] = usage.ru_utime.tv_usec;
    resourceData["system_time_seconds"] = usage.ru_stime.tv_sec;
    resourceData["system_time_microseconds"] = usage.ru_stime.tv_usec;
    resourceData["max_memory_kb"] = usage.ru_maxrss;

    // Leer el archivo existente para obtener el array de recursos
    json existingData;
    ifstream inFile("./json/resource_usage.json");
    if (inFile.is_open())
    {
        inFile >> existingData;
        inFile.close();
    }

    // Agregar el nuevo objeto JSON al array
    existingData.push_back(resourceData);

    // Guardar los datos en el archivo en formato de array JSON
    ofstream outFile("./json/resource_usage.json");
    if (outFile.is_open())
    {
        outFile << existingData.dump(4) << endl; // Escribir con indentación para una mejor legibilidad
        outFile.close();
    }
    else
    {
        cout << "Error al abrir el archivo para guardar el recurso." << endl;
    }
}

void loadAndPrintResourceUsage()
{
    std::ifstream inFile("./json/resource_usage.json");
    json root;

    // Verificar si el archivo se abre correctamente
    if (inFile.is_open())
    {
        try
        {
            inFile >> root; // Intentamos leer el archivo
            inFile.close();
        }
        catch (const json::exception &e)
        {
            mvprintw(14, 10, "Error al leer el archivo JSON: %s", e.what());
            refresh();
            getch();
            return;
        }
    }
    else
    {
        mvprintw(14, 10, "Error al abrir el archivo resource_usage.json.");
        refresh();
        getch();
        return;
    }

    // Verificar si el archivo tiene datos
    if (root.empty())
    {
        mvprintw(14, 10, "No hay datos de uso de recursos.");
        refresh();
        getch();
        return;
    }

    int row = 0;                 // Fila inicial donde comienza a mostrar
    int maxRows = 10;            // Número máximo de filas visibles
    int totalRows = root.size(); // Total de entradas de recursos
    int visibleRows = 0;         // Para contar cuántas filas se han mostrado

    // Inicializar las teclas para el desplazamiento
    int ch = 0; // Variable para capturar la tecla presionada

    while (true)
    {

        visibleRows = 0; // Reiniciar el contador de filas visibles

        // Mostrar los datos en la pantalla
        for (int i = row; i < root.size() && visibleRows < maxRows; ++i)
        {
            const auto &entry = root[i];

            if (entry.contains("function_name") &&
                entry.contains("user_time_seconds") &&
                entry.contains("user_time_microseconds") &&
                entry.contains("system_time_seconds") &&
                entry.contains("system_time_microseconds") &&
                entry.contains("max_memory_kb"))
            {

                mvprintw(visibleRows + 5, 15, "Function Name: %s", entry["function_name"].get<std::string>().c_str());
                mvprintw(visibleRows + 6, 15, "User Time: %ds %dμs", entry["user_time_seconds"].get<int>(), entry["user_time_microseconds"].get<int>());
                mvprintw(visibleRows + 7, 15, "System Time: %ds %dμs", entry["system_time_seconds"].get<int>(), entry["system_time_microseconds"].get<int>());
                mvprintw(visibleRows + 8, 15, "Max Memory: %d KB", entry["max_memory_kb"].get<int>());
                visibleRows += 10; // Incrementar por la cantidad de líneas por entrada
            }
            else
            {
                mvprintw(visibleRows + 5, 15, "Datos incompletos en la entrada.");
                visibleRows++;
            }
        }

        // Mostrar los controles para el usuario
        mvprintw(visibleRows + 5, 10, "Uso de teclas:");
        mvprintw(visibleRows + 6, 10, "ENTER para salir | Flecha Abajo para bajar | Flecha Arriba para subir");

        refresh(); // Refrescar la pantalla después de cada impresión

        ch = getch(); // Obtener la tecla presionada

        if (ch == 10)
        {
            break; // Salir si el usuario presiona 10
        }
        else if (ch == KEY_DOWN)
        {
            if (row + maxRows < totalRows)
            {
                row++; // Bajar una fila si no estamos al final
            }
        }
        else if (ch == KEY_UP)
        {
            if (row > 0)
            {
                row--; // Subir una fila si no estamos al inicio
            }
        }
    }

    clear(); // Limpiar la pantalla antes de salir
}

void loadAndPrintExecutionTimes()
{
    std::ifstream inFile("./json/execution_times.json");
    json root;

    // Verificar si el archivo se abre correctamente
    if (inFile.is_open())
    {
        try
        {
            inFile >> root; // Intentamos leer el archivo
            inFile.close();
        }
        catch (const json::exception &e)
        {
            mvprintw(14, 10, "Error al leer el archivo JSON: %s", e.what());
            refresh();
            getch();
            return;
        }
    }
    else
    {
        mvprintw(14, 10, "Error al abrir el archivo execution_times.json.");
        refresh();
        getch();
        return;
    }

    // Verificar si el archivo tiene datos
    if (root.empty())
    {
        mvprintw(14, 10, "No hay datos de tiempos de ejecución.");
        refresh();
        getch();
        return;
    }

    int row = 0;                 // Fila inicial donde comienza a mostrar
    int maxRows = 10;            // Número máximo de filas visibles
    int totalRows = root.size(); // Total de entradas de tiempos de ejecución
    int visibleRows = 0;         // Para contar cuántas filas se han mostrado

    // Inicializar las teclas para el desplazamiento
    int ch = 0; // Variable para capturar la tecla presionada

    while (true)
    {

        visibleRows = 0; // Reiniciar el contador de filas visibles

        // Mostrar los datos en la pantalla
        for (int i = row; i < root.size() && visibleRows < maxRows; ++i)
        {
            const auto &entry = root[i];

            if (entry.contains("id") &&
                entry.contains("function") &&
                entry.contains("execution_time"))
            {

                mvprintw(visibleRows + 5, 15, "ID: %d", entry["id"].get<int>());
                mvprintw(visibleRows + 6, 15, "Function: %s", entry["function"].get<std::string>().c_str());
                mvprintw(visibleRows + 7, 15, "Execution Time: %.6f seconds", entry["execution_time"].get<double>());
                visibleRows += 10; // Incrementar por la cantidad de líneas por entrada
            }
            else
            {
                mvprintw(visibleRows + 5, 15, "Datos incompletos en la entrada.");
                visibleRows++;
            }
        }

        // Mostrar los controles para el usuario
        mvprintw(visibleRows + 5, 10, "Uso de teclas:");
        mvprintw(visibleRows + 6, 10, "ENTER para salir | Flecha Abajo para bajar | Flecha Arriba para subir");

        refresh(); // Refrescar la pantalla después de cada impresión

        ch = getch(); // Obtener la tecla presionada

        if (ch == 10)
        {
            break; // Salir si el usuario presiona 10
        }
        else if (ch == KEY_DOWN)
        {
            if (row + maxRows < totalRows)
            {
                row++; // Bajar una fila si no estamos al final
            }
        }
        else if (ch == KEY_UP)
        {
            if (row > 0)
            {
                row--; // Subir una fila si no estamos al inicio
            }
        }
    }

    clear(); // Limpiar la pantalla antes de salir
}

// Función para guardar el tiempo de ejecución en un archivo JSON
void saveExecutionTimeToJson(int id, const string &functionName, double duration)
{
    // Leer el archivo JSON existente
    ifstream inputFile("./json/execution_times.json");
    json root;
    if (inputFile.is_open())
    {
        inputFile >> root;
        inputFile.close();
    }

    // Crear una entrada para la función con su tiempo de ejecución
    json executionEntry;
    executionEntry["id"] = id;
    executionEntry["function"] = functionName;
    executionEntry["execution_time"] = duration;

    // Agregar la entrada a la lista de tiempos de ejecución
    root.push_back(executionEntry); // Usar push_back en lugar de append

    // Guardar los datos actualizados en el archivo JSON
    ofstream outputFile("./json/execution_times.json");
    if (outputFile.is_open())
    {
        outputFile << root;
        outputFile.close();
    }
    else
    {
        std::cerr << "Error al abrir el archivo execution_times.json." << std::endl;
    }
}
