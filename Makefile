NAME_SERVER = server
NAME_CLIENT = client

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS_SERVER = server.c
SRCS_CLIENT = client.c

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME_SERVER) $(NAME_CLIENT)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME_SERVER): $(SRCS_SERVER)
	$(CC) $(CFLAGS) -o $(NAME_SERVER) $(SRCS_SERVER) $(LIBFT)

$(NAME_CLIENT): $(SRCS_CLIENT)
	$(CC) $(CFLAGS) -o $(NAME_CLIENT) $(SRCS_CLIENT) $(LIBFT)

clean:
	make clean -C $(LIBFT_DIR)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -f $(NAME_SERVER) $(NAME_CLIENT)

re: fclean all
