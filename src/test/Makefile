
#hard copy here the include path created by qmake in the src directory for now...
INCPATH       = 

all:
	g++ -std=c++17 -DQT_NO_VERSION_TAGGING $(INCPATH) main.cpp test.cpp persistence/pmanager_test.cpp util/eventutil_test.cpp util/dateutil_test.cpp ../persistence/pmanager.cpp ../util/eventutil.cpp ../util/dateutil.cpp -o test -lsqlite3

clean:
	rm test
