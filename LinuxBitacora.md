# Bitácora de Comandos NDG Linux Unhatched

## Índice

- [Comandos de Navegación del Sistema](#comandos-de-navegación-del-sistema)
- [Comandos de Gestión de Archivos y Directorios](#comandos-de-gestión-de-archivos-y-directorios)
- [Comandos de Búsqueda y Visualización](#comandos-de-búsqueda-y-visualización)
- [Comandos de Administración de Permisos y Redes](#comandos-de-administración-de-permisos-y-redes)

---

## Comandos de Navegación del Sistema

Los siguientes comandos son fundamentales para moverse a través del árbol de directorios de Linux. Permiten conocer tu ubicación actual, listar elementos y cambiar de directorios de trabajo en la terminal.

|         **_Comando_** |                                                 **_Uso_** | **_Aplicable a_** |              **_Ejemplo_** |                                               **_Notas_** |
|:----------------------------:|:--------------------------------------------------------------------------------------------------------:|:-----------------:|:--------------------------------------:|:-------------------------------------------------------------------------------------------------------:|
| pwd                          | Imprime la ruta absoluta del directorio de trabajo actual                                                |      Sistema      | pwd                                    | -                                                                                                       |
| ls                           | Lista el contenido de un directorio                                                                      |    Directorios    | ls -la                                 | El parámetro `-l` muestra detalles (permisos, propietario) y `-a` incluye archivos ocultos              |
| cd                           | Cambia el directorio de trabajo actual                                                                   |      Sistema      | cd /etc/                               | Usar `cd ..` permite subir un nivel en el árbol de directorios                                          |

## Comandos de Gestión de Archivos y Directorios

Esta sección reúne los comandos esenciales para la creación, copia, movimiento y eliminación de elementos en el sistema de archivos de Linux. Es vital tener precaución con los comandos de eliminación, ya que en Linux no existe una papelera de reciclaje por defecto en la terminal.

|         **_Comando_** |                                                 **_Uso_** | **_Aplicable a_** |              **_Ejemplo_** |                                               **_Notas_** |
|:----------------------------:|:--------------------------------------------------------------------------------------------------------:|:-----------------:|:--------------------------------------:|:-------------------------------------------------------------------------------------------------------:|
| mkdir                        | Crea un nuevo directorio (carpeta)                                                                       |    Directorios    | mkdir scripts                          | -                                                                                                       |
| touch                        | Crea un archivo vacío o actualiza su fecha de modificación                                               |      Archivos     | touch notas.txt                        | Útil para crear archivos rápidamente sin abrir un editor de texto                                       |
| cp                           | Copia archivos o directorios                                                                             | Archivos/Directorios| cp archivo.txt /backup/                | Para copiar directorios completos se debe utilizar la bandera `-r` (recursivo)                          |
| mv                           | Mueve o renombra archivos y directorios                                                                  | Archivos/Directorios| mv viejo.txt nuevo.txt                 | Se utiliza tanto para mover de ruta como para cambiar el nombre de un elemento en la misma ruta         |
| rm                           | Elimina archivos                                                                                         | Archivos/Directorios| rm -rf temporal/                       | Con la bandera `-r` elimina directorios y su contenido; `-f` fuerza la eliminación sin preguntar        |

## Comandos de Búsqueda y Visualización

Herramientas utilizadas para leer el contenido de los archivos de texto directamente desde la consola y buscar cadenas de caracteres específicas dentro de ellos o en la jerarquía del sistema.

|         **_Comando_** |                                                 **_Uso_** | **_Aplicable a_** |              **_Ejemplo_** |                                               **_Notas_** |
|:----------------------------:|:--------------------------------------------------------------------------------------------------------:|:-----------------:|:--------------------------------------:|:-------------------------------------------------------------------------------------------------------:|
| cat                          | Muestra todo el contenido de un archivo directamente en la terminal                                      |      Archivos     | cat config.txt                         | Ideal para archivos pequeños. Si el archivo es muy largo, el texto pasará muy rápido                    |
| less                         | Muestra el contenido de un archivo extenso página por página                                             |      Archivos     | less syslog                            | Presiona la tecla `q` para salir del visor interactivo                                                  |
| grep                         | Busca un texto o patrón específico dentro de un archivo                                                  |      Archivos     | grep "error" server.log                | Distingue entre mayúsculas y minúsculas por defecto                                                     |
| find                         | Busca archivos dentro de la jerarquía de directorios del sistema                                         |      Sistema      | find / -name "script.sh"               | Puede buscar por nombre, tamaño, fecha de modificación y propietario                                    |

## Comandos de Administración de Permisos y Redes

Comandos básicos orientados a la seguridad, el manejo de privilegios de usuario y la verificación de la conectividad de red del equipo.

|         **_Comando_** |                                                 **_Uso_** | **_Aplicable a_** |              **_Ejemplo_** |                                               **_Notas_** |
|:----------------------------:|:--------------------------------------------------------------------------------------------------------:|:-----------------:|:--------------------------------------:|:-------------------------------------------------------------------------------------------------------:|
| ping                         | Prueba la conectividad de red enviando paquetes ICMP a otro host                                         |        Red        | ping 8.8.8.8                           | A diferencia de Windows, en Linux el ping es infinito por defecto; usa `Ctrl+C` para detenerlo          |
| chmod                        | Modifica los permisos de lectura, escritura y ejecución                                                  | Archivos/Directorios| chmod +x script.sh                     | Los permisos también se pueden asignar utilizando notación octal (ej: `chmod 755`)                      |
| chown                        | Cambia el usuario propietario o el grupo de un archivo                                                   | Archivos/Directorios| chown root:root config.yml             | -                                                                                                       |
| sudo                         | Ejecuta un comando con privilegios temporales de administrador (root)                                    |      Sistema      | sudo apt update                        | Te solicitará la contraseña de tu usuario (si está en el archivo sudoers) y no se mostrará al escribirla|
| man                          | Abre el manual detallado de cualquier comando para ver todas sus opciones                                |      Sistema      | man grep                               | Es la documentación oficial del sistema. Presiona la tecla `q` para salir                               |
| exit                         | Cierra la sesión actual de la terminal                                                                   |      Sistema      | exit                                   | -                                                                                                       |