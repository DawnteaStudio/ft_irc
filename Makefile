CXX			= c++
NAME		= ircserv
HEADER_DIR	= ./include
CPP_FLAGS	= -Wall -Wextra -Werror -std=c++98
RM			= rm -rf
SRCSNAME	= main srcs/Client srcs/cmds srcs/cmds_utils srcs/Request srcs/Response srcs/Server
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