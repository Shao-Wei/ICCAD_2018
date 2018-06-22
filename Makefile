CFLAGS = -g -Wall

WM: WMMain.o WMMgr.o
	@echo "Checking..."
	@echo "done."
	@g++ -o $@ WMMain.o WMMgr.o $(CFLAGS)

WMMain.o: WMMain.cpp WMMgr.h
	@g++ -c WMMain.cpp $(CFLAGS)

WMMgr.o: WMMgr.cpp WMMgr.h
	@g++ -c WMMgr.cpp $(CFLAGS)

clean:
	@echo "Removing..."	
	@rm -f WM*.o
