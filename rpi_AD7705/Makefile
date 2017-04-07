LIBS    = -lbcm2835 -lrt

all: ad7705_test

ad7705_test: ad7705_test.o gz_clk.o gpio-sysfs.o
	gcc -o ad7705_test gpio-sysfs.o ad7705_test.o gz_clk.o $(LIBS)

ad7705_test.o: ad7705_test.cpp
	gcc -c ad7705_test.cpp

gz_clk.o: gz_clk.cpp gz_clk.h
	gcc -c gz_clk.cpp

gpio-sysfs.o: gpio-sysfs.cpp gpio-sysfs.h
	gcc -c gpio-sysfs.cpp

clean:
	rm -f ad7705_test ad7705_test.o gz_clk.o *~ gpio-sysfs.o
