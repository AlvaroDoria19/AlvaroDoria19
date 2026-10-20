# Bitácora de comandos Cisco: AAA y RADIUS


## Índice

- [1. Conceptos clave](#1-conceptos-clave)
- [2. Preparar el servidor RADIUS](#2-preparar-el-servidor-radius)
- [3. Declarar el servidor RADIUS en IOS](#3-declarar-el-servidor-radius-en-ios)
- [4. Crear listas AAA de autenticación](#4-crear-listas-aaa-de-autenticación)
- [5. Aplicar AAA a consola y VTY](#5-aplicar-aaa-a-consola-y-vty)
- [6. Autorización y nivel de privilegio](#6-autorización-y-nivel-de-privilegio)
- [7. Verificación y diagnóstico](#7-verificación-y-diagnóstico)
- [8. Conceptos clave de ACL](#8-conceptos-clave-de-acl)
- [9. Máscaras wildcard](#9-máscaras-wildcard)
- [10. ACL estándar](#10-acl-estándar)
- [11. ACL extendida](#11-acl-extendida)
- [12. Aplicar una ACL a una interfaz](#12-aplicar-una-acl-a-una-interfaz)
- [13. ACL para administración y RADIUS](#13-acl-para-administración-y-radius)
- [14. Edición, verificación y diagnóstico de ACL](#14-edición-verificación-y-diagnóstico-de-acl)

## 1. Conceptos clave

RADIUS centraliza la autenticación. El router actúa como **NAS/cliente RADIUS** y consulta un servidor cuando una persona inicia sesión. La lista AAA define qué método se intenta primero y cuál se usa si el anterior devuelve un error.

| Término | Función | Ejemplo |
|---|---|---|
| AAA | Control de autenticación, autorización y registro de actividad | `aaa new-model` habilita el marco AAA en IOS. |
| RADIUS client / NAS | Equipo que envía solicitudes al servidor | El router debe registrarse en el servidor con su IP de origen. |
| Shared secret | Secreto común entre NAS y servidor | Debe coincidir en ambos extremos; no es la contraseña del usuario. |
| Método `group radius` | Consulta todos los servidores RADIUS definidos en IOS | Útil con una configuración sencilla de uno o más hosts. |
| Método `group <nombre>` | Consulta servidores incluidos en un grupo AAA nombrado | Permite separar grupos de servidores para distintas listas AAA. |
| Método `local` | Consulta usuarios configurados con `username` en el router | Se puede usar como respaldo si RADIUS devuelve un error. |

**Límite del respaldo local:** `group radius local` no significa “intenta local cuando la contraseña RADIUS es incorrecta”. IOS pasa al siguiente método cuando el anterior devuelve un error (por ejemplo, RADIUS no responde); si el servidor devuelve un rechazo explícito, el inicio de sesión falla sin probar `local`. [Cisco documenta este comportamiento para las listas AAA](https://www.cisco.com/c/en/us/td/docs/ios-xml/ios/security/a1/sec-a1-cr-book/sec-cr-a1.html).

## 2. Preparar el servidor RADIUS

El router y el servidor deben tener conectividad IP. En la configuración del servidor RADIUS, registra cada router como cliente/NAS y crea las cuentas de los administradores.

| Dato del servidor | Qué configurar | Nota |
|---|---|---|
| Cliente/NAS | IP de origen con la que el router envía las solicitudes | Si hay varias interfaces, identifica o configura la interfaz de origen y registra esa IP. |
| Shared secret | El mismo secreto que se configura en IOS | Distingue mayúsculas/minúsculas; no es el password del usuario. |
| Usuario y contraseña | Cuenta de cada persona autorizada | Deben coincidir con las credenciales presentadas en el login. |
| Atributos de autorización | Nivel de privilegio u otras políticas, si se necesitan | Son distintos de la autenticación básica; dependen del servidor y de IOS. |

No añadir la IP de la PC desde la que se administra el router como cliente RADIUS. El cliente es el router, porque es quien envía el paquete Access-Request. Si hay más de un router, registra cada IP de origen como un cliente autorizado.

## 3. Declarar el servidor RADIUS en IOS

### 3.1 IOS clásico

El comando `radius-server host` registra un servidor. Configura los puertos de autenticación y accounting para que coincidan con los del servidor. Los puertos comunes actuales son UDP 1812/1813; algunas imágenes IOS antiguas o simuladores usan 1645/1646 como valores predeterminados.

```text
configure terminal
radius-server host <IP_RADIUS> auth-port 1812 acct-port 1813 key <SHARED_SECRET>
end
```

En algunas imágenes clásicas, el secreto se configura como un comando global separado:

```text
configure terminal
radius-server host <IP_RADIUS> auth-port 1812 acct-port 1813
radius-server key <SHARED_SECRET>
end
```

Usa una sola variante de clave por servidor según la sintaxis que acepte la imagen. Si el dispositivo y el servidor usan puertos distintos, la comunicación RADIUS no funcionará aunque la IP sea correcta.

### 3.3 Grupo de servidores

Un grupo nombrado permite limitar una lista de autenticación a servidores concretos. Define primero los servidores y luego agrúpalos.

**IOS clásico:**

```text
aaa group server radius <GRUPO_RADIUS>
 server <IP_RADIUS_1>
 server <IP_RADIUS_2>
exit
```


| Comando | Uso | Aplicable a | Ejemplo | Notas |
|---|---|---|---|---|
| `radius-server host <IP>` | Registrar host RADIUS | IOS clásico | `radius-server host 192.0.2.10` | Puede aceptar puertos, timeout, retransmisiones y clave en la misma línea. |
| `radius-server key <secreto>` | Definir secreto compartido global | Algunas imágenes IOS clásicas | `radius-server key <SHARED_SECRET>` | Comprueba si la plataforma permite una clave por host antes de usar varios servidores con secretos distintos. |
| `radius server <nombre>` | Crear objeto de servidor RADIUS | IOS XE | `radius server RAD1` | Se configura con dirección, puertos y clave dentro del submodo. |
| `aaa group server radius <grupo>` | Crear grupo de servidores | IOS / IOS XE | `aaa group server radius ADMIN-RADIUS` | Usa `server <IP>` en sintaxis clásica o `server name <nombre>` con objetos IOS XE. |
| `server <IP>` | Añadir una dirección a un grupo clásico | Modo `config-sg-radius` | `server 192.0.2.10` | El servidor debe estar definido en la configuración global. |
| `server name <nombre>` | Añadir un objeto a un grupo | IOS XE | `server name RAD1` | El nombre debe coincidir con el objeto `radius server`. |

## 4. Crear listas AAA de autenticación

`aaa new-model` habilita AAA. Después se define la lista de métodos. Una lista llamada `default` se usa de forma predeterminada; una lista con nombre se aplica explícitamente a las líneas con `login authentication`.

### Lista predeterminada: todos los servidores RADIUS configurados y luego local

```text
enable
configure terminal
username <USUARIO_LOCAL> privilege 15 secret <CLAVE_LOCAL>
aaa new-model
aaa authentication login default group radius local
end
```

### Lista con nombre: grupo RADIUS específico y luego local

```text
configure terminal
username <USUARIO_LOCAL> privilege 15 secret <CLAVE_LOCAL>
aaa new-model
aaa authentication login <LISTA_LOGIN> group <GRUPO_RADIUS> local
end
```

| Comando | Uso | Aplicable a | Ejemplo | Notas |
|---|---|---|---|---|
| `aaa new-model` | Habilitar AAA | IOS | `aaa new-model` | Activa el uso de listas AAA para autenticación. |
| `aaa authentication login default group radius local` | Autenticar con el grupo RADIUS global y luego con la base local ante error | Login de IOS | `... group radius local` | `group radius` utiliza los servidores RADIUS definidos globalmente. |
| `aaa authentication login <lista> group <grupo> local` | Crear una lista de login con grupo RADIUS específico y luego local | Login de IOS | `aaa authentication login ADMIN group RAD-GRP local` | La lista con nombre debe asociarse después a consola/VTY. |
| `username <usuario> privilege 15 secret <clave>` | Crear cuenta local de respaldo | Router/switch IOS | `username breakglass privilege 15 secret <CLAVE>` | `local` consulta esta base; `secret` cifra la contraseña local. |

## 5. Aplicar AAA a consola y VTY

Asocia la lista con nombre a las líneas que aceptarán conexiones. Si se usa la lista `default`, referencia `default`; también puedes dejar que sea la lista predeterminada siempre que la línea no tenga otra autenticación configurada.

```text
configure terminal
line console 0
 login authentication <LISTA_LOGIN>
exit
line vty 0 4
 login authentication <LISTA_LOGIN>
exit
end
copy running-config startup-config
```

| Comando | Uso | Aplicable a | Ejemplo | Notas |
|---|---|---|---|---|
| `line console 0` | Entrar a la configuración de consola | Router/switch | `line console 0` | Asocia AAA si también se requiere login RADIUS en consola. |
| `line vty 0 4` | Entrar a las líneas de acceso remoto | Router/switch | `line vty 0 4` | El rango disponible varía por modelo e imagen IOS. |
| `login authentication <lista>` | Asociar la lista AAA a la línea | Consola/VTY | `login authentication ADMIN` | No lo sustituyas por `login local` si quieres que esa línea consulte RADIUS. |
| `copy running-config startup-config` | Guardar cambios | IOS | `copy running-config startup-config` | Guarda después de verificar el acceso. |

Mantén una sesión de administración activa mientras cambias AAA. Crea primero la cuenta local de respaldo, aplica la lista a una segunda sesión y verifica el login antes de cerrar la sesión original.

## 6. Autorización y nivel de privilegio

Autenticar confirma quién inició sesión; autorización determina qué puede hacer después. Una autenticación RADIUS exitosa no implica necesariamente que la sesión entre directamente al modo privilegiado (`#`).

| Necesidad | Qué configurar | Nota |
|---|---|---|
| Login centralizado | `aaa authentication login ...` | Es la configuración principal de esta bitácora. |
| Respaldo local ante indisponibilidad | Añadir `local` al final de la lista y crear `username ... secret ...` | La siguiente opción se intenta ante error del método anterior, no ante un Access-Reject. |
| Entrar a modo privilegiado manualmente | Configurar `enable secret <CLAVE>` y usar `enable` | Aplica si la cuenta autenticada queda en nivel EXEC de usuario. |
| Asignar privilegio desde RADIUS | Configurar atributos de autorización compatibles en el servidor | En servidores compatibles con Cisco, puede usarse `Service-Type` y un atributo Cisco `shell:priv-lvl`; la sintaxis concreta depende del producto. |
| Autorización EXEC en IOS | `aaa authorization exec default group radius if-authenticated` | Opcional; valida compatibilidad y política del servidor antes de activarla. |

## 7. Verificación y diagnóstico

| Comando | Uso | Aplicable a | Ejemplo | Notas |
|---|---|---|---|---|
| `show running-config` | Consultar la configuración activa | IOS | Buscar `aaa`, `radius-server` o `radius server` | Comprueba también la autenticación asignada bajo las líneas. |
| `show aaa servers` | Consultar estado de servidores AAA | Algunas imágenes IOS | `show aaa servers` | La disponibilidad y salida cambian por versión. |
| `show radius statistics` | Consultar contadores RADIUS | Algunas imágenes IOS | `show radius statistics` | Útil para ver solicitudes, respuestas y timeouts si está soportado. |
| `show users` | Ver sesiones activas | IOS | `show users` | Confirma la sesión utilizada para verificar el login. |
| `show privilege` | Consultar nivel de privilegio de la sesión | IOS | `show privilege` | Distingue login correcto de autorización de privilegio. |
| `ping <IP_RADIUS>` | Verificar conectividad IP | IOS | `ping 192.0.2.10` | Un ping correcto no valida secreto compartido ni credenciales. |

| Síntoma | Revisar |
|---|---|
| No hay respuesta del servidor | IP/ruta, puertos UDP acordados, servicio RADIUS activo y ACL/firewall entre NAS y servidor. |
| Shared secret incorrecto | El secreto del host en IOS debe coincidir exactamente con el del NAS registrado en RADIUS. |
| Usuario rechazado | Cuenta activa, contraseña, política de acceso y atributos requeridos por el servidor. Un Access-Reject no dispara `local`. |
| El respaldo local no funciona | Confirma que existe el usuario IOS y que RADIUS realmente devuelve error/no responde; un rechazo explícito no continúa la lista. |
| Solo una línea falla | Revisa si esa consola/VTY tiene otra lista `login authentication` o configuración `login local`. |
| Login correcto, privilegio insuficiente | Revisa autorización EXEC, atributos de privilegio RADIUS o el uso de `enable secret`. |

## 8. Conceptos clave de ACL

Una lista de control de acceso (ACL) contiene entradas ACE (`permit` o `deny`) que IOS evalúa de arriba hacia abajo. La primera coincidencia decide el resultado y las entradas restantes ya no se revisan. Toda ACL IP termina con un `deny any` implícito, aunque no aparezca en la configuración.

| Concepto | Función | Nota |
|---|---|---|
| ACL estándar | Filtra únicamente por dirección IPv4 de origen | Conviene ubicarla cerca del destino para no bloquear ese origen hacia otros servicios. |
| ACL extendida | Filtra por protocolo, origen, destino y, para TCP/UDP, por puertos | Normalmente se coloca cerca del origen del tráfico que se quiere controlar. |
| ACL numerada | Se identifica mediante un número | Rangos habituales: estándar `1-99` y `1300-1999`; extendida `100-199` y `2000-2699`. |
| ACL nombrada | Se identifica mediante un nombre descriptivo | Facilita documentar y editar entradas mediante números de secuencia. |
| ACE | Una línea individual de la ACL | Ejemplo: `permit tcp 192.0.2.0 0.0.0.255 host 198.51.100.20 eq 443`. |
| Dirección `in` | Examina paquetes al entrar en una interfaz | El paquete se filtra antes de tomar la decisión de enrutamiento. |
| Dirección `out` | Examina paquetes antes de salir por una interfaz | El paquete ya pasó por la decisión de enrutamiento. |

Una ACL recién creada no filtra tráfico hasta que se aplica a una interfaz con `ip access-group`, a líneas VTY con `access-class` o a otra función que acepte una ACL. Antes de aplicarla, incluye explícitamente todo el tráfico necesario; de lo contrario, el `deny any` implícito puede causar pérdida de conectividad.

## 9. Máscaras wildcard

La máscara wildcard indica qué bits deben coincidir: un bit en `0` se compara y un bit en `1` se ignora. Para una red IPv4 convencional, se puede obtener restando cada octeto de la máscara de red a 255.

| Red o coincidencia | Máscara de red | Wildcard | Sintaxis equivalente |
|---|---|---|---|
| Un host | `255.255.255.255` | `0.0.0.0` | `host 192.0.2.10` o `192.0.2.10 0.0.0.0` |
| `192.0.2.0/24` | `255.255.255.0` | `0.0.0.255` | `192.0.2.0 0.0.0.255` |
| `172.16.8.0/21` | `255.255.248.0` | `0.0.7.255` | `172.16.8.0 0.0.7.255` |
| Cualquier dirección | No aplica | `255.255.255.255` | `any` |

No confundas una wildcard con una máscara de subred. Por ejemplo, para permitir `192.0.2.0/24` en una ACL se usa `0.0.0.255`, no `255.255.255.0`.

## 10. ACL estándar

### 10.1 ACL numerada

El siguiente ejemplo permite como origen la red de administración `192.0.2.0/24`. La denegación final se escribe para habilitar el registro de coincidencias; aun sin esa línea existiría un `deny any` implícito.

```text
configure terminal
access-list 10 remark RED-DE-ADMINISTRACION
access-list 10 permit 192.0.2.0 0.0.0.255
access-list 10 deny any log
end
```

### 10.2 ACL nombrada

```text
configure terminal
ip access-list standard ADMIN-SOURCES
 remark Origenes autorizados para administrar el equipo
 permit host 198.51.100.50
 permit 192.0.2.0 0.0.0.255
 deny any log
exit
end
```

| Comando | Uso | Ejemplo | Notas |
|---|---|---|---|
| `access-list <1-99> permit <origen> <wildcard>` | Crear una ACE estándar numerada | `access-list 10 permit 192.0.2.0 0.0.0.255` | También existe el rango ampliado `1300-1999`. |
| `ip access-list standard <nombre>` | Crear o abrir una ACL estándar nombrada | `ip access-list standard ADMIN-SOURCES` | Los comandos `permit`, `deny` y `remark` se introducen dentro del submodo. |
| `permit host <IP>` | Permitir un único origen | `permit host 198.51.100.50` | Equivale a usar wildcard `0.0.0.0`. |
| `deny any log` | Denegar los demás orígenes y registrar coincidencias | `deny any log` | El registro puede generar muchos mensajes; úsalo de acuerdo con la capacidad del equipo. |

## 11. ACL extendida

### 11.1 ACL numerada

Este ejemplo permite HTTPS desde la red de usuarios hacia un servidor concreto y deniega otros intentos de acceso IP al mismo servidor. La última línea permite que el resto del tráfico continúe.

```text
configure terminal
access-list 110 remark HTTPS-HACIA-SERVIDOR
access-list 110 permit tcp 192.0.2.0 0.0.0.255 host 198.51.100.20 eq 443
access-list 110 deny ip any host 198.51.100.20 log
access-list 110 permit ip any any
end
```

### 11.2 ACL nombrada

```text
configure terminal
ip access-list extended SERVIDOR-WEB
 remark Permitir HTTPS de usuarios al servidor
 permit tcp 192.0.2.0 0.0.0.255 host 198.51.100.20 eq 443
 remark Bloquear otros accesos al servidor
 deny ip any host 198.51.100.20 log
 permit ip any any
exit
end
```

| Comando o palabra clave | Uso | Ejemplo | Notas |
|---|---|---|---|
| `access-list <100-199> ...` | Crear una ACL extendida numerada | `access-list 110 permit tcp any host 198.51.100.20 eq 443` | También existe el rango ampliado `2000-2699`. |
| `ip access-list extended <nombre>` | Crear o abrir una ACL extendida nombrada | `ip access-list extended SERVIDOR-WEB` | Permite usar nombres y números de secuencia. |
| `tcp`, `udp`, `icmp`, `ip` | Seleccionar el protocolo | `permit icmp any any` | `ip` incluye cualquier protocolo transportado sobre IPv4. |
| `eq <puerto>` | Coincidir con un puerto TCP o UDP | `eq 22`, `eq 443`, `eq 1812` | Se coloca después del origen o destino al que pertenece el puerto. |
| `range <inicio> <fin>` | Coincidir con un intervalo de puertos | `range 10000 20000` | Ambos límites se incluyen. |
| `established` | Coincidir con TCP que tiene ACK o RST activo | `permit tcp any any established` | No convierte la ACL en un firewall con estado. |
| `log` | Registrar coincidencias de una ACE | `deny ip any any log` | Puede elevar el uso de CPU y el volumen de logs. |

## 12. Aplicar una ACL a una interfaz

Una ACL IPv4 se asocia a una interfaz mediante `ip access-group`. En cada interfaz se puede aplicar una ACL IPv4 por dirección (`in` y `out`). La dirección se interpreta desde el punto de vista de la interfaz del router.

```text
configure terminal
interface GigabitEthernet0/0
 description LAN-DE-USUARIOS
 ip access-group SERVIDOR-WEB in
exit
end
```

Para retirarla de la interfaz sin borrar la definición de la ACL:

```text
configure terminal
interface GigabitEthernet0/0
 no ip access-group SERVIDOR-WEB in
exit
end
```

| Comando | Uso | Ejemplo | Notas |
|---|---|---|---|
| `ip access-group <ACL> in` | Filtrar tráfico que entra por la interfaz | `ip access-group SERVIDOR-WEB in` | Revisa la topología para confirmar cuál es la interfaz de entrada. |
| `ip access-group <ACL> out` | Filtrar tráfico que sale por la interfaz | `ip access-group SERVIDOR-WEB out` | Considera también el tráfico de otros orígenes que use esa salida. |
| `no ip access-group <ACL> <dirección>` | Desasociar la ACL | `no ip access-group SERVIDOR-WEB in` | No elimina las ACE almacenadas en la configuración global. |

Aplica primero la ACL en una ventana controlada y mantén una sesión de administración abierta. Una dirección equivocada, una ACE mal ordenada o un permiso ausente puede bloquear SSH, RADIUS, DNS, DHCP u otros servicios necesarios.

## 13. ACL para administración y RADIUS

### 13.1 Restringir el acceso a las líneas VTY

Una ACL estándar puede limitar qué direcciones de origen llegan a las líneas VTY. Esto complementa AAA: la ACL decide desde dónde se permite conectar y AAA/RADIUS valida las credenciales.

```text
configure terminal
ip access-list standard VTY-ADMIN
 remark Solo red y host de administracion
 permit host 198.51.100.50
 permit 192.0.2.0 0.0.0.255
 deny any log
exit
line vty 0 4
 access-class VTY-ADMIN in
 transport input ssh
 login authentication <LISTA_LOGIN>
exit
end
```

Antes de aplicar `access-class`, confirma que la IP desde la que administras el equipo esté permitida. La ACL de VTY filtra la dirección de origen de la conexión de administración; no se aplica con `ip access-group` bajo una interfaz.

### 13.2 Permitir RADIUS y bloquear clientes no autorizados

El ejemplo siguiente protege un servidor RADIUS permitiendo autenticación y accounting únicamente desde el NAS `192.0.2.1`. Después bloquea esos puertos para otros orígenes, pero no afecta el resto del tráfico dirigido al servidor.

```text
configure terminal
ip access-list extended PROTEGER-RADIUS
 remark NAS autorizado hacia RADIUS
 permit udp host 192.0.2.1 host 198.51.100.10 eq 1812
 permit udp host 192.0.2.1 host 198.51.100.10 eq 1813
 remark Bloquear RADIUS desde otros origenes
 deny udp any host 198.51.100.10 eq 1812 log
 deny udp any host 198.51.100.10 eq 1813 log
 permit ip any any
exit
interface GigabitEthernet0/1
 description ENLACE-HACIA-RED-RADIUS
 ip access-group PROTEGER-RADIUS out
exit
end
```

Adapta las direcciones, la interfaz y la dirección `in`/`out` a la topología real. Si se usan puertos RADIUS antiguos, añade las ACE equivalentes para UDP 1645 y 1646. La ACL de red no sustituye el registro del NAS ni el secreto compartido en el servidor RADIUS.

Si la ACL se aplica en sentido contrario, por donde regresan las respuestas del servidor, los puertos 1812/1813 aparecen como **puertos de origen**. Por ejemplo:

```text
permit udp host 198.51.100.10 eq 1812 host 192.0.2.1
permit udp host 198.51.100.10 eq 1813 host 192.0.2.1
```

No asumas que una ACL de salida filtra tráfico generado localmente por el propio router: el comportamiento depende de la plataforma y de la versión de IOS/IOS XE. Verifica en el equipo y coloca el control en el punto de tránsito apropiado.

## 14. Edición, verificación y diagnóstico de ACL

### 14.1 Editar una ACL nombrada con secuencias

Las secuencias permiten insertar o retirar una ACE sin reconstruir toda la ACL.

```text
configure terminal
ip access-list extended SERVIDOR-WEB
 15 permit tcp host 192.0.2.50 host 198.51.100.20 eq 443
 no 20
 20 deny ip any host 198.51.100.20 log
exit
ip access-list resequence SERVIDOR-WEB 10 10
end
```

Comprueba siempre la ACL después de editarla. `no 20` elimina la ACE con secuencia 20; usar `no permit ...` exige escribir exactamente la entrada que se quiere retirar.

### 14.2 Comandos de verificación

| Comando | Uso | Notas |
|---|---|---|
| `show access-lists` | Mostrar todas las ACL y sus contadores | Los contadores ayudan a confirmar si una ACE recibe tráfico. |
| `show ip access-lists` | Mostrar las ACL IPv4 | Puede incluir secuencias y número de coincidencias. |
| `show ip access-lists <nombre>` | Mostrar una ACL IPv4 concreta | Ejemplo: `show ip access-lists PROTEGER-RADIUS`. |
| `show ip interface <interfaz>` | Ver las ACL aplicadas en entrada y salida | Confirma tanto el nombre como la dirección. |
| `show running-config \| section access-list` | Revisar definiciones de ACL en la configuración activa | La compatibilidad del filtro `section` depende de la imagen. |
| `show running-config interface <interfaz>` | Revisar la asociación bajo una interfaz | Busca `ip access-group`. |
| `clear access-list counters <ACL>` | Reiniciar contadores para una prueba controlada | La sintaxis o disponibilidad puede variar por plataforma. |

| Síntoma | Revisar |
|---|---|
| La ACL no filtra | Confirma que esté aplicada al punto correcto, en la dirección correcta y que el tráfico realmente pase por esa interfaz. |
| Todo el tráfico queda bloqueado | Busca permisos ausentes y recuerda el `deny any` implícito. |
| Una ACE nunca incrementa su contador | Una entrada anterior puede estar coincidiendo primero o el tráfico puede seguir otra ruta. |
| SSH dejó de funcionar | Revisa `access-class` en VTY, las ACE de origen y cualquier ACL aplicada a interfaces. |
| RADIUS dejó de responder | Revisa UDP 1812/1813 o 1645/1646, IP de origen del NAS, ambas direcciones del flujo y contadores de la ACL. |
| Los logs consumen demasiados recursos | Retira `log` de ACE con muchas coincidencias o limita su uso según las capacidades de la plataforma. |

## Referencias

- [Cisco IOS: configuración de RADIUS](https://www.cisco.com/c/en/us/td/docs/ios-xml/ios/sec_usr_rad/configuration/15-sy/sec-usr-rad-15-sy-book/sec-cfg-radius.html)
- [Cisco IOS Security Command Reference: métodos AAA de autenticación](https://www.cisco.com/c/en/us/td/docs/ios-xml/ios/security/a1/sec-a1-cr-book/sec-cr-a1.html)
- [Cisco: acceso administrativo IOS mediante RADIUS](https://www.cisco.com/c/en/us/support/docs/security-vpn/remote-authentication-dial-user-service-radius/116291-configure-freeradius-00.html)
- [Cisco IOS XE: creación y aplicación de listas de acceso IP](https://www.cisco.com/c/en/us/td/docs/routers/ios-xe/security-vpn/security-vpn/m_sec-create-ip-apply-0.html)
- [Cisco IOS XE 17: listas de acceso IP nombradas](https://www.cisco.com/c/en/us/td/docs/routers/ios/config/17-x/sec-vpn/b-security-vpn/m_sec-acl-named.html)
