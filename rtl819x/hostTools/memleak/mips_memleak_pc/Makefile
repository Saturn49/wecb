

all:	compile 

NAME	= debug/mem_leak


# Dependencies
CC := gcc
DEPEND_FILES	=  act_memleak_func_file_ref.o act_memleak_mem_ref.o act_memleak_mq.o act_avl_tree.o act_elf.o mc_socket.o mc_hash.o act_profile.o

CFLAGS	= -D_LINUX  -Wall -D_DEBUG -D__USE_GNU -g -rdynamic 
DEBUG	= -g -rdynamic

IFLAGS 	= -I./include -I./ 
LFLAGS = -lrt -ldl -lpthread

compile: $(NAME) $(PRE_LD)

#
#	Build archive of objects
#
$(NAME): $(DEPEND_FILES)
	$(CC) -o $(NAME) $(CFLAGS) $(DEPEND_FILES) $(LFLAGS)


$(PRE_LD):


#
#	Primary link
#


clean:
	rm -f $(NAME) $(DEPEND_FILES)
	rm -f debug/*


#
#	Transition rules (add -o to put object in right directory)
#
.c.o:
	$(CC) -c -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) $<
