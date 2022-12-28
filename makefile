CFLAGS = -Wall -Wextra -I ../hdr -I hdr -std=c++20
SFMLFLAGS =  -lsfml-graphics -lsfml-window -lsfml-system
OPTFLAGS = -Ofast

# don't touch or I'll steal your kneecaps
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
VPATH = source

# black magic
%.o: %.cpp
	g++ -c -o $@ $< $(CFLAGS) $(SFMLFLAGS) $(OPTFLAGS)

runsim.exe: $(OBJS)
	g++ -o $@ $^ $(CFLAGS) $(SFMLFLAGS) $(OPTFLAGS)

.PHONY: clean

clean:
	rm -f src/*.o
	rm -f runsim.exe
