IDIR=src
ODIR=obj

CXX=g++ -std=c++11
CXXFLAGS= -I $(IDIR) -l sqlite3 -pthread

_DEPS = property.h node.h networknode.h 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = source.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

networkmaker: $(OBJ)
	$(CXX) $^ -o $@ $(CXXFLAGS)

$(ODIR)/%.o: $(IDIR)/%.cpp $(DEPS)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.*