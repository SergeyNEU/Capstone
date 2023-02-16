sensehat:sensehat.o ICM20948.o
	g++ -Wall -o sensehat sensehat.o ICM20948.o -lwiringPi -lm -O3 
sensehat.o: sensehat.cpp ICM20948.h
	g++ -Wall -c sensehat.cpp -lwiringPi -lm -O3 
ICM20948.o: ICM20948.cpp ICM20948.h
	g++ -Wall -c ICM20948.cpp -lwiringPi -lm -O3 
clean:
	rm sensehat.o ICM20948.o sensehat