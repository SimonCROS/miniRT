LINUX				:= 1

# Styles

override _RESET		:= \033[0m
override _BOLD		:= \033[1m
override _UNDER		:= \033[4m
override _REV		:= \033[7m

# Colors

override _GREY		:= \033[30m
override _RED		:= \033[31m
override _GREEN		:= \033[32m
override _YELLOW	:= \033[33m
override _BLUE		:= \033[34m
override _PURPLE	:= \033[35m
override _CYAN		:= \033[36m
override _WHITE		:= \033[37m

# Backgrounds

override _IGREY		:= \033[40m
override _IRED		:= \033[41m
override _IGREEN	:= \033[42m
override _IYELLOW	:= \033[43m
override _IBLUE		:= \033[44m
override _IPURPLE	:= \033[45m
override _ICYAN		:= \033[46m
override _IWHITE	:= \033[47m

# Folders

override LIBFT_DIR	:= libft
override LIBFT		:= libft.a
ifeq ($(LINUX),0)
override MINILIBX_DIR	:= minilibx-macos
override MINILIBX	:= libmlx.dylib
else
override MINILIBX_DIR	:= minilibx-linux
override MINILIBX	:= libmlx.a
endif

override BIN		:= bin
override SRC		:= src
override INC		:= includes

override COLOR		:= color
override BITMAP		:= bitmap
override CONV		:= convert
override MAP		:= map
override DLIST		:= dlist
override LIST		:= list
override ITERAT		:= iterator
override MATH		:= math
override HASH		:= hash
override MEMORY		:= memory
override PRINT		:= print
override STRING		:= string
override TPOOL		:= tpool
override UTIL		:= util

# Properties

NAME				= miniRT

# Commands

override CC			:= gcc
override RM			:= rm -rf
override CFLAGS		:= -Wall -Wextra -Werror
override INCLUDES	:= -I$(INC) -I$(LIBFT_DIR)/$(INC) -I$(MINILIBX_DIR) -g3

# Sources

override SRCS		:=									\
				main.c									\
				exit.c									\
				parsing/parser.c						\
				parsing/reader.c						\
				parsing/node_parser.c					\
				parsing/misc_parser.c					\
				parsing/gnl/get_next_line.c				\
				parsing/gnl/get_next_line_utils.c		\
				element/camera.c						\
				element/archetype/plane.c				\
				element/archetype/light.c				\
				element/archetype/object.c				\
				element/archetype/quadric.c				\
				element/archetype/compound.c			\
				element/light/point.c					\
				element/light/laser.c					\
				element/plane/plane.c					\
				element/plane/circle.c					\
				element/plane/square.c					\
				element/plane/triangle.c				\
				element/quadric/cylinder.c				\
				element/quadric/hyperboloid.c			\
				element/quadric/sphere.c				\
				element/compound/cube.c					\
				element/compound/pyramid.c				\
				engine/ray.c							\
				engine/project.c						\
				engine/z_buffer.c						\
				engine/renderer.c						\
				engine/collision.c						\
				engine/thread_worker.c					\
				engine/chunk_worker.c					\
				engine/engine_manager.c					\
				impl/bmp/bmp.c							\
				impl/mlx/image.c						\
				impl/mlx/init.c							\
				impl/mlx/game.c							\
				impl/mlx/window.c						\
				impl/mlx/refresh.c						\
				impl/mlx/hook/key_hook.c				\
				impl/mlx/hook/mouse_hook.c				\
				impl/mlx/hook/close_hook.c				\
				impl/mlx/event/on_drag.c				\
				impl/mlx/event/on_click.c				\
				impl/mlx/event/on_scroll.c				\
				impl/mlx/event/on_change_camera.c		\
				provider/debug_mode_provider.c			\
				util/logs.c								\
				util/utils.c							\
				util/scene.c							\
				util/deserializers.c					\
				util/deserializers2.c					\
				graphic/vector3f/add.c					\
				graphic/vector3f/angle.c				\
				graphic/vector3f/cross.c				\
				graphic/vector3f/distance.c				\
				graphic/vector3f/div.c					\
				graphic/vector3f/negate.c				\
				graphic/vector3f/dot.c					\
				graphic/vector3f/length.c				\
				graphic/vector3f/mul.c					\
				graphic/vector3f/rotate.c				\
				graphic/vector3f/new.c					\
				graphic/vector3f/normalize.c			\
				graphic/vector3f/set.c					\
				graphic/vector3f/sub.c					\
				graphic/vector3f/deserialize.c			\
				graphic/quadric/resolve.c				\
				graphic/quadric/resolve2.c				\
				graphic/matrix/init.c					\
				graphic/matrix/mul.c					\
				graphic/matrix/inverse.c				\
				graphic/matrix/determin.c				\
				graphic/bounding_box/new.c				\
				graphic/bounding_box/intersecions.c		\
				graphic/line/new.c						\
				graphic/draw_line.c						\

override OBJS		:= $(addprefix $(BIN)/, $(SRCS:.c=.o))

override HEADERS	:= $(addprefix $(INC)/,				\
				minirt.h								\
				graphics.h								\
				object.h								\
				renderer.h								\
				)

LIBRARIES	:= -lpthread
ifeq ($(LINUX),1)
LIBRARIES	+= -lm -lXext -lX11
endif

all:		$(NAME)

$(BIN)/%.o:	$(SRC)/%.c $(HEADERS)
			@mkdir -p $(dir $@);
			$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME):	$(OBJS)
			$(MAKE) -C $(LIBFT_DIR)
			ln -sf $(LIBFT_DIR)/$(LIBFT)
			$(MAKE) -C $(MINILIBX_DIR)
			ln -sf $(MINILIBX_DIR)/$(MINILIBX)
			$(CC) $(CFLAGS) $(OBJS) $(MINILIBX) $(LIBFT) -o $(NAME) $(LIBRARIES)

clean:
			$(RM) $(OBJS)
			find $(BIN) -type d -empty -delete

fclean:		clean
			$(RM) $(NAME)
			$(RM) $(LIBFT)
			$(MAKE) -C $(LIBFT_DIR) fclean
			$(RM) $(MINILIBX)
			$(MAKE) -C $(MINILIBX_DIR) clean

re:			fclean all

.PHONY:		all clean fclean re
