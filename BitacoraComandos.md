## Comandos básicos

Los siguientes comandos son fundamentales para la configuración y administración de equipos Cisco, como routers y switches. Estos comandos permiten desde acceder a distintos modos del dispositivo, asignar configuraciones de red, modificar parámetros básicos como el nombre del equipo o la descripción de interfaces, hasta guardar o eliminar configuraciones en memoria.

Es importante tener en cuenta que muchos de estos comandos deben ejecutarse en modos específicos (como el modo privilegiado o el modo de configuración global), y que algunos requieren seleccionar una interfaz previamente. También se debe prestar especial atención a la persistencia de la configuración: si no se guarda adecuadamente, cualquier cambio realizado se perderá al reiniciar el equipo.

|         **_Comando_**        |                                                 **_Uso_**                                                | **_Aplicable a_** |              **_Ejemplo_**             |                                               **_Notas_**                                               |
|:----------------------------:|:--------------------------------------------------------------------------------------------------------:|:-----------------:|:--------------------------------------:|:-------------------------------------------------------------------------------------------------------:|
| enable                       | Entrar al modo privilegiado                                                                              |  Routers/Switches |                    -                   |                                                    -                                                    |
| configure terminal           | Entra al modo configuración                                                                              |  Routers/Switches |                    -                   |                                                    -                                                    |
| hostname (Nombre del equipo) | Cambia el nombre que se muestra en el equipo                                                             |  Routers/Switches | hostname RISP                          |                                                    -                                                    |
| banner motd (!Mensaje!)      | Muestra un mensaje cada vez que un usuario se logueé al equipo                                           | Routers/Switches  | banner motd !Hola A todos!             | El mensaje debe estar entre cualquier caracter especial especial                                        |
| interface (Interfaz)         | Elije una interfaz                                                                                       |  Routers/Switches | interface fa0/0                        |                    Se debe diferenciar entre interfaces Gigabit,Serial y FastEthernet                   |
| ip address (IP) (Mascara)    | Asigna una ip y una mascara a interfaz.                                                                  |      Routers      | ip address 192.187.2.1 255.255.255.248 | Este comando se puede usar en switches, pero solo en sus vlans                                          |
| no shutdown                  | Enciende la interfaz                                                                                     |  Routers/Switches |                    -                   |        Una vez hecho las configuraciones en una interfaz, este comando es obligatorio para usarla       |
| description (mensaje)        | Le da una descripción a una interfaz                                                                     |  Routers/Switches | description LAN1                       | Primero se debe seleccionar una interfaz para poder usar este comando.                                  |
| end                          | Sale del modo configuración y lleva al modo priviligiado                                                 |  Routers/Switches |                    -                   | Existe el comando alternativo, exit, que realiza la misma acción.                                       |
| copy running-startup-config  | Se usa para escribir en la memoria la configuracion actual, y así no se pierda en al reiniciar el equipo | Routers/Switches  |                    -                   |                                                    -                                                    |
| wr                           | Escribe en la memoria Flash la configuración actual                                                      | Routers/Switches  |                    -                   | Este comando está obsoleto, se recomienda usar el copy running-startup-config para guardar memoria      |
| erase startup-config         | Elimina la configuración de startup de la memoria                                                        | Routers/Switches  |                    -                   | Esto solo elimina la configuración guardada, no la que se está ejecutando                               |
| erase running-config         | Elimina la configuración que se está ejecutando en la memoria RAM                                        | Routers/Switches  |                    -                   | Esto solo elimina la configuración que se tiene ejecutando.                                             |
| we                           | Elimina la configuración guardada en la memoria y la que se está ejecutando actualmente                  | Routers/Switches  |                    -                   | Esto elimina todo los archivos de configuración.                                                        |
| reload                       | Reinicia el equipo                                                                                       | Routers/Switches  |                    -                   | Asegurate de guardar la configuración actual antes de usar este comando, de no hacerlo se perderá todo. |

## Comandos de configuración de acceso remoto y administración (TELNET)

Esta tabla presenta una serie de comandos esenciales para configurar el acceso remoto y la gestión de switches y routers Cisco. A través de estos comandos, se puede asignar una dirección IP a la interfaz VLAN, establecer una puerta de enlace predeterminada y preparar el dispositivo para ser administrado remotamente mediante Telnet.

También incluye comandos para crear usuarios con distintos niveles de privilegio y aplicar medidas básicas de seguridad, como el uso de contraseñas cifradas y la limitación del número de sesiones simultáneas permitidas. Además, se destacan configuraciones necesarias para habilitar interfaces y líneas virtuales de terminal (VTY), fundamentales para conexiones remotas.

Estos comandos son especialmente importantes en entornos donde se necesita gestionar dispositivos de forma remota y segura, garantizando al mismo tiempo un control adecuado sobre el acceso de los usuarios.

|                       **_Comando_**                      |                                                   **_Uso_**                                                  | **_Aplicable a_** |                **_Ejemplo_**                |                                       **_Notas_**                                       |
|:--------------------------------------------------------:|:------------------------------------------------------------------------------------------------------------:|:-----------------:|:-------------------------------------------:|:---------------------------------------------------------------------------------------:|
|                 interface vlan (nro vlan)                |                                         Selecciona una interfaz Vlan                                         |      Switchs      |               interface vlan 1              |                                            -                                            |
|                  ip address (IP) (Mask)                  |                                  Asigna a una IP a la interfaz seleccionada                                  |  Switches/Routers |    ip address 192.168.1.254 255.255.255.0   |                         En este caso le asigna una ip a la VLAN.                        |
|                           exit                           |                                  Sale del modo de configuración de interfaz                                  |  Switches/Routers |                      -                      |                                            -                                            |
|                  ip default-gateway (ip)                 |                       Configura la puerta de enlace predeterminada que el equipo usará                       |  Switches/Routers |       ip default-gateway 192.168.100.1      |                       En Switches, solo es aplicable en las vlans                       |
| username (usuario) privilege (nivel) secret (contraseña) |             Crea un usuario con el nivel de privilegio elegido y establece una contraseña cifrada            |  Switches/Routers | username admin privilege 15 secret 12345678 |       Si en lugar de usar secret se usa password, la contraseña no estará cifrada       |
|                 line vty 0 (sesiones max)                |               Gestiona el numero de sesiones máximas a las cuales se puede accesar remotamente.              |  Switches/Routers |                 line vty 0 4                | Es importante contabilizar las sesiones necesarias y solo las necesarias, por seguridad |
|                        no shutdown                       |                             Activa la linea VTY para permitir conexiones remotas.                            |  Switches/Routers |                      -                      |        También sirve para encender las interfaces (si estas están seleccionadas)        |
|                       login (user)                       |   Configura la linea VTY para requerir autenticación del usuario elegido antes de permitir el acceso remoto  |  Switches/Routers |                 login admin                 |                                            -                                            |
|                  transport input telnet                  | Permite el acceso remoto al dispositivo mediante el protocolo Telnet a través de las líneas VTY configuradas |  Switches/Routers |                      -                      |                                            -                                            |

## Comandos de configuración de RIP versión 2 (IPv4)

Estos comandos permiten activar y configurar el protocolo de enrutamiento dinámico **RIP** (Routing Information Protocol) en routers Cisco. RIP es un protocolo que permite a los routers intercambiar información de enrutamiento con otros dispositivos dentro de la misma red.

|         **_Comando_**        |                                          **_Uso_**                                         | **_Aplicable a_** |             **_Ejemplo_**            |                                           **_Notas_**                                           |
|:----------------------------:|:------------------------------------------------------------------------------------------:|:-----------------:|:------------------------------------:|:-----------------------------------------------------------------------------------------------:|
|          router rip          |         Entra al modo de configuración para activar y configurar el protocolo RIP.         |      Routers      |                   -                  |                                                -                                                |
|           version 2          |                              Habilita el uso de RIP versión 2                              |      Routers      |                   -                  |                 La versión 2 de RIP soporta Enrutamiento Classless, VLSM y Auth                 |
|  network (dirección_de_red)  |   Indica las redes conectadas que RIP debe anunciar y en las que debe escuchar mensajes.   |      Routers      |          network 192.168.1.0         |                      **Se debe usar la dirección de red, no una IP host.**                      |
| passive-interface (interfaz) | Evita que la interfaz envíe mensajes RIP, pero sigue escuchando actualizaciones entrantes. |      Routers      | passive-interface GigabitEthernet0/1 | ara interfaces conectadas a usuarios finales o segmentos donde no se desea enviar anuncios RIP. |
| no auto-summary              | Desactiva la sumarización automática de rutas en los límites de clase                      |      Routers      |                   -                  | Es necesario cuando se usa VLSM o subredes discontinuas.                                        |

## Comandos Switching y Multicapa
En esta sección se listan los comandos usados especificamente en Switches y Switches Multicapa

|             **_Comando_**             |                         **_Uso_**                         | **_Aplicable a_** |             **_Ejemplo_**             |                                      **_Notas_**                                     |
|:-------------------------------------:|:---------------------------------------------------------:|:-----------------:|:-------------------------------------:|:------------------------------------------------------------------------------------:|
| switchport mode [trunk/access]        | Cambia el rol de la interfaz a enlace troncal o de acceso |      Switches     |                   -                   |                                          -                                           |
| spanning-tree portfast                | Deshabilita el protocolo STP en el puerto                 |      Switches     |                   -                   | Este protocolo controla la rebundancia, y es deseable si es un enlace entre switches |
| switchport trunk allowed vlan [x1,x2] | Le da permisos para usar enlaces troncales a las VLAN     |      Switches     | switchport trunk allowed vlan [10,20] | Esto es solo por seguridad                                                           |
| no switchport                         | Deshabilita el switching en el puerto                     |    Switches L3    |                   -                   | Esto se hara para dar una ip al puerto en un L3                                      |
| ip routing                            | Le dice al switch que enrute dominios de broadcast        |    Switches L3    |                   -                   | Esto es necesario para el correcto funcionamiento                                    |

# Comandos dedicados a IPv6
Estos comandos forman la base para implementar redes IPv6 funcionales, y son especialmente importantes en entornos modernos que buscan adoptar gradualmente el nuevo protocolo de Internet.

##  COMANDOS BASICOS DE IMPLEMENTACIÓN IPV6
La siguiente tabla reúne comandos esenciales para comenzar con la configuración de IPv6 en routers y switches Cisco. 

|       **_Comando_**      |                                **_Uso_**                               | **_Aplicable a_** |               **_Ejemplo_**              |                              **_Notas_**                              |
|:------------------------:|:----------------------------------------------------------------------:|:-----------------:|:----------------------------------------:|:---------------------------------------------------------------------:|
|   ipv6 unicast-routing   |              Habilita el enrutamiento Ipv6 en los equipos              |      Routers      |                    -                     | Sin este comando, el router no reenvia paquetes Ipv6 entre interfaces |
|    inteface (interfaz)   |                         Selecciona una interfaz                        |  Switches/Routers |       interface GigabitEthernet0/0       |                                   -                                   |
|        mac-address       |                  Asigna manualmente una MAC al puerto                  |  Switches/Routers |        mac-address 0201.aa00.0001        |            No siempre es necesario, es util en laboratorios           |
| ipv6 address (ip) eui-64 | Asigna una IP usando EUI-64 que usa la MAC para generar parte del Host |      Routers      | ipv6 address 2001:DB8:1111:1::/64 eui-64 | Las IP usadas para este protocolo de asignación SI O SI deben ser /64 |

## Creación de un DHCP Server

Esta serie de comandos se utiliza para configurar y gestionar el servicio DHCP en routers y, en algunos casos, switches que soportan esta funcionalidad. A través de ellos, es posible definir grupos o pools de direcciones IP que se asignarán dinámicamente a los hosts de una red.

|            **_Comando_**           |                                                                 **_Uso_**                                                                 | **_Aplicable a_** |            **_Ejemplo_**           |                                                     **_Notas_**                                                     |
|:----------------------------------:|:-----------------------------------------------------------------------------------------------------------------------------------------:|:-----------------:|:----------------------------------:|:-------------------------------------------------------------------------------------------------------------------:|
|    ip dhcp pool (NombreDelPool)    |       Entra al modo de configuración para crear o modificar un grupo de direcciones que el servidor DHCP podrá asignar a los Hosts.       |      Routers      |       ip dhcp pool LAN_OFFICE      | Tambien es aplicable a Switches si su modelo lo permite                                                             |
|         network (IP) (Mask)        |                                             Define la red y el rango de direcciones a asignar                                             |      Routers      | network 192.168.10.0 255.255.255.0 | Es obligatorio para que el pool sepa qué rango administrar.                                                         |
|    default-router (IP_DE_ROUTER)   |                                          Define la puerta de enlace predeterminada para los hosts                                         | Routers           |     default-router 192.168.10.1    |                                                          -                                                          |
|   dns-server (IP_DEL_DNS_SERVER)   |                       Asigna la dirección IP del servidor DNS que el cliente usará para resolver nombres de dominio.                      |      Routers      |         dns-server 8.8.8.8         |                          Puede aceptar una lista de servidores DNS separados por espacios.                          |
| domain-name (url_del_dominio)      |                      Define el sufijo de dominio que los clientes DHCP agregarán automáticamente a las consultas DNS.                     |      Routers      |         domain-name usfx.bo        |                                                          -                                                          |
|       ip nd other-config-flag      | Indica que, aunque los hosts pueden configurar parte de su IPv6 por SLAAC, deben consultar un servidor DHCPv6 para información adicional. |      Routers      |                  -                 | Primero se debe elegir una interfaz.                                                                                |
| ip dhcp server (NombreDelPoolDHCP) |                                                          Activa el servicio DHCP                                                          | Routers           | ip dhcp server LAN_OFFICE          | En IOS Cisco tradicional, este comando no existe para activar el DHCP, se activa automaticamente al definir un pool |


## Configuración de RIPng (IPv6)

RIPng es la versión de RIP diseñada para redes IPv6. A diferencia del protocolo RIP para IPv4, **RIPng se configura a nivel de interfaz** y no completamente en modo global. Los siguientes comandos permiten habilitar este protocolo y gestionar su comportamiento en routers Cisco.

|            **_Comando_**             |                                     **_Uso_**                                     | **_Aplicable a_** |            **_Ejemplo_**             |                              **_Notas_**                               |
|:------------------------------------:|:---------------------------------------------------------------------------------:|:-----------------:|:------------------------------------:|:----------------------------------------------------------------------:|
|         ipv6 unicast-routing         | Este comando global permite que el router reenvíe paquetes IPv6 entre interfaces. |      Routers      |                  -                   | Necesario antes de configurar cualquier protocolo de enrutamiento IPv6 |
|         interface (INTERFAZ)         |                              Selecciona una interfaz                              | Routers/Switches  |           interface fa0/0            |                                   -                                    |
|             ipv6 address             |                            Define una ip a la interfaz                            | Routers/Switches  |    ipv6 address 2001:DB8:1::1/64     |                                   -                                    |
| ipv6 rip (NOMBRE_DEL_PROCESO) enable |                 Asignar nombre del proceso RIPng en cada interfaz                 |      Routers      |       ipv6 rip RIP-LAB enable        |   RIPng se configura por interfaz, no en modo global como RIP IPv4.    |
| ipv6 router rip (NOMBRE_DEL_PROCESO) |            Define el proceso RIPng que será usado por las interfaces.             |      Routers      |       ipv6 router rip RIP-LAB        |                                   -                                    |
|     passive-interface (INTERFAZ)     |    Hace que la interfaz no envíe actualizaciones RIPng, pero sí puede recibir.    |      Routers      | passive-interface GigabitEthernet0/2 | Primero se debe seleccionar la interfaz con el comando ipv6 router rip |


# Comandos de Verificación
En esta sección se listarán todos los comandos que se necesitan para la verificación o manejo en diferentes equipos Cisco

| Comando                             | Uso                                                          | Aplicable a | Nota                                                              |
|-------------------------------------|--------------------------------------------------------------|-------------|-------------------------------------------------------------------|
| show ip route rip                   | Muestra rutas aprendidas por RIP                             | IPv4        | Verifica si las rutas se están propagando correctamente           |
| show ip protocols                   | Muestra configuración del protocolo RIP                      | IPv4        | Útil para confirmar interfaces, redes y temporizadores            |
| show ipv6 route rip                 | Muestra rutas aprendidas por RIPng                           | IPv6        | Equivalente a `show ip route rip` en IPv6                         |
| show ipv6 protocols                 | Muestra configuración del protocolo RIPng                    | IPv6        | Incluye interfaces activas, métricas, etc.                        |
| show ipv6 rip                       | Verifica estado y estadísticas del proceso RIPng             | IPv6        | Comando base, proporciona estado por interfaz                     |
| show ipv6 rip database              | Muestra la base de datos de rutas RIPng                      | IPv6        | Muestra rutas recibidas, anunciadas y métricas                    |
| show ip route                       | Muestra toda la tabla de enrutamiento IPv4                   | IPv4        | Incluye rutas RIP, estáticas, conectadas, etc.                    |
| show ipv6 route                     | Muestra toda la tabla de enrutamiento IPv6                   | IPv6        | Muestra GUA, ULA, rutas RIPng, etc.                               |
| show ip dhcp binding                | Muestra las concesiones DHCP activas                         | IPv4        | Lista IPs entregadas, MAC del cliente y duración del lease        |
| show ip dhcp pool                   | Muestra estadísticas del pool DHCP                           | IPv4        | Verifica cuántas direcciones han sido usadas / quedan disponibles |
| show running-config \| section dhcp | Filtra solo la sección DHCP de la config                     | IPv4        | Ideal para revisar solo configuración relacionada a DHCP          |
| show ipv6 interface (interfaz)      | Muestra el estado y direcciones IPv6 asignadas a la interfaz | IPv6        | Incluye prefijos, link-local, EUI-64 y configuración ND           |
| show interface (interfaz)           | Muestra estado físico y lógico de la interfaz (IPv4/IPv6)    | Ambos       | Muestra velocidad, estado, MAC, IP, errores                       |
| netsh interface ipv6 show neighbors | Muestra la tabla de vecinos desde el cliente Windows         | IPv6        | Útil para verificar detección de routers y vecinos en cliente     |
| ping (ip o dirección IPv6)          | Verifica conectividad IP entre dispositivos                  | Ambos       | Para IPv6 se debe usar `ping ipv6` en algunos sistemas Cisco      |
| traceroute (ip o IPv6)              | Muestra la ruta de paquetes hacia el destino                 | Ambos       | Puede ayudar a detectar saltos donde ocurre una pérdida           |
| debug ipv6 rip                      | Muestra mensajes en tiempo real del proceso RIPng            | IPv6        | Solo usar para diagnóstico, puede saturar la consola              |
| debug ip rip                        | Muestra mensajes en tiempo real del proceso RIP              | IPv4        | Similar a `debug ipv6 rip`, para revisión de mensajes enviados    |
| show clock                          | Muestra la hora del router                                   | Ambos       | Útil si necesitas sincronización para logs o troubleshooting      |
| show cdp neighbors                  | Muestra dispositivos Cisco vecinos directamente conectados   | Ambos       | Ayuda a mapear la topología física de la red                      |
| show mac-address-table                 | Muestra la tabla CAM de un router                                   | Ambos       | -  
| show interfaces trunk                  | Muestra las interfaces troncales                          | Ambos       | - 