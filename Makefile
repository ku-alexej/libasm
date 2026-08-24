# names
NAME		= libasm.a

TEST_NAME		= test_libasm
TEST_BONUS_NAME	= test_libasm_bonus

# flags for compilation and compiler
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -I$(INC_DIR)

ASM			= nasm
ASMFLAGS	= -f elf64

AR			= ar
ARFLAGS		= rcs

# pathes
SRCS_DIR	= srcs
OBJS_DIR	= objs
INC_DIR		= includes

# sources
SRCS		= ft_strlen.s \
			  ft_strcpy.s \
			  ft_strcmp.s \
			  ft_write.s \
			  ft_read.s \
			  ft_strdup.s

# BONUS_SRCS	= ft_atoi_base_bonus.s \
# 			  ft_list_push_front_bonus.s \
# 			  ft_list_size_bonus.s \
# 			  ft_list_sort_bonus.s \
# 			  ft_list_remove_if_bonus.s

# objects
OBJS		= $(addprefix $(OBJS_DIR)/, $(SRCS:.s=.o))
# BONUS_OBJS	= $(addprefix $(OBJS_DIR)/, $(BONUS_SRCS:.s=.o))

##########################################

all: $(NAME)

$(NAME): $(OBJS_DIR) $(OBJS)
	$(AR) $(ARFLAGS) $(NAME) $(OBJS)

# bonus: $(OBJS_DIR) $(OBJS) $(BONUS_OBJS)
# 	$(AR) $(ARFLAGS) $(NAME) $(OBJS) $(BONUS_OBJS)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.s
	$(ASM) $(ASMFLAGS) $< -o $@

$(TEST_NAME): $(NAME) main.c
	$(CC) $(CFLAGS) main.c -L. -lasm -o $(TEST_NAME)

test: $(TEST_NAME)
	./$(TEST_NAME)

# $(TEST_BONUS_NAME): bonus main_bonus.c
# 	$(CC) $(CFLAGS) main_bonus.c -L. -lasm -o $(TEST_BONUS_NAME)

# test_bonus: $(TEST_BONUS_NAME)
# 	./$(TEST_BONUS_NAME)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME) $(TEST_NAME) $(TEST_BONUS_NAME)

re: fclean all

.PHONY: all bonus clean fclean re test test_bonus
