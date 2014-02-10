#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <limits>
#include <cmath>
using namespace std;

#define PEARSON_FACTOR 0.7
#define XOR 'x'
#define NXOR 'n'
#define TINY_VALUE 1e-20
#define DONT_CARE '-'

class Node{
		public:
	int val;
	int component;
	Node(int v, int comp){
		val = v;
		component = comp;
	}
};

bool comparator(Node*a, Node*b){
	return true;
}
map<string, int> proteins;
int proCount= 0;
int get_normal_vertex_labels(char* file, std::map<string, Node*> &vertex_normal){
	ifstream fn;
	fn.open(file);
	if (!fn.good() ){
		cout<<"unable to open file "<<file<<endl;
		return 0;
	}
	int component_count;
	string protein, garbage;
	int label;
	int component;
	fn>>garbage>>component_count;
	while(fn.good()){
		fn>>protein>>label>>component;
		vertex_normal[protein] = new Node(label, component);
		proteins[protein] = proCount;
		proCount++;
	}
	fn.close();
	return component_count;
}
void allSol(vector<vector<string> >& comp, int n, vector<bool> s, vector<vector<bool> > &sol){
	if (n >= comp.size()){
			sol.push_back(s);
			//cout<<s<<endl;
			return;
	}
	else{
			{
				vector<bool> s1 = s;
				for (int i(0); i<comp[n].size(); i++) 
					if(s1.size()> proteins[comp[n][i]])
						s1[proteins[comp[n][i] ]] = true;
					else{
						cout<<"error found "<<s1.size()<<" "<<proteins[comp[n][i]];
						return;
					}
				allSol( comp, n+2, s1, sol );
			}
			{
			vector<bool> s2 = s;
			for (int i(0); i<comp[n+1].size(); i++) 
				if(s2.size()> proteins[comp[n+1][i]])
					s2[proteins[comp[n+1][i] ]] = true;
				else{
					cout<<"error found"<<s2.size()<<" "<<proteins[comp[n+1][i]];

				}
			
			allSol( comp, n+2, s2, sol );
			}
	}
}
int get_tumor_vertex_labels(char* file, std::map<string, Node*> &vertex_tumor){
	ifstream ft;
	ft.open(file);
	if (!ft.good() ){
		cout<<"unable to open file "<<file<<endl;
		return 0;
	}
	int component_count;
	string protein, garbage;
	int label;
	int component;
	ft>>garbage>>component_count;
	while(ft.good()){
		ft>>protein>>label>>component;
		vertex_tumor[protein] = new Node(label, component);
	}
	ft.close();
	return component_count;
}

/*int get_flips(std::map<string,  Node*> &vertex_normal, std::map<string,  Node*> &vertex_tumor){
	int flips = 0;
	for( std::map<string,  int>::iterator i = vertex_normal.begin(); i != vertex_normal.end(); i++){
		if (i->second == 2 ||   vertex_tumor[i->first] == 2)
			continue;
		if (i->second != vertex_tumor[i->first]){
			cout<<i->first<<" "<<i->second<<vertex_tumor[i->first]<<endl;
			flips ++;
		}
	}
	return flips;
}*/
int main(int argc, char* argv[]){
	if (argc <3){
		cout<<"USAGE: ./a.out normal_vertex_label_file tumor_vertex_label_file \n";
	}


	map<string, Node* > vertex_normal;	
	map<string, Node* > vertex_tumor;
	//sort(vertex_normal.begin(), vertex_normal.end(), comparator);	
	int comp_count_n = 2*get_normal_vertex_labels(argv[1], vertex_normal);
	int comp_count_t = 2*get_tumor_vertex_labels(argv[2], vertex_tumor);
	cout<<comp_count_n<<endl<<comp_count_t<<endl;
	vector<string > temp ;
	vector< vector<string> > comp_n(comp_count_n, temp);
	vector< vector<string> > comp_t(comp_count_t, temp);
	int count = 0;
	for( map<string, Node*>::iterator i= vertex_normal.begin();
					i != vertex_normal.end(); i++){
		//cout<<i->first<<"\t"<<i->second->val<<"\t"<<i->second->component<<endl;
		comp_n[i->second->component].push_back(i->first); 	
		proteins[i->first] = count++;
	}
	for( map<string, Node*>::iterator i= vertex_tumor.begin();
					i != vertex_tumor.end(); i++){
		//cout<<i->first<<"\t"<<i->second->val<<"\t"<<i->second->component<<endl;
		comp_t[i->second->component].push_back(i->first); 	
	}
	//find all possible assignments
	//normal case
	vector< vector<bool> > solN;
	vector<bool> solString(proCount, false);
	//cout<<solString<<endl;
	allSol(comp_n, 0, solString, solN);
	ofstream fp;
	//sort(solN.begin(), solN.end());
	
	fp.open("allSolutionsN.txt");
	for ( int i = 0 ; i< solN.size() ; i++)
	{	for( int j = 0; j< solN[i].size(); j++)
			fp<<solN[i][j];
		fp<<endl;
	}
	fp.close();
	
	//cancer case
	//cout<<"minimum number of flips = "<<get_flips(vertex_normal, vertex_tumor, comp_n, comp_t)<<endl;
	return 0;
}
