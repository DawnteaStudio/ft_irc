#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include "ErrorCode.hpp"
# include "ResponseCode.hpp"

# define CRLF "\r\n"

class Response {
	private:
		Response();
		Response(const Response&);
		Response &operator=(const Response&);
		~Response();
		static std::string createMessage(const int&, const std::string&, const std::string&, const std::string &);
	public:
		static std::string success(const int&, const std::string&, const std::string&, const std::string&, const std::string&);
		static std::string failure(const int&, const std::string&, const std::string&, const std::string&);
		static std::string customMessageForJoin(const std::string&, const std::string&);
		static std::string customMessageForKick(const std::string&, const std::string&, const std::string&, const std::string&);
		static std::string customMessageForPart(const std::string&, const std::string&);
		static std::string customMessageForQuit(const std::string&, const std::string&);
		static std::string customErrorMessageForQuit(const std::string&, const std::string&);
};

#endif