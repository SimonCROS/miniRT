# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/23 11:39:11 by scros             #+#    #+#              #
#    Updated: 2021/02/02 16:14:13 by scros            ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

BIN			= bin
SRC			= src
INC			= includes
LIBFT		= libft
MINILIBX	= minilibx

FT			= libft.a
MLX			= libmlx.dylib

SRCS		=	main.c				\
				entity/camera.c		\
				object/light.c		\
				object/2d/square.c	\
				object/2d/plan.c	\
				# object/sphere.c		\

OBJS		= $(addprefix $(BIN)/, $(SRCS:.c=.o))

NAME		= minirt

CC			= gcc
RM			= rm -f

#CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -I$(INC) -I$(LIBFT)/$(INC) -I$(MINILIBX)

HEADERS		= includes/minirt.h

all:		$(NAME)

$(BIN)/%.o:	$(SRC)/%.c $(HEADERS)
			@ mkdir -p $(dir $@);
			@ $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME):	compile_lib $(OBJS)
			@ ln -sf $(MINILIBX)/$(MLX) .
			@ ln -sf $(LIBFT)/$(FT) .
			@ $(CC) $(CFLAGS) $(OBJS) $(MLX) $(FT) -o $(NAME)

compile_lib:
			@ $(MAKE) -C $(LIBFT)
			@ $(MAKE) -C $(MINILIBX)

re_lib:
			@ $(MAKE) -C $(LIBFT) re
			@ $(MAKE) -C $(MINILIBX) re

clean_lib:
			@ $(MAKE) -C $(LIBFT) clean

fclean_lib:
			@ $(MAKE) -C $(LIBFT) fclean

clean:		clean_lib
			@echo "Deleting objects...\n"
			@$(RM) $(OBJS) libmlx.dylib

fclean:		fclean_lib
			@echo "Deleting objects...\n"
			@$(RM) $(OBJS)
			@echo "Deleting $(NAME)\n"
			@$(RM) $(NAME) $(FT) $(MLX)

re:			fclean all

.PHONY:		all clean fclean re compile_lib re_lib clean_lib fclean_lib