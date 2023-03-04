all:
	g++ -std=c++11 main.cpp SAT2.cpp square.cpp -o main
clean:
	rm main