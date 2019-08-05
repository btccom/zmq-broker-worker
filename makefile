CXX = g++
CXXFLAGS += -std=c++11
LDFLAGS += -lzmq -lpthread 
INCLUDES += -I. -I ./include

all: broker worker

broker: taskbroker.cc
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(INCLUDES)

worker: taskworker.cc
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(INCLUDES)

clean:
	rm -f *.o broker worker