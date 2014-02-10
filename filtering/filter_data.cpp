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
	while(fp.good()){
		string pro1, pro2;
		float garbage;
		fp>>pro1>>pro2>>garbage;
		vector<string> pro1Processed = process(pro1);
		vector<string> pro2Processed = process(pro2);
		for ( int index = 0; index < pro1Processed.size(); index++ )
			edges[ pro1Processed[index] ] = false;
		for ( int index = 0; index < pro2Processed.size(); index++ )
			edges[ pro2Processed[index] ] = false;
				

		
	}
}

int filter(std::map<string, bool> &edges, char* genExFile, char* outputFile){
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
			if (proList[index] == "") continue;
			std::map<string, bool>::iterator it = edges.find( proList[index]);
			if( it == edges.end() || it->second == true) continue;
			it->second = true;
			count ++;
			fout<<proList[index]<<" "<<rest<<endl;
			//cout<<proList[index]<<" ";
		
		}

		
	}
	fp.close();
	fout.close();
	return count;
}
int main(int argc, char ** argv){
	if (argc <4){
		cout<<"usage: ./a.out ppi_file gene_expression_file filtered_output\n";
	}
	std::map<string, bool> edges;
	getNodes(edges, argv[1]);
	cout<<"number of nodes in ppi_file = " << edges.size()<<endl;
	int rec = filter(edges, argv[2], argv[3]);
	cout<<"number of nodes in filtered gene expression file = " << rec<<endl;
	cout<<"printing genes Not Found in gene expression data\n"; 
	for ( std::map<string, bool>::iterator i = edges.begin() ; i!=edges.end(); i++ ){
		if ( ! i->second )
			cout<<i->first<<endl;
	}
	return 0;
}
