# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/23 11:39:11 by scros             #+#    #+#              #
#    Updated: 2021/01/12 15:25:08 by scros            ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

BIN			= bin
SRC			= src
INC			= includes
LIBFT		= libft
MINILIBX	= minilibx

SRCS		=	main.c

OBJS		= $(addprefix $(BIN)/, $(SRCS:.c=.o))

NAME		= minirt

CC			= gcc
RM			= rm -f

CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -I$(INC) -I$(LIBFT)/$(INC) -I$(MINILIBX)

HEADERS		= 

all:		$(NAME)

$(BIN)/%.o:	$(SRC)/%.c $(HEADERS)
			$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME):	compile_lib $(OBJS)
			ln -sf $(MINILIBX)/libmlx.dylib .
			$(CC) $(CFLAGS) $(OBJS) -framework OpenGL -framework AppKit -o $(NAME) -L$(MINILIBX) -lmlx

compile_lib:
			@ $(MAKE) -C $(LIBFT)
			@ $(MAKE) -C $(MINILIBX)

re_lib:
			@$(MAKE) -C $(LIBFT) re
			@$(MAKE) -C $(MINILIBX) re

clean_lib:
			@$(MAKE) -C $(LIBFT) clean

fclean_lib:
			@$(MAKE) -C $(LIBFT) fclean

clean:		clean_lib
			@echo "Deleting objects...\n"
			@$(RM) $(OBJS)

fclean:		fclean_lib
			@echo "Deleting objects...\n"
			@$(RM) $(OBJS)
			@echo "Deleting $(NAME)\n"
			@$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re compile_lib re_lib clean_lib fclean_lib