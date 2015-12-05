
all:
	g++ ReadNetlist.cpp NetlistGraph.cpp node.cpp main.cpp -o sta 

clean: 
	rm parser

