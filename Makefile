# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/12 14:04:46 by fyuzhyk           #+#    #+#              #
#    Updated: 2022/05/14 15:49:47 by fyuzhyk          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

LIB = pipex_utils.a
LIBFT_DIR = libft

CFLAGS = -Wall -Wextra -Werror

PIPEX = pipex.c
SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)
SRC_LIBFT = $(wildcard $(LIBFT_DIR)/*.c)
OBJ_LIBFT = $(SRC_LIBFT:$(LIBFT_DIR)/%.c=$(LIBFT_DIR)/%.o)

all : $(NAME)

$(NAME) : $(PIPEX) $(LIB)
	$(CC) $(CFLAGS) $(PIPEX) $(LIB) -o $(NAME)

$(LIB) : $(OBJ) $(OBJ_LIBFT)
	Make -C libft
	cp libft/libft.a $(LIB)
	ar rc $(LIB) $(OBJ) $(OBJ_LIBFT)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

fclean: clean
	rm -f $(LIB) $(NAME) libft/libft.a

clean:
	rm -f $(OBJ) $(OBJ_LIBFT)

re: fclean all
