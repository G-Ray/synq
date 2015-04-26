CFLAGS = -Wall
TARGET = synq-client
OBJECTS = synq-client.o ../common/protocol.o ../common/utils.o ../common/linked_list.o

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)
