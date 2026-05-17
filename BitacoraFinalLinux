# 📋 Bitácora de Comandos de Administración de Sistemas (SysAdmin)

En esta bitacora estàn todos los comandos vistos en los laboratorios de Linux.

## 1. Gestión de Archivos y Directorios

| Comando | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `ls` / `ls -l` | Lista los archivos en un directorio. Con `-l` muestra formato detallado (permisos, propietario, tamaño). | `ls -l /etc/apache2/` |
| `cd <ruta>` | Cambia el directorio de trabajo actual al destino especificado. | `cd /var/log/` |
| `mkdir` / `mkdir -p` | Crea nuevos directorios. La bandera `-p` genera toda la estructura anidada automáticamente si no existe. | `mkdir -p /home/servicios/tecno1` |
| `cp <origen> <destino>` | Copia archivos o directorios de una ruta a otra. | `cp /etc/dhcp/dhcpd.conf ~/respaldo/` |
| `cat <archivo>` | Concatena y muestra el contenido completo de uno o más archivos en la consola. | `cat /etc/network/interfaces` |
| `nano` / `vi` | Editores de texto en terminal utilizados para crear y modificar scripts o archivos de configuración. | `nano /etc/vsftpd.conf` |
| `ln -s <origen> <enlace>` | Crea un enlace simbólico (acceso directo lógico) hacia un archivo o directorio. | `ln -s /etc/nginx/sites-available/web /etc/nginx/sites-enabled/` |
| `tail -n <num> <archivo>`| Muestra las últimas líneas de un archivo (ideal para monitorear logs en tiempo real). | `tail -n 20 /var/log/syslog` |
| `echo "texto" >` o `>>` | Imprime texto. Con `>` sobrescribe por completo el archivo, con `>>` añade el texto al final del archivo. | `echo "" > ~/.bash_history` |

---

## 2. Criptografía y Certificados SSL/TLS (Hardening)

En entornos de producción, asegurar el canal de control y de datos mediante el cifrado es crítico. El siguiente comando maestro permite la generación de la infraestructura criptográfica base de forma autónoma:

| Comando Principal | Descripción y Desglose de Parámetros | Ejemplo de Uso |
| :--- | :--- | :--- |
| `openssl req -x509...` | Genera un certificado autofirmado y su respectiva llave privada RSA en un solo paso para securizar servicios web (HTTPS) o de transferencia (FTPS).

---

## 3. Gestión de Servicios (Systemd)

Administración del ciclo de vida de los demonios (daemons) y servicios del sistema operativo.

| Comando | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `systemctl start` | Levanta o inicia un servicio de forma inmediata en la sesión actual. | `systemctl start isc-dhcp-server` |
| `systemctl enable` | Habilita el servicio para que arranque automáticamente durante el proceso de arranque (boot) del servidor. | `systemctl enable apache2` |
| `systemctl status` | Verifica el estado de ejecución, PID y logs recientes del servicio (esencial para depurar errores de sintaxis). | `systemctl status vsftpd` |
| `systemctl restart`| Reinicia el servicio. Es un paso obligatorio para aplicar cambios realizados en archivos de configuración `.conf`. | `systemctl restart nginx` |
| `systemctl stop` | Detiene la ejecución de un servicio en ejecución de forma segura. | `systemctl stop isc-dhcp-server` |

---

## 4. Contenedores y Entornos (Docker)

Comandos para la gestión de ciclo de vida de contenedores, imágenes y microservicios.

| Comando | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `docker pull` | Descarga una imagen específica (oficial o de terceros) desde Docker Hub hacia el almacenamiento local del host. | `docker pull nginx:latest` |
| `docker run` | Crea e inicia un contenedor a partir de una imagen. Flags clave: `-d` (segundo plano), `-p` (mapeo de puertos host:contenedor), `--name` (identificador único). | `docker run -d -p 80:80 --name web_nginx nginx` |
| `docker ps` / `-a` | Lista los contenedores activos. Con el modificador `-a` incluye también aquellos que se encuentran detenidos. | `docker ps -a` |
| `docker stop` / `start` | Detiene la ejecución o vuelve a iniciar un contenedor existente sin alterar su configuración interna. | `docker stop web_nginx` |
| `docker rm` | Elimina un contenedor del almacenamiento local (requiere estar detenido o usar `-f` para forzar). | `docker rm -f web_nginx` |
| `docker images` | Muestra un inventario detallado de todas las imágenes descargadas en el servidor local. | `docker images` |
| `docker rmi` | Elimina una imagen local para liberar espacio en disco (no debe estar en uso por ningún contenedor). | `docker rmi nginx:latest` |
| `docker exec -it` | Abre un canal interactivo (`-it`) con una shell interna dentro de un contenedor en ejecución para tareas de administración. | `docker exec -it web_nginx /bin/bash` |
| `docker logs` | Despliega los registros de salida estándar (stdout/stderr) del contenedor. Crucial para auditoría de hosting. | `docker logs -f web_nginx` |
| `docker compose up -d`| Despliega de forma orquestada toda una infraestructura multicontenedor definida en un archivo `docker-compose.yml`. | `docker compose up -d` |

---

## 5. Gestión de Usuarios, Permisos y Entorno

Control de acceso basado en usuarios, grupos, caducidad de cuentas y variables del entorno shell.

| Comando | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `useradd` | Crea cuentas de usuario parametrizando el directorio home (`-m`), la ruta (`-d`), el grupo primario (`-g`), metadatos (`-c`) y expiración (`-e`). | `useradd -m -d /home/servicios/tecno1 -g tecnologos -c "SysAdmin" usuario1` |
| `groupadd` | Crea un nuevo grupo lógico en el sistema para la asignación de permisos compartidos. | `groupadd tecnologos` |
| `passwd <usuario>` | Asigna o modifica de forma segura la contraseña encriptada de una cuenta de usuario. | `passwd usuario1` |
| `chage` | Configura políticas de caducidad y cambio obligatorio de contraseñas (`-M` fija la vigencia máxima en días). | `chage -M 90 usuario1` |
| `chmod` | Modifica los permisos de acceso (lectura, escritura, ejecución) sobre archivos o scripts. | `chmod +x script_backup.sh` |
| `su -` / `sudo su -` | Conmuta la sesión actual a la de otro usuario o al superusuario (root) cargando su entorno completo de variables. | `sudo su -` |
| `export` | Define o exporta variables de entorno globales válidas para la sesión de la shell (ej. configuraciones de historial). | `export HISTSIZE=200` |

---

## 6. Redes y Conectividad (Linux y Windows)

Herramientas de diagnóstico de red, direccionamiento IP interfaces y verificación de conectividad.

| Comando | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `ipconfig /release` | **(Windows)** Libera la concesión de la dirección IP actual asignada por un servidor DHCP en la interfaz activa. | `ipconfig /release` |
| `ipconfig /renew` | **(Windows)** Envía una solicitud de renovación (Request) para obtener parámetros IP desde el servidor DHCP. | `ipconfig /renew` |
| `ip addr show` | Despliega la configuración detallada de direccionamiento, máscaras y estados de las interfaces de red en Linux. | `ip addr show eth0` |
| `ip route show` | Muestra la tabla de enrutamiento activa del Kernel de Linux (rutas estáticas, dinámicas y pasarelas por defecto). | `ip route show` |
| `ping <destino>` | Envía paquetes de eco ICMP hacia un host remoto para diagnosticar la conectividad y latencia a nivel de red. | `ping 8.8.8.8` |
| `hostname -I`| Muestra de forma simplificada todas las direcciones IP lógicas asociadas a las interfaces del servidor. | `hostname -I` |
| `nmap --script ...` | Realiza auditorías de red. El script `broadcast-dhcp-discover` envía un paquete de descubrimiento global para mapear servidores DHCP. | `nmap --script broadcast-dhcp-discover` |

---

## 7. Procesos, Análisis de Sistema y Paquetería

Comandos de monitoreo de recursos de hardware, flujos de datos en terminal y manipulación de procesos.

| Comando | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `apt install` | Gestor de paquetes avanzado de distribuciones basadas en Debian/Ubuntu para instalar software de repositorios oficiales. | `apt install nmap vsftpd` |
| `history` | Administra y visualiza el registro histórico de comandos ejecutados en la terminal. El flag `-c` limpia la caché. | `history -c` |
| `find <ruta> -name` | Realiza búsquedas de archivos en tiempo real dentro de una estructura jerárquica aplicando filtros de nombre. | `find /var/log -name "*.log"` |
| `grep <patrón>` | Filtra y extrae líneas de texto que coincidan exactamente con una expresión regular o patrón específico. | `grep "root" /etc/passwd` |
| `sed 's/texto//'` | Editor de flujo no interactivo que transforma, busca o reemplaza cadenas directamente sobre tuberías o archivos. | `sed 's/localhost/127.0.0.1/' archivo.txt` |
| `free -h` | Muestra el estado de consumo, disponibilidad y buffers de la memoria RAM y Swap con unidades legibles. | `free -h` |
| `df -h` | Informa sobre el espacio total, utilizado y disponible en los diferentes sistemas de archivos y discos montados. | `df -h` |
| `ps aux` | Lista de forma estática una captura de todos los procesos en ejecución con detalles de usuario, CPU, memoria y comando base. | `ps aux | grep apache2` |
| `kill` / `kill -9` | Envía señales de terminación a los procesos a través de su PID. La señal `-9` (SIGKILL) fuerza el cierre inmediato. | `kill -9 1450` |
| `jobs` | Lista las tareas asociadas de forma directa al ciclo de control de la sesión de terminal actual en segundo plano. | `jobs` |
| `sleep <seg> &` | Introduce un retraso programado en segundos. El operador `&` desplaza su ejecución al background (segundo plano). | `sleep 60 &` |

---

## 8. Compresión y Transferencia de Archivos

Empaquetamiento seguro de datos, resguardos (backups) y utilidades de transferencia entre hosts locales y remotos.

| Comando | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `tar -cvzf` | Empaqueta y comprime directorios masivos aplicando el algoritmo gzip en un archivo unificado `.tar.gz`. | `tar -cvzf backup_web.tar.gz /var/www/html/` |
| `scp` | Copia archivos entre nodos de red de manera cifrada e íntegra utilizando el canal de transporte de SSH. | `scp archivo.zip usuario@192.168.1.10:/home/` |
| `get` / `put` | Comandos interactivos dentro de una sesión cliente FTP convencional para descargar (`get`) o subir (`put`) archivos. | `put index.html` |
