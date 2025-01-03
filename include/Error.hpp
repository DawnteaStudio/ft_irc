#ifndef ERROR_HPP
# define ERROR_HPP

# include <string>
# include "ErrorCode.hpp"

class Error {
	private:
		Error();
		Error(const Error&);
		Error &operator=(const Error&);
		~Error();
	public:
		static std::string &createErrorMessage(const int&, const std::string &);
};

#endif