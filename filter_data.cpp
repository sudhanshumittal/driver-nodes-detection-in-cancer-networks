/*
Author: Sudhanshu Mittal
Description: removes unwanted proteins from gene Expression data and ppi.

*/
#include <iostream>
#include <map>
#include <fstream>
#include <cstdlib>
#include <string>         // std::string
#include <locale>         // std::locale, std::toupper
#include <vector>
#include <sstream>
using namespace std;

vector<string> process(string line){
	vector<string> v;
	
	if (line[0] >='0' && line[0] <='9') return v;
	std::locale loc;
	string s = "";
	for (std::string::size_type i=0; i<line.length(); ++i)
	{
		if  ( line[i] != '/' ) 
			s+= std::toupper(line[i],loc);
		else 
			s+=' ';
	}
	stringstream ss;
	ss<<s;
	while( ss.good()){
		string temp;
		ss>>temp;
		v.push_back(temp);
	}
	return v;
} 
void getNodes(map<string, bool> &edges, char* ppi_file){
	ifstream fp;
	fp.open(ppi_file);
	if(!fp){
		cout<<"could not open ppi_file";
		exit(1);
	}
	int invalid=0;
	while(fp.good()){
		string pro1, pro2;
		float confidence;
		fp>>pro1>>pro2>>confidence;
		/*
			vector<string> pro1Processed = process(pro1);
			vector<string> pro2Processed = process(pro2);
			for ( int index = 0; index < pro1Processed.size(); index++ )
				edges[ pro1Processed[index] ] = false;
			for ( int index = 0; index < pro2Processed.size(); index++ )
				edges[ pro2Processed[index] ] = false;
		*/
		if (pro1[2]== '/' || pro2[2] == '/'){
			//cout<<"invalid protein pair"<<pro1<<" "<<pro2<<endl;
			invalid++;
			continue;
		}
		edges[pro1] = false;
		edges[pro2] = false;
	}
	cout<<"invalid edges = "<<invalid;
	
}

int filter(std::map<string, bool> &edges, char* genExFile, char* outputFile){
	/*remove gene from expression file if it is not in ppi*/
	cout<<"filtering...\n";
	int count = 0;
	ifstream fp;
	//cout<<genExFile<<" "<<outputFile;
	fp.open(genExFile);
	if (!fp){
		cout<<"could not open "<<genExFile<<endl ;
		exit(1);
	}
	ofstream fout;
	fout.open(outputFile);
	if (!fout){
		cout<<"could not open "<<outputFile<<endl ;
		exit(1);
	}
	string line, rest;
	while( fp.good() ){
		fp>>line;
		//cout<<line;
		getline(fp, rest);
		//cout<<rest<< " "  ;
		//continue;
		//string l = "";
    	vector<string> proList = process(line);
    	for ( int index = 0; index < proList.size(); index++ )
		{
			if (proList[index] == "") 
				continue;
			std::map<string, bool>::iterator it = edges.find( proList[index]);
			if( it == edges.end() || it->second == true) 
				continue;
			it->second = true;
			count ++;
			fout<<proList[index]<<" "<<rest<<endl;
			
		}

		
	}
	fp.close();
	fout.close();
	return count;
}
void filter_ppi(char* ppi_input_file, char* ppi_output_file, std::map<string, bool> &edges_n, std::map<string, bool> &edges_t){
	ifstream fp;
	ofstream fout;
	fp.open(ppi_input_file);
	fout.open(ppi_output_file);
	if(!fp){
		cout<<"could not open ppi_input_file";
		exit(1);
	}
	if(!fout){
		cout<<"could not open ppi_output_file";
		exit(1);
	}
	
	while(fp.good()){
		string pro1, pro2;
		float confidence;
		fp>>pro1>>pro2>>confidence;
		if (!( edges_n[pro1] == false || edges_n[pro2] == false || edges_t[pro1] == false ||edges_t[pro2] == false)){
			fout<<pro1<<" "<<pro2<<" "<<confidence<<endl;
		}
	}
}
int main(int argc, char ** argv){
	if (argc <7){
		cout<<"usage: ./a.out 1) ppi_file\n 2) gene_expression_file_normal \n3) gene_expression_file_disease\n 4) filtered_output_normal_file\n5) filtered_output_disease_file\n6) ppi_output_file\n";
		exit(1);
	}
	char *ppi_file = argv[1];
	char *gene_expression_file_normal=argv[2];
	char *gene_expression_file_disease =argv[3];
	char *filtered_output_normal_file=argv[4];
	char *filtered_output_disease_file=argv[5];
	char* ppi_output_file = argv[6];
	
	std::map<string, bool> edges_n;
	getNodes(edges_n, ppi_file);
	cout<<"number of nodes in ppi_file = " << edges_n.size()<<endl;
	//////////////////////////////////////////////////////////////////////////////
	cout<<endl<<"filtering normal gene expression data ...\n";

	int rec = filter(edges_n, gene_expression_file_normal, filtered_output_normal_file);
	cout<<"number of nodes not in normal filtered gene expression file = " << edges_n.size() - rec<<endl;
	cout<<"printing genes not found in normal gene expression data\n"; 
	for ( std::map<string, bool>::iterator i = edges_n.begin() ; i!=edges_n.end(); i++ ){
		if ( ! i->second )
			cout<<i->first<<" ";
	}
	cout<<endl;
	//////////////////////////////////////////////////////////////////////////////
	cout<<endl<<"filtering disease gene expression data...\n";

	std::map<string, bool> edges_t;
	getNodes(edges_t, ppi_file);
	int rec_t = filter(edges_t, gene_expression_file_disease, filtered_output_disease_file);
	cout<<"number of nodes not in diease filtered gene expression file = " << edges_t.size() - rec_t<<endl;

	cout<<"printing genes not found in disease gene expression data\n"; 
	for ( std::map<string, bool>::iterator i = edges_t.begin() ; i!=edges_t.end(); i++ ){
		if ( ! i->second )
			cout<<i->first<<" ";
	}
	cout<<endl<<"filtering ppi now...\n";
	////////////////////////////////// filter ppi as well
	filter_ppi(ppi_file,ppi_output_file, edges_n, edges_t);
	return 0;
}
