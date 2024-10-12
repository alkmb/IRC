# ft_irc - IRC Server in C++98

## Descripción

Este proyecto consiste en desarrollar un servidor IRC (Internet Relay Chat) en C++98. El servidor permite que los clientes se conecten, se autentiquen, se unan a canales y se envíen mensajes en tiempo real. La implementación sigue el protocolo IRC, cumpliendo con los comandos esenciales y características básicas.

## Tabla de Contenidos

- [Descripción](#descripción)
- [Características](#características)
- [Instalación](#instalación)
- [Ejecución](#ejecución)
- [Comandos IRC Implementados](#comandos-irc-implementados)
- [Uso del Cliente HexChat](#uso-del-cliente-hexchat)
- [Estructura del Código](#estructura-del-código)
- [Contribuciones](#contribuciones)
- [Licencia](#licencia)

## Características

- Maneja múltiples clientes de manera simultánea usando I/O no bloqueante.
- Soporte para comandos básicos del protocolo IRC:
  - `NICK`: Definir nickname del cliente.
  - `USER`: Autenticación del cliente con username.
  - `JOIN`: Unirse a canales.
  - `PRIVMSG`: Enviar mensajes privados a otros usuarios o a un canal.
  - `KICK`: Expulsar usuarios de un canal (solo para operadores).
  - `TOPIC`: Cambiar o visualizar el tema del canal.
  - `MODE`: Modificar los modos de los canales (canal con contraseña, limitado, etc.).
- Sistema de usuarios regulares y operadores.
- Implementado en C++98, con el uso de sockets y gestión de eventos no bloqueante.

## Instalación

### Requisitos

- **Sistema Operativo**: Linux, macOS o Windows (con WSL).
- **Compilador C++98**.
- **Make**.

### Clonar el Repositorio

```bash
git clone https://github.com/GPradaT/ft_irc.git
cd ft_irc
