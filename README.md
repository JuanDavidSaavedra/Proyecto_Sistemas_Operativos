# Proyecto Final - Sistemas Operativos: Construcción de un Servidor Web Básico

---

## Recursos Útiles

- [Página del curso](https://sites.google.com/saber.uis.edu.co/sistemas-operativos/inicio)
- [Repositorio original](https://github.com/Dungyichao/http_server/tree/master/Project)

---

## Descripción

Este proyecto consiste en la implementación de un **servidor web básico** utilizando **Linux** y **C++**, aplicando los fundamentos de programación de sistemas y redes. Su objetivo principal es enseñar la arquitectura cliente-servidor y cómo se maneja la comunicación HTTP mediante sockets en un entorno local.

> *“La nube no existe, es simplemente la computadora de alguien más.”*

## Integrantes

- Juan David Saavedra González - 2214111  
- Juan Diego Herrera Cáceres - 2151316  
- Erick Daniel Vargas Miranda - 2211861  

**Docente:** Luis Alejandro Torres  
**Asignatura:** Sistemas Operativos F1  
**Universidad:** Universidad Industrial de Santander  
**Período:** 2025-1

---

## Objetivo de Aprendizaje

- Comprender el funcionamiento de los servidores web.
- Aplicar conceptos clave como procesos, manejo de archivos y sockets.
- Implementar un servidor capaz de recibir solicitudes y responder adecuadamente.

---

## Tecnologías Utilizadas

- **Lenguaje:** C++
- **Sistema Operativo:** Distribución Linux
- **Editor:** Visual Studio Code
- **Networking:** Sockets TCP/IP

---

## Conceptos Clave

- **Web Socket:** Comunicación persistente en tiempo real entre servidor y cliente.
- **Fork:** Permite manejar múltiples solicitudes simultáneamente creando procesos hijo.
- **Token:** Fragmentos de texto útiles para el procesamiento de datos de cadenas.

---

## Requisitos Previos

- Tener una distribución de Linux instalada.
- Tener Visual Studio Code instalado y configurado para C++.

---

## Guía de Implementación

1. Seguir el tutorial de Visual Studio Code para configurar C++:
   👉 https://code.visualstudio.com/docs/cpp/config-linux

2. Descargar y configurar el contenido del proyecto desde el repositorio:
   👉 https://github.com/Dungyichao/http_server/tree/master/Project

3. Descargar contenido web demo:
   👉 https://npcasc2020.firebaseapp.com/

4. Descomprimir y ubicar los archivos dentro de la carpeta `src` del proyecto.

5. Sustituir el contenido de `helloworld.cpp` con el código fuente del servidor.

---

## Estructura del Servidor Web

El servidor implementado realiza las siguientes funciones:

- Escucha conexiones en el puerto 8081.
- Lee la solicitud entrante del cliente.
- Analiza el mensaje y determina el recurso solicitado.
- Devuelve la respuesta HTTP correspondiente (200 OK o 404 Not Found).
- Utiliza `fork()` para gestionar múltiples conexiones simultáneas.

---

## Paso a paso para implementar el servidor (Versión reducida)

### 1. Crear el archivo `helloworld.cpp`

### 2. Copiar y pegar las librerías necesarias

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
```

### 3. Definir el puerto

```c
#define PORT 8081
```

### 4. Crear la función principal

```c
int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    char buffer[1024] = {0};
```

### 5. Crear socket

```c
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
```

### 6. Configurar dirección

```c
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
```

### 7. Asociar socket con puerto

```c
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
```

### 8. Escuchar conexiones

```c
    listen(server_fd, 1);
    printf("Listening on port %d...\n", PORT);
```

### 9. Aceptar conexión

```c
    client_fd = accept(server_fd, (struct sockaddr*)&addr, &addrlen);
```

### 10. Leer solicitud

```c
    read(client_fd, buffer, 1024);
    printf("Request:\n%s\n", buffer);
```

### 11. Responder con archivo `index.html`

```c
    int file_fd = open("index.html", O_RDONLY);
    struct stat st;
    fstat(file_fd, &st);
    char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    write(client_fd, header, strlen(header));
    sendfile(client_fd, file_fd, NULL, st.st_size);
    close(file_fd);
```

### 12. Cerrar conexión

```c
    close(client_fd);
    close(server_fd);
    return 0;
}
```

---

## ¿Cómo compilar y ejecutar?

```bash
gcc helloworld.cpp -o helloworld
./helloworld
```

Si da error, instala `gcc` y herramientas de compilación:

```bash
sudo apt update
sudo apt install build-essential
```

---

## Opción 2: Servidor en Node.js

### Archivo: `server.js`

```js
const http = require('http');
const path = require('path');
const fs = require('fs');

const server = http.createServer((req, res) => {
    let filePath = path.join(__dirname, req.url === '/' ? 'index.html' : req.url);
    let file_extname = path.extname(filePath);
    let contentType = 'text/html';

    switch(file_extname){
        case '.js':
            contentType = 'text/javascript'; break;
        case '.css':
            contentType = 'text/css'; break;
        case '.json':
            contentType = 'application/json'; break;
        case '.png':
            contentType = 'image/png'; break;
        case '.JPG':
            contentType = 'image/jpg'; break;
        case '.ico':
            filePath = path.join(__dirname,'favicon.png');
            contentType = 'image/png'; break;
        case '.ttf':
            contentType = 'font/ttf'; break;
        case '.woff':
            contentType = 'font/woff'; break;
        case '.woff2':
            contentType = 'font/woff2'; break;
    }

    fs.readFile(filePath, (err, content) => {
        if(err){
            if(err.code == 'ENOENT'){
                console.log('Page not found');
            } else {
                res.writeHead(500);
                res.end(`Server Error: ${err.code}`);
            }
        } else {
            res.writeHead(200, {'Content-Type': contentType});
            res.end(content);    
        }
    });
});

const PORT = process.env.PORT || 8080;
server.listen(PORT, () => console.log(`Server is running and port is ${PORT}`));
```

---

### Ejecutar el servidor

```bash
node server.js
```

