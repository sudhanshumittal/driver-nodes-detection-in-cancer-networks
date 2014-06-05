/*
author: sudhanshu mittal
description : creates bipartite from given ppi and gene expression values (normal and disease)
and calculates costs for each pair of nodes in bipartite.
*/
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
#include <cassert> 
#include <algorithm>
using namespace std;

#define XOR 'x'
#define NXOR 'n'
#define TINY_VALUE 1e-20
#define DONT_CARE '-'
float PEARSON_FACTOR = 0.8;
float relaxation_coefficient = 1.0;
void printError(string a){
	/*ofstream fp;
	fp.open("log.txt", ios_base::app);
	fp<<a;
	fp.close();*/
	cout<<a;
	exit(1);
}
void printNodeMap(std::map <string, int> &nodes, char* outFile ){
//	int size = nodes.size();
	vector<string> nodesVec(nodes.size(), "");
	for(std::map<string, int>::iterator i = nodes.begin(); i!= nodes.end(); i++)
	{
		if (i->first == "")
			cout<<"error foudne empty protein";
		nodesVec[ (i->second)]= i->first;
	}
	ofstream fp;
	fp.open(outFile);
	for(int i(0); i<nodes.size(); i++){
		fp<<i+1<<" "<<nodesVec[i]<<endl;
	}
	fp.close();
}

void write_input_data(char* file, std::map<string, int> &nodes){
	ofstream fp;
	fp.open(file);
	fp<<"100\n200\n1\n0\n0\n"<<nodes.size()*2<<endl;
	for( int i(0); i<nodes.size()*2; i++)
		fp<<1<<" "<<0<<" "<<1<<endl;
	fp<<0.9<<endl<<"1.0";
	fp.close();

}

int min(int a, int b);
char getLabel (std::map<string,float>  &dataMap, string pro1, string pro2);
int getCorrelation(std::map< string,char > &dataMap, char* file, std::map<string, int> &nodes);
//	void createGraph(graph &g, char* ppi_file, char* edgeLabelFile, std::map< string,char> &dataMap);
void getNodes(std::map<string, string> &edges, char* );
void getNodes(std::map<string, int> &edges, char*, char* );
void getProtein(string s,string ans[2]);
void printMat(string A, char* outFile);
char getLabel (float val);

int main(int argc, char* argv[])
{
	if(argc < 6){
		printError("usage ./a.out\n\t1) nodeMapFile \n\t2) normal_correlation_file\n\t3) disease_correlation_file\n\t4) objective_definition_file\n5) ga_input_file");
	}
	cout<<"enter relaxation coefficent : ";
	cin>>relaxation_coefficient;
	cout<<relaxation_coefficient<<endl;
	cout<<"enter correlation coefficient threshold : ";
	cin>>PEARSON_FACTOR;
	cout<<PEARSON_FACTOR<<endl;
	char *nodeMapFile = argv[1];
	char *normal_correlation_file = argv[2];
	char *disease_correlation_file = argv[3];
	char *obj_def = argv[4];
	char *ga_input_file = argv[5]; 
	//datamap stores significant edges of ppi in the given graph  
	std::map< string , char > dataMap_normal, dataMap_disease;
	std::map<string, int> nodes;
	//---------------------------------------------------------------------------------------------------------
	cout<<"getting the correlation values for normal network ...\n";
	int interactions_normal = getCorrelation(dataMap_normal, normal_correlation_file,nodes);
	cout<<"getting the correlation values for disease network ...\n";
	int interactions_disease = getCorrelation(dataMap_disease, disease_correlation_file, nodes);
	cout<<"number of unique interacting nodes  ="<<nodes.size()<<endl; 
	cout<<"number of interactions in normal n/w= "<<interactions_normal<<endl;
	cout<<"number of interactions in disease n/w= "<<interactions_disease<<endl;
	printNodeMap(nodes, nodeMapFile);
	//---------------------------------------------------------------------------------------------------------
	stringstream A;
	cout<<"reading edges from normal graph...\n";
	for(std::map< string , char >::iterator i = dataMap_normal.begin(); i!= dataMap_normal.end(); i++){
		string pro[2] = {"",""};
		getProtein(i->first, pro); 
		int x = nodes[pro[0]];
		int y = nodes[pro[1]];
		if(i->second == XOR){
			/*+2xy -x -y;*/ 
			A <<"GN += 2.0*xbin["<<x<<"]*xbin["<<y<<"] - xbin["<<x<<"] - xbin["<<y<<"];\n";
		}	
		else if(i->second == NXOR){
			/*x+y-2xy -1;*/ 
			A <<"GN += -2.0*xbin["<<x<<"]*xbin["<<y<<"] + xbin["<<x<<"] +xbin["<<y<<"] -1.0;\n";
		}
		else 
			printError("error: unknown label found");
		
	}
	//---------------------------------------------------------------------------------------------------------
	cout<<"reading edges from disease network...\n";
	for(std::map< string , char >::iterator i = dataMap_disease.begin(); i!= dataMap_disease.end(); i++){
		string pro[2] = {"",""};
		getProtein(i->first, pro);
		int x = nodes[pro[0]]+nodes.size();
		int y = nodes[pro[1]]+nodes.size(); 
		if(i->second == XOR){
			/*+2xy -x -y;*/ 
			A <<"GT += 2.0*xbin["<<x<<"]*xbin["<<y<<"] - xbin["<<x<<"] - xbin["<<y<<"];\n";
		}
		else if(i->second == NXOR){
			/*x+y-2xy -1;*/ 
			A <<"GT += -2.0*xbin["<<x<<"]*xbin["<<y<<"] + xbin["<<x<<"] +xbin["<<y<<"] -1.0;\n";
		}
		else 
			printError("error: unknown label found");
	}
	//---------------------------------------------------------------------------------------------------------
	cout<<"reading cross edges ...\n";
	for(int i = 0; i<nodes.size(); i++){
		int x = i+1;
		int y = i+1+nodes.size();
			/*x+y-2xy -1;*/ 
			A <<"cross += -2.0*xbin["<<x<<"]*xbin["<<y<<"] + xbin["<<x<<"] +xbin["<<y<<"] -1.0;\n";
	}	
	//---------------------------------------------------------------------------------------------------------
	printMat(A.str(), obj_def);
	//printing binTest matrix
	write_input_data(ga_input_file, nodes);	
	cout<<"success!\n";
	return 0;
}
void getProtein(string s,string ans[2]){
	int proNo = 0;
	for( int i(0); i<s.size(); i++){
		if(s[i]== '|')
			proNo++;
		else
			//cout<<s[i];
			ans[proNo] += s[i]; 
	}
	//cout<<endl;
}


void printMat(string A, char* outFile){
	ofstream fout;
	fout.open(outFile);
	fout<<"# include <stdio.h>\n"
"		# include <stdlib.h>\n"
		"# include <math.h>\n"
		"# include \"global.h\"\n"
		"# include \"rand.h\"\n"
		"# define minflip\n"
		"#ifdef minflip\n"
		"void test_problem (double *xreal, double *xbin, int **gene, double *obj, double *constr)"
		"{"
	    "	double GN = 0.0,GT = 0.0, cross=0.0 ;";
		fout<<A;
		fout<<"obj[0] = GN+GT+cross;"
		    "return;}\n#endif";
	fout.close();
}
/*labels all interactions in PPI file as XOR/XNOR?DONT_CARE using correlation file and stores into into dataMap*/
int getCorrelation(std::map < string, char> &dataMap, char* correlation_file, std::map<string, int> &nodes)
{
	ifstream fcor;
	fcor.open(correlation_file);
	int i = nodes.size()-1;
	if(!fcor) 
		printError("could not open "+string(correlation_file)+"\n");
	int xor_count(0), nxor_count(0);
	while(fcor.good()){
		string pro1, pro2;
		float cor;
		fcor>>pro1>>pro2>>cor;
		if(!fcor.good())
			break;
		if (pro1==pro2){
			printError("self interaction found!\n");
		}
		char label = getLabel(cor);
		if(dataMap.count(pro1+"|"+pro2)+dataMap.count(pro2+"|"+pro1) > 0 )
			//cout<<"found duplicate! "<<pro1<<" "<<pro2<<"\n";
			continue;
		if(label == DONT_CARE)
		{	
			continue;
		}
		else if(label == XOR)
			xor_count++;
		else if(label == NXOR)
			nxor_count++;
		else
			printError("unknown lable found\n");
		dataMap[pro1+"|"+pro2] = label;
		if ( nodes.count(pro1) <= 0)
			nodes[pro1] = ++i;
		if ( nodes.count(pro2) <= 0)
			nodes[pro2] = ++i;
	}
	cout<<"number of positively correlated pairs in graph = "<<nxor_count<<endl;
	cout<<"number of negatively correlated pairs in graph  = "<<xor_count<<endl;
	cout<<"total significant interactions in graph  = "<<dataMap.size()<<endl;
	
	fcor.close();		
	return nxor_count+xor_count;
}
char getLabel (std::map<string, float > &correlation_Map, string pro1, string pro2){
//TODO check this shit	
	float val = correlation_Map[pro1+"|"+pro2];
	float val2 = correlation_Map[pro2+"|"+pro1];
	if (val != val2)
		cout<<"error found : correlation map inconsistent"<<endl;
	if(val >=PEARSON_FACTOR) return NXOR;
	else if (val <=-PEARSON_FACTOR) return XOR;
	else return DONT_CARE;
}

char getLabel (float val){
	if(val >=PEARSON_FACTOR) return NXOR;
	else if (val <=-PEARSON_FACTOR) return XOR;
	else return DONT_CARE;
}
