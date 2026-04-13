# Bitácora de Comandos de Laboratorio 3 (Kubuntu / Debian Shell)

## Índice

- [Comandos de Entorno y Navegación](#comandos-de-entorno-y-navegación)
- [Comandos de Red y Conectividad](#comandos-de-red-y-conectividad)
- [Comandos de Gestión de Archivos y Compresión](#comandos-de-gestión-de-archivos-y-compresión)
- [Operadores de Redirección y Tuberías](#operadores-de-redirección-y-tuberías)
- [Comandos de Administración, Usuarios y Permisos](#comandos-de-administración-usuarios-y-permisos)
- [Comandos de Gestión de Procesos y Paquetes](#comandos-de-gestión-de-procesos-y-paquetes)

---

## Comandos de Entorno y Navegación

Los siguientes comandos son fundamentales para moverse a través del árbol de directorios de Linux, conocer tu ubicación actual, listar elementos, revisar el historial y gestionar las variables del entorno.

| **_Comando_** | **_Uso_** | **_Aplicable a_** | **_Ejemplo_** | **_Notas_** |
|:---:|:---:|:---:|:---:|:---:|
| pwd | Imprime la ruta absoluta del directorio de trabajo actual | Sistema | pwd | - |
| cd | Cambia el directorio de trabajo actual | Sistema | cd /etc/ | Usar `cd /` lleva directamente al directorio raíz del sistema |
| ls | Lista el contenido de un directorio | Directorios | ls -l | El parámetro `-l` muestra una lista detallada (permisos, propietario, tamaño y fecha) |
| history | Muestra o manipula el historial de comandos ejecutados | Sistema | history -c | La bandera `-c` (clear) limpia el historial de la sesión actual en la memoria RAM |
| export | Establece o modifica variables de entorno para la sesión del shell | Sistema | export HISTSIZE=150 | Usado extensamente para configurar parámetros de auditoría como `HISTTIMEFORMAT` o `HISTCONTROL` |
| neofetch | Muestra información visual del sistema operativo y hardware | Sistema | neofetch | Ideal para mostrar rápidamente la distribución y detalles del kernel |

## Comandos de Red y Conectividad

Herramientas indispensables para el diagnóstico de interfaces, configuración de enrutamiento y accesos remotos seguros a servidores VPS.

| **_Comando_** | **_Uso_** | **_Aplicable a_** | **_Ejemplo_** | **_Notas_** |
|:---:|:---:|:---:|:---:|:---:|
| ip | Muestra o manipula interfaces de red, direcciones IP y enrutamiento | Redes | ip a | Reemplaza al comando antiguo `ifconfig`. Muestra el estado (UP/DOWN) y las IPs asignadas |
| ssh | Conexión remota segura a otro equipo mediante Secure Shell | Redes | ssh sysadmin@172.20.10.51 | Permite administrar servidores sin interfaz gráfica de forma cifrada |
| scp | Copia archivos de forma segura entre hosts usando el protocolo SSH | Redes | scp log.txt user@ip:~/ | Funciona como `cp`, pero a través de la red. Ideal para extraer respaldos |
| ss | Muestra estadísticas detalladas de sockets y conexiones de red | Redes | ss -ltunp | Parámetros clave: `-l` (listening), `-t` (tcp), `-u` (udp), `-n` (numérico), `-p` (procesos/PID) |
| hostnamectl | Consulta o cambia el nombre del host (identificador) del sistema | Sistema | sudo hostnamectl set-hostname webserver1 | Aplica el cambio de nombre de la máquina de forma inmediata sin reiniciar |

## Comandos de Gestión de Archivos y Compresión

Esta sección reúne los comandos esenciales para la creación, copia, descarga y empaquetado/compresión de elementos en el sistema de archivos de Linux.

| **_Comando_** | **_Uso_** | **_Aplicable a_** | **_Ejemplo_** | **_Notas_** |
|:---:|:---:|:---:|:---:|:---:|
| mkdir | Crea un nuevo directorio (carpeta) | Directorios | mkdir -p /var/www/html/web | La bandera `-p` crea la ruta completa si los directorios padres no existen |
| cp | Copia archivos o directorios | Archivos/Directorios | cp /etc/*.conf /home/ | Se puede usar el comodín `*` para copiar múltiples archivos con la misma extensión |
| mv | Mueve o renombra archivos y directorios | Archivos/Directorios | mv carpetaVieja carpetaNueva | Si el destino está en el mismo directorio, la acción que se realiza es renombrar |
| wget | Descarga archivos desde la web (HTTP, HTTPS, FTP) por consola | Archivos | wget https://sitio.com/web.zip | Ideal para descargar plantillas, scripts o instaladores directamente al servidor |
| tar | Empaqueta y comprime archivos en un único archivo de salida | Archivos/Directorios | tar -czvf respaldo.tar.gz | Parámetros: `-c` (crear), `-z` (gzip), `-v` (verbose), `-f` (archivo) |
| unzip | Descomprime archivos comprimidos en formato .zip | Archivos | unzip web.zip -d /var/www/ | El parámetro `-d` permite especificar la ruta exacta donde se extraerá el contenido |

## Operadores de Redirección y Tuberías

Herramientas y caracteres especiales (conectores) que permiten desviar la salida estándar de un comando hacia un archivo, o conectar múltiples comandos entre sí.

| **_Comando_** | **_Uso_** | **_Aplicable a_** | **_Ejemplo_** | **_Notas_** |
|:---:|:---:|:---:|:---:|:---:|
| echo | Imprime un texto o cadena de caracteres en la salida estándar | Sistema | echo "Hola" | Se usa frecuentemente junto con operadores de redirección para escribir en archivos |
| > | Redirección de salida (Sobrescribe) | Archivos | history > logs.txt | Si el archivo no existe, lo crea. Si existe, **borra todo su contenido previo** y lo reemplaza |
| >> | Redirección de adición (Añade) | Archivos | echo "más" >> archivo.txt | Añade el nuevo texto al **final** del archivo sin alterar el contenido existente |
| \| | Tubería o "Pipe" | Sistema | date \| tee -a log.txt | Toma la salida del comando de la izquierda y la inyecta como entrada al comando de la derecha |
| tee | Lee de la entrada estándar y escribe en un archivo y en la terminal | Archivos | tee -a datos.log | La bandera `-a` (append) funciona igual que `>>`, añadiendo el texto sin sobrescribir |
| date | Muestra la fecha y hora del sistema | Sistema | date | Frecuentemente inyectado en logs mediante tuberías |

## Comandos de Administración, Usuarios y Permisos

Comandos orientados a la seguridad, el manejo de privilegios de usuario, la lectura y edición de archivos protegidos, y la modificación de atributos de cuentas.

| **_Comando_** | **_Uso_** | **_Aplicable a_** | **_Ejemplo_** | **_Notas_** |
|:---:|:---:|:---:|:---:|:---:|
| sudo | Ejecuta un comando con privilegios temporales de administrador (root) | Sistema | sudo cat /etc/shadow | Requerido para ver, instalar o modificar componentes críticos del sistema |
| adduser | Crea un nuevo usuario en el sistema de forma interactiva | Usuarios | sudo adduser sysadmin | Recomendado en Debian/Ubuntu porque crea automáticamente la carpeta `/home/` y pide contraseñas |
| usermod | Modifica las propiedades de una cuenta de usuario existente | Usuarios | sudo usermod -aG sudo sysadmin | `-aG` añade al usuario a un grupo suplementario (como `sudo` o `www-data`) sin sacarlo de otros grupos |
| nano | Editor de texto en terminal fácil de usar | Archivos | sudo nano /etc/network/interfaces | Herramienta principal para editar archivos de configuración. Guardar con `Ctrl+O`, salir con `Ctrl+X` |
| cat | Muestra todo el contenido de un archivo directamente en la terminal | Archivos | cat datos.log | - |
| grep | Busca un texto o patrón específico dentro de un archivo o flujo | Archivos/Sistema | grep "usuario" shadow | Se puede combinar con tuberías (ej. `cat archivo \| grep "error"`) |
| chmod | Modifica los permisos de lectura (r), escritura (w) y ejecución (x) | Archivos/Directorios | chmod 755 carpeta | El modo octal `755` otorga control total al propietario, y lectura/ejecución al resto |
| chage | Modifica la información de caducidad de contraseñas y cuentas | Usuarios | sudo chage -E "2026-06-30" user | El parámetro `-E` (Expire) establece la fecha exacta en que la cuenta dejará de funcionar |

## Comandos de Gestión de Procesos y Paquetes

Herramientas para monitorear programas, reiniciar servicios de red, gestionar el consumo de recursos e instalar software nuevo en el servidor.

| **_Comando_** | **_Uso_** | **_Aplicable a_** | **_Ejemplo_** | **_Notas_** |
|:---:|:---:|:---:|:---:|:---:|
| systemctl | Controla el sistema y el administrador de servicios (systemd) | Servicios | sudo systemctl restart apache2 | Comandos comunes: `start` (iniciar), `stop` (detener), `restart` (reiniciar) y `status` (estado actual) |
| top | Muestra los procesos del sistema en tiempo real de forma interactiva | Procesos | top | Presiona la tecla `q` para salir del visor interactivo |
| ps | Muestra una captura estática de los procesos actuales | Procesos | ps aux | `aux` muestra absolutamente todos los procesos de todos los usuarios de forma detallada |
| kill | Envía una señal (por defecto, terminar) a un proceso específico | Procesos | kill 1234 | Requiere el PID. Usar `kill -9 PID` fuerza el cierre inmediato del proceso |
| apt | Gestor de paquetes para distribuciones basadas en Debian/Ubuntu | Sistema | sudo apt install apache2 | Siempre se debe ejecutar `sudo apt update` primero para sincronizar la lista de repositorios |