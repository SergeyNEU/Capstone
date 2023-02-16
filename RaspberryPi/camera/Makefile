camera:camera.o
	g++ -Wall -o camera camera.o -O3 `pkg-config --cflags --libs opencv4`
camera.o: camera.cpp
	g++ -Wall -c camera.cpp -O3 `pkg-config --cflags --libs opencv4`
clean:
	rm camera.o camera