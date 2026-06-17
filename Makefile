# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: evafletefolgueira <evafletefolgueira@st    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/12 16:40:00 by evaflete          #+#    #+#              #
#    Updated: 2026/06/17 12:40:00 by evafletefol      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        =   codexion

CC          =   gcc
CFLAGS      =   -Wall -Wextra -Werror -pthread

# Archivos Fuente
SRC         =   main.c \
                parsing.c \
                init.c \
                routine.c \
                routine_utils.c \
                utils.c \
                priority_queue.c \
                priority_queue_utils.c \
                threads.c

OBJ         =   $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
