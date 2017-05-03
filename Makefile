CXX=g++ -std=c++11
includes=-I ./log_info
.PHONY: clean memory.manager

memory.manager: memory.manager.exe
	./$@.exe

memory.manager.exe: memory.manager.h memory.manager.demo.h memory.manager.main.cpp
	$(CXX) $(includes) -o $@ memory.manager.main.cpp

clean:
	rm -f *.exe *.o
