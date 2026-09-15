# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/14 00:00:00 by bahbibe           #+#    #+#              #
#    Updated: 2026/09/14 00:00:00 by bahbibe          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus

CC = cc

CFLAGS = -Wall -Wextra -Werror

LDFLAGS = -pthread

SRCS = main.c parsing.c init.c time_utils.c print.c monitor.c philosopher.c

HEADERS = philo_bonus.h

OBJ = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)
fclean: clean
	rm -rf $(NAME)
re: fclean all
.PHONY: all clean fclean re
