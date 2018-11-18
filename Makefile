NAME = nouveau-indicator

CC = gcc

CFLAGS = -Wall -Wextra -O2

INCLUDES = `pkg-config --cflags gtk+-2.0` -I /usr/include/libappindicator-0.1

SRCS_PATH = src/

SRCS_NAME = nouveau-indicator.c

SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))

OBJS_PATH = obj/

OBJS_NAME = $(SRCS_NAME:.c=.o)

OBJS = $(addprefix $(OBJS_PATH), $(OBJS_NAME))

LIBRARIES = `pkg-config --libs gtk+-2.0` -lappindicator

all: odir $(NAME)

$(NAME): $(OBJS)
	@echo " - Making $(NAME)"
	@$(CC) $(CFLAGS) -o $(NAME) $^ $(LIBRARIES)

$(OBJS_PATH)%.o: $(SRCS_PATH)%.c
	@echo " - Compiling $<"
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

odir:
	@mkdir -p $(OBJS_PATH)

clean:
	rm -f $(OBJS)
	rm -f $(NAME)

.PHONY: clean odir
