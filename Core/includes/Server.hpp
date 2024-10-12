#pragma once

#include "Channel.hpp"
#include "IRCMessage.hpp"
#include "../../Commands/includes/ICommand.hpp"
#include <map>

class	Server
{

	private:
		std::vector<struct pollfd>	_fds;
		std::vector<Channel>		_channels;
		std::vector<Client>			_clients;
		int							_serverSocket;
		std::string					_passwd;
		unsigned short				_port;
		bool						_endServer;
		sockaddr_in						_serverAddress;
		struct pollfd   			*_currentFd;
		std::map<std::string, ICommand*> _commands;
	public:
		static Server		&Singleton()
		{
			static Server	instance;
			return			instance;
		}
		Server&			operator+=(std::string const& chan);
		Server&			operator-=(Client *client);
		Server&			operator-=(struct pollfd *fd);
		Server&			operator*=(IRCMessage const& msg);
		struct pollfd  *operator[](int idx);

		void			serverLoop();
		int				getServerSocket();
		int				initialize(const std::string &psswd, const unsigned short &port);

		struct pollfd	*getClientFdByNickName(const std::string &name);
		struct pollfd	*getClientFdByRealName(const std::string &name);
		Client			*getClientByNickName(const std::string &name);
		Client			*getClientByRealName(const std::string &name);
		Client			*getClientByFd(struct pollfd *fd);
		struct pollfd   *getCurrentFd();
		int				getFdSize();
		std::string		getPasswd();

		int				sendMsg(Client* client, const std::string &msg);
		int				sendMsgAll(const std::string &msg);

		void			createChannel(const std::string &name);
		void			createClient(const std::string &nick, const std::string &real, struct pollfd fd);

		Channel			*getChannelByName(const std::string &name);
		Channel			*getChannelByClient(Client *client);

		int				addClientToChannel(Client *client, Channel *channel);
		int				removeClientFromChannel(Client *client, Channel *channel);

		void			setCurrentFd(struct pollfd *current);
    Server();
    ~Server();
};

//	EN LA PARTE DE ABAJO TIENES LA INTENCIÓN DE COMO ME GUSTARÍA IMPLEMENTAR LOS COMANDOS


//std::map<std::string, ICommand*> *Commands; esto es un atributo del servidor puntero a los siguientes 4 std::maps
// que de deberían inicializar como esta puesto mas abajo y luego enlazar este puntero a los 4 maps de debajo, si se
// hace en una función interna a lomejor queda más bonito, llamame loco.

//std::map<std::string, ICommand*> authenticationCommands;
//std::map<std::string, ICommand*> channelCommands;
//std::map<std::string, ICommand*> messagingCommands;
//std::map<std::string, ICommand*> connectionCommands;

//void Server::initializeCommands()
//{
//    authenticationCommands["PASS"] = new PassCommand(); //Debe comparar el token con el comando del servidor
//    authenticationCommands["NICK"] = new NickCommand(); //Debe comparar el token con la lista de clientes ya existentes
//    authenticationCommands["USER"] = new UserCommand(); //Debe enlazar el usuario con el nick
//    channelCommands["JOIN"] = new JoinCommand(); //Debe comparar el token con el canal para ver si existe, si no existe crearlo,
//    channelCommands["PART"] = new PartCommand();
//    channelCommands["MODE"] = new ModeCommand();
//    channelCommands["TOPIC"] = new TopicCommand();
//    channelCommands["INVITE"] = new InviteCommand();
//    channelCommands["KICK"] = new KickCommand();

//    messagingCommands["PRIVMSG"] = new PrivmsgCommand();
//    messagingCommands["NOTICE"] = new NoticeCommand();

//    connectionCommands["QUIT"] = new QuitCommand();
//}

//		EJEMPLO RAPIDO DE IMPLEMENTACIÓN DE BÚSQUEDA,
//		SI NO SE USAN IFS QUEDA MÁS BONITO
/*
bool handled = false;
if (authenticationCommands.find(command) != authenticationCommands.end())
{
    ICommand *cmd = authenticationCommands[command];
    // ...
    handled = true;
}
else if (channelCommands.find(command) != channelCommands.end())
{
    ICommand *cmd = channelCommands[command];
    // ...
    handled = true;
}
// ... y así con los demás mapas.

if (!handled)
{
    // Comando no reconocido.
    sendMsg(client, "Error: Comando desconocido.");
}
*/

