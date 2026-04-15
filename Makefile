NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -Ilibft -g

LIBFT_DIR = libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a
LIBFT_FLAGS = -L$(LIBFT_DIR) -lft

SRC =	srcs/main.c \
		srcs/parser/cmd_parser.c \
		srcs/parser/tokenize/cmd_tokenizer_a.c \
		srcs/parser/tokenize/cmd_tokenizer_b.c \
		srcs/parser/tokenize/cmd_tokenizer_utils.c \
		srcs/parser/expansor/expansor_a.c \
		srcs/parser/expansor/expansor_b.c \
		srcs/parser/build_cmd_list.c \
		srcs/parser/free_memory.c \
		srcs/parser/validator.c

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