#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cmath>
using namespace std;
void printError(string a){
	cout<<a;
	exit(1);
}
int main(int argc, char* argv[]){
	if(argc < 4){
		printError("usage ./get_driver\n1) nodes_assignment_normal\n2) nodes_assignment_disease \n3) mincost_solution.txt\n");
	}
	char *nodes_n_file = argv[1];
	char *nodes_t_file = argv[2];
	char *mincost_file = argv[3];
	int comp_n, comp_t;

	cout<<"enter number of connected components in normal and disesase graph\n";
	cin>>comp_n>>comp_t;
	bool comp_n_arr[comp_n], comp_t_arr[comp_t];

	ifstream fin;
	bool temp;
	int counter = 0;
	cout<<"reading min cost solution\n";
	fin.open(mincost_file);
	if (!fin)
		printError("could not open file mincost_files\n");
	while(fin.good() && counter != comp_n){
		fin>>temp;
		comp_n_arr[counter]= temp;
		counter ++;
	}
	if (counter != comp_n)
		printError("invalid component count in normal graph");
	counter = 0;	 
	while(fin.good() && counter != comp_t){
		fin>>temp;
		comp_t_arr[counter]= temp;
		counter ++;
	}
	fin.close();
	if (counter != comp_t)
		printError("invalid component count in disease graph");
	cout<<"reading nodes assignements files now to gfind out driver nodes\n";
	ifstream f1,f2;
	string temp_str;
	f1.open(nodes_n_file);
	if (!f1)
		printError("could not open file nodes_assignment_normal\n");
	//removing first two line
	getline(f1, temp_str);
	getline(f1, temp_str);

	f2.open(nodes_t_file);
	if (!f2)
		printError("could not open file nodes_assignment_disease\n");
	//removing first two line
	getline(f2, temp_str);
	getline(f2, temp_str);
	counter = 0;
	//cout<<comp_n_arr[2]<<" "<<comp_t_arr[2]<<endl;
	while(f1.good() && f2.good()){
		//assuming that both graphs have same set of nodes
		string name1, name2;
		bool val1, val2;
		int comp1, comp2;
		f1>>name1>>val1>>comp1;
		f2>>name2>>val2>>comp2;
		if (name1 != name2)
			printError("something wrong, names dont match");
		if ( abs(val1 - comp_n_arr[comp1-1]) -  abs(val2 - comp_t_arr[comp2-1]) != 0)
		{
			cout<<name1<<" "; 
			counter ++;
		}
	}
	cout <<"number of driver nodes = "<<counter<<endl;
	return 0;

}