NAME = crypto_portfolio_analizer
SRCS = src/main.cpp src/CryptoPortfolio.cpp
OBJS = $(SRCS:src/%.cpp=obj/%.o)
CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++11

# Ensure obj directory exists
all: objdir $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

# Compile source files into obj folder
obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create obj directory if it doesn't exist
objdir:
	mkdir -p obj

clean:
	rm -f obj/*.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re objdir
