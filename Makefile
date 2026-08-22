NAME = codexion

SRCS = src/main.c \
       src/parser.c \
	   src/utils.c \
	   src/stop.c \
	   src/init.c \
	   src/cleanup.c \
	   src/dongle/dongle_take.c \
	   src/dongle/dongle_wait.c \
       src/dongle/dongle_release.c \
       src/coder/coder_compile.c \
       src/coder/coder_debug.c \
       src/coder/coder_refactor.c \
       src/coder/coder_routine.c \
	   src/monitor/monitor.c \
	   src/scheduler/heap.c \
	   src/scheduler/heap_utils.c \
	   src/scheduler/heap_access.c

OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror -pthread

all: $(NAME)

$(NAME): $(OBJS)
	cc $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c include/codexion.h include/types.h
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
