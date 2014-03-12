#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <cstdlib>
#include <limits>
#include <cmath>
#include <cstring>
using namespace std;
void printError(string s){
	cout<<s;
	exit(0);
}
main(int argc, char* argv[]){
	if(argc < 5){
		printError("usage ./a.out vertex_labels_file_normal vertex_labels_file_tumor vertex_label_all_normal vertex_label_all_normal\n");
	} 
	/*
		component from vertex_N, vertex_T (toggled only)
		values of nodes from vertex_all (all edges)
		Assumption: vertex_N, vertex_T have nodes mentioned in the same sequence
	*/
	
	
	ifstream finN, finT;
	map<string, bool> flip_per_node;
	map<string, bool> value_per_node_N;
	
	finN.open(argv[3]);
	finT.open(argv[4]);
	string garbage;
	getline(finN, garbage);
	getline(finT, garbage);
	
	while(finN.good()){
		string protein, component;
		bool valN;  //component useless
		
		finN>>protein;

		finN>>valN;

		finN>>component;
		value_per_node_N[protein] = valN;	
	}
	while(finT.good()){
		
		string protein, component;
		bool valT;  //component useless
		
		finT>>protein;

		finT>>valT;
		finT>>component;
		if( value_per_node_N.find(protein)!= value_per_node_N.end() )
				flip_per_node[protein] = (value_per_node_N[protein] != valT); 
	
	}
	finN.close();
	finT.close();

	finN.open(argv[1]);
	finT.open(argv[2]);
	int number_of_components;
	finN>>garbage>>number_of_components;
	cout<<"number_of_components : "<<number_of_components<<endl;
	getline(finT, garbage);
	vector<int> flips_per_component( number_of_components, 0 );
	vector<int> unflips_per_component( number_of_components, 0 );
	
	while(finN.good()&& finT.good()){
		string protein,protein2, valN, valT; //valN, valT useless
		int component;

		finN>>protein;
		finT>>protein2;
		if(protein!= protein2)
				cout<<"error, proteins not mathing in N and T\n";
		finN>>valN;
		finT>>valT;

		finN>>component;
		finT>>component;
		
		if (flip_per_node.find(protein) == flip_per_node.end()){
			printError("Error: node "+protein+" not found in complete graph");
		}
		if( flip_per_node[protein] == false ){
			unflips_per_component[(component)/2 ] ++;
		}
		else{
			flips_per_component[(component)/2  ] ++;
		}
		if(component == 1 || component == 0){
			cout<<protein<<endl;
		}

	}
	finN.close();
	finT.close();
	cout<<"flipped\t unflipped\n";
	for(int i(0); i<number_of_components; i++){
		cout<<flips_per_component[i]<<"\t"<<unflips_per_component[i]<<endl;
	}
	cout<<"success!\n";
}
