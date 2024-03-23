GSL_PATH ?= /net/ens/renault/save/gsl-2.6/install
LDFLAGS = -lm -ldl -lgsl -lgslcblas -L$(GSL_PATH)/lib -L$(GSL_PATH)/lib64 -Wl,--rpath=${GSL_PATH}/lib
CFLAGS = -std=c99 -Wall -Wextra -fPIC -g3 -I$(GSL_PATH)/include -Isrc/ --coverage
SOURCES_PLAYER = ./src/interface.c ./src/set.c ./src/player.c ./src/graph.c
SOURCES_SERVER = ./src/interface.c ./src/set.c ./src/game.c ./src/graph.c
SOURCES_TEST = ./tst/test_interface.c
OBJECTS_PLAYER = $(SOURCES_PLAYER:.c=.o)
OBJECTS_SERVER = $(SOURCES_SERVER:.c=.o)
OBJECTS_TEST = $(SOURCES_TEST:.c=.o)

all: build test install

%.o: %.c
	gcc -c $(CFLAGS) $(LDFLAGS) $< -o $@

build: libplayer libplayer2 server

test: alltests

install: 
	cp server *.so alltests ./install/

libplayer: $(OBJECTS_PLAYER)
	$(CC) -shared -fPIC -o libplayer.so $(OBJECTS_PLAYER) $(CFLAGS) $(LDFLAGS)

libplayer2: $(OBJECTS_PLAYER)
	$(CC) -shared -fPIC -o libplayer2.so $(OBJECTS_PLAYER) $(CFLAGS) $(LDFLAGS)

server: $(OBJECTS_SERVER) ./src/server.o
	$(CC) -o server $(OBJECTS_SERVER) ./src/server.o $(CFLAGS) $(LDFLAGS) 

alltests: $(OBJECTS_TEST) $(OBJECTS_SERVER) ./src/player.o
	$(CC) -o alltests $(OBJECTS_TEST) $(OBJECTS_SERVER) ./src/player.o $(CFLAGS) $(LDFLAGS) -lgcov --coverage

# Règles pour lancer le programme

run:
	./install/server install/libplayer.so install/libplayer2.so  -m 5
gdbrun:
	gdb -ex=r --args ./install/server install/libplayer.so install/libplayer2.so -m 5

# Règles pour nettoyer le dossier 

remake : clean all

valgrind : 
	valgrind --leak-check=full --track-origins=yes -s ./install/server install/libplayer2.so install/libplayer.so -m 5

clean:
	@rm -f *~ src/*~
	@rm -f *~ tst/*~
	@rm -f *.so *.o */*.o
	@rm -f install/*.so install/server install/alltests
	@rm -f server alltests *.so
	@rm -f *.gcda *.gcno *.gcov
	@rm -f src/*.gcda tst/*.gcda src/*.gcno tst/*.gcno
	@rm -f *.aux *.log *.out *.toc *.pdf 

.PHONY: client install test clean
