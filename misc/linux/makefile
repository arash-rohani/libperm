# linux makefile for libperm (c permutation library)

NAM=perm
BASEDIR=../..
SRC = $(BASEDIR)/$(NAM).c
HED = $(BASEDIR)/$(NAM).h
LIC = $(BASEDIR)/LICENSE
LIB = lib$(NAM).so

# get library version from header file
MAJ = $(shell grep -m 1 "PERM_VERSION_MAJOR" $(HED) 2>/dev/null | cut -d ' ' -f3)
MIN = $(shell grep -m 1 "PERM_VERSION_MINOR" $(HED) 2>/dev/null | cut -d ' ' -f3)
PAT = $(shell grep -m 1 "PERM_VERSION_PATCH" $(HED) 2>/dev/null | cut -d ' ' -f3)
TAR = $(LIB).$(MAJ).$(MIN).$(PAT)

# installation directory
DESTDIR ?= /usr/local
DIR = $(shell realpath $(DESTDIR))

.PHONY: all clean install uninstall

all: $(TAR)

clean:
	rm -f $(TAR)

$(TAR): $(SRC)
	$(CC) -O3 -Wall -Wextra -Wconversion -shared $^ -o $@

$(SRC): $(HED)

# install script
define INSTALL
install -Dm 644 $(HED) $(DIR)/include/$(NAM).h
install -Dm 644 $(TAR) $(DIR)/lib/$(TAR)
ln -sf $(TAR) $(DIR)/lib/$(LIB).$(MAJ)
ln -sf $(LIB).$(MAJ) $(DIR)/lib/$(LIB)
install -Dm 644 $(LIC) $(DIR)/share/licenses/lib$(NAM)/LICENSE
endef

# uninstall script
define UNINSTALL
rm -f $(DIR)/include/$(NAM).h
rm -f $(DIR)/lib/$(TAR)
rm -f $(DIR)/lib/$(LIB).$(MAJ)
rm -f $(DIR)/lib/$(LIB)
rm -rf $(DIR)/share/licenses/lib$(NAM)
endef

# export to have them as multiline text variables in the shell
export INSTALL
export UNINSTALL


install: $(TAR) $(HED) $(LIC)
	@i=0; \
	while read -r IL; do \
		echo $${IL}; \
		if ! $$($${IL}); then \
			j=0; \
			while read -r UL; do \
				if [ $${i} -eq $${j} ]; then \
					break; \
				else \
					$$($${UL}); \
					((j++)); \
				fi; \
			done <<< $${UNINSTALL}; \
			break; \
		else \
			((i++)); \
		fi; \
	done <<< $${INSTALL}

uninstall:
	@while read -r UL; do echo $${UL}; $$($${UL}); done <<< $${UNINSTALL}
