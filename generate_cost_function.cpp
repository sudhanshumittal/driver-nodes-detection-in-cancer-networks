#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;
void printError(string a){
	cout<<a;
	exit(1);
}
int main(int argc, char* argv[])
{
	if(argc < 2){
		printError("usage ./gen_cost\t bipartite_info_file\n");
	}
	char *bip_file = argv[1];
	return 0;
}