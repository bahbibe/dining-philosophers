# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/09 09:20:05 by bahbibe           #+#    #+#              #
#    Updated: 2023/05/12 06:25:58 by bahbibe          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc 

CFLAGS = -Wall -Wextra -Werror #-fsanitize=thread -g3

SRCS = main.c philo.c ft_atoi.c utils.c time.c

OBJ = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
clean:
	rm -rf $(OBJ)
fclean: clean
	rm -rf $(NAME)
re: fclean all
.PHONY: all clean fclean re