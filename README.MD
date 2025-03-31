# Proyecto de Sistema Medico de Gestión con Base de Datos LMDB

## Introducción

Este proyecto implementa un sistema de gestión basado en C++ que utiliza una base de datos en memoria eficiente, `LMDB` (Lightning Memory-Mapped Database), para almacenar y acceder a datos. El sistema emplea una interfaz en la terminal usando `ncurses` y realiza operaciones concurrentes con *hilos (threads)* para optimizar el rendimiento y la experiencia del usuario.

Además, el sistema garantiza la seguridad de los datos mediante el uso de `Crypto++` para cifrar la información sensible antes de almacenarla en la base de datos. A través de esta implementación, se busca demostrar cómo integrar bases de datos en memoria, criptografía, y concurrencia en un solo sistema de gestión eficiente y seguro.

## Características Clave

- *Base de Datos LMDB*: Base de datos en memoria que proporciona acceso rápido a los datos, lo que permite una lectura y escritura eficiente.
- *Cifrado de Datos*: Uso de `AES` para cifrar datos sensibles como contraseñas y otra información privada.
- *Hilos Concurrentes*: Gestión eficiente de tareas concurrentes utilizando hilos en C++ para asegurar que el programa se ejecute de manera eficiente y no se bloquee durante las operaciones.
- *Interfaz de Usuario en Terminal*: Interfaz interactiva en la terminal basada en `ncurses` que permite al usuario navegar entre opciones mediante teclas de dirección y seleccionarlas con Enter.
- *Optimización de Recursos*: Uso eficiente de la memoria y CPU, controlando el acceso concurrente a la base de datos `LMDB`.

## Requisitos

- *C++*: El código está escrito en C++ y hace uso de bibliotecas estándar y externas.
- *Compilador C++*: Debes tener instalado un compilador de C++ que soporte C++11 o superior.
- *Bibliotecas*:
  - *LMDB*: [Instrucciones de instalación de LMDB](https://symas.com/mdb/)
  - *Crypto++*: [Instrucciones de instalación de Crypto++](https://www.cryptopp.com/)
  - *ncurses*: [Instrucciones de instalación de ncurses](https://invisible-island.net/ncurses/)

### Instalación de Dependencias

Puedes instalar las dependencias necesarias utilizando tu sistema de gestión de paquetes. Aquí algunos ejemplos para sistemas basados en Debian/Ubuntu:

```bash
sudo apt-get update
sudo apt-get install liblmdb-dev libcrypto++-dev libncurses5-dev
```

## Instalación y Ejecución

### Base de Datos

Descomprima el archivo DB.rar que esta en la carpeta `Architecture-project` y borre el .rar

```bash
DB.rar --> ./DB/
```

### Con Makefile

1. Clona este repositorio a tu máquina local:

```bash
git clone <URL_del_repositorio>
```

2. Navega al directorio del proyecto:

```bash
cd <directorio_del_proyecto>
```

3. Si tienes un *Makefile* incluido en el proyecto, puedes compilar el proyecto ejecutando:

```bash
make
```

4. Ejecuta el programa compilado:

```bash
./nombre_del_programa
```

### Sin Makefile

Si no cuentas con un *Makefile*, compila manualmente utilizando un compilador C++ adecuado. Aquí hay un ejemplo de cómo compilar con g++:

```bash
g++ -std=c++17 -o nombre_del_programa main.cpp -l lmdb -l crypto++ -lncurses
```

Esto compilará el archivo main.cpp y generará un ejecutable llamado nombre_del_programa.

### Nota

Si se tiene algun problema a la hora de ejecutar, use el siguiente comando (tomando en cuenta que ya descargo las librerias correspondientes).

```bash
g++ main.cpp Librerias/Custom/src/interface.cpp    Librerias/Custom/src/parteMedica.cpp    Librerias/Custom/src/parteSistema.cpp    Librerias/Custom/src/loginAndJoin.cpp    -o program -L/usr/local/lib -L/usr/lib -lncurses -llmdb -lcryptopp
```

## Estructura del Proyecto

- *main.cpp*: Archivo principal donde se inicializa el programa, configura la base de datos y la interfaz de usuario.
- *header.h*: Contiene las bibliotecas necesarias, como `LMDB`, `Crypto++`, `ncurses`, y otras que forman la base del proyecto.
- *loginAndJoin.hpp*: Módulo encargado de gestionar el proceso de inicio de sesión y unirse al sistema.
- *parteMedica.hpp*: Encargado de gestionar la información médica y su almacenamiento cifrado.
- *parteSistema.hpp*: Se encarga de las funciones de administración del sistema, acceso a la base de datos, y operaciones de concurrencia.
- *json.hpp*: Permite la manipulación de archivos JSON para la configuración del sistema.
- *interface.hpp*: Define la interfaz de usuario utilizando ncurses para mostrar las opciones en la terminal.

## Flujo de Ejecución

1. *Inicio del Programa*: El programa inicia, configurando la base de datos `LMDB` y la interfaz de usuario interactiva con `ncurses`.
2. *Interacción con el Usuario: El usuario navega por el menú de opciones utilizando las teclas de dirección y selecciona opciones presionando Enter.
3. *Acceso a la Base de Datos*: LMDB mapea la base de datos en memoria para realizar lecturas y escrituras rápidas. Si los datos son sensibles, son cifrados antes de almacenarlos usando `AES`.
4. *Concurrencia y Hilos*: Las operaciones que requieren tiempo, como la lectura y escritura en la base de datos, son ejecutadas en hilos para evitar bloqueos y asegurar que la interfaz de usuario siga siendo interactiva.
5. *Finalización*: El programa cierra la base de datos de manera segura y finaliza cuando el usuario decide salir.

## Optimización de Recursos

### Control de Uso de Recursos

- *Memoria*: `LMDB` permite que los datos se almacenen en memoria, lo que optimiza el uso de memoria. Además, los hilos se gestionan de forma eficiente para evitar el uso excesivo de recursos.
- *CPU*: La implementación de hilos permite ejecutar varias operaciones simultáneamente, aprovechando al máximo la capacidad de la CPU sin bloquear el proceso principal.

### Concurrencia y Hilos

El proyecto utiliza *hilos* para mejorar el rendimiento y permitir que el sistema maneje múltiples tareas de manera eficiente. Esto incluye operaciones de acceso concurrente a la base de datos `LMDB`, lo que se maneja cuidadosamente para evitar conflictos y mantener la integridad de los datos.

## Seguridad y Protección de Datos

### Cifrado de Datos con AES

Para proteger la información sensible, el proyecto utiliza `AES` de la biblioteca `Crypto++` para cifrar y descifrar datos antes de almacenarlos en la base de datos LMDB. Este proceso garantiza que incluso si los datos son accesibles de forma no autorizada, no se puedan leer sin la clave de descifrado.

- Los datos cifrados se almacenan en la base de datos y se descifran solo cuando es necesario.

## Mejoras Futuras

1. *Interfaz Gráfica (GUI)*: Una posible mejora sería la integración de una interfaz gráfica para proporcionar una experiencia más amigable al usuario.
2. *Escalabilidad y Distribución*: Considerar la distribución de la base de datos entre varios servidores para aumentar la escalabilidad.
3. *Manejo de Errores*: Mejorar el manejo de errores, especialmente en operaciones concurrentes y accesos a la base de datos