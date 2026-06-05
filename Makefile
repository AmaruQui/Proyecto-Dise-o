CXX = g++
CXXFLAGS = -Wall -O3 -std=c++17

TARGET = main
OBJS = main.o Caso_1.o Caso_2.o Caso_3.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp Caso_1.hpp Caso_2.hpp Caso_3.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Caso_1.o: Caso_1.cpp Caso_1.hpp
	$(CXX) $(CXXFLAGS) -c Caso_1.cpp

Caso_2.o: Caso_2.cpp Caso_2.hpp
	$(CXX) $(CXXFLAGS) -c Caso_2.cpp

Caso_3.o: Caso_3.cpp Caso_3.hpp Caso_2.hpp
	$(CXX) $(CXXFLAGS) -c Caso_3.cpp

clean:
	rm -f *.o $(TARGET) $(TARGET).exe