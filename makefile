########################################################
CC=g++
CFLAGS= -O2
EJS = agg-posicion agg-uniforme age-posicion age-uniforme
########################################################
OBJECTSP2_AGGP = src/agg-posicion.cpp
OBJECTSP2_AGGU = src/agg-uniforme.cpp
OBJECTSP2_AGEP = src/age-posicion.cpp
OBJECTSP2_AGEU = src/age-uniforme.cpp
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

.PHONY: clean
clean:
	rm -rf bin/*
