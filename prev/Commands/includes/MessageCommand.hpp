#pragma once

#include "ICommand.hpp"

class	MessageCommand : public ICommand
{
	protected:
		void	sendToUser();
		void	sendToChannel();

	public:
		virtual	~MessageCommand();

		virtual void	execute() = 0;
		virtual bool	validate() = 0;
};
