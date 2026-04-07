NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -Ilibft

LIBFT_DIR = libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a
LIBFT_FLAGS = -L$(LIBFT_DIR) -lft

SRC =	srcs/main.c \
		srcs/parser/tokenizer.c \
		srcs/parser/validator.c \
		srcs/parser/expansor.c \
		srcs/parser/parser.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_FLAGS) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean:
	rm -f $(OBJ) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re