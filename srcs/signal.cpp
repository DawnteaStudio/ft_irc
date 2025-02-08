#include "../include/signal.hpp"

bool sig::stopServer = false;

void sig::signalHandler( int signum )
{
	if (signum == SIGINT)
		sig::stopServer = true;
	if (signum == SIGQUIT)
		sig::stopServer = true;
}
