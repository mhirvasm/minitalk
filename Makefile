# Project name
NAME_SERVER = server
NAME_CLIENT = client

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Source files
SRCS_SERVER = server.c
SRCS_CLIENT = client.c

# Object files
OBJS_SERVER = $(SRCS_SERVER:.c=.o)
OBJS_CLIENT = $(SRCS_CLIENT:.c=.o)

# Libft
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Default rule
all: $(LIBFT) $(NAME_SERVER) $(NAME_CLIENT)

# Build libft
$(LIBFT):
	make -C $(LIBFT_DIR)

# Build server
$(NAME_SERVER): $(OBJS_SERVER)
	$(CC) $(CFLAGS) -o $(NAME_SERVER) $(OBJS_SERVER) $(LIBFT)

# Build client
$(NAME_CLIENT): $(OBJS_CLIENT)
	$(CC) $(CFLAGS) -o $(NAME_CLIENT) $(OBJS_CLIENT) $(LIBFT)

# Compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -c $< -o $@

# Clean object files
clean:
	make clean -C $(LIBFT_DIR)
	rm -f $(OBJS_SERVER) $(OBJS_CLIENT)

# Full clean
fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -f $(NAME_SERVER) $(NAME_CLIENT)

# Rebuild
re: fclean all