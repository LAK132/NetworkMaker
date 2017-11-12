IDIR=src
ODIR=obj

CXX=g++ -std=c++11
CXXFLAGS= -I$(IDIR) -pthread

_DEPS = json.hpp property.hpp property_temp.hpp node.hpp node_temp.hpp networknode.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = json.o property.o node.o networknode.o source.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

networkmaker: $(OBJ)
	$(CXX) $^ -o $@ $(CXXFLAGS)

$(ODIR)/%.o: $(IDIR)/%.cpp $(DEPS)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.*