# for C++, replace CC (c compiler) with CXX (c++ compiler) which is used as default linker
CC=$(CXX)
CXXFLAGS=-fPIC -Wall -std=c++11 -O2 -g
LDFLAGS=-shared

LIBNAME=anomalydatagen
EXEC=datsa

LIBOBJS = ../src/data_generator.o ../src/event_data.o
OBJS = main.o function_vars/function_vars.o random_vars/random_vars.o sine/sine.o space_shuttle/space_shuttle.o speed_bump/speed_bump.o ecg/ecg.o nary/nary_anomaly.o step/step_anomaly.o exact/exact_anomaly.o

# "all" is name of the default target, running "make" without params would use it
all: lib$(LIBNAME).so $(EXEC)

lib$(LIBNAME).so: $(LIBOBJS)
	$(CC) ${LDFLAGS} -o $@ $^

install: lib$(LIBNAME).so
	cp lib$(LIBNAME).so /usr/local/lib
	ldconfig -v -n .

uninstall:
	rm -f /usr/local/lib/lib$(LIBNAME).so

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -L. -l$(LIBNAME) -lpthread

clean:
	rm -f *.so $(EXEC); rm -f ../src/*.o; find . -name "*.o" -type f -delete
