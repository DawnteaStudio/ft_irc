#ifndef ERRORCODE_HPP
# define ERRORCODE_HPP

enum ErrorCode {
	ERR_NONE = 0,
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHSERVER,
	ERR_NOSUCHCHANNEL,
	// ERR_CANNOTSENDTOCHANl,
	ERR_TOOMANYCHANNELS,
	// ERR_WASNOSUCHNICK,
	// ERR_TOOMANYTARGETS,
	// ERR_NORECIPIENT = 411,
	// ERR_NOTEXTTOSEND,
	// ERR_NOTOPLEVEL,
	// ERR_WILDTOPLEVEL,
	ERR_UNKNOWNCOMMAND = 421,
	// ERR_NOADMININFO = 423,
	// ERR_FILEERROR,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME,
	ERR_NICKNAMEINUSE,
	ERR_NOTONCHANNEL = 442,
	// ERR_USERONCHANNEL,
	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTRED,
	ERR_PASSWDMISMATCH = 464,
	ERR_CHANNELISFULL = 471,
	ERR_INVITEONLYCHAN = 473,
	ERR_BANNEDFROMCHAN,
	ERR_BADCHANNELKEY,
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_INVALIDFILEPATH = 500,
	ERR_FILEERROR,
};

#endif