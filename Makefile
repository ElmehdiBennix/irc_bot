EXE := ircBot

CPP := c++

CPPFLAGS := -g -Wall -Wextra -Wshadow -fsanitize=address -MMD

# HEADERS := ./include/defines.hpp ./include/ircBot.hpp ./bot/bot.hpp

SOURCES := ./src/bot.cpp ./src/commands.cpp ./src/tools.cpp ./main.cpp

OBJ := $(SOURCES:.cpp=.o)

DEPS = $(SOURCES:.cpp=.d)

M = MAKE_PUSH

################################### RULES ###################################

all : $(EXE)

$(EXE): $(OBJ)
	$(CPP) $(CPPFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

-include $(DEPS)

clean :
	rm -rf $(OBJ) $(DEPS)

fclean : clean
	rm -rf $(EXE)

re: fclean all

push: fclean
	git add .
	git status
	git commit -m "$(M)"
	git push

##############################################################################

.PHONY: clean fclean re push