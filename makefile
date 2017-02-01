CPP_FILES_TEST =  $(wildcard Tests/*.cpp)
CPP_FILES = $(wildcard src/*.cpp)

INC=-Iinclude
INC_THIRDPARTY = -Ithirdparty $(INC_TINYOBJLOADER) $(INC_CIMG) $(INC_SPDLOG) 


#ThirdParty Header Files
INC_TINYOBJLOADER=-Ithirdparty/tinyobjloader
INC_CIMG=-Ithirdparty/CImg 
INC_SPDLOG=-Ithirdparty/spdlog/include


CGAL =-lCGAL $(BOOST)
BOOST = -lboost_system -lboost_thread $(MPFR)
MPFR = -lmpfr $(GMP)
GMP  = -lgmp 
CIMG =  -lpthread -lX11 -lpng

CC = g++
CFLAGS = -std=c++11 -w -Wall -fexceptions -frounding-math -pthread -fPIC
OBJECTS = $(INC) $(INC_THIRDPARTY) $(CPP_FILES) $(CGAL) $(CIMG) 

all: Core

Core:
	$(CC) $(CFLAGS) $(OBJECTS) -o alphacut 
	

clean:
	rm *.o *.so *.a
