CXX = c++
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror

TARGET = handleRequest

SRCS = main.cpp srcs/Request.cpp

OBJS = $(SRCS:%.cpp=.obj/%.o)

$(TARGET) : $(OBJS)
	$(CXX) $(OBJS) -o $@

.obj/%.o : %.cpp Makefile includes/Request.hpp
	@if [ ! -d .obj ]; then mkdir -p .obj; fi
	@if [ ! -d $(@D) ]; then mkdir -p $(@D); fi
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY : clean fclean re
clean :
	rm -rf .obj

fclean : clean
	rm -rf $(TARGET)

re : fclean $(TARGET)
