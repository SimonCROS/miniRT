# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/23 11:39:11 by scros             #+#    #+#              #
#    Updated: 2021/02/19 15:53:38 by scros            ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

BIN			= bin
SRC			= src
INC			= includes
LIBFT		= libft

LINUX			= 0

MINILIBX_OS_X	= minilibx
MINILIBX_LINUX	= minilibx-linux
ifeq ($(LINUX),0)
MINILIBX		= $(MINILIBX_OS_X)
MLX				= libmlx.dylib
else
MINILIBX		= $(MINILIBX_LINUX)
MLX				= libmlx.a
endif

FT			= libft.a

SRCS		=	main.c				\
				entity/camera.c		\
				object/light.c		\
				object/2d/square.c	\
				object/2d/circle.c	\
				object/2d/triangle.c\
				object/2d/plan.c	\
				object/3d/sphere.c	\
				object/3d/cylinder.c	\

OBJS		= $(addprefix $(BIN)/, $(SRCS:.c=.o))

NAME		= miniRT

CC			= gcc
RM			= rm -f

CFLAGS		= # -Wall -Wextra -Werror
INCLUDES	= -I$(INC) -I$(LIBFT)/$(INC) -I$(MINILIBX)

HEADERS		= includes/minirt.h

all:		$(NAME)

LIBRARIES	= -lpthread
ifeq ($(LINUX),1)
LIBRARIES	+= -lm -lXext -lX11
endif

$(BIN)/%.o:	$(SRC)/%.c $(HEADERS)
			@ mkdir -p $(dir $@);
			$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ -g

$(NAME):	compile_lib $(OBJS)
			@ ln -sf $(MINILIBX)/$(MLX) .
			@ ln -sf $(LIBFT)/$(FT) .
			@ $(CC) $(CFLAGS) $(OBJS) $(MLX) $(FT) -o $(NAME) $(LIBRARIES) -g

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
			@$(RM) $(OBJS) $(MLX)

fclean:		fclean_lib
			@echo "Deleting objects...\n"
			@$(RM) $(OBJS)
			@echo "Deleting $(NAME)\n"
			@$(RM) $(NAME) $(FT) $(MLX)

re:			fclean all

.PHONY:		all clean fclean re compile_lib re_lib clean_lib fclean_lib