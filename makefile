########################################################
CC=g++
CFLAGS= -O2
EJS = agg-posicion agg-uniforme age-posicion age-uniforme am-10-1.0 am-10-0.1 am-10-0.1mejor
########################################################
OBJECTSP2_AGGP = src/agg-posicion.cpp
OBJECTSP2_AGGU = src/agg-uniforme.cpp
OBJECTSP2_AGEP = src/age-posicion.cpp
OBJECTSP2_AGEU = src/age-uniforme.cpp
OBJECTSP2_AM1 = src/am-10-1.0.cpp
OBJECTSP2_AM01 = src/am-10-0.1.cpp
OBJECTSP2_AM01M = src/am-10-0.1mejor.cpp
########################################################

.PHONY: all
all: $(EJS)

agg-posicion: $(OBJECTSP2_AGGP)
	$(CC) $(CFLAGS) -o bin/agg-posicion $(OBJECTSP2_AGGP)

agg-uniforme: $(OBJECTSP2_AGGU)
	$(CC) $(CFLAGS) -o bin/agg-uniforme $(OBJECTSP2_AGGU)

age-posicion: $(OBJECTSP2_AGEP)
	$(CC) $(CFLAGS) -o bin/age-posicion $(OBJECTSP2_AGEP)

age-uniforme: $(OBJECTSP2_AGEU)
	$(CC) $(CFLAGS) -o bin/age-uniforme $(OBJECTSP2_AGEU)

am-10-1.0: $(OBJECTSP2_AM1)
	$(CC) $(CFLAGS) -o bin/am-10-1.0 $(OBJECTSP2_AM1)

am-10-0.1: $(OBJECTSP2_AM01)
	$(CC) $(CFLAGS) -o bin/am-10-0.1 $(OBJECTSP2_AM01)

am-10-0.1mejor: $(OBJECTSP2_AM01M)
	$(CC) $(CFLAGS) -o bin/am-10-0.1mejor $(OBJECTSP2_AM01M)

.PHONY: clean
clean:
	rm -rf bin/*
