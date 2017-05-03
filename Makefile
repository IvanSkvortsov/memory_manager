CXX=g++ -std=c++11 -I ./log_info
includes=-I ./log_info -I ./pointer_type -I ./container_type
.PHONY: clean memory.manager

memory.manager: memory.manager.exe
	./$@.exe

memory.manager.exe: memory.manager.h memory.manager.demo.h memory.manager.main.cpp $(mman_files)
	$(CXX) $(includes) -o $@ memory.manager.main.cpp

clean:
	rm -f *.exe *.o *.exe.stackdump
