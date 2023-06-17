CXX=g++
RM=rm -f
CPPFLAGS=-O3 -std=c++11

# SRCS=main.cpp Image.cpp Filter.cpp helpers.cpp Volume.cpp Projection.cpp Slice.cpp
SRC_DIR = ./src
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
SRCS := $(filter-out $(SRC_DIR)/main.cpp $(SRC_DIR)/test.cpp,$(SRCS))
OBJS=$(subst .cpp,.o,$(SRCS))
all: main

test: $(OBJS) $(SRC_DIR)/test.o
	$(CXX) $(CPPFLAGS) -o test $(OBJS) $(SRC_DIR)/test.o

$(SRC_DIR)/test.o: $(SRC_DIR)/test.cpp $(SRCS)

main: $(OBJS) $(SRC_DIR)/main.o
	$(CXX) $(CPPFLAGS) -o main $(OBJS) $(SRC_DIR)/main.o

# $(SRC_DIR)/main.o: $(SRC_DIR)/main.cpp

# $(OBJS): %.o: %.cpp %.h
$(SRC_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/Image.h $(SRC_DIR)/Filter.h $(SRC_DIR)/Projection.h $(SRC_DIR)/Volume.h $(SRC_DIR)/helpers.h $(SRC_DIR)/Slice.h $(SRC_DIR)/MemManager.h $(SRC_DIR)/Exceptions.h

$(SRC_DIR)/Exceptions.o: $(SRC_DIR)/Exceptions.cpp $(SRC_DIR)/Exceptions.h

$(SRC_DIR)/Image.o: $(SRC_DIR)/Image.cpp $(SRC_DIR)/Image.h

$(SRC_DIR)/Filter.o: $(SRC_DIR)/Filter.cpp $(SRC_DIR)/Filter.h $(SRC_DIR)/helpers.h $(SRC_DIR)/Image.h $(SRC_DIR)/Volume.h

$(SRC_DIR)/helpers.o:$(SRC_DIR)/helpers.cpp $(SRC_DIR)/helpers.h $(SRC_DIR)/Exceptions.h $(SRC_DIR)/Projection.h $(SRC_DIR)/MemManager.h $(SRC_DIR)/Slice.h

$(SRC_DIR)/Volume.o:$(SRC_DIR)/Volume.cpp $(SRC_DIR)/Volume.h $(SRC_DIR)/Filter.h $(SRC_DIR)/Exceptions.h $(SRC_DIR)/MemManager.h

$(SRC_DIR)/Projection.o:$(SRC_DIR)/Projection.cpp $(SRC_DIR)/Projection.h $(SRC_DIR)/Image.h $(SRC_DIR)/Volume.h $(SRC_DIR)/MemManager.h

$(SRC_DIR)/Slice.o:$(SRC_DIR)/Slice.cpp $(SRC_DIR)/Slice.h $(SRC_DIR)/Image.h $(SRC_DIR)/Volume.h
 
$(SRC_DIR)/MemManager.o: $(SRC_DIR)/MemManager.cpp $(SRC_DIR)/MemManager.h $(SRC_DIR)/Image.h $(SRC_DIR)/Volume.h $(SRC_DIR)/Slice.h

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) main