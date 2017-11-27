NAME                =   asio

EXTENSION_DIR       =   ${shell php-config --extension-dir}
EXTENSION           =   ${NAME}.so

COMPILER            =   g++
LINKER              =   g++

COMPILER_FLAGS      =   -Wall -c -std=c++11 -fpic
LINKER_FLAGS        =   -shared
LINKER_DEPENDENCIES =   -lphpcpp -lboost_system -lboost_filesystem

RM                  =   rm -f
CP                  =   cp -rf

INCLUDE_DIR         =   src/include/php-asio
SOURCES             =   $(wildcard src/*.cpp)
OBJECTS             =   $(SOURCES:%.cpp=%.o)

HEADER_DIR          =   /usr/local/include

all: COMPILER_FLAGS += -O2 -s
all: ${OBJECTS} ${EXTENSION}

debug: COMPILER_FLAGS += -g -Og
debug: ${OBJECTS} ${EXTENSION}

${EXTENSION}: ${OBJECTS}
	${LINKER} ${LINKER_FLAGS} -o $@ ${OBJECTS} ${LINKER_DEPENDENCIES}

${OBJECTS}:
	${COMPILER} ${COMPILER_FLAGS} -o $@ ${@:%.o=%.cpp} -I${INCLUDE_DIR}

install:
	${CP} ${EXTENSION} ${EXTENSION_DIR}/${EXTENSION}

header:
	${CP} ${INCLUDE_DIR} ${HEADER_DIR}

clean:
	${RM} ${EXTENSION} ${OBJECTS}