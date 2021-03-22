BIN			= bin
SRC			= src
INC			= includes
LIBFT		= libft

LINUX			= 0

MINILIBX_OS_X	= minilibx-macos
MINILIBX_LINUX	= minilibx-linux
ifeq ($(LINUX),0)
MINILIBX		= $(MINILIBX_OS_X)
MLX				= libmlx.dylib
else
MINILIBX		= $(MINILIBX_LINUX)
MLX				= libmlx.a
endif

FT			= libft.a

SRCS		=	main.c								\
				parsing/parser.c					\
				parsing/node_parser.c				\
				parsing/misc_parser.c				\
				parsing/gnl/get_next_line.c			\
				parsing/gnl/get_next_line_utils.c	\
				element/camera.c					\
				element/light.c						\
				element/2d/square.c					\
				element/2d/circle.c					\
				element/2d/triangle.c				\
				element/2d/plane.c					\
				element/3d/sphere.c					\
				element/3d/cylinder.c				\
				engine/ray.c						\
				engine/collision.c					\
				impl/mlx/window.c					\
				impl/mlx/image.c					\
				impl/mlx/refresh.c					\
				hook/key_hook.c						\
				hook/close_hook.c					\
				event/on_change_camera.c			\
				event/on_close.c					\
				provider/scene_provider.c			\
				provider/debug_mode_provider.c		\
				util/logs.c							\
				util/scene.c						\
				util/deserializers.c				\
				util/deserializers2.c				\

OBJS		= $(addprefix $(BIN)/, $(SRCS:.c=.o))

NAME		= miniRT

CC			= gcc
RM			= rm -f

CFLAGS		= -Wall -Wextra# -Werror
INCLUDES	= -I$(INC) -I$(LIBFT)/$(INC) -I$(MINILIBX)

HEADERS		= includes/minirt.h

all:		$(NAME)

LIBRARIES	= -lpthread
ifeq ($(LINUX),1)
LIBRARIES	+= -lm -lXext -lX11
endif

$(BIN)/%.o:	$(SRC)/%.c $(HEADERS)
			@ mkdir -p $(dir $@);
			$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ -O3

$(NAME):	compile_lib $(OBJS)
			@ ln -sf $(MINILIBX)/$(MLX) .
			@ ln -sf $(LIBFT)/$(FT) .
			@ $(CC) $(CFLAGS) $(OBJS) $(MLX) $(FT) -o $(NAME) $(LIBRARIES)

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
