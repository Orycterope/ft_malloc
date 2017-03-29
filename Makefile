# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/18 19:13:31 by tvermeil          #+#    #+#              #
#    Updated: 2017/03/29 17:01:48 by tvermeil         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS += --no-print-directory

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

GENERIC_NAME = libft_malloc.so
NAME = libft_malloc_$(HOSTTYPE).so

SRC_PATH = src
HEADER_PATH = include/malloc
PRIVATE_HEADER_PATH = $(SRC_PATH)
ROOT = .
OBJ_FOLDER = obj/$(NAME)
INC_PATH =	$(ROOT)/include/libft \
			$(HEADER_PATH) \
			$(PRIVATE_HEADER_PATH) \

OBJ_PATH = $(ROOT)/$(OBJ_FOLDER)
DST_PATH = $(ROOT)
LFT_PATH = $(ROOT)

CC = gcc -g
CFLAGS = -Wall -Wextra -Werror -fPIC
CPPFLAGS = $(addprefix -I ,$(INC_PATH))
LDFLAGS = -L$(LFT_PATH) -fPIC -shared 
LDLIBS = -lft 

SRC_NAME =		malloc.c \
				table.c \
				entry.c \
				page.c \
				mapping.c \
				delete_mapping.c \
				len_counters.c \
				search.c \
				buffer.c \
				location_utils.c \

HEADER_NAME =	malloc.h \

PRIVATE_HEADER_NAME =	malloc_structures.h \
						malloc_functions.h \
						malloc_settings.h \
						global.h \

OBJ_NAME = $(SRC_NAME:.c=.o)

SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))
DST = $(addprefix $(DST_PATH)/,$(NAME))
HEADER_FILES = $(addprefix $(HEADER_PATH)/,$(HEADER_NAME)) \
			   $(addprefix $(SRC_PATH)/,$(PRIVATE_HEADER_NAME))

all: $(ROOT)/libft.a $(DST)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(HEADER_FILES) libft.a
	@mkdir -p $(OBJ_PATH)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	@echo -n .

$(DST): $(OBJ) $(HEADER_FILES)
	@$(CC) $(OBJ) $(LDFLAGS) $(LDLIBS) -o $(DST)
	@-cd $(DST_PATH) && ln -sf $(NAME) $(GENERIC_NAME)
	@echo " . \033[32m$(NAME)\033[0m done"

libft.a:
	@make -C libft all
clean:
	@-cd $(ROOT) && rm -fv $(addprefix $(OBJ_FOLDER)/,$(OBJ_NAME))
	@rmdir $(OBJ_PATH) $(ROOT)/obj 2> /dev/null || true
	@make -C libft clean

fclean: clean
	@-cd $(DST_PATH) && rm -fv $(NAME) && rm -fv $(GENERIC_NAME)
	@make -C libft fclean

re: fclean all

ac: all clean

norme:
	@norminette $(SRC) $(HEADER_FILES) | grep --color=always -B 1 'Error' || echo "\033[32m$(NAME)\033[0m norme OK"
	@make -C libft norme

.PHONY: all clean fclean re
