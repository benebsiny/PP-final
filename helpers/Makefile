CXX=g++
FLAGS=-O3 -Wall -Wextra -Werror -std=c++11
NAME=gen_testcase

all: $(NAME)

$(NAME): $(NAME).cpp
	$(CXX) $(FLAGS) $< -o $@

clean:
	rm -f $(NAME)