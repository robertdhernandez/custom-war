CXXFLAGS=-std=c++0x
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(patsubst src/%.cpp,obj/%.o,$(SOURCES))
EXECUTABLE=custom-war

all: $(SOURCES) $(EXECUTABLE)

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)

$(OBJECTS): obj/%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
