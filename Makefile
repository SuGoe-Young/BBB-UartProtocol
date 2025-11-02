CC := $(CROSS_COMPILE)gcc
STRIP := $(CROSS_COMPILE)strip

TARGET := uart_interface
SRCS := main.c uart_protocol.c
OBJS := $(SRCS:.c=.o)

INC := -Iinclude
CFLAGS := -Wall -g -O2 $(INC)
LDFLAGS :=

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "[LINK] $@"
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	$(STRIP) $@   

%.o: %.c
	@echo "[CC] $<"
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "[CLEAN]"
	rm -f $(OBJS) $(TARGET)
