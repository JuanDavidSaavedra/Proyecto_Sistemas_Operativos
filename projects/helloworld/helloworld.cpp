// Server side C program to demonstrate Socket programming
#include <stdio.h>              // Entrada/salida estándar (printf, perror, etc.)
#include <stdlib.h>             // Funciones generales (exit, malloc, free, etc.)
#include <unistd.h>             // Para llamadas al sistema como read, write, close, fork
#include <string.h>             // Para manejo de cadenas (strcpy, strtok, etc.)
#include <sys/socket.h>         // Funciones de sockets (socket, bind, listen, etc.)
#include <netinet/in.h>         // Estructuras para direcciones (sockaddr_in)
#include <sys/types.h>          // Tipos de datos adicionales usados por sockets
#include <fcntl.h>              // Manipulación de descriptores de archivos (open, O_RDONLY)
#include <sys/sendfile.h>       // Para enviar archivos directamente por sockets
#include <sys/stat.h>           // Para obtener información de archivos (stat)
#include <errno.h>              // Manejo de errores


#define PORT 8081               // Puerto donde el servidor escuchará conexiones

char http_header[25] = "HTTP/1.1 200 OK\r\n";  // Encabezado HTTP base para respuestas exitosas

// Extrae el segundo token (palabra) separado por `symbol` de la cadena `line`
char* parse(char line[], const char symbol[]) {
    char *copy = strdup(line); // Copia la línea
    char *token = strtok(copy, symbol); // Primer token
    token = strtok(NULL, symbol);       // Segundo token
    char *message = token ? strdup(token) : strdup(""); // Devuelve el segundo o una cadena vacía
    free(copy); // Libera memoria
    return message;
}


// Extrae el primer token (usualmente el método HTTP como GET o POST)
char* parse_method(char line[], const char symbol[]) {
    char *copy = strdup(line);
    char *token = strtok(copy, symbol);
    char *message = token ? strdup(token) : strdup("");
    free(copy);
    return message;
}

// Busca un token que contenga la subcadena `match` dentro de una línea separada por `symbol`
char* find_token(char line[], const char symbol[], const char match[]) {
    char *copy = strdup(line);
    char *token = strtok(copy, symbol);
    while (token != NULL) {
        if (strstr(token, match) != NULL) { // Coincidencia parcial
            char *result = strdup(token);
            free(copy);
            return result;
        }
        token = strtok(NULL, symbol);
    }
    free(copy);
    return strdup("");
}

int send_message(int fd, char image_path[], char head[]) {
    int file_fd = open(image_path, O_RDONLY); // Abre archivo solicitado
    if (file_fd < 0) { // Si no se pudo abrir, responde 404
        perror("File open error");
        char not_found[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found</h1>";
        write(fd, not_found, strlen(not_found));
        return -1;
    }

    struct stat stat_buf;
    fstat(file_fd, &stat_buf); // Obtiene tamaño del archivo
    int file_size = stat_buf.st_size;

    write(fd, head, strlen(head));        // Envía encabezado HTTP
    sendfile(fd, file_fd, NULL, file_size); // Envía contenido del archivo
    close(file_fd); // Cierra archivo
    return 0;
}

// Declara variables para sockets, procesos e información de conexión:
int main(int argc, char const *argv[]) {
    int server_fd, new_socket, pid; 
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Crea un socket TCP (SOCK_STREAM) y verifica errores:
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    // Configura la dirección y puerto del servidor:
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    // Asocia el socket a la dirección:
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("In bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Pone el socket en modo escucha (máx. 10 conexiones en cola):
    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    // Bucle principal del servidor
    while (1) {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        // Acepta una conexión entrante:
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        // Crea un proceso hijo para manejar la conexión:
        pid = fork();
        if (pid < 0) {
            perror("Error on fork");
            exit(EXIT_FAILURE);
        }

        // En el proceso hijo:
        if (pid == 0) {
            // Lee el mensaje del cliente:
            char buffer[30000] = {0};
            valread = read(new_socket, buffer, 30000);
            printf("\n buffer message: %s \n", buffer);

            // Extrae método HTTP y ruta solicitada:
            char *parse_string_method = parse_method(buffer, " ");
            printf("Client method: %s\n", parse_string_method);

            char *parse_string = parse(buffer, " ");
            printf("Client ask for path: %s\n", parse_string);

            // Obtiene extensión del archivo solicitado:
            char *copy = strdup(parse_string);
            char *parse_ext = parse(copy, ".");

            // Prepara encabezado HTTP:
            char *copy_head = (char *)malloc(strlen(http_header) + 200);
            strcpy(copy_head, http_header);

            // Procesamiento de petición GET
            if (strncmp(parse_string_method, "GET", 3) == 0) {
                // Decide qué archivo devolver y qué Content-Type usar según la extensión:
                char path_head[500] = ".";
                if (strlen(parse_string) <= 1) {
                    strcat(path_head, "/index.html");
                    strcat(copy_head, "Content-Type: text/html\r\n\r\n");
                } else if (strstr(parse_ext, "jpg") || strstr(parse_ext, "JPG")) {
                    strcat(path_head, parse_string);
                    strcat(copy_head, "Content-Type: image/jpeg\r\n\r\n");
                } else if (strstr(parse_ext, "ico")) {
                    strcat(path_head, "/img/favicon.png");
                    strcat(copy_head, "Content-Type: image/vnd.microsoft.icon\r\n\r\n");
                } else if (strstr(parse_ext, "ttf")) {
                    strcat(path_head, parse_string);
                    strcat(copy_head, "Content-Type: font/ttf\r\n\r\n");
                } else if (strstr(parse_ext, "js")) {
                    strcat(path_head, parse_string);
                    strcat(copy_head, "Content-Type: text/javascript\r\n\r\n");
                } else if (strstr(parse_ext, "css")) {
                    strcat(path_head, parse_string);
                    strcat(copy_head, "Content-Type: text/css\r\n\r\n");
                } else if (strstr(parse_ext, "woff")) {
                    strcat(path_head, parse_string);
                    strcat(copy_head, "Content-Type: font/woff\r\n\r\n");
                } else if (strstr(parse_ext, "m3u8")) {
                    strcat(path_head, parse_string);
                    strcat(copy_head, "Content-Type: application/vnd.apple.mpegurl\r\n\r\n");
                } else if (strstr(parse_ext, "ts")) {
                    strcat(path_head, parse_string);
                    strcat(copy_head, "Content-Type: video/mp2t\r\n\r\n");
                } else {
                    strcat(path_head, parse_string);
                    strcat(copy_head, "Content-Type: text/plain\r\n\r\n");
                }
                // Envía el archivo solicitado:
                send_message(new_socket, path_head, copy_head);
                printf("\n------------------Server sent------------------\n");
            // Procesamiento de petición POST, Busca una línea que contenga "action" y la devuelve como respuesta
            } else if (strncmp(parse_string_method, "POST", 4) == 0) {
                char *find_string = find_token(buffer, "\r\n", "action");
                strcat(copy_head, "Content-Type: text/plain\r\n\r\n");
                strcat(copy_head, "User Action: ");
                strcat(copy_head, find_string);
                write(new_socket, copy_head, strlen(copy_head));
                free(find_string);
            }

            //Limpieza
            close(new_socket);             // Cierra socket del cliente
            free(copy);
            free(copy_head);
            free(parse_string);
            free(parse_string_method);
            exit(0);                       // Termina proceso hijo
        } else {
            close(new_socket); // Parent closes its copy
        }
    }

    close(server_fd);
    return 0;
}






