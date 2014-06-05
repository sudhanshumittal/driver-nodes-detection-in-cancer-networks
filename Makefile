all: filter correlation bipartite
filter:
	g++ filter_data.cpp -o filter
correlation:
	g++ pearson.cpp -o correlation
bipartite:
	g++ create_bipartite.cpp -o bipartite
clean:	
	rm -rf filter correlation bipartite
