NAME					:= miniRT
LINUX					:= 0

override HEADERS		:= includes/minirt.h

override BIN			:= bin
override SRC			:= src
override INC			:= includes
override LIBFT_DIR		:= libft

override LIBFT			:= libft.a

override MINILIBX_OS_X	:= minilibx-macos
override MINILIBX_LINUX	:= minilibx-linux
ifeq ($(LINUX),0)
override MINILIBX		:= $(MINILIBX_OS_X)
override MLX			:= libmlx.dylib
else
override MINILIBX		:= $(MINILIBX_LINUX)
override MLX			:= libmlx.a
endif

override CC				:= gcc
override RM				:= rm -f

override CFLAGS			:= -Wall -Wextra -Werror
override INCLUDES		:= -I$(INC) -I$(LIBFT_DIR)/$(INC) -I$(MINILIBX) -g3

override SRCS	:=									\
				main.c								\
				exit.c								\
				parsing/parser.c					\
				parsing/node_parser.c				\
				parsing/misc_parser.c				\
				parsing/gnl/get_next_line.c			\
				parsing/gnl/get_next_line_utils.c	\
				element/camera.c					\
				element/archetype/plane.c			\
				element/archetype/light.c			\
				element/archetype/object.c			\
				element/archetype/quadric.c			\
				element/archetype/compound.c		\
				element/light/point.c				\
				element/light/laser.c				\
				element/plane/plane.c				\
				element/plane/circle.c				\
				element/plane/square.c				\
				element/plane/triangle.c			\
				element/quadric/cylinder.c			\
				element/quadric/hyperboloid.c		\
				element/quadric/sphere.c			\
				element/compound/cube.c				\
				element/compound/pyramid.c			\
				engine/ray.c						\
				engine/z_buffer.c					\
				engine/renderer.c					\
				engine/collision.c					\
				engine/draw_line.c					\
				engine/engine_manager.c				\
				impl/bmp/bmp.c						\
				impl/mlx/image.c					\
				impl/mlx/window.c					\
				impl/mlx/refresh.c					\
				impl/mlx/hook/key_hook.c			\
				impl/mlx/hook/mouse_hook.c			\
				impl/mlx/hook/close_hook.c			\
				impl/mlx/event/on_drag.c			\
				impl/mlx/event/on_click.c			\
				impl/mlx/event/on_scroll.c			\
				impl/mlx/event/on_change_camera.c	\
				provider/scene_provider.c			\
				provider/debug_mode_provider.c		\
				util/logs.c							\
				util/scene.c						\
				util/deserializers.c				\
				util/deserializers2.c				\

override OBJS	:= $(addprefix $(BIN)/, $(SRCS:.c=.o))

LIBRARIES	:= -lpthread
ifeq ($(LINUX),1)
LIBRARIES	+= -lm -lXext -lX11
endif

all:		$(NAME)

$(BIN)/%.o:	$(SRC)/%.c $(HEADERS)
			@mkdir -p $(dir $@);
			$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ -O3

$(NAME):	compile_lib $(OBJS)
			ln -sf $(MINILIBX)/$(MLX) .
			ln -sf $(LIBFT_DIR)/$(LIBFT) .
			$(CC) $(CFLAGS) $(OBJS) $(MLX) $(LIBFT) -o $(NAME) $(LIBRARIES)

compile_lib:
			@$(MAKE) -C $(LIBFT_DIR)
			@$(MAKE) -C $(MINILIBX)

clean:
			@$(MAKE) -C $(MINILIBX) clean
			@$(MAKE) -C $(LIBFT_DIR) clean
			@echo "Deleting objects...\n"
			@$(RM) $(OBJS)
			@find . -type d -empty -delete

fclean:
			@$(MAKE) -C $(MINILIBX) clean
			@$(MAKE) -C $(LIBFT_DIR) fclean
			@echo "Deleting objects...\n"
			@$(RM) $(OBJS)
			@echo "Deleting $(NAME)\n"
			@$(RM) $(NAME) $(LIBFT) $(MLX)
			@find . -type d -empty -delete

re:			fclean all

.PHONY:		all clean fclean re compile_lib
