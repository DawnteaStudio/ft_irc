#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include "ErrorCode.hpp"
# include "ResponseCode.hpp"

class Response {
	private:
		Response();
		Response(const Response&);
		Response &operator=(const Response&);
		~Response();
	public:
		static std::string &success(const int&, const std::string &);
		static std::string &failure(const int&, const std::string &);
};

#endif