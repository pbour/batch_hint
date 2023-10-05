CC      = g++
CFLAGS  = -O3 -mavx -march=native -std=c++14 -w -fopenmp
LDFLAGS =

SOURCES = utils.cpp containers/relation.cpp containers/offsets_templates.cpp containers/offsets.cpp indices/hierarchicalindex.cpp indices/hint_m_subs+sort+ss+cm.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: qbatch


qbatch: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) utils.o containers/relation.o containers/offsets_templates.o containers/offsets.o indices/hierarchicalindex.o indices/hint_m_subs+sort+ss+cm.o main_selection.cpp -o query_batch.exec $(LDADD)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf utils.o
	rm -rf containers/*.o
	rm -rf indices/*.o
	rm -rf query_batch.exec
