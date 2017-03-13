all:
	g++ main.cpp YYDebug.cpp Gwd376Linux.cpp InputCmdLinux.cpp -o server -lpthread
		
arm:
	arm-linux-gcc main.cpp YYDebug.cpp Gwd376Linux.cpp InputCmdLinux.cpp -o serverARM -lpthread
		
clean:
	rm -f server client s c *.o serverARM 
.PHONY : all clean
