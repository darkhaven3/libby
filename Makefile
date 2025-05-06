EXECUTABLE=libby.exe
DEBUGNAME=libby-debug.exe
 
CC="C:\msys64\mingw32\bin\g++.exe"
CCFLAGS=-O3 -fdata-sections -ffunction-sections -Wl,--gc-sections
LDFLAGS=-static -lmingw32
 
src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)
 
all: myprog
 
myprog: $(obj)
	$(CC) $(CCFLAGS) -o $(EXECUTABLE) $^ $(LDFLAGS)
	cp $(EXECUTABLE) ./bin/$(EXECUTABLE)
	cp $(EXECUTABLE) ./bin/$(DEBUGNAME)
	cp $(obj) ./bin/
	strip -s ./bin/$(EXECUTABLE)
	rm -rf $(EXECUTABLE)
	rm -rf $(obj)
 
.PHONY: clean
clean:
	rm ./bin/$(EXECUTABLE) ./bin/$(DEBUGNAME) ./bin/*.o