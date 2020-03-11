# Chess
SFML Chess game which use external chess engine

## Installation
To get SFML (2d graphics library) type:

```sudo apt-get install libsfml-dev```

To get source code and compile the project type:

```git clone https://github.com/0xbeba267/Chess.git```

```cd Chess```

```g++ -o Chess src/*.cpp -std=c++11 -lsfml-graphics -lsfml-window -lsfml-system```

If compiled sucessfully, type
```./Chess```
for start playing.

## Playing versus chess engine
To enable playing vs AI, user need to download and install external software.

To get the stockfish type:

```sudo apt-get install stockfish```

or get it from offical website https://stockfishchess.org/download/
