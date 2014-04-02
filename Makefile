RM = rm -f

CCX = clang++
CFLAGS = -O2 -std=c++11 -Ijsoncpp/include

SRCDIR = src
OBJDIR = obj
BINDIR = bin

FILES = Directed_weighted_graph.h Bellman_Ford_algorithms.h

DEPS = $(FILES:%.h=$(SRCDIR)/%.h)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
EXEC = $(BINDIR)/driver

all: $(EXEC)

$(EXEC): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CCX) $(CFLAGS) $^ -o $@ 

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	@mkdir -p $(OBJDIR)
	$(CCX) $(CFLAGS) -c $< -o $@ 

clean:
	$(RM) -r bin
	$(RM) -r obj

.PHONY: all tar clean
