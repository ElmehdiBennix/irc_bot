EXE := ircBot

CPP := c++

CPPFLAGS := -g -Wall -Wextra -Wshadow -fsanitize=address

HEADER := ./include/defines.hpp ./include/ircBot.hpp ./bot/bot.hpp

SOURCES := ./bot/bot.cpp ./bot/commands.cpp ./bot/tools.cpp ./main.cpp

OBJ := $(SOURCES:.cpp=.o)

M = MAKE_PUSH

################################### RULES ###################################

all : $(EXE)

$(EXE): $(OBJ)
	$(CPP) $(CPPFLAGS) $(OBJ) -o $(EXE)

%.o: %.cpp $(HEADER)
	$(CPP) $(CPPFLAGS) -c -o $@ $<

clean :
	rm -rf $(OBJ)

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