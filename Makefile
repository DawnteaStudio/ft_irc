CXX			= c++
NAME		= ircserv
HEADER_DIR	= ./include
CPP_FLAGS	= -Wall -Wextra -Werror -std=c++98
RM			= rm -rf
FILENAME	= broadcast Channel Client cmds cmdsUtils File removeConnection Request Response Server setConnection applyModeFlag
SRCSNAME	= main $(addprefix srcs/, $(FILENAME))
SRCS		= $(addsuffix .cpp, $(SRCSNAME))
OBJS		= $(addsuffix .o, $(SRCSNAME))
DEPS		= $(addsuffix .d, $(SRCSNAME))

all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CPP_FLAGS) $(OBJS) -o $(NAME)

-include $(DEPS)

%.o : %.cpp
	$(CXX) -I $(HEADER_DIR) $(CPP_FLAGS) -c $< -o $@ -MMD

clean:
	$(RM) $(OBJS)
	$(RM) $(DEPS)

fclean : clean
	$(RM) $(NAME)

re :
	make fclean
	make all

.PHONY : all clean fclean re