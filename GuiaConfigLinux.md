```markdown
# Bitácora de Comandos de Administración de Sistemas y Configuración de Servicios

En esta bitacora estàn todos los comandos vistos en los laboratorios de Linux.[cite: 1]

## 1. Gestión de Archivos y Directorios[cite: 1]

| Comando | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `ls` / `ls -l` | Lista los archivos en un directorio. Con `-l` muestra formato detallado (permisos, propietario, tamaño).[cite: 1] | `ls -l /etc/apache2/`[cite: 1] |
| `cd <ruta>` | Cambia el directorio de trabajo actual al destino especificado.[cite: 1] | `cd /var/log/`[cite: 1] |
| `mkdir` / `mkdir -p` | Crea nuevos directorios. La bandera `-p` genera toda la estructura anidada automáticamente si no existe.[cite: 1] | `mkdir -p /home/servicios/tecno1`[cite: 1] |
| `cp <origen> <destino>` | Copia archivos o directorios de una ruta a otra.[cite: 1] | `cp /etc/dhcp/dhcpd.conf ~/respaldo/`[cite: 1] |
| `cat <archivo>` | Concatena y muestra el contenido completo de uno o más archivos en la consola.[cite: 1] | `cat /etc/network/interfaces`[cite: 1] |
| `nano` / `vi` | Editores de texto en terminal utilizados para crear y modificar scripts o archivos de configuración.[cite: 1] | `nano /etc/vsftpd.conf`[cite: 1] |
| `ln -s <origen> <enlace>` | Crea un enlace simbólico (acceso directo lógico) hacia un archivo o directorio.[cite: 1] | `ln -s /etc/nginx/sites-available/web /etc/nginx/sites-enabled/`[cite: 1] |
| `tail -n <num> <archivo>`| Muestra las últimas líneas de un archivo (ideal para monitorear logs en tiempo real).[cite: 1] | `tail -n 20 /var/log/syslog`[cite: 1] |
| `echo "texto" >` o `>>` | Imprime texto. Con `>` sobrescribe por completo el archivo, con `>>` añade el texto al final del archivo.[cite: 1] | `echo "" > ~/.bash_history`[cite: 1] |

---

## 2. Criptografía y Certificados SSL/TLS (Hardening)[cite: 1]

En entornos de producción, asegurar el canal de control y de datos mediante el cifrado es crítico. El siguiente comando maestro permite la generación de la infraestructura criptográfica base de forma autónoma:[cite: 1]

| Comando Principal | Descripción y Desglose de Parámetros | Ejemplo de Uso |
| :--- | :--- | :--- |
| `openssl req -x509...` | Genera un certificado autofirmado y su respectiva llave privada RSA en un solo paso para securizar servicios web (HTTPS) o de transferencia (FTPS).[cite: 1] | N/A |

---

## 3. Gestión de Servicios (Systemd)[cite: 1]

Administración del ciclo de vida de los demonios (daemons) y servicios del sistema operativo.[cite: 1]

| Comando | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `systemctl start` | Levanta o inicia un servicio de forma inmediata en la sesión actual.[cite: 1] | `systemctl start isc-dhcp-server`[cite: 1] |
| `systemctl enable` | Habilita el servicio para que arranque automáticamente durante el proceso de arranque (boot) del servidor.[cite: 1] | `systemctl enable apache2`[cite: 1] |
| `systemctl status` | Verifica el estado de ejecución, PID y logs recientes del servicio (esencial para depurar errores de sintaxis).[cite: 1] | `systemctl status vsftpd`[cite: 1] |
| `systemctl restart`| Reinicia el servicio. Es un paso obligatorio para aplicar cambios realizados en archivos de configuración `.conf`.[cite: 1] | `systemctl restart nginx`[cite: 1] |
| `systemctl stop` | Detiene la ejecución de un servicio en ejecución de forma segura.[cite: 1] | `systemctl stop isc-dhcp-server`[cite: 1] |

---

## 4. Contenedores y Entornos (Docker)[cite: 1]

Comandos para la gestión de ciclo de vida de contenedores, imágenes y microservicios.[cite: 1]

| Comando | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `docker pull` | Descarga una imagen específica (oficial o de terceros) desde Docker Hub hacia el almacenamiento local del host.[cite: 1] | `docker pull nginx:latest`[cite: 1] |
| `docker run` | Crea e inicia un contenedor a partir de una imagen. Flags clave: `-d` (segundo plano), `-p` (mapeo de puertos host:contenedor), `--name` (identificador único).[cite: 1] | `docker run -d -p 80:80 --name web_nginx nginx`[cite: 1] |
| `docker ps` / `-a` | Lista los contenedores activos. Con el modificador `-a` incluye también aquellos que se encuentran detenidos.[cite: 1] | `docker ps -a`[cite: 1] |
| `docker stop` / `start` | Detiene la ejecución o vuelve a iniciar un contenedor existente sin alterar su configuración interna.[cite: 1] | `docker stop web_nginx`[cite: 1] |
| `docker rm` | Elimina un contenedor del almacenamiento local (requiere estar detenido o usar `-f` para forzar).[cite: 1] | `docker rm -f web_nginx`[cite: 1] |
| `docker images` | Muestra un inventario detallado de todas las imágenes descargadas en el servidor local.[cite: 1] | `docker images`[cite: 1] |
| `docker rmi` | Elimina una imagen local para liberar espacio en disco (no debe estar en uso por ningún contenedor).[cite: 1] | `docker rmi nginx:latest`[cite: 1] |
| `docker exec -it` | Abre un canal interactivo (`-it`) con una shell interna dentro de un contenedor en ejecución para tareas de administración.[cite: 1] | `docker exec -it web_nginx /bin/bash`[cite: 1] |
| `docker logs` | Despliega los registros de salida estándar (stdout/stderr) del contenedor. Crucial para auditoría de hosting.[cite: 1] | `docker logs -f web_nginx`[cite: 1] |
| `docker compose up -d`| Despliega de forma orquestada toda una infraestructura multicontenedor definida en un archivo `docker-compose.yml`.[cite: 1] | `docker compose up -d`[cite: 1] |

---

## 5. Gestión de Usuarios, Permisos y Entorno[cite: 1]

Control de acceso basado en usuarios, grupos, caducidad de cuentas y variables del entorno shell.[cite: 1]

| Comando | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `useradd` | Crea cuentas de usuario parametrizando el directorio home (`-m`), la ruta (`-d`), el grupo primario (`-g`), metadatos (`-c`) y expiración (`-e`).[cite: 1] | `useradd -m -d /home/servicios/tecno1 -g tecnologos -c "SysAdmin" usuario1`[cite: 1] |
| `groupadd` | Crea un nuevo grupo lógico en el sistema para la asignación de permisos compartidos.[cite: 1] | `groupadd tecnologos`[cite: 1] |
| `passwd <usuario>` | Asigna o modifica de forma segura la contraseña encriptada de una cuenta de usuario.[cite: 1] | `passwd usuario1`[cite: 1] |
| `chage` | Configura políticas de caducidad y cambio obligatorio de contraseñas (`-M` fija la vigencia máxima en días).[cite: 1] | `chage -M 90 usuario1`[cite: 1] |
| `chmod` | Modifica los permisos de acceso (lectura, escritura, ejecución) sobre archivos o scripts.[cite: 1] | `chmod +x script_backup.sh`[cite: 1] |
| `su -` / `sudo su -` | Conmuta la sesión actual a la de otro usuario o al superusuario (root) cargando su entorno completo de variables.[cite: 1] | `sudo su -`[cite: 1] |
| `export` | Define o exporta variables de entorno globales válidas para la sesión de la shell (ej. configuraciones de historial).[cite: 1] | `export HISTSIZE=200`[cite: 1] |

---

## 6. Redes y Conectividad (Linux y Windows)[cite: 1]

Herramientas de diagnóstico de red, direccionamiento IP interfaces y verificación de conectividad.[cite: 1]

| Comando | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `ipconfig /release` | **(Windows)** Libera la concesión de la dirección IP actual asignada por un servidor DHCP en la interfaz activa.[cite: 1] | `ipconfig /release`[cite: 1] |
| `ipconfig /renew` | **(Windows)** Envía una solicitud de renovación (Request) para obtener parámetros IP desde el servidor DHCP.[cite: 1] | `ipconfig /renew`[cite: 1] |
| `ip addr show` | Despliega la configuración detallada de direccionamiento, máscaras y estados de las interfaces de red en Linux.[cite: 1] | `ip addr show eth0`[cite: 1] |
| `ip route show` | Muestra la tabla de enrutamiento activa del Kernel de Linux (rutas estáticas, dinámicas y pasarelas por defecto).[cite: 1] | `ip route show`[cite: 1] |
| `ping <destino>` | Envía paquetes de eco ICMP hacia un host remoto para diagnosticar la conectividad y latencia a nivel de red.[cite: 1] | `ping 8.8.8.8`[cite: 1] |
| `hostname -I`| Muestra de forma simplificada todas las direcciones IP lógicas asociadas a las interfaces del servidor.[cite: 1] | `hostname -I`[cite: 1] |
| `nmap --script ...` | Realiza auditorías de red. El script `broadcast-dhcp-discover` envía un paquete de descubrimiento global para mapear servidores DHCP.[cite: 1] | `nmap --script broadcast-dhcp-discover`[cite: 1] |

---

## 7. Procesos, Análisis de Sistema y Paquetería[cite: 1]

Comandos de monitoreo de recursos de hardware, flujos de datos en terminal y manipulación de procesos.[cite: 1]

| Comando | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `apt install` | Gestor de paquetes avanzado de distribuciones basadas en Debian/Ubuntu para instalar software de repositorios oficiales.[cite: 1] | `apt install nmap vsftpd`[cite: 1] |
| `history` | Administra y visualiza el registro histórico de comandos ejecutados en la terminal. El flag `-c` limpia la caché.[cite: 1] | `history -c`[cite: 1] |
| `find <ruta> -name` | Realiza búsquedas de archivos en tiempo real dentro de una estructura jerárquica aplicando filtros de nombre.[cite: 1] | `find /var/log -name "*.log"`[cite: 1] |
| `grep <patrón>` | Filtra y extrae líneas de texto que coincidan exactamente con una expresión regular o patrón específico.[cite: 1] | `grep "root" /etc/passwd`[cite: 1] |
| `sed 's/texto//'` | Editor de flujo no interactivo que transforma, busca o reemplaza cadenas directamente sobre tuberías o archivos.[cite: 1] | `sed 's/localhost/127.0.0.1/' archivo.txt`[cite: 1] |
| `free -h` | Muestra el estado de consumo, disponibilidad y buffers de la memoria RAM y Swap con unidades legibles.[cite: 1] | `free -h`[cite: 1] |
| `df -h` | Informa sobre el espacio total, utilizado y disponible en los diferentes sistemas de archivos y discos montados.[cite: 1] | `df -h`[cite: 1] |
| `ps aux` | Lista de forma estática una captura de todos los procesos en ejecución con detalles de usuario, CPU, memoria y comando base.[cite: 1] | `ps aux | grep apache2`[cite: 1] |
| `kill` / `kill -9` | Envía señales de terminación a los procesos a través de su PID. La señal `-9` (SIGKILL) fuerza el cierre inmediato.[cite: 1] | `kill -9 1450`[cite: 1] |
| `jobs` | Lista las tareas asociadas de forma directa al ciclo de control de la sesión de terminal actual en segundo plano.[cite: 1] | `jobs`[cite: 1] |
| `sleep <seg> &` | Introduce un retraso programado en segundos. El operador `&` desplaza su ejecución al background (segundo plano).[cite: 1] | `sleep 60 &`[cite: 1] |

---

## 8. Compresión y Transferencia de Archivos[cite: 1]

Empaquetamiento seguro de datos, resguardos (backups) y utilidades de transferencia entre hosts locales y remotos.[cite: 1]

| Comando | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `tar -cvzf` | Empaqueta y comprime directorios masivos aplicando el algoritmo gzip en un archivo unificado `.tar.gz`.[cite: 1] | `tar -cvzf backup_web.tar.gz /var/www/html/`[cite: 1] |
| `scp` | Copia archivos entre nodos de red de manera cifrada e íntegra utilizando el canal de transporte de SSH.[cite: 1] | `scp archivo.zip usuario@192.168.1.10:/home/`[cite: 1] |
| `get` / `put` | Comandos interactivos dentro de una sesión cliente FTP convencional para descargar (`get`) o subir (`put`) archivos.[cite: 1] | `put index.html`[cite: 1] |

---

## 9. Servicio DHCP (isc-dhcp-server)

Configuración del servidor para la asignación dinámica de direcciones IP y parámetros de red.

| Comando / Archivo | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `apt install isc-dhcp-server` | Instala el paquete del servidor DHCP de ISC desde los repositorios oficiales de Debian. | `apt install isc-dhcp-server` |
| `/etc/default/isc-dhcp-server`| Archivo donde se define la interfaz de red lógica por la cual el servidor escuchará peticiones. | `nano /etc/default/isc-dhcp-server` |
| `/etc/dhcp/dhcpd.conf` | Archivo principal de configuración. Aquí se declara el pool de IPs, subredes, tiempos de concesión y enrutadores. | `nano /etc/dhcp/dhcpd.conf` |
| `dhcpd -t` | Verifica la sintaxis del archivo de configuración antes de reiniciar el servicio, evitando caídas por errores tipográficos. | `dhcpd -t` |
| `systemctl restart isc-dhcp-server`| Reinicia el demonio de DHCP para que el sistema aplique los cambios guardados en `dhcpd.conf`. | `systemctl restart isc-dhcp-server` |

---

## 10. Servicio FTP Seguro y Enjaulamiento (vsftpd)

Implementación de transferencias de archivos aislando a los usuarios en sus respectivos directorios personales.

| Comando / Archivo | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `apt install vsftpd` | Instala el demonio "Very Secure FTP Daemon" para gestionar el canal de transferencia. | `apt install vsftpd` |
| `/etc/vsftpd.conf` | Archivo central. Para habilitar enjaulamiento se usa `chroot_local_user=YES`, y para permitir edición, `write_enable=YES`. | `nano /etc/vsftpd.conf` |
| `/etc/shells` | Archivo que lista los shells válidos. Es vital registrar rutas como `/bin/false` para evitar el **Error 530** al conectar usuarios. | `echo "/bin/false" >> /etc/shells` |
| `systemctl status vsftpd` | Comprueba los logs recientes y el estado de ejecución del servicio FTP tras modificar sus parámetros. | `systemctl status vsftpd` |

---

## 11. Gestión de Cuentas Webmaster y Permisos

Creación de credenciales de acceso restringido y establecimiento de la arquitectura de carpetas para el FTP.

| Comando / Archivo | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `useradd -s /bin/false` | Crea un usuario asignándole un shell falso por seguridad, impidiendo que inicie sesión por consola (SSH/TTY). | `useradd -d /home/webmaster -s /bin/false webmaster` |
| `passwd <usuario>` | Establece o modifica la contraseña cifrada para la cuenta de usuario FTP. | `passwd webmaster` |
| `chown root:root` | En configuraciones chroot seguras, el directorio raíz de la jaula debe pertenecer a `root` y no ser escribible por el usuario FTP. | `chown root:root /home/webmaster` |
| `mkdir` / `chown` | Genera un subdirectorio dentro de la jaula y le transfiere la propiedad para que el usuario FTP sí pueda subir archivos ahí. | `mkdir /home/webmaster/html && chown webmaster:webmaster /home/webmaster/html` |

---

## 12. Montajes Transparentes y Persistencia (Bind Mount)

Técnicas para mapear directorios base de servicios (como la carpeta `/var/www/html` de Apache/Nginx) dentro de la jaula de un webmaster.

| Comando / Archivo | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `mount --bind` | Vincula de forma temporal el contenido de un directorio origen en una carpeta destino de manera transparente (ideal para edición remota). | `mount --bind /var/www/html /home/webmaster/html` |
| `/etc/fstab` | Archivo de tablas de sistemas de ficheros. Su edición garantiza que el enlace lógico persista automáticamente tras un reinicio del servidor. | `nano /etc/fstab` |
| *Sintaxis en fstab* | Estructura obligatoria a introducir en el archivo para definir el punto de montaje permanente. | `/var/www/html /home/webmaster/html none bind 0 0` |
| `mount -a` | Obliga al kernel a leer el archivo `/etc/fstab` y montar inmediatamente cualquier partición o enlace faltante. | `mount -a` |

---

## 13. Servicio de Correo Electrónico (Postfix y Dovecot)

Configuración de la infraestructura para el envío (SMTP) y la recepción/lectura (IMAP/POP3) de correos electrónicos en el servidor.

### 13.1. Servidor de Envío SMTP (Postfix)

Postfix es el Agente de Transferencia de Correo (MTA) encargado de enrutar y entregar los mensajes salientes.

| Comando / Archivo | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `apt install postfix` | Instala el servidor Postfix. Durante la instalación, suele solicitar el tipo de configuración (ej. "Sitio de Internet") y el nombre del dominio. | `apt install postfix` |
| `dpkg-reconfigure postfix`| Vuelve a ejecutar el asistente de configuración interactivo de Postfix en caso de que necesites cambiar el dominio raíz o el tipo de servidor. | `dpkg-reconfigure postfix` |
| `/etc/postfix/main.cf` | Archivo de configuración principal. Aquí se definen parámetros como `myhostname`, `mydomain`, `myorigin` y las redes de confianza (`mynetworks`). | `nano /etc/postfix/main.cf` |
| `systemctl reload postfix`| Recarga la configuración de Postfix sin interrumpir el servicio para aplicar los cambios realizados en `main.cf`. | `systemctl reload postfix` |
| `postconf -n` | Imprime en pantalla únicamente los parámetros de configuración de Postfix que han sido modificados respecto a los valores por defecto. | `postconf -n` |

### 13.2. Servidor de Recepción IMAP/POP3 (Dovecot)

Dovecot funciona como el Agente de Entrega de Correo (MDA) que permite a los clientes (como Outlook o Thunderbird) conectarse y leer sus buzones.

| Comando / Archivo | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `apt install dovecot-imapd dovecot-pop3d` | Instala los demonios de Dovecot específicos para soportar los protocolos de lectura de correo IMAP (puerto 143) y POP3 (puerto 110). | `apt install dovecot-core dovecot-imapd dovecot-pop3d` |
| `/etc/dovecot/dovecot.conf`| Archivo base de Dovecot. Permite habilitar los protocolos en uso modificando la directiva `protocols = imap pop3 lmtp`. | `nano /etc/dovecot/dovecot.conf` |
| `/etc/dovecot/conf.d/10-mail.conf` | Define la ubicación y el formato de los buzones de correo. Es crucial decidir entre el formato tradicional `mbox` o el formato por directorios `Maildir`. | `nano /etc/dovecot/conf.d/10-mail.conf` |
| `/etc/dovecot/conf.d/10-auth.conf` | Gestiona los mecanismos de autenticación. Aquí se puede permitir el inicio de sesión en texto plano (solo recomendado si se usa en conjunto con SSL/TLS). | `nano /etc/dovecot/conf.d/10-auth.conf` |
| `systemctl restart dovecot`| Reinicia el servicio para que reconozca los nuevos formatos de buzón o protocolos habilitados. | `systemctl restart dovecot` |

### 13.3. Herramientas de Prueba y Diagnóstico de Correo

Utilidades esenciales de línea de comandos para verificar que el flujo de correo local y externo funciona correctamente.

| Comando / Archivo | Descripción y Uso Práctico | Ejemplo de Uso |
| :--- | :--- | :--- |
| `apt install mailutils` | Instala un conjunto de utilidades (incluyendo el comando `mail`) que facilitan el envío y lectura de correos directamente desde la terminal. | `apt install mailutils` |
| `mail -s "Asunto" <destino>`| Envía un correo de prueba rápido. Una vez escrito el comando, se redacta el cuerpo del mensaje y se presiona `Ctrl+D` para enviarlo. | `mail -s "Prueba de Servidor" admin@midominio.com` |
| `tail -f /var/log/mail.log` | Muestra el registro en tiempo real de toda la actividad de correo. Es la herramienta número uno para depurar correos rebotados o errores de conexión. | `tail -f /var/log/mail.log` |
| `telnet localhost 25` | Abre una conexión directa al puerto SMTP para probar la respuesta de Postfix y enviar comandos de correo manualmente (`EHLO`, `MAIL FROM`, etc.). | `telnet localhost 25` |

```
