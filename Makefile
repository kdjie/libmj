INCLUDE = -I. -I..
LIBRARY =

CXXFLAGS    = -Wall -ggdb
CXXFLAGS_R    = -Wall -O2 -O3

LINKFLAGS   = -Wl,-rpath,./:../bin
LINKFLAGS_MT = $(LINKFLAGS)

CXXFLAGS_EXTERN =
LINKFLAGS_EXTERN =

TARGET   = libmj.a
TARGET_R   = libmj.ra

SRC_FILES   = $(wildcard *.cpp)

OBJ_FILES   = $(SRC_FILES:.cpp=.o)
OBJ_FILES_R   = $(SRC_FILES:.cpp=.ro)

$(TARGET) : $(OBJ_FILES)
	ar -rus -o $@ $(OBJ_FILES)
	
$(TARGET_R) : $(OBJ_FILES_R)
	ar -rus -o $@ $(OBJ_FILES_R)

%.o : %.cpp
	g++ -c -o $@ $< $(CXXFLAGS) $(CXXFLAGS_EXTERN) $(INCLUDE)
%.ro : %.cpp
	g++ -c -o $@ $< $(CXXFLAGS_R) $(CXXFLAGS_EXTERN) $(INCLUDE)

.PHONY : all clean

all : $(TARGET) $(TARGET_R)

clean :
	-rm $(OBJ_FILES) $(OBJ_FILES_R)
	-rm $(TARGET) $(TARGET_R)
