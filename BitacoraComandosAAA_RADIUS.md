# Bitácora de comandos Cisco: AAA y RADIUS


## Índice

- [1. Conceptos clave](#1-conceptos-clave)
- [2. Preparar el servidor RADIUS](#2-preparar-el-servidor-radius)
- [3. Declarar el servidor RADIUS en IOS](#3-declarar-el-servidor-radius-en-ios)
- [4. Crear listas AAA de autenticación](#4-crear-listas-aaa-de-autenticación)
- [5. Aplicar AAA a consola y VTY](#5-aplicar-aaa-a-consola-y-vty)
- [6. Autorización y nivel de privilegio](#6-autorización-y-nivel-de-privilegio)
- [7. Verificación y diagnóstico](#7-verificación-y-diagnóstico)

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

## Referencias

- [Cisco IOS: configuración de RADIUS](https://www.cisco.com/c/en/us/td/docs/ios-xml/ios/sec_usr_rad/configuration/15-sy/sec-usr-rad-15-sy-book/sec-cfg-radius.html)
- [Cisco IOS Security Command Reference: métodos AAA de autenticación](https://www.cisco.com/c/en/us/td/docs/ios-xml/ios/security/a1/sec-a1-cr-book/sec-cr-a1.html)
- [Cisco: acceso administrativo IOS mediante RADIUS](https://www.cisco.com/c/en/us/support/docs/security-vpn/remote-authentication-dial-user-service-radius/116291-configure-freeradius-00.html)
