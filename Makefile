NAME = webserver

CPP	= c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g

CLASSES =	srcs/Location/Location.cpp\
			srcs/VirtualServer/VirtualServer.cpp\
			srcs/ConfigParser/ConfigParser.cpp\
			srcs/WebServer/WebServer.cpp\
			srcs/Client/Client.cpp

SRCS = $(CLASSES) srcs/main.cpp

OBJS = $(SRCS:.cpp=.o)

%.o: %.cpp
	$(CPP) -c $< $(CPPFLAGS) -o $@

$(NAME): $(OBJS)
	$(CPP) $^ $(CPPFLAGS) -o $@
	printf "COMPILATION SUCCESSFUL!\n"

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	printf "FULLY CLEANED!\n"

re: fclean all

.SILENT:

.PHONY: all clean fclean re
