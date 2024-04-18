NAME := webserv

HEADER := ./includes

CPP := c++

FLAGS := -Wall -Wextra -Werror -std=c++98

OBJDIR := build/

SRCS := main.cpp \
	./srcs/config/GlobalConfig.cpp ./srcs/config/ServerConfig.cpp ./srcs/config/Location.cpp ./srcs/config/DefaultErrors.cpp \
	./srcs/server/Server.cpp \
	./srcs/cgi/CgiHandler.cpp \
	./srcs/Globals.cpp ./srcs/utils.cpp

HEADERS := \
	$(HEADER)/config/GlobalConfig.hpp $(HEADER)/config/ServerConfig.hpp $(HEADER)/config/Location.hpp $(HEADER)/config/DefaultErrors.hpp \
	$(HEADER)/server/Server.hpp \
	$(HEADER)/cgi/CgiHandler.hpp \
	$(HEADER)/Globals.hpp $(HEADER)/utils.hpp

OBJS := $(addprefix $(OBJDIR),$(SRCS:.cpp=.o))

all: begin $(NAME)
	@echo "\033[1;97mwebserv: \033[1;32mwebserv DONE\033[0;m"

$(NAME): $(OBJDIR) $(OBJS)
	@echo "\033[1;97m\nwebserv: \033[0;33mCompiling webserv...\033[0;m"
	$(CPP) $(OBJS) -I$(HEADER) -o $(NAME)

$(OBJDIR)%.o : %.cpp $(HEADERS) Makefile
	@echo "\033[1;97mwebserv: \033[0;3mBuilding $< into $@\033[0;m"
	$(CPP) $(FLAGS) -I$(HEADER) -c $< -o $@

$(OBJDIR):
	@echo "\033[1;97mwebserv: \033[0;33mMaking build directories ...\033[0;m"
	mkdir -p $(OBJDIR)srcs/config
	mkdir -p $(OBJDIR)srcs/server
	mkdir -p $(OBJDIR)srcs/cgi
	@echo "\033[1;97mwebserv: \033[1;32mbuild directory done.\n\033[0;m"

begin:
	@echo "\033[1;97mwebserv: \033[1;32mMaking webserv ...\n\033[0;m"

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean