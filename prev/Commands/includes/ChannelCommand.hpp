#pragma once

#include "ICommand.hpp"

class	ChannelCommand : public ICommand
{
	protected:
		bool	_isChannelOperator();

	public:
		virtual	~ChannelCommand();

		virtual void	execute() = 0;
		virtual bool	validate() = 0;
};
