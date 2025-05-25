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
