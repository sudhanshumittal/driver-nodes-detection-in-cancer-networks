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

#define PEARSON_FACTOR 0.8
#define XOR 'x'
#define NXOR 'n'
#define TINY_VALUE 1e-20
#define DONT_CARE '-'

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
		nodesVec[ (i->second)-1]= i->first;
	}
	ofstream fp;
	fp.open(outFile);
	for(int i(0); i<nodes.size(); i++){
		fp<<i+1<<" "<<nodesVec[i]<<endl;
	}
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
	if(argc < 5){
		printError("usage ./a.out\n\t1) nodeMapFile \n\t2) normal_correlation_file\n\t3) disease_correlation_file\n\t4) max_sat_output\n");
	}
	char *nodeMapFile = argv[1];
	char *normal_correlation_file = argv[2];
	char *disease_correlation_file = argv[3];
	char *max_sat_output = argv[4];
	//datamap stores significant edges of ppi in the given graph  
	std::map< string , char > dataMap_normal, dataMap_disease;
	std::map<string, int> nodes;
	//getNodes(nodes, normal_correlation_file, disease_correlation_file);
	//normal and disease graphs
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"getting the correlation values for normal network ...\n";
	int interactions_normal = getCorrelation(dataMap_normal, normal_correlation_file,nodes);
	cout<<"getting the correlation values for disease network ...\n";
	int interactions_disease = getCorrelation(dataMap_disease, disease_correlation_file, nodes);
	cout<<"number of unique interacting nodes  ="<<nodes.size()<<endl; 
	cout<<"number of interactions in normal n/w= "<<interactions_normal<<endl;
	cout<<"number of interactions in disease n/w= "<<interactions_disease<<endl;
	printNodeMap(nodes, nodeMapFile);
	/*
	// Ax <= b
	///assert(dataMap_normal.size() == dataMap_disease.size());
	int height = interactions_normal+interactions_disease+ nodes.size();

	int width = 2*nodes.size()+height;
	cout<<height<<" "<<width<<endl;
	//vector< vector<int> > A(height, vector<int> (width, 0));
	vector<int> b(height,0);
	*/
	//for all edges add a constraint
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	stringstream A;
	A<<"function f = objfun(x)\n \% OBJFUN Objective function.\n \% Documentation example.\n\%   Copyright 1990-2008 The MathWorks, Inc.\n\%   $Revision: 1.1.6.3 $  $Date: 2008/02/29 12:47:47 $\n\%   Author: Sudhanshu Mittal\nf = 0;\n";
	cout<<"reading edges from normal graph...\n";
	int row = 0;
	//cout<<nodes.size()<<" "<<dataMap_normal.size()<<endl;
	
	for(std::map< string , char >::iterator i = dataMap_normal.begin(); i!= dataMap_normal.end(); i++){
		row++;
		string pro[2] = {"",""};
		getProtein(i->first, pro); 
	/* 		if(nodes.find(pro[0]) == nodes.end() || nodes.find(pro[1]) == nodes.end() ){
			cout<<"not found";
			cout<<pro[0]<<" "<<pro[1]<<endl;
			return 1;
		}   	
	*/    	
		int x = nodes[pro[0]];
		int y = nodes[pro[1]];
		if(i->second == XOR){
			/*+2xy -x -y;*/ 
			A <<"f = f + 2*x("<<x<<")*x("<<y<<") -x("<<x<<") -x("<<y<<");\n";
		}
		else if(i->second == NXOR){
			/*-2xy -1;*/ 
			A <<"f = f+x("<<x<<")+x("<<y<<") - 2*x("<<x<<")*x("<<y<<") -1;\n";
		}
		else 
			printError("error: unknown label found");
		
	}
	cout<<"rows = "<<row<<endl;
	cout<<"reading edges from disease network...\n";
	for(std::map< string , char >::iterator i = dataMap_disease.begin(); i!= dataMap_disease.end(); i++){
		string pro[2] = {"",""};
		getProtein(i->first, pro);
		int x = nodes[pro[0]]+nodes.size();
		int y = nodes[pro[1]]+nodes.size(); 
		if(i->second == XOR){
			A <<"f = f + 2*x("<<x<<")*x("<<y<<") -x("<<x<<") -x("<<y<<");\n";
		}
		else if(i->second == NXOR){
			A <<"f = f+x("<<x<<")+x("<<y<<") - 2*x("<<x<<")*x("<<y<<") -1;\n";
		}
	}
	cout<<"rows = "<<row<<endl;
	/////////////////////////////////////////////////////////////////
	cout<<"reading cross edges ...\n";
	//cout<<nodes.size()<<" "<<dataMap_normal.size()<<endl;
	for(int i = 0; i<nodes.size(); i++){
		int x = i+1;
		int y = i+1+nodes.size();
			A <<"f = f+x("<<x<<")+x("<<y<<") - 2*x("<<x<<")*x("<<y<<") -1;\n";
	}
	printMat(A.str(), max_sat_output);
	//printing b matrix
	
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


void getNodes (std::map<string, int> &nodes,char* normal_correlation_file,char* disease_correlation_file){
	ifstream fp;
	fp.open(normal_correlation_file);
	if(!fp){
		printError("could not open normal_correlation_file");
	}
	int i = 0; //should always be zero because matlab indices start with 0
	while(fp.good()){
		string pro1, pro2;
		float correlation;
		
		fp>>pro1>>pro2>>correlation;
		if (!fp.good())
			break;
		if ( abs(correlation) < PEARSON_FACTOR ){
			
			continue;
		}
		if ( nodes.count(pro1) <= 0)
			nodes[pro1] = ++i;
		if ( nodes.count(pro2) <= 0)
			nodes[pro2] = ++i;
		
	}
	fp.close();
	fp.open(disease_correlation_file);
	if(!fp){
		printError("could not open disease_correlation_file");
	}
	while(fp.good()){
		string pro1, pro2;
		float correlation;
		fp>>pro1>>pro2>>correlation;
		if (!fp.good())
			break;
		if ( abs(correlation) < PEARSON_FACTOR )
			continue;
		if ( nodes.count(pro1) <= 0)
			nodes[pro1] = ++i;
		if ( nodes.count(pro2) <= 0)
			nodes[pro2] = ++i;
		
	}
	fp.close();
	
}
void printMat(string A, char* outFile){
	ofstream fout;
	fout.open(outFile);
	fout<<A;
	fout.close();
}
void getNodes (std::map<string, string> &edges, char* ppi_file){
	ifstream fp;
	fp.open(ppi_file);
	if(!fp){
		printError("could not open ppi_file");
	}
	while(fp.good()){
		string pro1, pro2;
		float garbage;
		
		fp>>pro1>>pro2>>garbage;
		if (pro1 == "" || pro2 == "")
			continue;
		if(pro1[2] ==  '/' || pro2[2] == '/' )
			continue;
		if( (pro1[0] >= '0' && pro1[0] <= '9') || (pro2[0] >= '0' && pro2[0] <= '9') )
			continue;
		//cout<<pro1<<"\t"<<pro2<<endl;
		edges[pro1] = pro2;
		edges[pro2] = pro1;
		//fp>>pro1;		
	}
}
/*labels all interactions in PPI file as XOR/XNOR?DONT_CARE using correlation file and stores into into dataMap*/
int getCorrelation(std::map < string, char> &dataMap, char* correlation_file, std::map<string, int> &nodes)
{
	ifstream fcor;
	fcor.open(correlation_file);
	int i = nodes.size();
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
