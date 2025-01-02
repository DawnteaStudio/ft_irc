#include <iostream>
#include "include/Server.hpp"

int main(int argc, char *argv[]) {
	if (argc != 3) std::cout << "[valid parameter format] ./ircserv <port> <password>" << std::endl;
	
	std::string port(argv[1]);
	std::string password(argv[2]);
	Server server(port, password);

	try {
		server.run();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}
