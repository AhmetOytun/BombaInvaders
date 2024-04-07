build:
	c++ -std=c++11 -I/opt/homebrew/Cellar/raylib/5.0/include -L/opt/homebrew/Cellar/raylib/5.0/lib -o main src/main.cpp src/obstacle.cpp src/block.cpp src/spaceship.cpp src/laser.cpp src/alien.cpp src/mysteryship.cpp src/game.cpp -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -lraylib 
run:
	./main