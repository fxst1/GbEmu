#################################
#                               #
#  PROJECT GENERATOR MAKEFILE   #
#                               #
#################################


#### Globals
NAME=lib
VERSION=0.1

#### Path Code
DEFAULT_ACTION=staticlib
SOURCE_DIRECTORY=source
INCLUDE_DIRECTORY=include
BIN_DIRECTORY=bin
SAMPLES_DIRECTORY=examples
DEPS_DIRECTORY=deps

#### Path Object
OBJECT_DIRECTORY=$(BIN_DIRECTORY)/obj
BUILD_DIRECTORY=$(BIN_DIRECTORY)/build
TARGET_NAME=$(BUILD_DIRECTORY)/$(NAME)

#### Path Output Types
STATIC_LIB_EXT=a
DYMAMIC_LIB_EXT=so
OBJECT_EXT=o

#### C
C_COMPILER = cc
C_COMPILER_FLAGS = -g -Wall -Wextra -I $(INCLUDE_DIRECTORY)
C_FILE_EXT = c
C_SOURCE = $(filter %.$(C_FILE_EXT), $(shell find $(SOURCE_DIRECTORY) -type f))
C_OBJECT := $(subst $(SOURCE_DIRECTORY),,$(addprefix $(OBJECT_DIRECTORY), $(C_SOURCE:.$(C_FILE_EXT)=.$(OBJECT_EXT))))

#### CPP
CPP_COMPILER := g++
CPP_COMPILER_FLAGS :=  -g --std=gnu++17 -Wall -Wextra -I $(INCLUDE_DIRECTORY)
CPP_FILE_EXT := cc
CPP_SOURCE = $(filter %.$(CPP_FILE_EXT), $(shell find $(SOURCE_DIRECTORY) -type f))
CPP_OBJECT := $(subst $(SOURCE_DIRECTORY),,$(addprefix $(OBJECT_DIRECTORY), $(CPP_SOURCE:.$(CPP_FILE_EXT)=.$(OBJECT_EXT))))

#### Outputs Types Marcros
STATIC_LIB := $(TARGET_NAME).$(STATIC_LIB_EXT)
DYNAMIC_LIB := $(TARGET_NAME).$(DYMAMIC_LIB_EXT)
SOURCES := $(C_SOURCE) $(CPP_SOURCE)
OBJECTS := $(C_OBJECT) $(CPP_OBJECT)
SAMPLES_SRC := $(shell find $(SAMPLES_DIRECTORY) -type d -maxdepth 1 -mindepth 1)
SAMPLES_EXEC := $(addprefix $(BUILD_DIRECTORY)/, $(subst $(SAMPLES_DIRECTORY)/,,$(SAMPLES_SRC)))

ifeq ($(DEFAULT_ACTION),staticlib)
	OUTPUT_NAME := $(NAME).$(STATIC_LIB_EXT)
else ifeq ($(DEFAULT_ACTION),dymlib)
	OUTPUT_NAME := $(NAME).$(DYNAMIC_LIB)
endif

all: $(DEFAULT_ACTION)

$(BIN_DIRECTORY):
	@mkdir -vp $(BIN_DIRECTORY)

deps: re ncurses

ncurses:
	@brew install ncurses

debug:
	@echo "== Global"
	@echo "File Output:" $(TARGET_NAME)
	@echo "Source Dir:" $(SOURCE_DIRECTORY)
	@echo "Output Dir:" $(OBJECT_DIRECTORY)
	@echo "All Source:" $(SOURCES)
	@echo "All Output:" $(OBJECTS)
	@echo "-> :" $(SAMPLES_SRC)
	@echo "-> :" $(SAMPLES_EXEC)
	@echo ""
	@echo "== C++ Configs"
	@echo "Compiler: " $(CPP_COMPILER) $(CPP_COMPILER_FLAGS)
	@echo "Sources:  " $(CPP_SOURCE)
	@echo "Output:   " $(CPP_OBJECT)
	@echo ""
	@echo "== C Configs"
	@echo "Compiler: " $(C_COMPILER) $(C_COMPILER_FLAGS)
	@echo "Sources:  " $(C_SOURCE)
	@echo "Output:   "  $(C_OBJECT)
	@echo ""
	@echo "== Samples"
	@echo $(SAMPLES_SRC)


$(OBJECT_DIRECTORY)%.$(OBJECT_EXT): $(SOURCE_DIRECTORY)%.$(C_FILE_EXT)
	@mkdir -p $(@D)
	$(C_COMPILER) $(C_COMPILER_FLAGS) -o $@ -c $<

$(OBJECT_DIRECTORY)%.$(OBJECT_EXT): $(SOURCE_DIRECTORY)%.$(CPP_FILE_EXT)
	@mkdir -p $(@D)
	$(CPP_COMPILER) $(CPP_COMPILER_FLAGS) -o $@ -c $<


exec: $(OBJECTS)
	@mkdir -p $(BUILD_DIRECTORY)
	$(CPP_COMPILER) $(CPP_COMPILER_FLAGS) $(OBJECTS) -o $(TARGET_NAME)

staticlib: $(OBJECTS)
	@mkdir -p $(BUILD_DIRECTORY)
	@ar -rc $(STATIC_LIB) $(OBJECTS)
	ln -sf $(STATIC_LIB) $(NAME).$(STATIC_LIB_EXT)

#dymlib: $(DYMLIB_LIB)

clean:
	@rm -rf $(OBJECT_DIRECTORY)

fclean:
	@rm -rf $(BIN_DIRECTORY)

re: fclean all



$(BUILD_DIRECTORY)/%: $(SAMPLES_DIRECTORY)/%
	make -C $< re

init: fclean
	@mkdir -vp ./$(SOURCE_DIRECTORY)
	@mkdir -vp ./$(INCLUDE_DIRECTORY)
	@mkdir -vp ./$(SAMPLES_DIRECTORY)

.ONESHELL:
initsample: init
	@while [ -z "$$projname" ]; do \
	        read -r -p "Enter project name: " projname; \
	    done ; \
	echo "Creating project : $$projname" \
	@mkdir -vp $(SAMPLES_DIRECTORY)/$$projname \
	@cat .fxstdev/Makefile.sample | sed \
		-e "s/<INCLUDE_DIRECTORY>/$INCLUDE_DIRECTORY/g" \
	 	-e "s/<SAMPLE_NAME>/$$projname/g" \
		-e "s/<LIBS>/$$projname/g" \
		> $(SAMPLES_DIRECTORY)/"$$projname"/Makefile

examples: all $(SAMPLES_EXEC)
.PHONY: re fclean clean all staticlib dymlib exec
