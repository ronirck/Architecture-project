# Usar una imagen de Ubuntu con las herramientas necesarias
FROM ubuntu:latest

# Instalar dependencias
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    cmake \
    liblmdb-dev \
    libjsoncpp-dev \
    libncurses-dev \
    && rm -rf /var/lib/apt/lists/*

# Crear y establecer el directorio de trabajo en el contenedor
WORKDIR /app

# Copiar todo el proyecto al contenedor
COPY . /app

# Compilar el proyecto (ajusta el comando según tu Makefile o compilación)
RUN g++ -o mi_proyecto /app/Codigo/main.cpp -I /app/Librerias -llmdb -ljsoncpp -lncurses

# Comando por defecto al ejecutar el contenedor
CMD ["./mi_proyecto"]
