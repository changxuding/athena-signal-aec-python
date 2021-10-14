SWIG := swig
CXX := g++
CC := gcc
AR := ar

DIR_SRC := ./src
DIR_OBJ := ./src
DIR_LIB := ./lib

CXXFLAGS :=  -fPIC -std=c++11 -I. -I./src $(shell python3-config --cflags) 
LDFLAGS := -shared $(shell python3-config --ldflags) -L$(DIR_LIB) -laec

CFLAGS := -O3 -I$(DIR_SRC)
CLDFLAGS := -O3  -lm

SRCS := $(wildcard $(DIR_SRC)/*.c)
OBJS := $(patsubst %.c, %.o,$(SRCS))
LIB := $(DIR_LIB)/libaec.a
TARGET := _athenaaec.so

all: $(LIB) $(TARGET)
$(LIB) : $(OBJS)
	$(AR) -r $@ $^

$(DIR_OBJ)/%.o : $(DIR_SRC)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(CLDFLAGS)

athenaaec_wrap.cpp: athenaaec.i
	$(SWIG) -I. -c++ -python -o $@ $^

$(TARGET): athenaaec_wrap.o echo_canceller.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) 

clean:
	rm -f athenaaec_wrap.cpp *.o athenaaec.py *.pyc _athenaaec.so
	rm -f $(DIR_OBJ)/*.o
	rm -f $(LIB) $(TARGET)



