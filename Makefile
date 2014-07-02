all: filter correlation bipartite gen_cost
filter:
	g++ filter_data.cpp -o filter
correlation:
	g++ pearson.cpp -o correlation
bipartite:
	g++ create_bipartite.cpp -o bipartite
gen_cost:
	g++  generate_cost_function.cpp -o gen_cost
clean:	
	rm -rf filter correlation bipartite gen_cost
