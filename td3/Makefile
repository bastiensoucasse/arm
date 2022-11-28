# https://stackoverflow.com/questions/287259/minimum-c-make-file-for-linux
NAME = my_project

FILES = $(shell basename -a $$(ls *.cpp) | sed 's/\.cpp//g')
SRC = $(patsubst %, %.cpp, $(FILES))
OBJ = $(patsubst %, %.o, $(FILES))
HDR = $(patsubst %, -include %.h, $(FILES))
CXX = g++ -Wall

%.o : %.cpp
	$(CXX) $(HDR) -c -o $@ $<

build: $(OBJ)
	$(CXX) -o $(NAME) $(OBJ)

clean:
	rm -vf $(NAME) $(OBJ)
