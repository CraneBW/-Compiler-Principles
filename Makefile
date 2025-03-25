CC = g++
CFLAGS = -std=c++11 -Wall -Wextra
TARGET = lexer
SRCS = main.cpp lexer.cpp token.cpp symbol_table.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean 