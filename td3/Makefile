TARGET := arm-td3

CXX := clang++
CXXFLAGS := -std=c++20 -O3 -Wall -I/usr/local/include -I/opt/homebrew/include

FILES := $(shell basename -a $$(ls *.cpp) | sed 's/\.cpp//g')
SRC := $(patsubst %, %.cpp, $(FILES))
OBJ := $(patsubst %, %.o, $(FILES))
HDR := $(patsubst %, -include %.h, $(FILES))

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(HDR) -c -o $@ $<

build: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -vf $(TARGET) $(OBJ)
