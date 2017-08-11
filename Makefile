NAME				=	asio

EXTENSION_DIR		=	${shell php-config --extension-dir}

EXTENSION 			=	${NAME}.so

COMPILER			=	g++
LINKER				=	g++

COMPILER_FLAGS		=	-Wall -c -O2 -std=c++11 -fpic -o
LINKER_FLAGS		=	-shared
LINKER_DEPENDENCIES	=	-lphpcpp -lboost_system

RM					=	rm -f
CP					=	cp -f

SOURCES				=	$(wildcard src/*.cpp)
OBJECTS				=	$(SOURCES:%.cpp=%.o)

all:					${OBJECTS} ${EXTENSION}

${EXTENSION}:			${OBJECTS}
						${LINKER} ${LINKER_FLAGS} -o $@ ${OBJECTS} ${LINKER_DEPENDENCIES}

${OBJECTS}:
						${COMPILER} ${COMPILER_FLAGS} $@ ${@:%.o=%.cpp}

install:
						${CP} ${EXTENSION} ${EXTENSION_DIR}/${EXTENSION}
				
clean:
						${RM} ${EXTENSION} ${OBJECTS}