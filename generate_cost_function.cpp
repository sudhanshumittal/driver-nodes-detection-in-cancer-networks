#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;
void printError(string a){
	cout<<a;
	exit(1);
}
int main(int argc, char* argv[])
{
	if(argc < 3){
		printError("usage ./gen_cost\t cost_fuunction_output\t bipartite_info_file\n");
	}
	char *out_file = argv[1];
	char *bip_file = argv[2];
	string comp1, cost_eq, cost_ueq, s;
	int comp2; 
	string output;
	ifstream fin ;
	ofstream fout;
	fin.open(bip_file);
	getline(fin,s);
	output = "function score = cost(x)\n\tscore =0;\n";
	stringstream ss;
	while(fin.good()){
		ss.str("");
		fin	>>comp1>> comp2>> cost_ueq>> cost_eq;
		ss<<comp2+612; //hardcoded 612 = no of  components in Gn
		//cout<<ss.str()<<endl;
		output += "\tscore = score + (x("+comp1+") + x("+ss.str()+") - 2*"+"x("+comp1+") * "+"x("+ss.str()+"))*("+cost_ueq+"-"+cost_eq+") + "+cost_eq+";\n";
	}
	fout.open(out_file);
	fout<<output;
	fout.close();
	fin.close();
	return 0;
}