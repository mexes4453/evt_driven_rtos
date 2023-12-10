
SRCS:= main.c  os.c clk.c utils.c app.c

OBJS:= $(SRCS:.c=.o)
MSG = 
REP = mexes

NAME = rtos

CC = gcc
STD = c89
DEBUG = 
VALGRIND =
VAL =
CFLAGS = -Werror -Wall -Wextra -g -pthread -lrt #-std=$(STD) 
#CFLAGS += -D__thread__
INCLUDES = -I./

#=== DEBUG ====
ifeq ($(DEBUG), 1)
	CFLAGS := $(CFLAGS) -g -D_DEBUG_=1
endif

ifeq ($(VALGRIND), 1)
	VAL = valgrind
endif
	
#=== COLORS ====
COL_Y = "\033[1;32m"
COL_G = "\033[1;33m"
COL_D = "\033[0m"

NAME : all
all : $(NAME)
$(NAME) : $(OBJS) 
	@echo "\033[1;33mCompiling Executables: $(NAME) \033[0m"
	$(CC) $^ $(CFLAGS) $(INCLUDES) -o $@
	@echo; echo "\033[1;32mCompilation Successful. \033[0m"
	@echo; echo

	
# obj files output
%.o : %.c
	@echo
	@echo "\033[1;33mCompiling OBJ files \033[0m"
	$(CC) -c $^ $(CFLAGS) $(INCLUDES) -o $@
	@echo


# remove all object files
fclean:
	rm -rf *.o

# remove final target files
clean: fclean
	rm -rf $(NAME)

# recompile everything
re: clean all

#=================== GIT ===============================
#define comments
push:	clean
	$(info Pushing to Git Repository)
ifeq ($(REP), 42)
	@git push wolfsburg main
	@echo $(COL_G)Pushed to repo: $(REP)$(COL_D)
#	@git rm -f --cached *.pdf *.md			#	2> /dev/null
else 
	@git add ../*
	@git ls-files; git status
	@git commit -m "$(MSG)"
	@git push mexes main
	@echo $(COL_G)Pushed to repo: mexes$(COL_D)
	@echo $(COL_G)$(MSG)$(COL_D)
endif

#endef # comments


# ========== TEST ====================
test:
	@echo
#	@echo $(COL_G)=== TEST === 1 = NO ARGS$(COL_D)
#	@$(VAL) ./$(NAME)
.PHONY : all fclean clean re push test
