# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: macbook <macbook@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/21 19:23:13 by hghoutan          #+#    #+#              #
#    Updated: 2025/07/01 16:46:43 by macbook          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC			= cc
CFLAGS	= -Wall -Wextra -Werror -g
NAME		= philo

SRC			= main.c utils.c simulation.c philosopher.c monitor.c cleanup.c libft_functions.c utils_sup.c
OBJ			= $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -lpthread

%.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

leaks:
	leaks --atExit -- ./$(NAME) 5 800 200 200

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re leaks