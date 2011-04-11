# $Id $
                                                                                                                             
# Makefile for Rom24. Works fine on my Debian system.
# You may need to use 'gmake' on BSD systems.
# Enable IMC Uncomment
#IMC = 1
                                                                                                                             
CC = gcc
RM = rm
EXE = dystopia
PROF = -g3 -ggdb3
                                                                                                                             
# Use these two lines to use crypt(), ie on Linux systems.
#ifdef IMC
#C_FLAGS = $(PROF) -Wall -O -s -DIMC -DMERC
#else
C_FLAGS = $(PROF) -Wall -O
#endif
#L_FLAGS = $(PROF) -lcrypt -lz
                                                                                                                             
# Uncomment these two lines to use plaintext passwords.
# This is how you fix the 'crypt' linking errors!
#C_FLAGS = $(PROF) -s -Dlinux -O2
#C_FLAGS = $(PROF) -s -Dlinux -g -p -O -Wuninitialized
#C_FLAGS = $(PROF) -DIMC -IMCMERC
L_FLAGS = $(PROF) -lz -lcrypt -lpthread -lm -Dlinux
                                                                                                                             
# Source Files
SRC_FILES := $(wildcard *.c)
                                                                                                                             

# Object Files
OBJ_DIR = obj
OBJ_FILES := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

ifdef IMC
   SRC_FILES = imc.o sha256.o $(OBJ_FILES)
#   C_FLAGS = $(C_FLAGS) -DIMC -DIMCMERC
endif
                                                                                                                             
dystopia: $(OBJ_FILES)
	$(RM) -f $(EXE)
	chmod g+w $(OBJ_DIR)/*.o
	$(CC) $(L_FLAGS) -o $(EXE) $(OBJ_FILES)
                                                                                                                             
$(OBJ_DIR)/%.o: %.c
	$(CC) $(C_FLAGS) -c -o $@ $<
                                                                                                                             
clean:
	$(RM) -f $(OBJ_FILES) $(EXE) *~ *.bak *.orig *.rej

