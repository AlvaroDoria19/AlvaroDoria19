# Bitácora de Comandos Juniper (Firewalls SRX)

## Índice

- [Modos CLI, Navegación y Gestión de Configuración](#modos-cli-navegación-y-gestión-de-configuración)
- [Configuración Inicial del Sistema y Administración Remota](#configuración-inicial-del-sistema-y-administración-remota)
- [Autenticación AAA (RADIUS y TACACS+)](#autenticación-aaa-radius-y-tacacs)
- [Interfaces, Subinterfaces (802.1Q), VLANs, IRB y Loopback](#interfaces-subinterfaces-8021q-vlans-irb-y-loopback)
- [Zonas de Seguridad y Tráfico Dirigido al Equipo (Host-Inbound-Traffic)](#zonas-de-seguridad-y-tráfico-dirigido-al-equipo-host-inbound-traffic)
- [Libreta de Direcciones (Address Book) y Aplicaciones](#libreta-de-direcciones-address-book-y-aplicaciones)
- [Políticas de Seguridad (Security Policies)](#políticas-de-seguridad-security-policies)
- [Configuración de NAT (Source, Destination, Static y Proxy ARP)](#configuración-de-nat-source-destination-static-y-proxy-arp)
- [Control de Flujo, ALG y Modo de Reenvío (Flow vs Packet Mode)](#control-de-flujo-alg-y-modo-de-reenvío-flow-vs-packet-mode)
- [Servidor DHCP, Cliente DHCP y DHCP Relay](#servidor-dhcp-cliente-dhcp-y-dhcp-relay)
- [Enrutamiento Estático, por Defecto y Routing Instances (Virtual Router)](#enrutamiento-estático-por-defecto-y-routing-instances-virtual-router)
- [Enrutamiento Dinámico (OSPF, BGP y Políticas de Enrutamiento)](#enrutamiento-dinámico-ospf-bgp-y-políticas-de-enrutamiento)
- [Protección Perimetral: Screen (IDS/DoS) y Filtros Firewall Stateless](#protección-perimetral-screen-idsdos-y-filtros-firewall-stateless)
- [Configuración de VPN IPsec (Route-Based y Policy-Based)](#configuración-de-vpn-ipsec-route-based-y-policy-based)
- [Alta Disponibilidad: Chassis Cluster (HA en SRX)](#alta-disponibilidad-chassis-cluster-ha-en-srx)
- [Comandos de Verificación, Monitoreo y Troubleshooting](#comandos-de-verificación-monitoreo-y-troubleshooting)

---

## Modos CLI, Navegación y Gestión de Configuración

A diferencia de Cisco IOS donde cada comando se aplica inmediatamente a la configuración activa (*running-config*), los firewalls Juniper SRX y vSRX ejecutan **Junos OS**, un sistema operativo basado en una arquitectura de **configuración candidata**. Todos los cambios que realizas se editan en un borrador aislado y no entran en vigor ni afectan el tráfico hasta que ejecutas `commit`.

Además, Junos distingue claramente tres niveles de acceso: el **Shell UNIX (FreeBSD)** (`%`), el **Modo Operacional** (`>`) para consultas y diagnóstico, y el **Modo Configuración** (`#`) donde se modifica la jerarquía del firewall. Dominar la navegación, el `commit confirmed` y los filtros con tuberías (`|`) evita bloqueos remotos y acelera cualquier auditoría.

### Modos de Operación y Navegación

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `cli` | Inicia la interfaz de comandos de Junos desde el shell UNIX/FreeBSD. | SRX / vSRX (Shell) | `cli` | Al acceder por consola como `root`, el sistema suele iniciar en shell (`root@%`); escribe `cli` para entrar al prompt `>`. |
| `start shell` | Sale temporalmente del modo operacional de Junos hacia el shell UNIX subyacente. | SRX / vSRX (Operacional) | `start shell user root` | Útil para revisar archivos en `/var/log` o usar utilidades UNIX. Vuelve con `exit`. |
| `configure` | Entra al modo de configuración compartido (prompt `#`). | SRX / vSRX (Operacional) | `configure` | Permite que otros administradores editen simultáneamente y compartan la misma configuración candidata. |
| `configure private` | Entra a una copia privada de la configuración candidata. | SRX / vSRX (Operacional) | `configure private` | Muy recomendado en equipos multiadministrador: tu `commit` solo aplica tus propios cambios. |
| `configure exclusive` | Bloquea la base de datos de configuración para uso exclusivo de tu sesión. | SRX / vSRX (Operacional) | `configure exclusive` | Descarta cambios no confirmados si existieran y evita que otro usuario haga `commit` mientras trabajas. |
| `edit <jerarquía>` | Navega hacia un subnivel específico dentro del árbol de configuración. | SRX / vSRX (Configuración) | `edit security policies` | Reduce lo que debes escribir en comandos posteriores; el prompt muestra la ruta actual (`[edit security policies]`). |
| `top` | Regresa a la raíz de la jerarquía de configuración (`[edit]`). | SRX / vSRX (Configuración) | `top` | Útil antes de lanzar comandos completos `set ...` o revisar `show \| compare` global. |
| `up [n]` | Sube uno o `n` niveles en la jerarquía de configuración actual. | SRX / vSRX (Configuración) | `up 2` | Sin argumento sube un único nivel. |
| `exit` | Sale del nivel de jerarquía actual o abandona el modo configuración. | SRX / vSRX (Configuración) | `exit` | Si hay cambios sin confirmar, Junos avisará antes de salir al modo operacional (`>`). |
| `run <comando>` | Ejecuta un comando de modo operacional sin salir del modo configuración. | SRX / vSRX (Configuración) | `run show interfaces terse` | Equivalente al `do` de Cisco IOS. Funciona con `show`, `ping`, `clear`, `monitor`, etc. |

### Edición, Commit y Rollback

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set <ruta> <valor>` | Añade o modifica una sentencia en la configuración candidata. | SRX / vSRX (Configuración) | `set system host-name FW-SRX01` | Puede ejecutarse completo desde `[edit]` o relativo a la jerarquía actual. |
| `delete <ruta>` | Elimina un parámetro, objeto o bloque completo de la configuración candidata. | SRX / vSRX (Configuración) | `delete interfaces ge-0/0/1 disable` | Ten precaución al borrar nodos padre, ya que elimina toda su subjerarquía. |
| `deactivate <ruta>` | Deshabilita temporalmente una regla, interfaz o bloque sin borrarlo. | SRX / vSRX (Configuración) | `deactivate security policies from-zone trust to-zone untrust policy TEST` | Aparece marcado como `inactive:` en la configuración; ideal para pruebas controladas. |
| `activate <ruta>` | Vuelve a habilitar un bloque marcado previamente como `inactive:`. | SRX / vSRX (Configuración) | `activate security policies from-zone trust to-zone untrust policy TEST` | Restaura la sentencia sin tener que reescribir sus parámetros. |
| `rename <objeto> to <nuevo>` | Renombra una política, zona, regla NAT o interfaz lógica conservando su contenido. | SRX / vSRX (Configuración) | `rename security zones security-zone LAN to security-zone trust` | Si el objeto está referenciado en otras secciones, verifica que no queden referencias rotas con `commit check`. |
| `copy <origen> to <destino>` | Duplica un bloque de configuración bajo un nuevo nombre. | SRX / vSRX (Configuración) | `copy security policies from-zone trust to-zone untrust policy WEB to policy WEB-ALT` | Muy práctico para clonar reglas complejas o pools existentes. |
| `insert <item> before\|after <ref>` | Reordena elementos donde la secuencia importa (políticas, reglas NAT, términos). | SRX / vSRX (Configuración) | `insert security policies from-zone trust to-zone untrust policy BLOQUEO before policy PERMITIR-TODO` | En firewalls SRX las políticas y reglas NAT se evalúan de arriba hacia abajo; usa `insert` para subir una regla nueva. |
| `annotate <ruta> "<texto>"` | Añade un comentario documental sobre una línea o bloque de configuración. | SRX / vSRX (Configuración) | `annotate security nat source "PAT corporativo salida ISP"` | Se muestra en formato jerárquico sobre el elemento comentado (`/* ... */`). |
| `show` | Muestra la configuración candidata en formato jerárquico (con llaves `{}`). | SRX / vSRX (Configuración) | `show security zones` | Puedes indicar una subjerarquía para inspeccionar solo esa sección. |
| `show \| display set` | Muestra la configuración candidata como comandos `set` lineales. | SRX / vSRX (Configuración) | `show security nat \| display set` | Es el formato más cómodo para copiar, documentar o auditar la configuración. |
| `show \| compare` | Compara los cambios pendientes (`+` y `-`) frente a la configuración activa. | SRX / vSRX (Configuración) | `show \| compare` | Ejecútalo siempre antes de `commit` para verificar que no borraste ni alteraste nada por error. |
| `show \| compare rollback <n>` | Compara la configuración candidata actual contra un archivo histórico `rollback`. | SRX / vSRX (Configuración) | `show \| compare rollback 1` | Útil para auditar qué cambió en el commit anterior (`rollback 1`). |
| `commit` | Valida y aplica definitivamente la configuración candidata como configuración activa. | SRX / vSRX (Configuración) | `commit` | Reemplaza al `copy running-config startup-config` de Cisco: al hacer `commit` ya queda persistente en disco. |
| `commit check` | Verifica la sintaxis y dependencias lógicas de los cambios sin aplicarlos. | SRX / vSRX (Configuración) | `commit check` | Detecta zonas sin asignar, Address Books inexistentes o errores de referencia antes de afectar tráfico. |
| `commit comment "<texto>"` | Aplica la configuración registrando una nota descriptiva en el historial. | SRX / vSRX (Configuración) | `commit comment "Apertura puerto 443 DMZ"` | El comentario queda visible luego al ejecutar `show system commit`. |
| `commit confirmed <minutos>` | Aplica los cambios temporalmente y los revierte automático si no confirmas. | SRX / vSRX (Configuración) | `commit confirmed 5` | **Salvavidas remoto**: si pierdes acceso SSH tras cambiar rutas, NAT o políticas, el SRX vuelve atrás solo tras `N` minutos (defecto 10 min). Debes ejecutar `commit` para hacerlo permanente. |
| `commit at "<hora>"` | Programa la aplicación de la configuración candidata para una fecha/hora futura. | SRX / vSRX (Configuración) | `commit at "23:00:00"` | Útil para ventanas de mantenimiento nocturno. |
| `rollback <0..49>` | Carga en la candidata una versión previa (`0` descarta cambios sin confirmar). | SRX / vSRX (Configuración) | `rollback 0` | `rollback 1` carga la configuración inmediatamente anterior al último commit; requiere `commit` después (salvo `rollback 0`). |

### Respaldo, Carga y Filtros de Salida (Pipes)

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `save <ruta-archivo>` | Exporta la configuración candidata actual a un archivo local o remoto. | SRX / vSRX (Configuración) | `save /var/tmp/backup-srx.conf` | Combínalo con `\| display set` si deseas respaldo en comandos `set`: `show \| display set \| save /var/tmp/srx.set`. |
| `load override <archivo>` | Reemplaza completamente la configuración candidata con el contenido de un archivo. | SRX / vSRX (Configuración) | `load override /var/tmp/backup-srx.conf` | Requiere `commit` para entrar en vigor; `load merge` fusiona y `load set` carga líneas `set` (incluso pegadas desde `load set terminal`). |
| `request system configuration rescue save` | Guarda la configuración activa actual como configuración de rescate (*rescue*). | SRX / vSRX (Operacional) | `request system configuration rescue save` | Sirve como punto de restauración seguro conocido ante fallos graves; se recupera en configuración con `rollback rescue`. |
| `<cmd> \| match <patrón>` | Filtra la salida mostrando únicamente las líneas que coinciden con el texto o regex. | SRX / vSRX (Operacional/Config) | `show interfaces terse \| match ge-0/0` | Equivalente a `\| include` en Cisco IOS. Acepta expresiones regulares (ej. `"up.*up"`). |
| `<cmd> \| except <patrón>` | Oculta las líneas que coincidan con el patrón indicado. | SRX / vSRX (Operacional/Config) | `show interfaces terse \| except down` | Equivalente a `\| exclude` en Cisco IOS. |
| `<cmd> \| find <patrón>` | Comienza a imprimir la salida a partir de la primera coincidencia encontrada. | SRX / vSRX (Operacional/Config) | `show configuration \| find security` | Equivalente a `\| begin` en Cisco IOS. |
| `<cmd> \| count` | Cuenta el número total de líneas resultantes del comando o filtro anterior. | SRX / vSRX (Operacional/Config) | `show security flow session \| count` | Muy útil para medir sesiones activas o rutas en la tabla sin listar miles de líneas. |
| `<cmd> \| display xml` | Muestra la configuración o salida operacional en formato XML estructurado. | SRX / vSRX (Operacional/Config) | `show security zones \| display xml` | Útil para automatización con NETCONF / PyEZ o para inspeccionar etiquetas exactas de la API. |
| `<cmd> \| no-more` | Desactiva la paginación (`---(more)---`) y muestra toda la salida de una vez. | SRX / vSRX (Operacional/Config) | `show configuration \| display set \| no-more` | Evita tener que pulsar la barra espaciadora al capturar logs o respaldos extensos. |
| `<cmd> \| refresh <seg>` | Reejecuta automáticamente el comando cada cierto intervalo de segundos. | SRX / vSRX (Operacional) | `show interfaces ge-0/0/0 \| refresh 2` | Actúa como `watch` en Linux para ver contadores de tráfico en tiempo real. Pulsa `Ctrl+C` o `q` para salir. |

---

## Configuración Inicial del Sistema y Administración Remota

En esta sección se agrupan los parámetros base que se configuran al poner en marcha un firewall Juniper SRX o vSRX: identidad del equipo, contraseñas obligatorias, usuarios locales con roles RBAC, sincronización horaria NTP, servicios de acceso remoto (SSH/HTTPS/NETCONF) y registro de eventos hacia servidores Syslog o archivos locales.

Ten en cuenta que Junos **no permite hacer `commit`** si no existe una contraseña configurada para el usuario `root` (`system root-authentication`). Además, habilitar un servicio en `set system services ...` no basta por sí solo: la zona de seguridad de la interfaz receptora también debe autorizarlo en `host-inbound-traffic`.

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set system host-name <nombre>` | Asigna el nombre del firewall mostrado en el prompt CLI. | SRX / vSRX (Configuración) | `set system host-name FW-SRX320-01` | A diferencia de Cisco (`hostname`), en Junos lleva guion medio (`host-name`). |
| `set system domain-name <dominio>` | Define el sufijo de dominio DNS del equipo. | SRX / vSRX (Configuración) | `set system domain-name lab.local` | Recomendado antes de generar certificados autofirmados o usar SSH con FQDN. |
| `set system root-authentication plain-text-password` | Solicita de forma interactiva la contraseña cifrada para `root`. | SRX / vSRX (Configuración) | `set system root-authentication plain-text-password` | **Obligatorio** en cualquier configuración nueva; Junos almacena el hash cifrado automáticamente. |
| `set system login user <usuario> class <clase> authentication plain-text-password` | Crea un usuario administrador local con su nivel de privilegios. | SRX / vSRX (Configuración) | `set system login user admin-lab class super-user authentication plain-text-password` | Clases predefinidas: `super-user` (total), `operator` (operación y reset), `read-only` (solo lectura), `unauthorized`. |
| `set system login message "<texto>"` | Configura un banner previo/posterior al inicio de sesión. | SRX / vSRX (Configuración) | `set system login message "Acceso restringido - Solo personal autorizado"` | Equivalente a `banner motd` de Cisco. Usa `\n` para saltos de línea si se requiere. |
| `set system time-zone <zona>` | Establece la zona horaria del firewall. | SRX / vSRX (Configuración) | `set system time-zone America/La_Paz` | Esencial para correlacionar timestamps de logs de tráfico, VPN e IPS. |
| `set system ntp server <ip> [prefer]` | Configura uno o más servidores NTP para sincronizar el reloj. | SRX / vSRX (Configuración) | `set system ntp server 192.168.104.10 prefer` | También puedes fijar `boot-server <ip>` para sincronizar en el arranque del equipo. |
| `set system name-server <ip>` | Define los servidores DNS que usa el propio firewall. | SRX / vSRX (Configuración) | `set system name-server 8.8.8.8` | Necesario si el SRX resuelve FQDNs para NTP, Address Books DNS o actualizaciones. |
| `set system services ssh` | Habilita el servidor SSH para administración remota cifrada. | SRX / vSRX (Configuración) | `set system services ssh protocol-version v2` | Añade `root-login deny` por seguridad y exige usuario nominativo. Requiere permitir `ssh` en `host-inbound-traffic`. |
| `set system services web-management https system-generated-certificate` | Habilita la interfaz gráfica web (J-Web) por HTTPS con certificado propio. | SRX / vSRX (Configuración) | `set system services web-management https system-generated-certificate` | Puedes restringir la interfaz con `interface <if.unit>`. Requiere permitir `https` en la zona de seguridad. |
| `set system services netconf ssh` | Habilita el protocolo NETCONF sobre SSH (puerto TCP 830). | SRX / vSRX (Configuración) | `set system services netconf ssh` | Indispensable para automatización con Ansible, Terraform, Python PyEZ o Junos Space. |
| `set system syslog host <ip> <facilidad> <severidad>` | Envía los registros de eventos a un servidor Syslog externo. | SRX / vSRX (Configuración) | `set system syslog host 192.168.104.10 any info` | Permite centralizar auditoría y tráfico (`RT_FLOW`). Si usas Syslog externo para sesiones, considera `structured-data`. |
| `set system syslog file <nombre> <facilidad> <severidad>` | Guarda logs en un archivo local dentro de `/var/log/`. | SRX / vSRX (Configuración) | `set system syslog file messages any notice` | Por defecto `messages` almacena eventos del sistema; puedes crear `interactive-commands any` para auditar comandos ejecutados. |
| `set system syslog file <nombre> match "<filtro>"` | Filtra qué mensajes específicos se escriben en un archivo de log local. | SRX / vSRX (Configuración) | `set system syslog file traffic-log match RT_FLOW` | Aísla los logs de sesiones permitidas/denegadas (`RT_FLOW_SESSION_CREATE`/`CLOSE`/`DENY`) para no saturar `/var/log/messages`. |
| `set snmp community <nombre> authorization <read-only\|read-write>` | Configura SNMPv2c para monitoreo del firewall. | SRX / vSRX (Configuración) | `set snmp community MONITOR_RO authorization read-only` | Para mayor seguridad utiliza SNMPv3 (`set snmp v3 ...`) y lista clientes permitidos. |
| `delete chassis auto-image-upgrade` | Desactiva el servicio de autoaprovisionamiento ZTP/auto-upgrade de fábrica. | SRX Físico (Configuración) | `delete chassis auto-image-upgrade` | En SRX300/320 recién reseteados evita mensajes periódicos de alarma/log por búsqueda de imagen DHCP. |

---

## Autenticación AAA (RADIUS y TACACS+)

Junos OS soporta autenticación centralizada mediante **RADIUS** y **TACACS+** para el acceso administrativo al firewall. A través de `authentication-order` defines en qué secuencia consulta el SRX los métodos configurados (por ejemplo, primero RADIUS y luego la base local `password`).

**Límite importante del respaldo local:** al configurar `set system authentication-order [ radius password ]`, el firewall solo recurre a `password` cuando el servidor RADIUS **no responde o está inalcanzable** (timeout). Si el servidor RADIUS responde con un rechazo explícito (*Access-Reject*, por ejemplo contraseña errónea), Junos deniega el acceso inmediatamente sin intentar autenticar con la cuenta local.

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set system authentication-order [ <m1> <m2> ]` | Define el orden de consulta de autenticación de administradores. | SRX / vSRX (Configuración) | `set system authentication-order [ radius password ]` | Valores válidos: `radius`, `tacplus`, `password` (base local). Si omites `password`, no habrá respaldo local si cae el servidor. |
| `set system radius-server <ip> secret "<clave>"` | Declara el servidor RADIUS y el secreto compartido con el firewall (NAS). | SRX / vSRX (Configuración) | `set system radius-server 192.168.104.20 secret "RadiusClave2026"` | El secreto distingue mayúsculas y minúsculas y debe coincidir en el servidor FreeRADIUS / NPS. |
| `set system radius-server <ip> port <p> accounting-port <p> source-address <ip> timeout <seg> retry <n>` | Ajusta puertos, IP origen de paquetes RADIUS, tiempo de espera y reintentos. | SRX / vSRX (Configuración) | `set system radius-server 192.168.104.20 port 1812 accounting-port 1813 source-address 10.104.1.1 timeout 5 retry 3` | Fijar `source-address` (por ejemplo una Loopback o interfaz de gestión) garantiza que el servidor RADIUS reconozca siempre al NAS. |
| `set system tacplus-server <ip> secret "<clave>" source-address <ip>` | Registra un servidor TACACS+ y la IP origen del firewall. | SRX / vSRX (Configuración) | `set system tacplus-server 192.168.104.21 secret "TacacsClave2026" source-address 10.104.1.1` | TACACS+ cifra todo el paquete sobre TCP 49 y permite autorización granular por comandos. |
| `set system login user remote class <clase>` | Crea una cuenta plantilla para usuarios autenticados externamente sin atributo local. | SRX / vSRX (Configuración) | `set system login user remote class super-user` | Cuando RADIUS/TACACS+ no devuelve el atributo `Juniper-Local-User-Name`, el usuario mapea sus permisos desde esta plantilla `remote`. |
| `set system accounting events [ <eventos> ]` | Habilita el registro de auditoría (*accounting*) de accesos y comandos ejecutados. | SRX / vSRX (Configuración) | `set system accounting events [ login change-log interactive-commands ]` | `change-log` audita cambios de configuración y `interactive-commands` cada comando ingresado. |
| `set system accounting destination radius server <ip> secret "<clave>"` | Envía los registros de accounting al servidor RADIUS (o `tacplus`). | SRX / vSRX (Configuración) | `set system accounting destination radius server 192.168.104.20 secret "RadiusClave2026"` | También puede apuntar a `tacplus` usando `set system accounting destination tacplus server <ip> secret "<clave>"`. |

---

## Interfaces, Subinterfaces (802.1Q), VLANs, IRB y Loopback

En Junos OS las interfaces físicas (`ge-0/0/x`, `xe-0/0/x`) contienen interfaces lógicas llamadas **`unit`**. Por defecto, las interfaces enrutadas usan `unit 0` con `family inet` (IPv4) o `family inet6` (IPv6). La interfaz `fxp0` está dedicada a gestión fuera de banda (*OOB*), `lo0` es la Loopback del equipo y `st0` representa las interfaces de túnel IPsec (*Secure Tunnel*).

**Diferencia crítica entre versiones SRX (ELS vs Legacy):** en modelos SRX modernos con **ELS** (*Enhanced Layer 2 Software*, como SRX300/SRX320 y vSRX recientes), el switching usa `interface-mode` (`access`/`trunk`) y las interfaces SVI de Capa 3 se llaman **`irb.<vlan-id>`**. En imágenes antiguas **Legacy** (como **vSRX 12.1X47**), el switching usa `port-mode` y las interfaces SVI se llaman **`vlan.<vlan-id>`**.

### Interfaces L3 (IPv4 / IPv6) y Gestión

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set interfaces <if> unit 0 family inet address <ip>/<prefijo>` | Asigna una dirección IPv4 y máscara CIDR a una interfaz enrutada. | SRX / vSRX (Configuración) | `set interfaces ge-0/0/0 unit 0 family inet address 200.1.1.2/30` | En SRX las interfaces físicas están habilitadas por defecto; recuerda asociar `ge-0/0/0.0` a una zona de seguridad. |
| `set interfaces <if> unit 0 family inet6 address <ipv6>/<prefijo>` | Asigna una dirección IPv6 a la interfaz lógica. | SRX / vSRX (IPv6) | `set interfaces ge-0/0/1 unit 0 family inet6 address 2001:db8:10::1/64` | En Junos SRX el tráfico IPv6 también puede inspeccionarse en modo `flow-based` o `packet-based` según `security forwarding-options`. |
| `set interfaces <if> description "<texto>"` | Añade una etiqueta descriptiva a la interfaz física o a su `unit`. | SRX / vSRX (Configuración) | `set interfaces ge-0/0/0 description "WAN - Enlace ISP Principal"` | Puedes describir tanto la interfaz física como cada subinterfaz (`unit`). |
| `set interfaces <if> mtu <bytes>` | Modifica la MTU de la interfaz física o lógica. | SRX / vSRX (Configuración) | `set interfaces ge-0/0/0 mtu 1514` | Útil cuando se encapsula VLAN tagging o túneles GRE/IPsec; en `st0` se ajusta comúnmente a `1436`. |
| `set interfaces <if> disable` | Desactiva administrativamente la interfaz (equivalente a `shutdown`). | SRX / vSRX (Configuración) | `set interfaces ge-0/0/2 disable` | Para volver a encenderla (equivalente a `no shutdown`), usa `delete interfaces ge-0/0/2 disable`. |
| `set interfaces fxp0 unit 0 family inet address <ip>/<prefijo>` | Configura la interfaz dedicada de gestión fuera de banda (OOB). | SRX Físico / vSRX (Configuración) | `set interfaces fxp0 unit 0 family inet address 192.168.104.50/24` | `fxp0` no enruta tráfico de tránsito ni pertenece a zonas de seguridad; en algunos modelos físicos la gestión inicial puede ser `me0`. |

### Subinterfaces 802.1Q, Loopback, Túneles y LACP

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set interfaces <if> vlan-tagging` | Habilita el etiquetado IEEE 802.1Q en la interfaz física para usar subinterfaces L3. | SRX / vSRX (Configuración) | `set interfaces ge-0/0/1 vlan-tagging` | Equivalente a preparar un puerto para *Router on a Stick*. Incompatible con `family ethernet-switching` en la misma interfaz. |
| `set interfaces <if> unit <n> vlan-id <id> family inet address <ip>/<prefijo>` | Crea una subinterfaz 802.1Q con su VLAN ID y direccionamiento IPv4. | SRX / vSRX (Configuración) | `set interfaces ge-0/0/1 unit 10 vlan-id 10 family inet address 10.10.10.1/24` | Cada `unit <n>` (ej. `ge-0/0/1.10`) debe asignarse después a su zona de seguridad correspondiente. |
| `set interfaces lo0 unit 0 family inet address <ip>/32` | Configura la dirección IP de la interfaz Loopback del firewall. | SRX / vSRX (Configuración) | `set interfaces lo0 unit 0 family inet address 1.1.1.1/32` | Usada como Router ID en OSPF/BGP, origen de RADIUS/Syslog y punto de aplicación de filtros de protección al plano de control. |
| `set interfaces st0 unit <n> family inet [address <ip>/<pref>]` | Crea una interfaz lógica de túnel seguro (*Secure Tunnel*) para VPN IPsec. | SRX / vSRX (Configuración) | `set interfaces st0 unit 0 family inet mtu 1436` | Puede ser *unnumbered* (solo `family inet`, sin IP) para IPsec *route-based* o tener una IP `/30` para ejecutar OSPF/BGP sobre el túnel. |
| `set chassis aggregated-devices ethernet device-count <n>` | Reserva la cantidad de interfaces agregadas (`ae0`, `ae1`...) en el chasis. | SRX Físico / vSRX (Configuración) | `set chassis aggregated-devices ethernet device-count 2` | **Prerrequisito obligatorio** antes de configurar enlaces LACP (`ae0`). |
| `set interfaces <if> gigether-options 802.3ad <aeX>` | Asocia un puerto físico como miembro de un enlace agregado (`ae0`). | SRX / vSRX (Configuración) | `set interfaces ge-0/0/3 gigether-options 802.3ad ae0` | La interfaz física miembro no debe tener configuraciones `unit 0` propias ni direccionamiento IP. |
| `set interfaces <aeX> aggregated-ether-options lacp active` | Habilita negociación LACP en modo activo sobre la interfaz agregada. | SRX / vSRX (Configuración) | `set interfaces ae0 aggregated-ether-options lacp active` | Luego configura las unidades L2 o L3 directamente sobre `ae0` (ej. `set interfaces ae0 unit 0 family inet ...`). |

### VLANs, Switching L2 e Interfaces IRB / RVI (ELS vs Legacy)

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set interfaces <if> unit 0 family ethernet-switching interface-mode access vlan members <vlan>` | Configura un puerto L2 en modo acceso asignado a una VLAN (ELS). | SRX300/320 y SRX ELS (Configuración) | `set interfaces ge-0/0/4 unit 0 family ethernet-switching interface-mode access vlan members VLAN_LAN` | **Legacy (vSRX 12.1X47):** reemplaza `interface-mode` por `port-mode`. |
| `set interfaces <if> unit 0 family ethernet-switching interface-mode trunk vlan members [ <v1> <v2> ]` | Configura un puerto L2 en modo trunk permitiendo múltiples VLANs (ELS). | SRX300/320 y SRX ELS (Configuración) | `set interfaces ge-0/0/5 unit 0 family ethernet-switching interface-mode trunk vlan members [ VLAN_10 VLAN_20 ]` | Usa `vlan members all` si deseas transportar todas las VLANs declaradas. En Legacy usa `port-mode trunk`. |
| `set vlans <nombre> vlan-id <id> l3-interface irb.<id>` | Crea una VLAN y la vincula con su interfaz enrutada IRB (ELS). | SRX300/320 y SRX ELS (Configuración) | `set vlans VLAN_10 vlan-id 10 l3-interface irb.10` | **Legacy (vSRX 12.1X47):** usa `l3-interface vlan.10` en lugar de `irb.10`. |
| `set interfaces irb unit <id> family inet address <ip>/<prefijo>` | Asigna la puerta de enlace IPv4 (SVI) para la VLAN asociada (ELS). | SRX300/320 y SRX ELS (Configuración) | `set interfaces irb unit 10 family inet address 10.104.10.1/24` | **Legacy:** configura `set interfaces vlan unit 10 family inet address ...`. Recuerda agregar `irb.10` (o `vlan.10`) a una zona de seguridad. |

---

## Zonas de Seguridad y Tráfico Dirigido al Equipo (Host-Inbound-Traffic)

En un firewall Juniper SRX, toda interfaz L3 que procese tráfico de red **debe pertenecer a una zona de seguridad** (`security-zone`). Si una interfaz tiene dirección IP pero no está asignada a ninguna zona, el firewall descartará todo el tráfico que entre o salga por ella.

Además, el SRX separa el tráfico de **tránsito** (que atraviesa el firewall entre zonas y se controla con políticas) del tráfico dirigido **hacia el propio firewall** (SSH, Ping, HTTPS, DHCP, OSPF, BGP, IKE). Por defecto, **todo tráfico hacia el propio SRX está bloqueado**, incluso en la zona `trust`, hasta que lo habilitas explícitamente con `host-inbound-traffic`.

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set security zones security-zone <zona> interfaces <if.unit>` | Asigna una interfaz lógica (física `.0`, subinterfaz `.10`, `irb.10` o `st0.0`) a una zona. | SRX / vSRX (Configuración) | `set security zones security-zone trust interfaces ge-0/0/1.0` | Zonas típicas: `trust` (LAN), `untrust` (WAN/ISP), `dmz`, `vpn`. Una interfaz solo puede estar en una zona. |
| `set security zones security-zone <zona> host-inbound-traffic system-services <servicio>` | Permite que servicios del sistema del SRX respondan en toda la zona. | SRX / vSRX (Configuración) | `set security zones security-zone trust host-inbound-traffic system-services ssh` | Servicios comunes: `ping`, `ssh`, `https`, `dhcp`, `dns`, `ike`, `snmp`, `ntp`, `traceroute`, `all`. Sin `ping` el firewall no responde ICMP. |
| `set security zones security-zone <zona> host-inbound-traffic protocols <protocolo>` | Autoriza protocolos de enrutamiento o control dirigidos al firewall en la zona. | SRX / vSRX (Configuración) | `set security zones security-zone trust host-inbound-traffic protocols ospf` | Protocolos válidos: `ospf`, `ospf3`, `bgp`, `rip`, `bfd`, `vrrp`, `all`. Indispensable para levantar adyacencias OSPF o vecinos BGP. |
| `set security zones security-zone <zona> interfaces <if.unit> host-inbound-traffic system-services <servicio>` | Habilita un servicio entrante únicamente en una interfaz concreta de la zona. | SRX / vSRX (Configuración) | `set security zones security-zone untrust interfaces ge-0/0/0.0 host-inbound-traffic system-services ike` | Mejor práctica de seguridad: en `untrust` permite solo `ike` o `ping` si es necesario, nunca `ssh` o `https` abiertos a Internet sin filtro. |
| `set security zones security-zone <zona> screen <perfil>` | Asocia un perfil de protección contra ataques DoS/reconocimiento (Screen) a la zona. | SRX / vSRX (Configuración) | `set security zones security-zone untrust screen PROTECCION-WAN` | El perfil debe crearse previamente bajo `set security screen ids-option <perfil>`. |
| `set security zones security-zone <zona> tcp-rst` | Envía paquetes TCP Reset cuando una sesión inválida llega fuera de ventana/estado. | SRX / vSRX (Configuración) | `set security zones security-zone trust tcp-rst` | Evita que aplicaciones internas queden esperando hasta timeout si una conexión fue cerrada por el firewall. |

---

## Libreta de Direcciones (Address Book) y Aplicaciones

Antes de escribir políticas granulares de seguridad, Junos permite definir objetos reutilizables de red (**Address Book**) y servicios/puertos (**Applications**). Esto evita repetir direcciones IP o números de puerto en múltiples reglas y facilita el mantenimiento del firewall.

**Regla clave del Address Book:** en Junos moderno se recomienda usar `address-book global` para compartir objetos entre cualquier zona. Ten mucho cuidado de **no mezclar** `address-book global` con un `address-book` adjunto a una zona específica (`attach zone`) cuando configures políticas que involucren esa zona, ya que Junos no permite combinar ambos tipos sobre la misma zona y devolverá error en el `commit check`.

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set security address-book global address <nombre> <ip>/<prefijo>` | Crea un objeto de dirección IPv4 o subred disponible en todas las zonas. | SRX / vSRX (Configuración) | `set security address-book global address SRV-WEB 10.104.1.50/32` | Usa `/32` para hosts individuales y la máscara correspondiente para subredes (ej. `10.104.1.0/24`). |
| `set security address-book global address <nombre> range-address <ini> to <fin>` | Define un rango continuo de direcciones IP en el Address Book global. | SRX / vSRX (Configuración) | `set security address-book global address RANGO-DHCP range-address 10.104.1.100 to 10.104.1.200` | Útil cuando un segmento no coincide exactamente con una máscara CIDR. |
| `set security address-book global address <nombre> dns-name <fqdn>` | Crea un objeto basado en nombre de dominio DNS resuelto dinámicamente. | SRX / vSRX (Configuración) | `set security address-book global address UPDATE-SERVER dns-name archive.ubuntu.com` | Requiere tener configurado `set system name-server <ip>` operativo en el SRX. |
| `set security address-book global address-set <grupo> address <nombre>` | Agrupa varios objetos de dirección bajo un mismo nombre de grupo. | SRX / vSRX (Configuración) | `set security address-book global address-set SERVIDORES-DMZ address SRV-WEB` | Puedes añadir múltiples `address` o incluso anidar otro `address-set` dentro. |
| `set security address-book <libro> attach zone <zona>` | Crea un Address Book dedicado exclusivamente a una zona específica. | SRX / vSRX (Configuración) | `set security address-book BOOK-LAN attach zone trust` | Método clásico por zona. No uses objetos `global` en políticas de una zona que tenga un libro propio adjunto. |
| `set applications application <nombre> protocol <proto> destination-port <puerto>` | Crea una definición de servicio/aplicación personalizada (TCP/UDP/ICMP). | SRX / vSRX (Configuración) | `set applications application APP-HTTPS-ALT protocol tcp destination-port 8443` | También acepta rangos de puertos (`8000-8080`). Para servicios complejos puedes definir `term` múltiples dentro de la aplicación. |
| `set applications application <nombre> inactivity-timeout <seg>` | Ajusta el tiempo máximo de inactividad antes de cerrar la sesión en la tabla de flujos. | SRX / vSRX (Configuración) | `set applications application APP-SQL-LARGO inactivity-timeout 3600` | Muy útil para conexiones de base de datos o terminales SSH/RDP que mantienen sesiones largas sin tráfico constante. |
| `set applications application-set <grupo> application <nombre>` | Agrupa varias aplicaciones predefinidas o personalizadas en un conjunto. | SRX / vSRX (Configuración) | `set applications application-set PORTAL-WEB application junos-http` | Permite combinar aplicaciones estándar (`junos-http`, `junos-https`) con puertos propios en una sola línea de política. |
| `show configuration groups junos-defaults applications` | Lista todas las aplicaciones predefinidas integradas en Junos OS (`junos-*`). | SRX / vSRX (Operacional) | `show configuration groups junos-defaults applications \| match ssh` | Antes de crear una aplicación personalizada, verifica si ya existe como `junos-ssh`, `junos-icmp-all`, `junos-dns-udp`, etc. |

---

## Políticas de Seguridad (Security Policies)

Las políticas de seguridad controlan el flujo de tráfico **entre zonas** (`from-zone X to-zone Y`) o dentro de una misma zona. El firewall SRX es un equipo **stateful**: cuando una política permite el tráfico de ida y crea la sesión en su tabla de flujos, **el tráfico de retorno queda permitido automáticamente** sin necesidad de crear una regla inversa.

Las reglas dentro de cada contexto (`from-zone` -> `to-zone`) se evalúan **de arriba hacia abajo** en orden estricto: la primera regla que coincide detiene la búsqueda. Si ninguna política coincide, aplica la política por defecto del firewall (`deny-all` implícito).

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set security policies from-zone <z1> to-zone <z2> policy <pol> match source-address <obj\|any>` | Define la dirección o grupo de origen que debe cumplir la política. | SRX / vSRX (Configuración) | `set security policies from-zone trust to-zone untrust policy SALIDA-WEB match source-address RED-LAN` | `any` coincide con cualquier IP; también soporta `any-ipv4` y `any-ipv6`. |
| `set security policies from-zone <z1> to-zone <z2> policy <pol> match destination-address <obj\|any>` | Define la dirección de destino de la política de seguridad. | SRX / vSRX (Configuración) | `set security policies from-zone trust to-zone untrust policy SALIDA-WEB match destination-address any` | **Clave con NAT:** si el tráfico sufre Destination NAT o Static NAT, la política debe coincidir con la **IP interna real (post-NAT)**. |
| `set security policies from-zone <z1> to-zone <z2> policy <pol> match application <app\|any>` | Define el protocolo y puertos permitidos o bloqueados por la regla. | SRX / vSRX (Configuración) | `set security policies from-zone trust to-zone untrust policy SALIDA-WEB match application junos-https` | Puedes usar aplicaciones predefinidas (`junos-http`, `junos-ping`) o creadas en `set applications`. |
| `set security policies from-zone <z1> to-zone <z2> policy <pol> then <permit\|deny\|reject>` | Define la acción a tomar cuando un paquete coincide con la política. | SRX / vSRX (Configuración) | `set security policies from-zone trust to-zone untrust policy SALIDA-WEB then permit` | `deny` descarta en silencio; `reject` descarta y envía ICMP Unreachable / TCP RST al origen. |
| `set security policies from-zone <z1> to-zone <z2> policy <pol> then log session-init` | Registra un log Syslog en el instante en que se crea la sesión. | SRX / vSRX (Configuración) | `set security policies from-zone untrust to-zone dmz policy WEB-IN then log session-init` | Útil para auditar inicios de conexión o bloqueos (`deny` con `session-init`). |
| `set security policies from-zone <z1> to-zone <z2> policy <pol> then log session-close` | Registra un log Syslog cuando la sesión finaliza, incluyendo bytes y duración. | SRX / vSRX (Configuración) | `set security policies from-zone trust to-zone untrust policy SALIDA-WEB then log session-close` | Recomendado en reglas `permit` para auditoría completa con métricas de tráfico sin duplicar logs. |
| `set security policies from-zone <z1> to-zone <z2> policy <pol> then count` | Habilita contadores específicos por política (si la plataforma lo requiere). | SRX / vSRX (Configuración) | `set security policies from-zone trust to-zone untrust policy SALIDA-WEB then count` | Permite auditar el volumen exacto de bytes y paquetes mediante `show security policies hit-count`. |
| `set security policies global policy <pol> match ...` | Crea una política global evaluada después de todas las políticas por zona. | SRX / vSRX (Configuración) | `set security policies global policy BLOQUEO-FINAL match source-address any destination-address any application any` | Útil para aplicar reglas transversales (por ejemplo, registrar todo lo bloqueado al final) usando `from-zone` / `to-zone` dentro del `match`. |
| `set security policies default-policy <deny-all\|permit-all>` | Configura la acción predeterminada cuando ninguna política coincide. | SRX / vSRX (Configuración) | `set security policies default-policy deny-all` | En fábrica algunos equipos traen reglas `default-permit` entre `trust` y `untrust`; asegúrate de usar `deny-all` en producción. |
| `insert security policies from-zone <z1> to-zone <z2> policy <p1> before policy <p2>` | Mueve una política específica por encima de otra para alterar su prioridad. | SRX / vSRX (Configuración) | `insert security policies from-zone trust to-zone untrust policy BLOQUEO-RED before policy PERMITIR-TODO` | Indispensable cuando creas una excepción `deny` después de haber definido una regla amplia `permit`. |
| `deactivate security policies from-zone <z1> to-zone <z2> policy <pol>` | Desactiva temporalmente una política sin eliminar su configuración. | SRX / vSRX (Configuración) | `deactivate security policies from-zone trust to-zone untrust policy SALIDA-LAB` | Se reactiva con `activate`. Requiere `commit`. |
| `show security match-policies from-zone <z1> to-zone <z2> source-ip <ip> destination-ip <ip> source-port <p> destination-port <p> protocol <proto>` | Simula qué política coincidiría con un flujo de red concreto. | SRX / vSRX (Operacional) | `show security match-policies from-zone trust to-zone untrust source-ip 10.104.1.10 destination-ip 8.8.8.8 source-port 50000 destination-port 443 protocol tcp` | Herramienta de diagnóstico instantáneo para confirmar qué regla está permitiendo o bloqueando una conexión antes de probar tráfico real. |

---

## Configuración de NAT (Source, Destination, Static y Proxy ARP)

Para configurar NAT sin errores en un firewall Juniper SRX es fundamental recordar el **orden exacto de procesamiento del flujo** en Junos OS:

**Static NAT → Destination NAT → Búsqueda de Ruta (Routing) → Determinación de Zonas → Políticas de Seguridad → Source NAT**

Esto tiene dos implicaciones directas en tu configuración:
1. En **Destination NAT** y **Static NAT**, las **políticas de seguridad** deben permitir la **IP privada de destino** (post-NAT), aunque el cliente externo envíe el paquete hacia la IP pública.
2. En **Source NAT**, la política de seguridad evalúa la **IP privada de origen** (pre-NAT), y el NAT se aplica justo antes de salir hacia la zona destino. Si la IP pública traducida no coincide con la IP configurada físicamente en la interfaz WAN del SRX, debes habilitar **Proxy ARP** para que el router del ISP reciba respuesta ARP.

### Source NAT (PAT y Pools)

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set security nat source pool <pool> address <ip-ini>/<pref> [to <ip-fin>/<pref>]` | Define un pool de una o varias IPs públicas para traducción de origen. | SRX / vSRX (Configuración) | `set security nat source pool POOL-WAN address 200.1.1.10/32 to 200.1.1.14/32` | Si usarás la misma IP física de la interfaz WAN (PAT clásico), no necesitas crear un pool. |
| `set security nat source rule-set <rs> from zone <z-origen>` | Define la zona desde donde entra el tráfico que será traducido al salir. | SRX / vSRX (Configuración) | `set security nat source rule-set NAT-SALIDA from zone trust` | También puedes usar `from interface <if.unit>` o `from routing-instance <vr>`. |
| `set security nat source rule-set <rs> to zone <z-destino>` | Define la zona de salida hacia donde se dirige el tráfico a traducir. | SRX / vSRX (Configuración) | `set security nat source rule-set NAT-SALIDA to zone untrust` | En conjunto con `from zone`, delimita exactamente el sentido del NAT. |
| `set security nat source rule-set <rs> rule <r> match source-address <red>/<pref>` | Selecciona qué direcciones IP privadas de origen aplican a la regla NAT. | SRX / vSRX (Configuración) | `set security nat source rule-set NAT-SALIDA rule PAT-LAN match source-address 10.104.0.0/16` | Puedes añadir múltiples redes o combinar con `match destination-address`. |
| `set security nat source rule-set <rs> rule <r> match destination-address <red>/<pref>` | Filtra el destino de la regla Source NAT (clave para excluir tráfico VPN). | SRX / vSRX (Configuración) | `set security nat source rule-set NAT-SALIDA rule NO-NAT-VPN match destination-address 10.200.0.0/16` | Si el tráfico VPN comparte la zona `untrust`, coloca primero una regla con `destination-address` de la red remota y `source-nat off`. |
| `set security nat source rule-set <rs> rule <r> then source-nat interface` | Traduce múltiples IPs privadas usando la IP de la interfaz de salida (PAT/overload). | SRX / vSRX (Configuración) | `set security nat source rule-set NAT-SALIDA rule PAT-LAN then source-nat interface` | Equivalente a `ip nat inside source ... overload` en Cisco IOS. |
| `set security nat source rule-set <rs> rule <r> then source-nat pool <pool>` | Traduce el origen utilizando las direcciones públicas definidas en un pool. | SRX / vSRX (Configuración) | `set security nat source rule-set NAT-SALIDA rule NAT-POOL then source-nat pool POOL-WAN` | Por defecto realiza PAT sobre las IPs del pool; añade `port no-translation` en el pool para 1:1 dinámico. |
| `set security nat source rule-set <rs> rule <r> then source-nat off` | Exime explícitamente de Source NAT al tráfico que coincida con la regla. | SRX / vSRX (Configuración) | `set security nat source rule-set NAT-SALIDA rule NO-NAT-VPN then source-nat off` | **Vital para VPN IPsec**: evita que el tráfico hacia la red remota del túnel sea nateado. Debe ubicarse antes de la regla PAT general. |

### Destination NAT (Port Forwarding)

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set security nat destination pool <pool> address <ip-priv>/32 [port <puerto>]` | Crea el destino interno real al que se redirigirán los paquetes entrantes. | SRX / vSRX (Configuración) | `set security nat destination pool SRV-WEB address 10.104.1.50/32 port 80` | Si omites `port`, conserva el mismo puerto de destino original. |
| `set security nat destination rule-set <rs> from zone <z-origen>` | Indica desde qué zona ingresan las conexiones destinadas a ser traducidas. | SRX / vSRX (Configuración) | `set security nat destination rule-set NAT-ENTRADA from zone untrust` | A diferencia de Source NAT, Destination NAT **no lleva** `to zone`. |
| `set security nat destination rule-set <rs> rule <r> match destination-address <ip-pub>/32` | Selecciona la IP pública a la cual apuntan los clientes externos. | SRX / vSRX (Configuración) | `set security nat destination rule-set NAT-ENTRADA rule WEB match destination-address 200.1.1.2/32` | Si la IP pública no es la de la interfaz física WAN, agrega una entrada de **Proxy ARP**. |
| `set security nat destination rule-set <rs> rule <r> match destination-port <puerto>` | Especifica el puerto TCP/UDP público recibido antes de la traducción. | SRX / vSRX (Configuración) | `set security nat destination rule-set NAT-ENTRADA rule WEB match destination-port 80` | Puedes combinarlo con `match protocol tcp` para mayor precisión si el puerto existe en TCP y UDP. |
| `set security nat destination rule-set <rs> rule <r> then destination-nat pool <pool>` | Aplica la traducción de la IP y puerto destino hacia el pool interno. | SRX / vSRX (Configuración) | `set security nat destination rule-set NAT-ENTRADA rule WEB then destination-nat pool SRV-WEB` | **Recuerda:** la política `from-zone untrust to-zone dmz` debe permitir la IP `10.104.1.50` (post-NAT). |

### Static NAT (1:1) y Proxy ARP

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set security nat static rule-set <rs> from zone <z-origen>` | Define la zona desde la cual ingresa el tráfico hacia la IP pública 1:1. | SRX / vSRX (Configuración) | `set security nat static rule-set NAT-1A1 from zone untrust` | Static NAT es **bidireccional**: cuando el servidor interno inicia conexiones hacia `untrust`, su IP origen también se traduce a la pública. |
| `set security nat static rule-set <rs> rule <r> match destination-address <ip-pub>/32` | Define la IP o prefijo público que representa al servidor publicado. | SRX / vSRX (Configuración) | `set security nat static rule-set NAT-1A1 rule DMZ-MAIL match destination-address 200.1.1.10/32` | Soporta prefijos completos para mapeos estáticos bloque a bloque. |
| `set security nat static rule-set <rs> rule <r> then static-nat prefix <ip-priv>/32` | Traduce de forma simétrica 1:1 la IP pública a la IP interna del servidor. | SRX / vSRX (Configuración) | `set security nat static rule-set NAT-1A1 rule DMZ-MAIL then static-nat prefix 10.104.2.25/32` | La política de seguridad entrante debe permitir destino `10.104.2.25` (post-NAT). |
| `set security nat proxy-arp interface <if.unit> address <ip-pub>/32` | Hace que el SRX responda peticiones ARP por IPs públicas adicionales en su WAN. | SRX / vSRX (Configuración) | `set security nat proxy-arp interface ge-0/0/0.0 address 200.1.1.10/32` | **Imprescindible** cuando haces Static NAT, Destination NAT o Source NAT Pool con IPs públicas de la subred WAN que no están asignadas directamente en `ge-0/0/0.0`. |

---

## Control de Flujo, ALG y Modo de Reenvío (Flow vs Packet Mode)

Un firewall Juniper SRX opera por defecto en modo **flow-based** (*stateful*), inspeccionando sesiones completas. Sin embargo, en escenarios de VPN IPsec, enlaces PPPoE, telefonía VoIP o enrutamiento asimétrico es necesario ajustar el tamaño de segmento TCP (**MSS**), desactivar **ALGs** (*Application Layer Gateways*) que alteran cabeceras SIP/DNS, o usar **traceoptions** para depurar paso a paso por qué se descarta un paquete.

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set security flow tcp-mss all-tcp mss <valor>` | Limita el tamaño máximo de segmento TCP para evitar fragmentación en enlaces WAN/PPPoE. | SRX / vSRX (Configuración) | `set security flow tcp-mss all-tcp mss 1350` | Muy útil cuando páginas HTTPS o transferencias grandes se congelan por problemas de Path MTU Discovery. |
| `set security flow tcp-mss ipsec-vpn mss <valor>` | Ajusta automáticamente el MSS para tráfico TCP que atraviesa túneles IPsec. | SRX / vSRX (Configuración) | `set security flow tcp-mss ipsec-vpn mss 1350` | Compensa la sobrecarga de cabeceras ESP/UDP de la VPN sin afectar el tráfico normal hacia Internet. |
| `set security flow tcp-session no-syn-check` | Permite crear sesiones TCP incluso si el primer paquete visto no tiene el flag SYN. | SRX / vSRX (Configuración) | `set security flow tcp-session no-syn-check` | Solo usar como medida temporal ante enrutamiento asimétrico inevitable; reduce la protección stateful. Suele acompañarse de `no-sequence-check`. |
| `set security alg <protocolo> disable` | Desactiva el ALG de capa aplicación que inspecciona o reescribe cargas útiles. | SRX / vSRX (Configuración) | `set security alg sip disable` | Protocolos disponibles: `sip`, `ftp`, `dns`, `icmp`, `h323`, `rtsp`, `tftp`. En telefonía VoIP sobre SIP o DNS detrás de NAT casi siempre se recomienda desactivar `sip` y `dns` ALG si causan fallos. |
| `set security forwarding-options family inet6 mode <modo>` | Configura si el tráfico IPv6 se procesa como firewall (`flow-based`) o router (`packet-based`). | SRX / vSRX (IPv6) | `set security forwarding-options family inet6 mode flow-based` | Según la versión de Junos, cambiar el modo de reenvío puede requerir reiniciar el equipo (`reboot`). |
| `set security forwarding-options family mpls mode packet-based` | Cambia el procesamiento de MPLS (o IPv4 con `family iso`/`inet`) a modo stateless sin flujo. | SRX / vSRX (Configuración) | `set security forwarding-options family mpls mode packet-based` | Para convertir un vSRX de laboratorio en un router puro sin firewall, se puede configurar `family mpls mode packet-based` junto con un filtro `packet-mode` (requiere reinicio en varias plataformas). |
| `set security flow traceoptions file <archivo>` | Define el archivo en `/var/log/` donde se guardará la traza detallada de paquetes del datapath. | SRX / vSRX (Configuración) | `set security flow traceoptions file flow-debug` | **Importante:** siempre aplica un `packet-filter` antes de activar `traceoptions` en producción para no saturar la CPU. |
| `set security flow traceoptions flag <bandera>` | Selecciona qué nivel de detalle de flujo se registrará en la traza. | SRX / vSRX (Configuración) | `set security flow traceoptions flag basic-datapath` | `basic-datapath` muestra claramente el ingreso, NAT, búsqueda de ruta, evaluación de política y motivo exacto de `drop` o `permit`. |
| `set security flow traceoptions packet-filter <nombre> source-prefix <ip>/<pref> destination-prefix <ip>/<pref>` | Limita el `flow traceoptions` únicamente al flujo IP que deseas diagnosticar. | SRX / vSRX (Configuración) | `set security flow traceoptions packet-filter PF-TEST source-prefix 10.104.1.10/32 destination-prefix 8.8.8.8/32` | Al terminar de diagnosticar, ejecuta `delete security flow traceoptions` y `commit` para evitar consumo innecesario de disco y CPU. |

---

## Servidor DHCP, Cliente DHCP y DHCP Relay

Los firewalls Juniper SRX modernos implementan el servicio **JDHCP** (*Juniper Dynamic Host Configuration Protocol*) bajo la jerarquía `access address-assignment` junto con `system services dhcp-local-server`. También pueden actuar como clientes DHCP en la interfaz WAN hacia el ISP o como agente **DHCP Relay** hacia un servidor DHCP centralizado en Windows/Linux.

**Paso obligatorio en zonas:** para que el SRX responda peticiones DHCP como servidor o relay en una LAN, debes habilitar `set security zones security-zone <zona> host-inbound-traffic system-services dhcp`.

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set access address-assignment pool <pool> family inet network <red>/<pref>` | Crea un pool DHCP y define la subred que administrará. | SRX / vSRX (Configuración) | `set access address-assignment pool POOL-LAN family inet network 10.104.1.0/24` | Equivalente a `ip dhcp pool` + `network` en Cisco IOS. |
| `set access address-assignment pool <pool> family inet range <nombre> low <ini> high <fin>` | Define el rango dinámico de direcciones IP que se asignarán a los clientes. | SRX / vSRX (Configuración) | `set access address-assignment pool POOL-LAN family inet range RANGO-1 low 10.104.1.50 high 10.104.1.200` | Excluye automáticamente IPs fuera del rango (como gateways, servidores e impresoras). |
| `set access address-assignment pool <pool> family inet dhcp-attributes router <ip-gw>` | Define la puerta de enlace predeterminada entregada a los clientes DHCP. | SRX / vSRX (Configuración) | `set access address-assignment pool POOL-LAN family inet dhcp-attributes router 10.104.1.1` | Equivalente a `default-router` en Cisco IOS. |
| `set access address-assignment pool <pool> family inet dhcp-attributes name-server <ip-dns>` | Asigna los servidores DNS que recibirán los hosts clientes. | SRX / vSRX (Configuración) | `set access address-assignment pool POOL-LAN family inet dhcp-attributes name-server 8.8.8.8` | Puedes añadir múltiples servidores DNS repitiendo el comando con otra IP. |
| `set access address-assignment pool <pool> family inet dhcp-attributes maximum-lease-time <seg>` | Configura la duración máxima de la concesión DHCP en segundos. | SRX / vSRX (Configuración) | `set access address-assignment pool POOL-LAN family inet dhcp-attributes maximum-lease-time 86400` | `86400` segundos equivale a 24 horas. |
| `set access address-assignment pool <pool> family inet host <nombre> hardware-address <mac> ip-address <ip>` | Crea una reserva DHCP estática amarrada a la dirección MAC de un equipo. | SRX / vSRX (Configuración) | `set access address-assignment pool POOL-LAN family inet host IMPRESORA hardware-address aa:bb:cc:dd:ee:ff ip-address 10.104.1.30` | La IP reservada debe pertenecer a la subred `network` del pool y conviene que quede fuera del `range` dinámico. |
| `set system services dhcp-local-server group <grupo> interface <if.unit>` | Activa el servidor JDHCP escuchando en la interfaz LAN indicada. | SRX / vSRX (Configuración) | `set system services dhcp-local-server group LAN-DHCP interface ge-0/0/1.0` | En imágenes antiguas Legacy (vSRX 12.1X47) podía usarse el servicio clásico `system services dhcp`, reemplazado por `dhcp-local-server` en Junos moderno. |
| `set interfaces <if> unit 0 family inet dhcp` | Configura una interfaz WAN del firewall como cliente DHCP automático hacia el ISP. | SRX / vSRX (Configuración) | `set interfaces ge-0/0/0 unit 0 family inet dhcp` | Recibe IP, máscara y ruta por defecto automáticamente del proveedor; permite también `dhcp-client` en JDHCP moderno. |
| `set forwarding-options dhcp-relay server-group <grupo> <ip-srv>` | Registra el grupo de servidores DHCP externos para reenviar peticiones (`BOOTP/DHCP`). | SRX / vSRX (Configuración) | `set forwarding-options dhcp-relay server-group SRV-CENTRAL 10.104.10.20` | Equivalente a `ip helper-address` en Cisco IOS. Actívalo como grupo activo con `active-server-group`. |
| `set forwarding-options dhcp-relay active-server-group <grupo>` | Selecciona qué grupo de servidores DHCP utilizará el agente relay. | SRX / vSRX (Configuración) | `set forwarding-options dhcp-relay active-server-group SRV-CENTRAL` | Se complementa declarando las interfaces origen con `set forwarding-options dhcp-relay group <nombre> interface <if.unit>`. |
| `set forwarding-options dhcp-relay group <nombre> interface <if.unit>` | Indica en qué interfaces locales escucha el agente DHCP Relay para reenviar solicitudes. | SRX / vSRX (Configuración) | `set forwarding-options dhcp-relay group CLIENTES interface irb.20` | Requiere `host-inbound-traffic system-services dhcp` en la zona de `irb.20` y política que permita el tráfico hacia el servidor si están en zonas distintas. |

---

## Enrutamiento Estático, por Defecto y Routing Instances (Virtual Router)

En Junos OS todas las rutas estáticas, el Router ID y los parámetros globales de la tabla de enrutamiento (`inet.0` para IPv4 e `inet6.0` para IPv6) se configuran bajo `routing-options`. Cuando necesitas aislar tablas de enrutamiento independientes dentro del mismo firewall —por ejemplo, para separar dos proveedores ISP (Dual-WAN) o segregar redes de clientes— se utilizan `routing-instances` de tipo **`virtual-router`**.

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set routing-options static route <red>/<pref> next-hop <ip\|if>` | Crea una ruta estática IPv4 hacia una red remota mediante IP o interfaz punto a punto. | SRX / vSRX (Configuración) | `set routing-options static route 10.200.0.0/16 next-hop st0.0` | Para túneles IPsec *route-based* se usa directamente el nombre de la interfaz `st0.0` como `next-hop`. |
| `set routing-options static route 0.0.0.0/0 next-hop <ip-gw>` | Define la ruta por defecto IPv4 hacia Internet o el router del ISP. | SRX / vSRX (Configuración) | `set routing-options static route 0.0.0.0/0 next-hop 200.1.1.1` | Si el next-hop no es directamente alcanzable por interfaz, añade `resolve` (útil en escenarios específicos). |
| `set routing-options static route <red>/<pref> qualified-next-hop <ip> preference <val>` | Configura un salto alternativo con preferencia administrativa distinta (ruta flotante). | SRX / vSRX (Configuración) | `set routing-options static route 0.0.0.0/0 qualified-next-hop 200.2.2.1 preference 10` | En Junos, la preferencia por defecto de una ruta estática es `5` y **un valor menor es mejor** (al revés de la métrica, igual que la AD de Cisco). `preference 10` actúa como respaldo. |
| `set routing-options rib inet6.0 static route ::/0 next-hop <ipv6>` | Configura la ruta estática o por defecto para la tabla de enrutamiento IPv6 (`inet6.0`). | SRX / vSRX (IPv6) | `set routing-options rib inet6.0 static route ::/0 next-hop 2001:db8:1::1` | En Junos, las rutas estáticas IPv6 van explícitamente dentro de `rib inet6.0`. |
| `set routing-options router-id <ip>` | Establece manualmente el Router ID de 32 bits usado por OSPF, BGP y otros protocolos. | SRX / vSRX (Configuración) | `set routing-options router-id 1.1.1.1` | Buena práctica configurarlo igual a la IP de `lo0.0` para estabilizar los procesos de enrutamiento. |
| `set routing-instances <vr> instance-type virtual-router` | Crea una tabla de enrutamiento virtual independiente (similar a un VRF-Lite). | SRX / vSRX (Configuración) | `set routing-instances VR-ISP2 instance-type virtual-router` | Su tabla se denomina `<vr>.inet.0` (ej. `VR-ISP2.inet.0`). A diferencia de `vrf`, `virtual-router` no requiere MPLS ni route-distinguisher. |
| `set routing-instances <vr> interface <if.unit>` | Mueve una interfaz lógica desde la tabla global (`inet.0`) hacia el Virtual Router. | SRX / vSRX (Configuración) | `set routing-instances VR-ISP2 interface ge-0/0/2.0` | La interfaz puede estar en la misma o en distinta zona de seguridad; las zonas y el VR son conceptos ortogonales en SRX. |
| `set routing-instances <vr> routing-options static route <red>/<pref> next-hop <ip>` | Agrega rutas estáticas o por defecto dentro de la tabla privada del Virtual Router. | SRX / vSRX (Configuración) | `set routing-instances VR-ISP2 routing-options static route 0.0.0.0/0 next-hop 198.51.100.1` | También permite ejecutar protocolos dinámicos propios dentro de `set routing-instances <vr> protocols ospf\|bgp`. |

---

## Enrutamiento Dinámico (OSPF, BGP y Políticas de Enrutamiento)

A diferencia de Cisco IOS, donde `network` dentro del proceso OSPF o BGP anuncia redes automáticamente, **Junos OS separa el descubrimiento de vecinos de la redistribución de rutas**. En OSPF declaras directamente las interfaces que participan por área; en BGP declaras grupos de vecinos y, si deseas anunciar prefijos locales o redistribuir rutas entre protocolos, debes definir una política explícita en `policy-options` y aplicarla con `export` o `import`.

**Requisito en SRX:** recuerda habilitar `ospf` / `ospf3` o `bgp` bajo `host-inbound-traffic protocols` en la zona de cada interfaz vecina, de lo contrario el firewall descartará los paquetes Hello de OSPF o la sesión TCP 179 de BGP.

### Configuración de OSPF (IPv4 y OSPFv3 IPv6)

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set protocols ospf area <area> interface <if.unit>` | Activa OSPFv2 en una interfaz específica asociándola al área indicada. | SRX / vSRX (Configuración) | `set protocols ospf area 0.0.0.0 interface ge-0/0/1.0` | Las áreas pueden escribirse en formato IP (`0.0.0.0`) o entero (`0`). Añade `lo0.0` a `area 0.0.0.0` (como `passive`) para anunciar la Loopback. |
| `set protocols ospf area <area> interface <if.unit> passive` | Anuncia el prefijo de la interfaz en OSPF sin enviar ni formar adyacencias Hello por ella. | SRX / vSRX (Configuración) | `set protocols ospf area 0.0.0.0 interface irb.10 passive` | Recomendado en interfaces de usuario final (LAN/DMZ) y Loopbacks para evitar inyección de rutas no autorizada. |
| `set protocols ospf area <area> interface <if.unit> interface-type p2p` | Fuerza el tipo de enlace punto a punto omitiendo la elección de DR/BDR. | SRX / vSRX (Configuración) | `set protocols ospf area 0.0.0.0 interface st0.0 interface-type p2p` | Acelera la convergencia en enlaces dedicados entre dos equipos y en túneles IPsec `st0`. |
| `set protocols ospf area <area> interface <if.unit> metric <costo>` | Asigna manualmente un costo OSPF a la interfaz, sobrescribiendo el cálculo por ancho de banda. | SRX / vSRX (Configuración) | `set protocols ospf area 0.0.0.0 interface ge-0/0/2.0 metric 100` | Un costo menor tiene mayor prioridad en el algoritmo SPF. |
| `set protocols ospf area <area> interface <if.unit> authentication md5 <id> key "<clave>"` | Habilita autenticación MD5 en la interfaz OSPF para validar vecinos. | SRX / vSRX (Configuración) | `set protocols ospf area 0.0.0.0 interface ge-0/0/1.0 authentication md5 1 key "OspfClave2026"` | Tanto el ID de llave como la clave deben coincidir con el router/firewall vecino. |
| `set protocols ospf export <politica>` | Aplica una política de exportación para redistribuir rutas estáticas, directas o por defecto hacia OSPF. | SRX / vSRX (Configuración) | `set protocols ospf export REDISTRIBUIR-ESTATICAS` | En Junos no existe `default-information originate` ni `network` en OSPF; todo anuncio externo se hace mediante `export <policy-statement>`. |
| `set protocols ospf3 area <area> interface <if.unit>` | Activa OSPFv3 para el intercambio dinámico de rutas IPv6. | SRX / vSRX (IPv6) | `set protocols ospf3 area 0.0.0.0 interface ge-0/0/1.0` | Requiere `host-inbound-traffic protocols ospf3` en la zona de seguridad correspondiente. |

### Configuración de BGP (eBGP e iBGP)

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set routing-options autonomous-system <asn>` | Define el número de Sistema Autónomo (AS) local del firewall. | SRX / vSRX (Configuración) | `set routing-options autonomous-system 65001` | Requisito previo obligatorio para levantar cualquier sesión BGP. |
| `set protocols bgp group <grupo> type <external\|internal>` | Crea un grupo de vecinos BGP especificando si es eBGP (`external`) o iBGP (`internal`). | SRX / vSRX (Configuración) | `set protocols bgp group ISP-EBGP type external` | Agrupar vecinos en Junos simplifica heredar `peer-as`, `export`, `import` y parámetros comunes. |
| `set protocols bgp group <grupo> peer-as <asn-vecino>` | Define el número de AS del vecino remoto (en eBGP) o propio (en iBGP). | SRX / vSRX (Configuración) | `set protocols bgp group ISP-EBGP peer-as 65002` | También puede definirse por vecino individual si un mismo grupo eBGP tiene múltiples AS distintos. |
| `set protocols bgp group <grupo> neighbor <ip-vecino>` | Declara la dirección IP del peer BGP con el que se establecerá sesión TCP 179. | SRX / vSRX (Configuración) | `set protocols bgp group ISP-EBGP neighbor 200.1.1.1` | Recuerda permitir `host-inbound-traffic protocols bgp` en la zona de la interfaz hacia el vecino. |
| `set protocols bgp group <grupo> local-address <ip-local>` | Especifica la IP origen local que usará el firewall para la sesión BGP. | SRX / vSRX (Configuración) | `set protocols bgp group IBGP-CORE local-address 1.1.1.1` | Indispensable en sesiones **iBGP entre Loopbacks** para que el origen TCP coincida con lo esperado por el peer. |
| `set protocols bgp group <grupo> multihop ttl <saltos>` | Permite establecer sesiones eBGP con vecinos que no están directamente conectados. | SRX / vSRX (Configuración) | `set protocols bgp group ISP-LOOPBACK multihop ttl 4` | Equivalente a `ebgp-multihop` de Cisco. Por defecto eBGP usa TTL=1. |
| `set protocols bgp group <grupo> export <politica>` | Aplica una política de salida para anunciar prefijos hacia los vecinos del grupo. | SRX / vSRX (Configuración) | `set protocols bgp group ISP-EBGP export ANUNCIAR-PREFIJOS` | Del mismo modo, `import <politica>` filtra o modifica atributos (`local-preference`, `as-path`) de rutas recibidas. |

### Políticas de Enrutamiento (Policy-Options)

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set policy-options policy-statement <pol> term <t> from protocol <proto>` | Filtra rutas candidatas por su protocolo origen (`direct`, `static`, `ospf`, `bgp`). | SRX / vSRX (Configuración) | `set policy-options policy-statement ANUNCIAR-BGP term T1 from protocol static` | Las políticas evalúan cada `term` en orden secuencial de arriba hacia abajo. |
| `set policy-options policy-statement <pol> term <t> from route-filter <red>/<pref> <op>` | Coincide con un prefijo específico usando operadores (`exact`, `orlonger`, `longer`, `upto`). | SRX / vSRX (Configuración) | `set policy-options policy-statement ANUNCIAR-BGP term T1 from route-filter 203.0.113.0/24 exact` | La ruta debe existir activa en la tabla `inet.0` para ser anunciada al usar `export`. |
| `set policy-options policy-statement <pol> term <t> then <acción>` | Define la acción sobre la ruta coincidente (`accept`, `reject`, `next-hop self`, `metric`). | SRX / vSRX (Configuración) | `set policy-options policy-statement ANUNCIAR-BGP term T1 then accept` | Puedes añadir un segundo término final (`term RECHAZAR then reject`) para bloquear el resto de prefijos. |

---

## Protección Perimetral: Screen (IDS/DoS) y Filtros Firewall Stateless

Los firewalls Juniper SRX cuentan con dos mecanismos complementarios a las políticas de seguridad con estado (*stateful*):
- **Security Screen (IDS/DoS):** protege en la capa de flujo de cada zona contra ataques de denegación de servicio (`SYN flood`, `ICMP flood`, `Ping of Death`), escaneos de red (`port-scan`, `ip-sweep`), suplantación de IP (`ip spoofing`) y agotamiento de sesiones.
- **Filtros de Firewall Stateless (`firewall family inet`):** funcionan de forma similar a las ACLs extendidas de Cisco IOS sin tabla de estado. Su uso más crítico en un SRX es proteger la interfaz Loopback (`lo0.0`) para restringir qué IPs de administración pueden alcanzar al plano de control del firewall (SSH, HTTPS, SNMP, BGP, OSPF).

### Perfiles Screen (Protección contra DoS, Flood y Escaneos)

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set security screen ids-option <perfil> icmp ping-death` | Bloquea paquetes ICMP fragmentados o malformados de tamaño superior a 65,535 bytes. | SRX / vSRX (Configuración) | `set security screen ids-option SCREEN-WAN icmp ping-death` | Protección básica recomendada para toda zona `untrust` / WAN. |
| `set security screen ids-option <perfil> icmp flood threshold <pps>` | Limita el número de paquetes ICMP por segundo permitidos antes de descartar el exceso. | SRX / vSRX (Configuración) | `set security screen ids-option SCREEN-WAN icmp flood threshold 1000` | Ajusta el umbral (`threshold`) según el volumen legítimo de tráfico ICMP de tu entorno. |
| `set security screen ids-option <perfil> tcp syn-flood ...` | Mitiga ataques TCP SYN Flood limitando conexiones semiabiertas por segundo. | SRX / vSRX (Configuración) | `set security screen ids-option SCREEN-WAN tcp syn-flood alarm-threshold 1024 attack-threshold 200 source-threshold 500 destination-threshold 2000 timeout 20` | Activa el mecanismo *SYN-Cookie* del SRX cuando se supera `attack-threshold`, protegiendo servidores tras el firewall. |
| `set security screen ids-option <perfil> tcp syn-ack-ack-proxy threshold <n>` | Protege contra ataques de agotamiento TCP enviando proxy de handshake de 3 vías. | SRX / vSRX (Configuración) | `set security screen ids-option SCREEN-WAN tcp syn-ack-ack-proxy threshold 512` | Se utiliza en conjunto o alternativa según la capacidad de la plataforma SRX. |
| `set security screen ids-option <perfil> tcp port-scan threshold <usec>` | Detecta y bloquea escaneos rápidos de múltiples puertos TCP desde un mismo origen. | SRX / vSRX (Configuración) | `set security screen ids-option SCREEN-WAN tcp port-scan threshold 5000` | El umbral mide el intervalo en microsegundos entre intentos a 10 puertos distintos (ej. `5000` = 5 ms; valor común `1000000` = 1 s). |
| `set security screen ids-option <perfil> ip ip-sweep threshold <usec>` | Detecta y bloquea barridos ICMP hacia múltiples direcciones IP consecutivas. | SRX / vSRX (Configuración) | `set security screen ids-option SCREEN-WAN ip ip-sweep threshold 5000` | Previene reconocimiento automático de hosts activos en subredes internas o DMZ. |
| `set security screen ids-option <perfil> ip spoofing` | Descarta paquetes cuya IP origen pertenece a una ruta que apunta a una interfaz distinta. | SRX / vSRX (Configuración) | `set security screen ids-option SCREEN-WAN ip spoofing` | Actúa como validación uRPF estricta en la zona; **no usar** en redes con enrutamiento asimétrico. |
| `set security screen ids-option <perfil> limit-session source-ip-based <max>` | Limita la cantidad máxima de sesiones simultáneas permitidas desde una misma IP origen. | SRX / vSRX (Configuración) | `set security screen ids-option SCREEN-WAN limit-session source-ip-based 500` | Evita que un único host infectado agote toda la tabla de sesiones del firewall. |
| `set security zones security-zone <zona> screen <perfil>` | Activa el perfil Screen configurado sobre todo el tráfico entrante de la zona. | SRX / vSRX (Configuración) | `set security zones security-zone untrust screen SCREEN-WAN` | El perfil Screen **no surte efecto** hasta vincularse a una o más zonas de seguridad. |

### Filtros de Firewall Stateless (Protección de Loopback e Interfaces)

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set firewall family inet filter <filtro> term <t> from source-address <red>/<pref>` | Coincide con la dirección o subred origen en un término de filtro stateless. | SRX / vSRX (Configuración) | `set firewall family inet filter PROTECT-RE term PERMITIR-ADMIN from source-address 10.104.1.0/24` | Puedes usar `source-prefix-list <lista>` para reutilizar `policy-options prefix-list`. |
| `set firewall family inet filter <filtro> term <t> from protocol <proto> destination-port <p>` | Filtra tráfico por protocolo L4 y puerto destino en la ACL stateless. | SRX / vSRX (Configuración) | `set firewall family inet filter PROTECT-RE term PERMITIR-ADMIN from protocol tcp destination-port ssh` | Soporta múltiples puertos entre corchetes, ej. `[ ssh https snmp ]`. |
| `set firewall family inet filter <filtro> term <t> then <count\|log\|accept\|discard\|reject>` | Define acciones de conteo, registro y decisión sobre paquetes del filtro stateless. | SRX / vSRX (Configuración) | `set firewall family inet filter PROTECT-RE term PERMITIR-ADMIN then accept` | Todo filtro de firewall posee un `discard` implícito al final. Siempre agrega un término explícito final para evitar bloquear OSPF/BGP/NTP/ARP o tráfico legítimo. |
| `set interfaces lo0 unit 0 family inet filter input <filtro>` | Aplica el filtro stateless al tráfico dirigido hacia el Routing Engine del firewall. | SRX / vSRX (Configuración) | `set interfaces lo0 unit 0 family inet filter input PROTECT-RE` | **Advertencia:** en Junos un filtro en `lo0.0 input` protege todas las IPs locales del SRX (incluyendo interfaces físicas). Si bloqueas OSPF/BGP/IKE/NTP sin permitirlo en un término, esos protocolos dejarán de funcionar. |

---

## Configuración de VPN IPsec (Route-Based y Policy-Based)

Los firewalls Juniper SRX destacan por su implementación **Route-Based VPN** (usando la interfaz lógica `st0.0`), considerada mejor práctica frente al modelo tradicional *Policy-Based* porque permite enrutar dinámicamente con OSPF/BGP, aplicar políticas limpias por zonas y gestionar múltiples subredes sin modificar túneles.

Para que una VPN IPsec *Route-Based* levante y curse tráfico correctamente en un SRX deben cumplirse **4 requisitos obligatorios**:
1. Permitir `host-inbound-traffic system-services ike` en la interfaz WAN (`untrust`).
2. Vincular la interfaz de túnel `st0.0` a una zona de seguridad (ej. `vpn`) y crear una ruta estática o dinámica hacia la red remota apuntando a `next-hop st0.0`.
3. Crear políticas de seguridad bidireccionales (`trust -> vpn` y `vpn -> trust`) que permitan el tráfico entre ambas subredes.
4. Asegurar una regla **Source NAT `off`** si la red local se comunica con la red remota evitando que se aplique PAT de salida a Internet.

### Fase 1: IKE (Proposal, Policy y Gateway)

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set security ike proposal <prop> authentication-method pre-shared-keys` | Define el método de autenticación de Fase 1 por llave precompartida (PSK). | SRX / vSRX (Configuración) | `set security ike proposal P1-IKE authentication-method pre-shared-keys` | También soporta certificados digitales (`rsa-signatures` o `ecdsa-signatures`). |
| `set security ike proposal <prop> dh-group <grupo> authentication-algorithm <alg> encryption-algorithm <cifrado>` | Configura los parámetros criptográficos de la Fase 1 (DH, Hash y Cifrado). | SRX / vSRX (Configuración) | `set security ike proposal P1-IKE dh-group group14 authentication-algorithm sha-256 encryption-algorithm aes-256-cbc` | Ambos extremos del túnel deben coincidir exactamente en DH group (`group14`, `group19`, `group20`), algoritmo de hash y cifrado (incl. `lifetime-seconds`). |
| `set security ike policy <pol> mode <main\|aggressive>` | Define el modo de negociación en IKEv1. | SRX / vSRX (Configuración) | `set security ike policy POL-IKE mode main` | Si utilizas IKEv2 (`version v2-only`), este comando **no es necesario** (Junos omite `mode` en IKEv2). |
| `set security ike policy <pol> proposals <prop>` | Vincula uno o más proposals IKE a la política de Fase 1. | SRX / vSRX (Configuración) | `set security ike policy POL-IKE proposals P1-IKE` | Alternativamente pueden usarse conjuntos estándar (`proposal-set standard`). |
| `set security ike policy <pol> pre-shared-key ascii-text "<clave>"` | Configura la llave secreta compartida para autenticar el túnel VPN. | SRX / vSRX (Configuración) | `set security ike policy POL-IKE pre-shared-key ascii-text "ClaveVPN2026!"` | Junos cifra el texto en la configuración activa mostrando el hash `$9$...`. |
| `set security ike gateway <gw> ike-policy <pol>` | Crea el objeto Gateway remoto y le asigna la política de Fase 1. | SRX / vSRX (Configuración) | `set security ike gateway GW-SEDE2 ike-policy POL-IKE` | Identifica al peer remoto. |
| `set security ike gateway <gw> address <ip-peer>` | Indica la dirección IP pública del firewall remoto. | SRX / vSRX (Configuración) | `set security ike gateway GW-SEDE2 address 200.1.2.2` | Si el extremo remoto tiene IP dinámica, se usa `dynamic hostname ...` con IKE agresivo o IKEv2. |
| `set security ike gateway <gw> external-interface <if.unit>` | Especifica la interfaz WAN local por la cual se negocia el túnel IKE. | SRX / vSRX (Configuración) | `set security ike gateway GW-SEDE2 external-interface ge-0/0/0.0` | Asegúrate de tener `ike` permitido en `host-inbound-traffic` de esta interfaz/zona. |
| `set security ike gateway <gw> version <v2-only\|v1-only>` | Fuerza la versión de protocolo IKE utilizada con el peer remoto. | SRX / vSRX (Configuración) | `set security ike gateway GW-SEDE2 version v2-only` | `v2-only` es la opción recomendada hoy por seguridad, menor latencia de negociación y compatibilidad entre fabricantes. |
| `set security ike gateway <gw> dead-peer-detection interval <seg> threshold <n>` | Habilita DPD para detectar caídas del peer remoto y renegociar automáticamente. | SRX / vSRX (Configuración) | `set security ike gateway GW-SEDE2 dead-peer-detection interval 10 threshold 3` | Evita que el túnel quede "colgado" arriba cuando el equipo remoto pierde conectividad sin avisar. |
| `set security ike gateway <gw> nat-keepalive <seg>` | Envía paquetes keepalive UDP 4500 periódicos cuando el SRX está detrás de un router con NAT. | SRX / vSRX (Configuración) | `set security ike gateway GW-SEDE2 nat-keepalive 20` | Mantiene viva la traducción NAT-T (UDP 4500) en equipos intermedios. |

### Fase 2: IPsec (Proposal, Policy, VPN y Túnel st0)

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set security ipsec proposal <prop> protocol esp` | Establece ESP como protocolo de encapsulación cifrada de Fase 2. | SRX / vSRX (Configuración) | `set security ipsec proposal P2-IPSEC protocol esp` | ESP cifra y autentica el tráfico; es el estándar actual frente a AH. |
| `set security ipsec proposal <prop> authentication-algorithm <alg> encryption-algorithm <cifrado>` | Configura los algoritmos de integridad y cifrado para los datos del túnel. | SRX / vSRX (Configuración) | `set security ipsec proposal P2-IPSEC authentication-algorithm hmac-sha-256-128 encryption-algorithm aes-256-cbc` | Si seleccionas cifrado AEAD como `aes-256-gcm`, no debes configurar `authentication-algorithm` separado. |
| `set security ipsec policy <pol> perfect-forward-secrecy keys <grupo>` | Activa PFS en Fase 2 para generar claves independientes de la Fase 1. | SRX / vSRX (Configuración) | `set security ipsec policy POL-IPSEC perfect-forward-secrecy keys group14` | Si el equipo remoto (Cisco ASA/IOS/FortiGate) usa PFS, ambos extremos deben coincidir en el grupo DH. |
| `set security ipsec policy <pol> proposals <prop>` | Asocia el proposal IPsec de Fase 2 a la política IPsec. | SRX / vSRX (Configuración) | `set security ipsec policy POL-IPSEC proposals P2-IPSEC` | Verifica que el `lifetime-seconds` de Fase 2 sea compatible si conectas con otro fabricante. |
| `set security ipsec vpn <vpn> bind-interface st0.<n>` | Vincula la VPN a una interfaz de túnel lógica (`st0`) para VPN **Route-Based**. | SRX / vSRX (Configuración) | `set security ipsec vpn VPN-SEDE2 bind-interface st0.0` | **Paso clave:** requiere haber creado `st0.0` con `family inet`, asignarla a una zona y enrutar hacia `next-hop st0.0`. |
| `set security ipsec vpn <vpn> ike gateway <gw>` | Asocia el túnel IPsec con el Gateway de Fase 1 configurado previamente. | SRX / vSRX (Configuración) | `set security ipsec vpn VPN-SEDE2 ike gateway GW-SEDE2` | Conecta la configuración de Fase 2 con la Fase 1. |
| `set security ipsec vpn <vpn> ike ipsec-policy <pol>` | Asocia la política de cifrado IPsec de Fase 2 al túnel VPN. | SRX / vSRX (Configuración) | `set security ipsec vpn VPN-SEDE2 ike ipsec-policy POL-IPSEC` | Define las propuestas de encriptación válidas para las SAs de datos. |
| `set security ipsec vpn <vpn> establish-tunnels immediately` | Levanta el túnel IPsec automáticamente al hacer `commit` sin esperar tráfico. | SRX / vSRX (Configuración) | `set security ipsec vpn VPN-SEDE2 establish-tunnels immediately` | La opción por defecto `on-traffic` espera al primer paquete coincidente para negociar el túnel. |
| `set security ipsec vpn <vpn> ike proxy-identity local <red>/<pref> remote <red>/<pref> service any` | Declara selectores de tráfico (Proxy-ID) explícitos para compatibilidad con VPNs **Policy-Based**. | SRX / vSRX (Configuración) | `set security ipsec vpn VPN-SEDE2 ike proxy-identity local 10.104.0.0/16 remote 10.200.0.0/16 service any` | En VPN Route-Based contra otro SRX suele ser `0.0.0.0/0` automático; al conectar contra Cisco ASA/FortiGate por políticas es casi siempre obligatorio definir los `proxy-identity` exactos. |

---

## Alta Disponibilidad: Chassis Cluster (HA en SRX)

Dos firewalls Juniper SRX idénticos (mismo modelo y misma versión de Junos OS) pueden unirse en **Chassis Cluster** para operar como un único equipo lógico en alta disponibilidad (Activo/Pasivo o Activo/Activo). El clúster sincroniza sesiones stateful y configuración mediante dos enlaces dedicados entre nodos: el plano de control (**control link**, puerto dedicado en hardware o `fxp1`) y el plano de datos/fabric (**`fab0` / `fab1`**).

Las interfaces de red físicas de ambos nodos se agrupan en interfaces redundantes virtuales llamadas **`reth`** (*Redundant Ethernet*), cuya conmutación controla un **Redundancy Group (RG)**: `redundancy-group 0` controla al Routing Engine (plano de control), mientras `redundancy-group 1` en adelante controla las interfaces de datos `reth` y las IPs virtuales.

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `set chassis cluster cluster-id <1-15> node <0\|1> reboot` | Habilita el modo Chassis Cluster en cada nodo y reinicia el equipo. | SRX Cluster HA (Operacional) | `set chassis cluster cluster-id 1 node 0 reboot` | Se ejecuta en **modo operacional** (`>`), no en configuración. El nodo primario será `node 0` y el secundario `node 1`, con el mismo `cluster-id`. |
| `set chassis cluster disable reboot` | Desactiva el modo Chassis Cluster y devuelve el equipo a modo standalone. | SRX Cluster HA (Operacional) | `set chassis cluster disable reboot` | Requiere reinicio y restablece el nombrado estándar de interfaces (`ge-0/0/x`). |
| `set groups node0 system host-name <nombre>` | Define parámetros específicos para el Nodo 0 (nombre, IP `fxp0`, ruta de respaldo). | SRX Cluster HA (Configuración) | `set groups node0 system host-name FW-SRX-N0` | Se replica su equivalente en `set groups node1 ...` para que cada nodo mantenga su identidad y su propia IP de gestión `fxp0`. |
| `set groups node0 interfaces fxp0 unit 0 family inet address <ip>/<pref>` | Asigna la dirección IP de gestión individual OOB al Nodo 0 (y análogo en `node1`). | SRX Cluster HA (Configuración) | `set groups node0 interfaces fxp0 unit 0 family inet address 192.168.104.51/24` | Permite administrar individualmente cada firewall del clúster por `fxp0` aunque el otro nodo esté caído. |
| `set apply-groups "${node}"` | Activa la aplicación dinámica de los grupos `node0` y `node1` en el clúster. | SRX Cluster HA (Configuración) | `set apply-groups "${node}"` | **Obligatorio** para que cada miembro lea su bloque `groups node0` o `groups node1` correspondiente. |
| `set chassis cluster reth-count <cantidad>` | Declara cuántas interfaces redundantes `reth` tendrá el clúster. | SRX Cluster HA (Configuración) | `set chassis cluster reth-count 4` | Debe cubrir al menos la cantidad total de interfaces `reth0`, `reth1`, etc. configuradas. |
| `set chassis cluster redundancy-group <0\|1> node <0\|1> priority <1-254>` | Establece la prioridad de cada nodo en el grupo de redundancia (`0` = RE, `1+` = datos). | SRX Cluster HA (Configuración) | `set chassis cluster redundancy-group 1 node 0 priority 200` | El nodo con mayor prioridad (ej. `200` vs `100`) asume el rol **Primary** (activo). Configura prioridades para `redundancy-group 0` y `redundancy-group 1`. |
| `set chassis cluster redundancy-group <id> interface-monitor <if> weight <peso>` | Monitoriza una interfaz física y resta peso de prioridad si cae el enlace. | SRX Cluster HA (Configuración) | `set chassis cluster redundancy-group 1 interface-monitor ge-0/0/0 weight 255` | Si el umbral inicial es `255` y el enlace `ge-0/0/0` cae (resta `255`), el RG llega a 0 y conmuta al nodo secundario. Repite para puertos de `node1` (ej. `ge-3/0/0` en SRX320). |
| `set interfaces fab0 fabric-options member-interfaces <if>` | Asigna la interfaz física usada como enlace Fabric (`fab0`) en el Nodo 0 (y `fab1` en Nodo 1). | SRX Cluster HA (Configuración) | `set interfaces fab0 fabric-options member-interfaces ge-0/0/2` | Transfiere los datos de sincronización de sesión en tiempo real entre nodos. |
| `set interfaces <if> gigether-options redundant-parent <rethX>` | Une interfaces físicas de ambos nodos como miembros de una misma interfaz `reth`. | SRX Cluster HA (Configuración) | `set interfaces ge-0/0/0 gigether-options redundant-parent reth0` | Asocia un puerto de `node0` (`ge-0/0/0`) y su par de `node1` (ej. `ge-3/0/0` en SRX320 o `ge-7/0/0` en vSRX) al mismo `reth0`. |
| `set interfaces <rethX> redundant-ether-options redundancy-group <id>` | Vincula la interfaz `reth` con el grupo de redundancia que controla su conmutación. | SRX Cluster HA (Configuración) | `set interfaces reth0 redundant-ether-options redundancy-group 1` | Las IPs IPv4/IPv6 y la zona de seguridad se configuran directamente sobre `reth0.0`, nunca sobre `ge-0/0/0`. |
| `request chassis cluster failover redundancy-group <id> node <0\|1>` | Fuerza manualmente el failover de un grupo de redundancia hacia el nodo indicado. | SRX Cluster HA (Operacional) | `request chassis cluster failover redundancy-group 1 node 1` | Útil en pruebas de contingencia o mantenimientos planificados. |
| `request chassis cluster failover reset redundancy-group <id>` | Libera el bloqueo de failover manual para que el grupo retorne según sus prioridades. | SRX Cluster HA (Operacional) | `request chassis cluster failover reset redundancy-group 1` | Tras forzar un failover manual, Junos bloquea con bandera *manual failover* hasta que ejecutes este `reset`. |

---

## Comandos de Verificación, Monitoreo y Troubleshooting

En esta sección se reúnen los comandos de **modo operacional** (`>`) esenciales para auditar el estado del hardware, la tabla de enrutamiento, las sesiones stateful del firewall, contadores de políticas, traducciones NAT, asociaciones IPsec y captura de tráfico en tiempo real.

### Sistema, Hardware y Configuración

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `show version` | Muestra el modelo del firewall, hostname y versión exacta de Junos OS instalada. | SRX / vSRX (Operacional) | `show version` | Útil para verificar compatibilidad ELS vs Legacy y paquetes instalados. |
| `show chassis hardware` | Lista los componentes físicos del equipo, números de serie y módulos instalados. | SRX Físico / vSRX (Operacional) | `show chassis hardware` | Indispensable para inventario, soporte JTAC y verificación de módulos PIM/SFP. |
| `show chassis alarms` | Muestra las alarmas activas de hardware o sistema (fuentes, ventiladores, enlaces, disco). | SRX / vSRX (Operacional) | `show chassis alarms` | Equivalente rápido a revisar LEDs de alarma en el frontal del equipo; revisa también `show system alarms`. |
| `show chassis routing-engine` | Despliega uso de CPU, memoria RAM, temperatura del RE y causa del último reinicio. | SRX / vSRX (Operacional) | `show chassis routing-engine` | Permite detectar cuellos de botella de memoria o reinicios inesperados. |
| `show system uptime` | Indica cuánto tiempo lleva encendido el firewall y la hora exacta del sistema. | SRX / vSRX (Operacional) | `show system uptime` | Confirma que no haya reinicios recientes ni desincronización de reloj antes de leer logs. |
| `show system storage` | Muestra el espacio libre y ocupado en las particiones de almacenamiento del equipo. | SRX / vSRX (Operacional) | `show system storage` | Si `/var` se llena por trazas o logs antiguos, puedes liberar archivos temporales con `request system storage cleanup`. |
| `show system commit` | Muestra el historial de los últimos commits realizados, usuario, hora y comentario. | SRX / vSRX (Operacional) | `show system commit` | Facilita auditar quién aplicó un cambio reciente y qué número de `rollback` corresponde. |
| `show system rollback compare <n> <m>` | Muestra las diferencias exactas entre dos versiones históricas de configuración guardadas. | SRX / vSRX (Operacional) | `show system rollback compare 1 0` | Compara `rollback 1` contra la configuración activa `0` sin necesidad de entrar al modo configuración. |
| `show configuration \| display set` | Imprime toda la configuración activa en formato plano de comandos `set`. | SRX / vSRX (Operacional) | `show configuration \| display set \| match nat` | Añade `\| match <texto>` para revisar rápidamente solo una sección (ej. `ike`, `ospf`, `dhcp`). |

### Interfaces, Enrutamiento y DHCP

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `show interfaces terse` | Muestra un resumen tabular del estado físico (`Admin`), lógico (`Link`) e IPs de todas las interfaces. | SRX / vSRX (Operacional) | `show interfaces terse \| match ge-0/0` | Equivalente a `show ip interface brief` de Cisco IOS. Verifica que `ge-0/0/x` esté `up up`. |
| `show interfaces <if> extensive` | Despliega estadísticas detalladas, errores CRC, descartes, velocidad, duplex y MAC. | SRX / vSRX (Operacional) | `show interfaces ge-0/0/0 extensive` | Clave para detectar errores físicos de cableado, desajustes de velocidad/MTU o saturación de colas. |
| `show ethernet-switching table` | Muestra la tabla de direcciones MAC aprendidas por los puertos L2 en modo switch. | SRX / vSRX (Operacional) | `show ethernet-switching table` | Equivalente a `show mac address-table` en switches Cisco. |
| `show vlans` | Lista las VLANs configuradas, su VLAN ID, interfaces miembro e interfaz IRB/RVI asociada. | SRX / vSRX (Operacional) | `show vlans` | Verifica que los puertos trunk y access pertenezcan a la VLAN esperada. |
| `show arp` | Muestra la tabla ARP IPv4 aprendida por el firewall (IP, dirección MAC e interfaz). | SRX / vSRX (Operacional) | `show arp no-resolve` | Usar `no-resolve` acelera la consulta evitando resolución DNS inversa de cada IP. |
| `show ipv6 neighbors` | Muestra la tabla de descubrimiento de vecinos (NDP) para direcciones IPv6. | SRX / vSRX (IPv6) | `show ipv6 neighbors` | Equivalente a `show arp` en redes IPv6. |
| `show route` | Despliega toda la tabla de enrutamiento del firewall (`inet.0`, `inet6.0` y VRs). | SRX / vSRX (Operacional) | `show route table inet.0` | Muestra con `*` la ruta activa seleccionada cuando existen múltiples orígenes. |
| `show route <destino>` | Consulta la ruta más específica (*longest prefix match*) que usará el SRX hacia una IP. | SRX / vSRX (Operacional) | `show route 8.8.8.8` | Primer paso obligatorio de troubleshooting para comprobar por qué interfaz saldrá el paquete. |
| `show ospf neighbor` | Muestra los vecinos OSPF descubiertos, su estado (`Full`, `2Way`) y la interfaz. | SRX / vSRX (Operacional) | `show ospf neighbor` | Para OSPFv3 IPv6 usa `show ospf3 neighbor`. |
| `show bgp summary` | Muestra el estado de las sesiones BGP (`Establ`, `Active`, `Connect`) y prefijos recibidos. | SRX / vSRX (Operacional) | `show bgp summary` | Si aparece un número entero en la última columna (`Active/Accepted/Damped`), la sesión está establecida (`Established`). |
| `show dhcp server binding` | Lista las concesiones activas entregadas por el servidor JDHCP (IP, MAC, tiempo restante). | SRX / vSRX (Operacional) | `show dhcp server binding` | En modo relay usa `show dhcp relay binding`. Si el equipo usa el servidor DHCP antiguo (12.1X47) usa `show system services dhcp binding`. |

### Seguridad, Flujos, NAT, Screen, VPN y Cluster

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `show security zones` | Lista todas las zonas de seguridad, sus interfaces asociadas y servicios `host-inbound` permitidos. | SRX / vSRX (Operacional) | `show security zones untrust` | Comprueba al instante si olvidaste incluir `ge-0/0/x.0` o habilitar `ping`/`ssh`/`ike`. |
| `show security policies` | Muestra todas las políticas configuradas agrupadas por par de zonas origen/destino. | SRX / vSRX (Operacional) | `show security policies from-zone trust to-zone untrust` | Útil para verificar el orden real en que se evalúan las políticas tras ediciones con `insert`. |
| `show security policies hit-count` | Muestra cuántas veces ha coincidido cada política de seguridad con tráfico real. | SRX / vSRX (Operacional) | `show security policies hit-count` | **Herramienta estrella de depuración:** confirma exactamente qué regla está dejando pasar o bloqueando paquetes. |
| `show security flow status` | Muestra el modo de reenvío (`flow-based`), estado de TCP MSS y resumen global de flujos. | SRX / vSRX (Operacional) | `show security flow status` | Permite confirmar si IPv4/IPv6 están operando en modo firewall o packet-mode. |
| `show security flow session [filtros]` | Muestra las sesiones activas en la tabla stateful del firewall (origen, destino, NAT, interfaz). | SRX / vSRX (Operacional) | `show security flow session destination-prefix 8.8.8.8` | Usa `summary` para ver solo totales o filtra por `source-prefix`, `destination-port`, `protocol` o `policy-name`. |
| `show security nat source summary` | Muestra el resumen de reglas Source NAT configuradas y el conteo de traducciones activas. | SRX / vSRX (Operacional) | `show security nat source summary` | Usa `show security nat source rule all` para ver los `translation-hits` individuales de cada regla. |
| `show security nat destination rule all` | Muestra las reglas Destination NAT, sus pools destino y el número de aciertos (*hits*). | SRX / vSRX (Operacional) | `show security nat destination rule all` | Si `Successful translations` no sube al probar desde fuera, el paquete no está llegando o no coincide el match. |
| `show security nat static rule all` | Lista todas las reglas Static NAT 1:1 y sus contadores de traducción. | SRX / vSRX (Operacional) | `show security nat static rule all` | Útil para verificar traducciones bidireccionales activas hacia servidores en DMZ. |
| `show security screen statistics zone <zona>` | Muestra los contadores de ataques detectados y bloqueados por Screen en una zona. | SRX / vSRX (Operacional) | `show security screen statistics zone untrust` | Permite comprobar si Screen está descartando tráfico legítimo como falso positivo (ej. `SYN flood` o `port-scan`). |
| `show security ike security-associations` | Verifica el estado de la Fase 1 IKE de las VPNs (`UP` o en negociación). | SRX / vSRX (Operacional) | `show security ike security-associations` | Usa `detail` al final para ver algoritmos negociados, versión IKE y tiempos de vida restantes. |
| `show security ipsec security-associations` | Verifica los túneles activos de Fase 2 IPsec, SPIs, interfaz `st0` y algoritmos ESP. | SRX / vSRX (Operacional) | `show security ipsec security-associations` | Un túnel operativo debe aparecer en `show security ipsec security-associations` con ambos SPIs (entrada/salida) creados. |
| `show security ipsec statistics` | Muestra contadores detallados de paquetes cifrados, descifrados y errores ESP/replay. | SRX / vSRX (Operacional) | `show security ipsec statistics` | Si `Encrypted packets` sube pero `Decrypted packets` permanece en 0, el extremo remoto no responde o existe bloqueo/NAT intermedio. |
| `show security alg status` | Muestra qué ALGs de aplicación (`sip`, `dns`, `ftp`, etc.) están habilitados o desactivados. | SRX / vSRX (Operacional) | `show security alg status` | Confirma que `sip` o `dns` estén en estado `Disabled` tras desactivarlos para VoIP o troubleshooting. |
| `show firewall` | Muestra todos los filtros stateless (`firewall family inet`) y los contadores de cada término. | SRX / vSRX (Operacional) | `show firewall` | Equivalente a `show access-lists` con contadores en Cisco IOS. |
| `show chassis cluster status` | Muestra el estado de cada nodo (`primary`/`secondary`), prioridad y motivo de conmutación. | SRX Cluster HA (Operacional) | `show chassis cluster status` | En operación normal ambos nodos deben estar en línea y uno como `primary` y otro como `secondary` sin errores. |
| `show chassis cluster interfaces` | Muestra el estado del enlace de control, enlace fabric (`fab0`/`fab1`) y puertos `reth`. | SRX Cluster HA (Operacional) | `show chassis cluster interfaces` | Confirma que `control-link` y `fabric-link` aparezcan `Up` y verifica qué interfaces monitorizadas están activas. |

### Diagnóstico Activo, Logs, Limpieza y Mantenimiento

| **Comando** | **Uso** | **Aplicable a** | **Ejemplo** | **Notas** |
| :--- | :--- | :--- | :--- | :--- |
| `ping <destino> [source <ip>] [routing-instance <vr>]` | Prueba conectividad ICMP desde el firewall especificando opcionalmente IP origen o VR. | SRX / vSRX (Operacional) | `ping 8.8.8.8 source 200.1.1.2 count 5` | Especificar `source` permite simular el tráfico desde la subred LAN sin salir desde la IP WAN. Añade `do-not-fragment size 1472` para probar MTU. |
| `traceroute <destino> [source <ip>]` | Muestra los saltos de enrutamiento desde el firewall hasta el destino. | SRX / vSRX (Operacional) | `traceroute 8.8.8.8 no-resolve` | Usar `no-resolve` acelera la traza cuando no hay acceso DNS funcional. |
| `monitor traffic interface <if> [opciones]` | Captura paquetes en vivo tipo `tcpdump` que entran o salen hacia/desde el propio SRX. | SRX / vSRX (Operacional) | `monitor traffic interface ge-0/0/0 matching "host 200.1.2.2 and port 500" no-resolve` | **Nota importante:** en SRX, `monitor traffic` captura tráfico del **plano de control** (IKE, BGP, OSPF, ICMP al firewall); para tráfico de **tránsito** entre zonas usa `security flow traceoptions` o port-mirroring. |
| `monitor start <archivo-log>` | Muestra en vivo nuevas líneas escritas en un archivo de `/var/log/` directamente en tu terminal. | SRX / vSRX (Operacional) | `monitor start traffic-log` | Muy práctico durante pruebas de reglas de firewall; detén el streaming en pantalla con `monitor stop`. |
| `show log <archivo> \| last <n>` | Lee las últimas `n` líneas de un archivo de registro almacenado en el firewall. | SRX / vSRX (Operacional) | `show log messages \| last 50` | Usa `show log messages \| match RT_FLOW_SESSION_DENY` para encontrar rápidamente qué sesiones está bloqueando la política. |
| `clear security flow session [filtro]` | Elimina sesiones activas de la tabla de flujos para forzar su reevaluación con nuevas reglas/NAT. | SRX / vSRX (Operacional) | `clear security flow session destination-prefix 10.104.1.50` | **Muy importante:** tras crear una regla `deny` o ajustar NAT, las conexiones previas siguen activas hasta que expiren o ejecutes `clear security flow session`. Evita `all` en producción si no es necesario. |
| `clear security ike security-associations` | Reinicia las negociaciones de Fase 1 IKE de los túneles VPN activos. | SRX / vSRX (Operacional) | `clear security ike security-associations` | Puedes pasar el parámetro `<index>` si solo quieres reiniciar un túnel concreto. |
| `clear security ipsec security-associations` | Fuerza la renegociación de las asociaciones de Fase 2 IPsec. | SRX / vSRX (Operacional) | `clear security ipsec security-associations` | Útil tras ajustar propuestas Fase 2 o selectores Proxy-ID. |
| `clear security policies hit-count` | Pone en cero los contadores de coincidencias de las políticas de seguridad. | SRX / vSRX (Operacional) | `clear security policies hit-count` | Ideal antes de iniciar una prueba controlada para verificar qué reglas se activan desde cero. |
| `request system reboot` | Reinicia de forma ordenada el sistema operativo Junos OS en el firewall. | SRX / vSRX (Operacional) | `request system reboot` | Pide confirmación (`yes/no`). En Chassis Cluster usa `all-members` o reinicia nodo por nodo con cuidado. |
| `request system power-off` | Apaga de manera limpia el firewall antes de desconectar la alimentación eléctrica. | SRX / vSRX (Operacional) | `request system power-off` | Evita corrupción del sistema de archivos FreeBSD en hardware SRX físico. |
| `request system zeroize` | Borra por completo configuraciones, logs, llaves y datos dejando el equipo de fábrica. | SRX / vSRX (Operacional) | `request system zeroize` | **Destructivo e irreversible**: equivalente a borrar `startup-config` + formatear datos en Cisco; el equipo se reinicia limpio. |
