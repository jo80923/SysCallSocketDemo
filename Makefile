CXX := g++
LINK := g++

# Includes
INCLUDES = -I. -I./include

# Common flags
COMMONFLAGS += ${INCLUDES}
CXXFLAGS += ${COMMONFLAGS}
CXXFLAGS += -Wall -std=c++11 -Iinclude

SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin

$(shell mkdir -p $(OBJDIR))
$(shell mkdir -p $(BINDIR))
$(shell mkdir -p ./data)
$(shell mkdir -p ./data)

_OBJS = client.cpp.o
_OBJS += server.cpp.o
_OBJS += io_util.cpp.o
_OBJS += SocketDemo.cpp.o
OBJS = ${patsubst %, ${OBJDIR}/%, ${_OBJS}}

ifeq ($(DEBUG),ON)
	CXXFLAGS += -DDEBUG
	LINK += -DDEBUG
endif

TARGET = SocketDemo
LINKLINE = ${LINK} ${OBJS} -o ${BINDIR}/${TARGET} ${LIB_CUDA}

.SUFFIXES: .cpp .o

${OBJDIR}/%.cpp.o: ${SRCDIR}/%.cpp
	${CXX} ${CXXFLAGS} ${INCLUDES} -c $< -o $@

${BINDIR}/${TARGET}: ${OBJS} Makefile
	${LINKLINE}

clean:
	rm -f bin/*
	rm -f obj/*
	rm -f data/nohup.out
