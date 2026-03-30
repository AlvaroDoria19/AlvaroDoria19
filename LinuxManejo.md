# Bitácora de Comandos de Laboratorio 3 (Kubuntu Shell)

## Índice

- [Comandos de Entorno y Navegación](#comandos-de-entorno-y-navegación)
- [Comandos de Gestión de Archivos y Compresión](#comandos-de-gestión-de-archivos-y-compresión)
- [Operadores de Redirección y Tuberías](#operadores-de-redirección-y-tuberías)
- [Comandos de Administración, Usuarios y Permisos](#comandos-de-administración-usuarios-y-permisos)
- [Comandos de Gestión de Procesos y Paquetes](#comandos-de-gestión-de-procesos-y-paquetes)

---

## Comandos de Entorno y Navegación

Los siguientes comandos son fundamentales para moverse a través del árbol de directorios de Linux, conocer tu ubicación actual, listar elementos y revisar el historial de la sesión en la terminal.

| **_Comando_** | **_Uso_** | **_Aplicable a_** | **_Ejemplo_** | **_Notas_** |
|:---:|:---:|:---:|:---:|:---:|
| pwd | Imprime la ruta absoluta del directorio de trabajo actual | Sistema | pwd | - |
| cd | Cambia el directorio de trabajo actual | Sistema | cd /etc/ | Usar `cd /` lleva directamente al directorio raíz del sistema |
| ls | Lista el contenido de un directorio | Directorios | ls -l | El parámetro `-l` muestra una lista detallada (permisos, propietario, tamaño y fecha) |
| history | Muestra o manipula el historial de comandos ejecutados | Sistema | history -c | La bandera `-c` (clear) limpia el historial de la sesión actual en la memoria RAM |
| neofetch | Muestra información visual del sistema operativo y hardware | Sistema | neofetch | Ideal para mostrar rápidamente la distribución y detalles del kernel |

## Comandos de Gestión de Archivos y Compresión

Esta sección reúne los comandos esenciales para la creación, copia y empaquetado/compresión de elementos en el sistema de archivos de Linux.

| **_Comando_** | **_Uso_** | **_Aplicable a_** | **_Ejemplo_** | **_Notas_** |
|:---:|:---:|:---:|:---:|:---:|
| mkdir | Crea un nuevo directorio (carpeta) | Directorios | mkdir lab3Shell | - |
| cp | Copia archivos o directorios | Archivos/Directorios | cp /etc/*.conf /home/ | Se puede usar el comodín `*` para copiar múltiples archivos con la misma extensión |
| tar | Empaqueta y comprime archivos en un único archivo de salida | Archivos/Directorios | tar -czvf respaldo.tar.gz | Parámetros: `-c` (crear), `-z` (comprimir con gzip), `-v` (verbose/detallado), `-f` (archivo) |

## Operadores de Redirección y Tuberías

Herramientas y caracteres especiales (conectores) que permiten desviar la salida estándar de un comando hacia un archivo, o conectar múltiples comandos entre sí.

| **_Comando_** | **_Uso_** | **_Aplicable a_** | **_Ejemplo_** | **_Notas_** |
|:---:|:---:|:---:|:---:|:---:|
| echo | Imprime un texto o cadena de caracteres en la salida estándar | Sistema | echo "Hola" | Se usa frecuentemente junto con operadores de redirección para escribir en archivos |
| > | Redirección de salida (Sobrescribe) | Archivos | echo "texto" > archivo.txt | Si el archivo no existe, lo crea. Si existe, **borra todo su contenido previo** y lo reemplaza |
| >> | Redirección de adición (Añade) | Archivos | echo "más" >> archivo.txt | Añade el nuevo texto al **final** del archivo sin alterar el contenido existente |
| \| | Tubería o "Pipe" | Sistema | date \| tee -a log.txt | Toma la salida del comando de la izquierda y la inyecta como entrada al comando de la derecha |
| tee | Lee de la entrada estándar y escribe en un archivo y en la terminal | Archivos | tee -a datos.log | La bandera `-a` (append) funciona igual que `>>`, añadiendo el texto sin sobrescribir |
| date | Muestra la fecha y hora del sistema | Sistema | date | Frecuentemente inyectado en logs mediante tuberías |

## Comandos de Administración, Usuarios y Permisos

Comandos orientados a la seguridad, el manejo de privilegios de usuario, la lectura de archivos protegidos y la modificación de atributos de cuentas.

| **_Comando_** | **_Uso_** | **_Aplicable a_** | **_Ejemplo_** | **_Notas_** |
|:---:|:---:|:---:|:---:|:---:|
| sudo | Ejecuta un comando con privilegios temporales de administrador (root) | Sistema | sudo cat /etc/shadow | Requerido para ver o modificar archivos críticos del sistema |
| cat | Muestra todo el contenido de un archivo directamente en la terminal | Archivos | cat datos.log | - |
| grep | Busca un texto o patrón específico dentro de un archivo o flujo | Archivos/Sistema | grep "usuario" shadow | Se puede combinar con tuberías (ej. `cat archivo \| grep "error"`) |
| chmod | Modifica los permisos de lectura, escritura y ejecución | Archivos/Directorios | chmod +x hello.sh | `+x` otorga permisos de ejecución al propietario, grupo y otros |
| chage | Modifica la información de caducidad de contraseñas y cuentas | Usuarios | sudo chage -E "2026-06-30" user | El parámetro `-E` (Expire) establece la fecha exacta en que la cuenta dejará de funcionar |

## Comandos de Gestión de Procesos y Paquetes

Herramientas para monitorear qué programas se están ejecutando, gestionar el consumo de recursos de la máquina e instalar software nuevo.

| **_Comando_** | **_Uso_** | **_Aplicable a_** | **_Ejemplo_** | **_Notas_** |
|:---:|:---:|:---:|:---:|:---:|
| top | Muestra los procesos del sistema en tiempo real de forma interactiva | Procesos | top | Presiona la tecla `q` para salir del visor interactivo |
| ps | Muestra una captura estática de los procesos actuales | Procesos | ps aux | `aux` muestra absolutamente todos los procesos de todos los usuarios de forma detallada |
| kill | Envía una señal (por defecto, terminar) a un proceso específico | Procesos | kill 1234 | Requiere el PID (Process ID). Usar `kill -9 PID` fuerza el cierre inmediato del proceso |
| apt | Gestor de paquetes para distribuciones basadas en Debian/Ubuntu | Sistema | sudo apt install lynx | Se debe usar `sudo apt update` primero para actualizar la lista de repositorios |