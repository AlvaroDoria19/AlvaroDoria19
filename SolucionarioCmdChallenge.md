Soluciones a cmdchallenge (Command Challenge)

| N° | Descripción del Reto | Comando (Solución) |
|:---|:---|:---|
| 1 | Imprime "hello world" en la terminal. | `echo "hello world"` |
| 2 | Imprime el directorio de trabajo actual. | `pwd` |
| 3 | Lista los nombres de los archivos en el directorio actual. | `ls` |
| 4 | Imprime el contenido del archivo `access.log`. | `cat access.log` |
| 5 | Imprime las últimas 5 líneas del archivo `access.log`. | `tail -n 5 access.log` |
| 6 | Imprime las líneas de `access.log` que contengan el string "GET". | `grep GET access.log` |
| 7 | Cuenta cuántas líneas de `access.log` contienen "GET". | `grep -c GET access.log` |
| 8 | Lista los nombres de los archivos en el directorio actual que contienen "500". | `grep -l 500 *` |
| 9 | Lista los archivos en el directorio actual que empiezan con `access.log`. | `ls access.log*` |
| 10 | Lista los archivos en el directorio actual que terminan en `.txt`. | `ls *.txt` |
| 11 | Crea un archivo vacío llamado `take-the-command-challenge`. | `touch take-the-command-challenge` |
| 12 | Crea un directorio llamado `tmp/files`. | `mkdir -p tmp/files` |
| 13 | Copia el archivo `tf` al directorio `tmp/files`. | `cp tf tmp/files` |
| 14 | Mueve el archivo `tf` al directorio `tmp/files`. | `mv tf tmp/files` |
| 15 | Crea un enlace simbólico (symlink) llamado `symlink` hacia el archivo `tf`. | `ln -s tf symlink` |
| 16 | Borra todos los archivos del directorio actual. | `rm *` |
| 17 | Borra todos los archivos del directorio actual y de sus subdirectorios. | `rm -rf *` |
| 18 | Encuentra y borra todos los archivos con extensión `.doc` recursivamente. | `find . -name "*.doc" -delete` |
| 19 | Imprime todos los archivos del directorio actual, uno por línea. | `ls -1` |
| 20 | Imprime todos los archivos del directorio actual, uno por línea (excluyendo directorios). | `find . -maxdepth 1 -type f -printf "%f\n"` |
| 21 | Imprime el número de archivos en el directorio actual. | `find . -maxdepth 1 -type f \| wc -l` |
| 22 | Imprime el contenido de `access.log` ordenado alfabéticamente. | `sort access.log` |
| 23 | Imprime el número de líneas totales que tiene `access.log`. | `wc -l < access.log` |
| 24 | Extrae y muestra solo las direcciones IP de `access.log`. | `awk '{print $1}' access.log` |
| 25 | Reemplaza la primera instancia de "hate" con "love" en `text.txt`. | `sed 's/hate/love/' text.txt` |
| 26 | Reemplaza todas las instancias de "hate" con "love" en `text.txt`. | `sed 's/hate/love/g' text.txt` |
| 27 | Imprime los números del 1 al 100, uno por línea. | `seq 1 100` |
| 28 | Encuentra archivos que han sido modificados en los últimos 7 días. | `find . -mtime -7` |
| 29 | Muestra el tamaño de un directorio y su contenido de forma legible. | `du -sh` |
| 30 | Lista los procesos que están corriendo en el sistema. | `ps aux` |
