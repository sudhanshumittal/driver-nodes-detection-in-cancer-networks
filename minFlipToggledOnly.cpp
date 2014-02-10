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
void intersect( vector<string > &first, vector<string> &second, vector<string> &out){
	int a(0),b(0);
	if (first.size() ==0 or second.size() == 0 )
		return;
	while(a<first.size() && b<second.size()){
	//	cout<<a<<endl;
		if (first[a]<second[b])
			a++;
		else if (first[a]>second[b])
			b++;
		else {
			out.push_back(first[a]);
			a++;b++;
		}
	}
}
void minFlip( vector<string> &minVertex, vector<vector<string> > &comp_n, vector<vector<string> > &comp_t){
	for( int i(0); i<comp_n.size(); i+=2){
		//cout<<comp_n.size();
		sort(comp_n[i].begin(), comp_n[i].end());
		sort(comp_n[i+1].begin(), comp_n[i+1].end());
		sort(comp_t[i].begin(), comp_t[i].end());
		sort(comp_t[i+1].begin(), comp_t[i+1].end());
		vector<string> A ,B , X, Y;
		intersect(comp_n[i], comp_t[i], A);
		intersect(comp_n[i+1], comp_t[i+1], B);
		intersect(comp_n[i], comp_t[i+1], X);
		intersect(comp_n[i+1], comp_t[i], Y);
		cout<<A.size()<<" "<<B.size()<<" "<<X.size()<<" "<<Y.size()<<" "<<comp_n[i].size()<<endl;
		
		if ( A.size()+ X.size() != comp_n[i].size() )
		{
			cout<<"error1\n";
			return;
		}
		if ( B.size()+ Y.size() != comp_n[i+1].size() )
		{
			cout<<"error2\n";
			return;
		}
		if ( (A.size() + B.size() )< (X.size() + Y.size()) ){
			//a and b are important
			for( int j(0); j<A.size(); j++)
				minVertex.push_back(A[j]);
			for( int j(0); j<B.size(); j++)
				minVertex.push_back(B[j]);
		}
		else{
			for( int j(0); j<X.size(); j++)
				minVertex.push_back(X[j]);
			for( int j(0); j<Y.size(); j++)
				minVertex.push_back(Y[j]);
		}

	}
}
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
		//proteins[i->first] = count++;
	}
	for( map<string, Node*>::iterator i= vertex_tumor.begin();
					i != vertex_tumor.end(); i++){
		//cout<<i->first<<"\t"<<i->second->val<<"\t"<<i->second->component<<endl;
		comp_t[i->second->component].push_back(i->first); 	
	}
	/*
	for( int i(0); i<comp_n.size(); i++){
		cout<<"normal: ";
		for( int j(0); j<comp_n[i].size(); j++)
			cout<<comp_n[i][j]<<",";
		cout<<"\t cancer: ";
		for( int j(0); j<comp_t[i].size(); j++)
			cout<<comp_t[i][j]<<",";
		
		cout<<endl;
	}*/
	//find min flips
	vector<string> minVertex;
	minFlip(minVertex, comp_n, comp_t);
	for( int j(0); j<minVertex.size(); j++)
			cout<<minVertex[j]<<endl;
	/*
	vector<string> a,b,c;	
	a.push_back("hello");
	a.push_back("hi");
	a.push_back("hiya");
	b.push_back("amgio");
	b.push_back("hello");
	b.push_back("hiya");
	intersect(a,b,c);
	cout<<c[0]<<c[1]<<c.size();
	*/
	//cancer case
	//cout<<"minimum number of flips = "<<get_flips(vertex_normal, vertex_tumor, comp_n, comp_t)<<endl;
	return 0;
}
