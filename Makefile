CFLAGS = -O2 -std=c99 -Wall -Wextra -Werror -pedantic-errors
LIB_SRCS = CTFPatch.c
MAIN_SRC = main.c
LIB_OBJS = $(LIB_SRCS:.c=.o)
MAIN_OBJ = $(MAIN_SRC:.c=.o)
ifneq ($(OS),Windows_NT)
	MAIN = CTFPatch
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		SHARED_LIB = libctfpatch.dylib
		SO_LIB_CMD = $(CC) $(CFLAGS) -dynamiclib -o $(SHARED_LIB) $(LIB_OBJS)
	else
		SHARED_LIB = libctfpatch.so
		SO_LIB_CMD = $(CC) $(CFLAGS) -o $(SHARED_LIB) $(LIB_OBJS) -shared
	endif
	STATIC_LIB = libctfpatch.a
else
	MAIN = CTFPatch.exe
	SHARED_LIB = ctfpatch.dll
	STATIC_LIB = ctfpatch.lib
	SO_LIB_CMD = $(CC) $(CFLAGS) -o $(SHARED_LIB) $(LIB_OBJS) -shared
endif

all:			$(SHARED_LIB) $(STATIC_LIB) $(MAIN)

libs:			$(SHARED_LIB) $(STATIC_LIB)

shared:			$(SHARED_LIB)

$(SHARED_LIB):	$(LIB_OBJS)
				$(SO_LIB_CMD)

static:			$(STATIC_LIB)

$(STATIC_LIB):	$(LIB_OBJS)
				$(AR) -rv $(STATIC_LIB) $(LIB_OBJS)

app:			$(MAIN)

$(MAIN):		$(LIB_OBJS) $(MAIN_OBJ)
				$(CC) $(CFLAGS) -o $(MAIN) $(LIB_OBJS) $(MAIN_OBJ)

clean:
				$(RM) $(LIB_OBJS) $(MAIN_OBJ) $(SHARED_LIB) $(STATIC_LIB) *~ $(MAIN)