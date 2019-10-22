myserver:main.o loginserver.o db.o
	g++ main.o loginserver.o db.o -o myserver -lsqlite3 -lpthread
main.o:main.cpp
	g++ -c main.cpp -o main.o
loginserver.o:loginserver.cpp
	g++ -c loginserver.cpp -o loginserver.o
db.o:db.cpp
	g++ -c db.cpp -o db.o

