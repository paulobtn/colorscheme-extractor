TARGET_EXEC ?= extractor

IDIR ?= ./include
BUILD_DIR=./build
SRC_DIR ?= ./src

SRCS := $(shell find $(SRC_DIR) -name *.c)
HEADERS := $(shell find $(IDIR) -name *.h)
OBJS := $(SRCS:./src/%.c=$(BUILD_DIR)/%.o)

CC=gcc
CFLAGS=-I$(IDIR)

LIBS=-lm

define AddRule
$(2):$(1) $(HEADERS)
	@mkdir -p $(dir $(2))
	$(CC) -c -o $$@ $$< $(CFLAGS)
endef

all: $(OBJS) $(TARGET_EXEC)

$(foreach s, $(SRCS), $(eval $(call AddRule, $s, $(filter %$(basename $(notdir $s)).o,$(OBJS)) ) ) )

$(TARGET_EXEC): $(OBJ)
	$(CC) $(OBJS) -o $@ $(CFLAGS) $(LIBS)

print-%  : ; @echo $* = $($*)

clean:
	rm -rf $(BUILD_DIR)/
	rm -f $(TARGET_EXEC)
