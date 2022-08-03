#PROGRAM NAME-------------------------------------------------------------------

NAME 			= 	pipex

#SYSTEM VAR---------------------------------------------------------------------

R = $(shell tput -Txterm setaf 1)
G = $(shell tput -Txterm setaf 2)
C = $(shell tput -Txterm setaf 6)
W = $(shell tput -Txterm setaf 7)
Y = $(shell tput -Txterm setaf 3)
Z = $(shell tput -Txterm setaf 5)

CFLAGS 			= 	-g -Wall -Werror -Wextra
CC				= 	gcc
RM				= 	rm -rf
VALG_LEAK		=	valgrind --leak-check=full
UNAME_S		 	= 	$(shell uname -s)
REL_PATH		=	$(shell pwd)

#DIRECTORIES--------------------------------------------------------------------

SRCS_DIR 		= 	./src
OBJS_DIR		= 	./obj
LIBFT_DIR		= 	./libft

#FILES--------------------------------------------------------------------------

SRCS_FILES	 	= 	0_pipex.c \
					1_child.c

LIBFT_FILES		= 	libft.a

#FILES VAR----------------------------------------------------------------------
SRCS 			= 	$(addprefix $(SRCS_DIR)/, $(SRCS_FILES))

OBJS 			= 	$(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

LIBFT 			= 	$(addprefix $(LIBFT_DIR)/, $(LIBFT_FILES))

LIBFT_LINUX		=	-L$(REL_PATH)/libft -lft

#SYSTEM RULES-------------------------------------------------------------------

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

#COMPILING RULES------------------------------------------------------------------

all : 				init $(NAME)

init:
					@$(MAKE) -s -C $(LIBFT_DIR)
					@mkdir -p $(OBJS_DIR)

$(NAME):			$(OBJS) 
ifeq ($(UNAME_S),Linux)
						@gcc -g $(CFLAGS) libft/*c $(SRCS) -o $(NAME)
else
						$(CC) $(CFLAGS) $(LIBFT) -o $(NAME) $(OBJS)
endif
					@echo "$G$(NAME) compiled$W"
					
$(LIBFT):
					@cd $(LIBFT_DIR)/ && make
					

clean:
					@$(MAKE) -s clean -C $(LIBFT_DIR)
					@$(RM) $(OBJS)
					@$(RM) $(OBJS_DIR)
					@echo "$RAll objects deleted$W"

fclean: 			clean
					@$(MAKE) -s fclean -C $(LIBFT_DIR)
					@$(RM) $(NAME)
					@echo "$R$(NAME) deleted$W"
	
re: 				fclean all

debug:
					gcc -g $(CFLAGS) libft/*c $(SRCS) -o $(NAME)

clear:
					@clear

test1:
					cat file1
					./pipex file1 "grep file" "grep 2" file99
					cat file99
					$(RM) file99

test2:
					cat file1
					./pipex file1 "grep file" "grep 2" "grep a" file99
					cat file99
					$(RM) file99

#PARCING TEST-------------------------------------------------------------------
args_test:
					./pipex
					./pipex file1
					./pipex file1 "grep a"
					./pipex file1 "grep a" "grep b"

empty_pipe:
					./pipex file1 "grep z" "grep w" fileout
					cat fileout
					$(RM) fileout
					
file_not_exist:
					./pipex nofile "grep a" "grep b" fileout

file_no_right:
					touch filenr
					chmod 000 filenr
					./pipex filenr "grep a" "grep b" fileout
					chmod 777 filenr
					$(RM) filenr

wrong_cmd:
					./pipex file1 "cmd1" "grep b" fileout
					./pipex file1 "grep a" "cmd2" fileout
					./pipex file1 "grep a" "grep b" "cmd3" fileout

p_test:				args_test empty_pipe file_not_exist file_no_right wrong_cmd

#--------------------

val1:
					cat file1
					$(VALG_LEAK) ./pipex file1 "grep file" "grep 2" "grep a" file99
					cat file99
					$(RM) file99

lldb:
					lldb ./pipex file1 "grep file" "grep 2" file99


#PHONY--------------------------------------------------------------------------

.PHONY:				all clean fclean re leaks clear debug

