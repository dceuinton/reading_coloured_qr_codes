CC = g++
BIN = main
OBJS = $(BUILD)/$(BIN).o 
CFLAGS = -std=c++11
INC = 
LIB = `pkg-config --libs opencv`

BUILD = ./src/obj
SRC = ./src

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LIB) $(LIBS)

$(BUILD)/%.o: $(SRC)/%.cpp 
	$(CC) -c -o $@ $< $(INC) $(CFLAGS)

clean: 
	rm $(BUILD)/*.o
	rm $(BIN)