cpp = g++ -std=c++11 -O3
objects = process.o

compile : process
	$(cpp) mfqs.cpp $(objects) -o mfqs
	$(cpp) rts.cpp $(objects) -o rts
	$(cpp) whs.cpp $(objects) -o whs
process :
	$(cpp) -c process.cpp
clean :
	rm -rf *.o *.out mfqs rts whs

