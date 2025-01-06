# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/29 17:21:09 by kbaridon          #+#    #+#              #
#    Updated: 2025/01/04 14:07:48 by kbaridon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = pipex
SRCDIR = src
LIBFT = libft
HEADERS = $(LIBFT)/libft.h $(SRCDIR)/pipex.h

SRC =	pipex.c \
		init.c \
		utils.c \
		exec_bonus.c

OBJS = $(addprefix $(SRCDIR)/, $(SRC:.c=.o))

all: $(NAME)

bonus: all

$(NAME): $(OBJS)
	$(MAKE) -C $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT)/libft.a -o $(NAME)

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I $(LIBFT) -c $< -o $@

$(OBJS): $(HEADERS)

clean:
	$(MAKE) clean -C $(LIBFT)
	rm -rf $(OBJS)

fclean: clean
	$(MAKE) fclean -C $(LIBFT)
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re