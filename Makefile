PANorama = $(HOME)/PANorama2.0
SRC=$(PANorama)/src
BIN=$(PANorama)/bin
LIB=$(PANorama)/libift
INCLUDE=$(PANorama)/include

FLAGS= -O3 -Wall -Wno-unused-result -fopenmp -pthread -std=gnu99 -pedantic
IFTLIB  = -L$(LIB) -lift-64bits -llapack -lblas -lm 
IFTINC  = -I$(INCLUDE) 

all:
	gcc $(FLAGS) $(SRC)/iftPanicleInfo.c $(SRC)/iftPANorama.c -o $(BIN)/iftPanicleInfo $(IFTINC) $(IFTLIB) ; \

	gcc $(FLAGS) $(SRC)/iftPanicleMeasures.c $(SRC)/iftPANorama.c -o $(BIN)/iftPanicleMeasures $(IFTINC) $(IFTLIB) ; \

	gcc $(FLAGS) $(SRC)/iftPanicleSkel.c $(SRC)/iftPANorama.c -o $(BIN)/iftPanicleSkel $(IFTINC) $(IFTLIB) ; \

	gcc $(FLAGS) $(SRC)/iftTempPanicleSkel.c $(SRC)/iftPANorama.c -o $(BIN)/iftTempPanicleSkel $(IFTINC) $(IFTLIB) ; \

	gcc $(FLAGS) $(SRC)/iftCorrectBranches.c $(SRC)/iftPANorama.c -o $(BIN)/iftCorrectBranches $(IFTINC) $(IFTLIB) ; \

clean:
	rm $(BIN)/iftPanicleSkel; rm $(BIN)/iftTempPanicleSkel; rm $(BIN)/iftPanicleInfo; rm $(BIN)/iftPanicleMeasures; rm $(BIN)/iftCorrectBranches;










