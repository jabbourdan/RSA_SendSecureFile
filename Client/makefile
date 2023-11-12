Client: Client.o EncryptionHandler.o FilesHandler.o NetworkHandler.o Protocol.o RequestsGenerator.o StringHandler.o
	g++ -g -ansi -std=c++11 -I ../include  Client.o EncryptionHandler.o FilesHandler.o NetworkHandler.o Protocol.o RequestsGenerator.o StringHandler.o -lcryptopp


Client.o: Client.cpp constants.h
	g++ -c -Wall -ansi -pedantic -std=c++11 -I ../include Client.cpp -o Client.o


EncryptionHandler.o: EncryptionHandler.cpp EncryptionHandler.h constants.h
	g++ -c -Wall -ansi -pedantic -std=c++11 -I ../include  EncryptionHandler.cpp -o EncryptionHandler.o 

FilesHandler.o: FilesHandler.cpp FilesHandler.h constants.h
	g++ -c -Wall -ansi -pedantic -std=c++11 -I ../include  FilesHandler.cpp -o FilesHandler.o 
	
NetworkHandler.o: NetworkHandler.cpp NetworkHandler.h constants.h
	g++ -c -Wall -ansi -pedantic -std=c++11 -I ../include  NetworkHandler.cpp -o NetworkHandler.o 

Protocol.o: Protocol.cpp Protocol.h constants.h
	g++ -c -Wall -ansi -pedantic -std=c++11 -I ../include  Protocol.cpp -o Protocol.o 
	
RequestsGenerator.o: RequestsGenerator.cpp RequestsGenerator.h constants.h
	g++ -c -Wall -ansi -pedantic -std=c++11 -I ../include  RequestsGenerator.cpp -o RequestsGenerator.o 
	
StringHandler.o: StringHandler.cpp StringHandler.h constants.h
	g++ -c -Wall -ansi -pedantic -std=c++11 -I ../include  StringHandler.cpp -o StringHandler.o
	
