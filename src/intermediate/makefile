#OBJS specifies which files to compile as part of the project
OBJS = gameOfLife.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w -std=c++20

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2main

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = gameOfLife

packages:
	sudo apt install libsdl2-dev

#This is the target that compiles our executable
all : $(OBJS) packages
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)