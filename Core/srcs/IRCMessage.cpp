
#include "../includes/IRCMessage.hpp"
#include<algorithm>

IRCMessage::IRCMessage(const std::string &buffer)
{
	std::string mutableBuffer = buffer;
	mutableBuffer.erase(std::remove(mutableBuffer.begin(), mutableBuffer.end(), '\r'), mutableBuffer.end());
	mutableBuffer.erase(std::remove(mutableBuffer.begin(), mutableBuffer.end(), '\n'), mutableBuffer.end());
	parseMessage(mutableBuffer);
}

IRCMessage::~IRCMessage() {}

bool	IRCMessage::isValid() const
{
	return _isValid;
}

bool	IRCMessage::isSpecial(const char c) const
{
	return (c >= 0x5B && c <= 0x60) || (c >= 0x7B && c <= 0x7D);
}

bool	IRCMessage::isLetter(const char c) const
{
	return std::isalpha(static_cast<unsigned char>(c)) != 0;
}

bool	IRCMessage::isDigit(const char c) const
{
	return std::isdigit(static_cast<unsigned char>(c)) != 0;
}

bool	IRCMessage::isValidNickname(const std::string &nickname) const
{
	if (nickname.empty() || nickname.length() > 9) // no puede estar vacío y debe tener menos de 9 caracteres
		return false;
	if (!isLetter(nickname[0]) && !isSpecial(nickname[0])) // debe comenzar con una letra o caracter especial
		return false;
	for (size_t i = 1; i < nickname.length(); ++i)
	{
		if (!isLetter(nickname[i]) || !isDigit(nickname[i]) || !isSpecial(nickname[i]) || nickname[i] != '-')
			return false;
	}
	return true;
}


bool	IRCMessage::isAllAlpha(const std::string &str)
{
	for (int i = 0; str[i]; i++)
	{
		if (!isLetter(str[i]))
			return false;
	}
	return true;
}

bool	IRCMessage::isAllDigit(const std::string &str)
{
	for (int i = 0; str[i]; ++i)
	{
		if (!isDigit(str[i]))
			return false;
	}
	return true;
}

bool	IRCMessage::isValidCommand(const std::string &command)
{
	if (command.empty())
		return false;
	if (isAllAlpha(command))
		return true;
	if (isAllDigit(command) && command.length() == 3)
		return true;
	return false;
}

void	IRCMessage::parseMessage(const std::string &buffer)
{
	_isValid = true;

	size_t	pos = 0;
	size_t	end = buffer.size();

	if (buffer[pos] == ':')
	{
		size_t	prefix_end = buffer.find(' ', pos);
		if (prefix_end == std::string::npos) // sólo hay prefijo
		{
			_isValid = false;
			return;
		}
		_prefix = buffer.substr(pos + 1, prefix_end - pos - 1);
		if (!isValidPrefix(_prefix))
		{
			_isValid = false;
			return;
		}
		pos = prefix_end + 1;
	}
	while (pos < end && buffer[pos] == ' ')
		++pos;
	if (pos == std::string::npos)
	{
		_isValid = false;
//		Server::Singleton().sendMsg(client, "461 NOT ENOUGH PARAMETERS");
	}
	size_t	command_end = buffer.find(' ', pos);
	_command = buffer.substr(pos, command_end - pos);
	if (command_end == std::string::npos)
	{
		if (!isValidCommand(_command))
		{
			_isValid = false;
//			Server::Singleton().sendMsg(client, "461 NOT ENOUGH PARAMETERS");
			return;
		}
	}
	pos = command_end + 1;

	while (pos < end)
	{
		while (pos < end && buffer[pos] == ' ')
			++pos;
		if (pos == end)
			break;
		if (buffer[pos] == ':') // set trailing
		{
			_trailing = buffer.substr(pos + 1);
			if (!isValidTrailingParam(_trailing))
				_isValid = false;
			break;
		}
		else
		{
			size_t param_end = buffer.find(' ', pos);
			if (param_end == std::string::npos)
				param_end = end;
			std::string param = buffer.substr(pos, param_end - pos);
			if (!isValidMiddleParam(param))
			{
				//std::cout << "Param: " << param << std::endl;
				_isValid = false;
				return;
			}
			_params.push_back(param);
			pos = param_end;
		}
	}
}

bool	IRCMessage::isValidPrefix(const std::string &prefix)
{
	size_t	atPos = prefix.find('@');
	if (atPos != std::string::npos)
	{
		std::string beforeAt = prefix.substr(0, atPos);
		std::string host = prefix.substr(atPos + 1);
		if (host.empty() || host.find(' ') != std::string::npos)
			return false;
		size_t		excPos = beforeAt.find('!');
		std::string	nickname;
		if (excPos != std::string::npos)
		{
			nickname = beforeAt.substr(0, excPos);
			std::string user = beforeAt.substr(excPos + 1);
			if (user.empty() || user.find_first_of(" \r\n\0") != std::string::npos)
				return false;
			for (size_t i = 0; i < user.size(); ++i)
			{
				if (user[i] <= 0x20 || user [i] == 0x7F || user[i] == '@')
					return false;
			}
		}
		else
			std::string nickname = beforeAt;
		if (!isValidNickname(nickname))
			return false;
	}
	else
	{
		if (!isValidNickname(prefix))
			return false;
	}
	return true;
}

bool	IRCMessage::isValidMiddleParam(const std::string &param)
{
	if (param.empty())
		return false;
	if (param[0] == ':')
		return false;
	for (size_t i = 0; i < param.size(); ++i)
	{
		if (param[i] == ' ' || param[i] == ':')
		{
			//std::cout << static_cast<int>(param[i]) << std::endl;
			return false;
		}
	}
	return true;
}

bool	IRCMessage::isValidTrailingParam(const std::string &param)
{
	if (param.find_first_of("\r\n\0") != std::string::npos)
		return false;
	return true;
}

const std::string	&IRCMessage::getPrefix() const
{
	return _prefix;
}

const std::string	&IRCMessage::getCommand() const
{
	return _command;
}

const std::vector<std::string>	&IRCMessage::getParams() const
{
	return _params;
}

const std::string	&IRCMessage::getTrailing() const
{
	return _trailing;
}

void	IRCMessage::print() const
{
	std::cout << "Prefix: " << _prefix << std::endl;
	std::cout << "Command: " << _command << std::endl;
	std::cout << "Params: ";
	for (size_t i = 0; i < _params.size(); ++i)
		std::cout << "Params[" << i << "].size() = " << _params[i].size() << "   " << _params[i] << "\n";
	std::cout << std::endl;
	std::cout << "Trailing: " << _trailing << std::endl;
}

//void	IRCMessage::clean()
//{

//	_prefix.erase(std::remove(_prefix.begin (), _prefix.end (), '\r'), _prefix.end());
//	_prefix.erase(std::remove(_prefix.begin (), _prefix.end (), '\n'), _prefix.end());

//	_command.erase(std::remove(_command.begin (), _command.end (), '\r'), _command.end());
//	_command.erase(std::remove(_command.begin (), _command.end (), '\r'), _command.end());

//	for (size_t i = 0; i < _params.size(); ++i)
//	{
//		_params[i].erase(std::remove(_params[i].begin (), _params[i].end (), '\r'), _params[i].end());
//		_params[i].erase(std::remove(_params[i].begin (), _params[i].end (), '\n'), _params[i].end());
//	}

//	_trailing.erase(std::remove(_trailing.begin (), _trailing.end (), '\r'), _trailing.end());
//}


//PART OF OLD CONSTRUCTOR
	// std::stringstream	new_buffer(buffer);
	// std::string	token;
	// while (new_buffer >> token)
	// {
	// 	if (token[0] == ':')
	// 	{
	// 		setTrailing(new_buffer, token);
	// 		break;
	// 	}
	// 	_params.push_back(token);
	// }

// void	IRCMessage::setPrefix(const std::string &prefix)
// {
// 	_prefix = prefix.substr(1);
// }

// void	IRCMessage::setCommand(const std::string &command)
// {
// 	_command = command;
// }

// void	IRCMesbsage::setParams(std::vector<std::string> &params)
// {
// 	_params = params;
// }

// void	IRCMessage::setTrailing(std::stringstream &new_buffer, const std::string &trailing)
// {
// 	std::string	temp;
// 	new_buffer >> temp;
// 	_trailing = trailing.substr(1) + " " + temp;
// 	while (new_buffer >> temp)
// 		_trailing += " " + temp;
// }

