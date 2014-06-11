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
using namespace std;

#define PEARSON_FACTOR 0.7
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
class node
{
	public:
	    class edge{
	        public:
	            char label_; //edge label
	            node * n; //pointer to the second node of the edge
				char label(){
	                return label_;
	            }
	            void setLabel(char a){
	                label_=a;
	            }

	    };
	private:
	    string name_;//name of node
	    int value;		// 0 for normal and 1 for cancer
	    				//is node 1=> on or 0=>off 2=>dont care
	   				 	//char label_; //edge label

	    std::map<string, edge> adj_;//adjacency list or list of edges
	    int  visited_;  //if node is visited(2)/in queue(1)/ unvisited(0) in bfs
	    typedef  std::map<string, edge>::iterator it; //iterator over adj_ type

	public:

	    int component;
	    node(string s){
	        name_ =s;
	        visited_= 0;
	        value = 2;
	    }
	    node(string s, char l){
	        name_ = s;
	        visited_= 0;
	        value = 2;
	    }
	    int val(){
	    	return value;
	    	 
	    }
	    void setVal( int a ){
	        value =a;
	    }
	    bool visited(){
	        return visited_== 2;
	    }
	    void visit(){
	        visited_ = 2;
	    }
	    bool pushed(){
	    	return visited_ == 1;
	    }
	    void push(){
	    	visited_=1;
	    }
	    void unvisit(){
	        visited_ = 0;
	    	value = 2; 
		}
	    string name(){
	        return name_;
	    }
	    void setName(string s){
	        name_ = s;
	    }
	    bool isNeighbour(string s){
	        return adj_.find(s) != adj_.end() ;
	    }
	    bool addNeighbour(string s, node *n, char label){
	        it i= adj_.find(s);
	        if(i == adj_.end() ) {
	            //new neighbour
	            edge e;
	            e.label_ = label;
	            e.n = n;
	            adj_[s]= e;
	        	return true;
	        }
			else {
				//reset edge
				adj_[s].label_ = label;
				return false;
			}
	    }
	    std::map<string, edge> getNeighbours(){
	        return adj_;
	    }
	    string printNeighbours(){
	        string s="";
	        for(it i = adj_.begin(); i != adj_.end(); i++)
	        {
	            s+="--";
	            s+=(i->second.label_);
	            s+="--";
	            s+=i->first;
	            //s+=", ";
	        }
	        return s;
	    }
	    char getLabel(string s2){
	    	/*return the label of the edge connecting this node to node s2*/
	    	if (isNeighbour(s2))
	    		return adj_[s2].label();
	    	else
	    	 	return DONT_CARE;
	    }
};

class graph
{
    public:
		std::map < string ,node* > nodes;
		typedef std::map<string ,node*>::iterator itNodes;
		int component_count ;
		float usatEdges; //number of unsatisfied edges

	 	graph(){
			component_count = 0;
			usatEdges = 0;
		}
		
		bool initBfs(int init){
			if (nodes.size() == 0 )
				printError("empty input graph");
            node* root = unvisitedNode(); //search for unlabeled node
			bool success = true;
    		while(root!=NULL)
    		{	
    			success = success && bfs(root, init);
	            root = unvisitedNode(); //search for unlabeled node
			}
	        return success;
	    }

	    bool bfs(node * root, int init){
			if ( root == NULL ) return true;
			component_count++;
			root->component = component_count;
			//cout<<"new root is "<<root->name()<<endl; 
			queue< node* > q;
	        root->setVal(init);
	        q.push(root);
	        while(!q.empty()){
	            
	            node* curr = q.front();
	            
	            q.pop();
	            //cout<<curr->name()<<" ";
	            std::map<string, node::edge> adj = curr->getNeighbours();
	            
	            for(std::map<string, node::edge>::iterator i = adj.begin(); i!=adj.end(); i++){
	                node::edge next = i->second;
	                //labeled edge and the node is dont_care then label it
	                
	                if( !next.n->visited() && !next.n->pushed() ){
	                	//label unvisited node
						int satisfying_value =  satisfy(curr->val(), next.label()); 
	                  	next.n->setVal(satisfying_value);
	                    next.n->component = root->component;
						q.push(next.n);
						next.n->push();
				
					}
	                else{
	                	//check satisfaction of back edge
	                 	//if some back edge is not getting satisfied count unsatisfied edge 
						int satisfyingVal = satisfy(curr->val(), next.label()) ;
						if(next.n->val() != satisfyingVal ){
							usatEdges+=0.5;
							cout<<curr->name()<<" "<<next.n->name()<<" unstatisfied\nroot is "<<root->name()<<endl;
						}
						
	                }
	                
	            }
	            curr->visit();
	            //cout<<curr->name();
	        }
	        //one tree from the forest completely labeled
	    	return true;
	    }
	    
	    node* unvisitedNode(){
	        itNodes i;
	        for(i = nodes.begin(); i != nodes.end(); i++)
	            if( !i->second->visited() ) return i->second;
	        return NULL;
	    }

	    //find satisfying solution to a pair of nodes given an edge for a label
	    int satisfy(int a, char label){
	        //if label = DONT_CARE then any value can be given. we denote this by 2 
			if ( a == 2 ) return 2;
			if(label == DONT_CARE) return 2;
	        else if (label == XOR) return (a==1)?0:1;
	        else return a;
	    }
	    void addNode(string s){
	        if (nodes.find(s)== nodes.end())
	        	nodes[s] = new node(s);
	    }
	    bool addEdge(string s1, string s2, char label){
	        if (s1 == s2) return false; /*no self loops*/
	        bool success = nodes[s1]->addNeighbour(s2, nodes[s2], label) && nodes[s2]->addNeighbour(s1, nodes[s1], label);
	    	return success;
	    }
	    void printNodes(char* file){
			ofstream fp;
	        fp.open(file);
			if( !fp ){
				printError("could not open output file for vertex labels");
			}
	        fp<<"number_of_components "<<component_count<<endl;
			fp<<"Node_name\tsat_assign.\tcomponent_no.\n";
			for(itNodes i = nodes.begin(); i != nodes.end(); i++){
				fp<<i->second->name()<<"\t"<<i->second->val()<<"\t"<<i->second->component<<endl;//"\t"<<i->second->val(1)<<endl;
			}
			fp.close();
		}
		void printGraph(char* file){
			ofstream fp;
	        fp.open(file);
			if( !fp ){
				printError("could not open output file for vertex labels");
			}
	        for(itNodes i = nodes.begin(); i != nodes.end(); i++){
				std::map<string, node::edge> adj = i->second->getNeighbours();
	        	fp<<i->first<<"\t";
	        	for(std::map<string, node::edge>::iterator j = adj.begin(); j!=adj.end(); j++){
	                node::edge next = j->second;
	                fp<<next.n->name()<<"\t";
    	   		}
	       		fp<<endl;
			}
	        fp.close();
		
		} 
};

int min(int a, int b);
char getLabel (std::map<string,float>  &dataMap, string pro1, string pro2);
void getCorrelation(std::map< string,char > &dataMap, char* file, char* ppi);
void createGraph(graph &g, char* ppi_file, char* edgeLabelFile, std::map< string,char> &dataMap);
void getNodes(std::map<string, string> &edges, char* );
char* filter_ppi(char* ppi_file, char* filtered_ppi , std::map< string,char> &dataMap_normal, std::map< string,char> &dataMap_disease);
	
//void create_component_graph(char* ppi_file, char* comp_file,  std::map< string ,char > &dataMap, std::map < string ,int > &nodeCCList);
//int component_counter;
/*
std::map < string ,int > find_connected_components_flip_only(char* ppi_file, char* correlation_file, char* isolated_nodes_file){
	cout<<"finding connected components in ppi graph with flipped/unchanged edges only...\n";
	//getting list of nodes from ppi file
	std::map<string, string> edges;
	getNodes(edges, ppi_file);
	std::map< string , float > dataMap;
	getCorrelation(dataMap, edges, correlation_file);
	graph g;
	char garbFile[] = "garbageFile.txt";
    createGraph(g, ppi_file, garbFile, dataMap);
	if(!g.initBfs(1)) { //for normal case
		cout<<"\nERROR: Normal graph unsatisfiable";
	}
	//get components for each node in flip-only graph
	std::map < string ,int > nodeCCList;
	for(graph::itNodes i = g.nodes.begin(); i != g.nodes.end(); i++ )
		nodeCCList[i->first] = i->second->component;
	
	cout<<"non-isolated components = "<<g.component_count<<endl;
	//assuming each isolated node as a new connected component add them too to nodeCCList
	ifstream fin ;
	fin.open(isolated_nodes_file);
	int counter = g.component_count;
	while(fin.good()){
		counter++;
		string protein;
		fin>>protein;
		nodeCCList[protein] = counter;
	}
	cout<<"total connected components(including isoalted nodes) = "<<counter<<endl;
	component_counter = counter;
	return nodeCCList;
	
}
*/
int main(int argc, char* argv[])
{
	if(argc < 9){
		/*printError("usage ./a.out correlation_file ppi_file_name edge_label_output_file 
		vertex_labels_file component_graph_file filtered_ppi_with_flipped_only_edges isolated_nodes_file\n");*/
		printError("usage ./a.out\n\t1) ppi_file_name\n\t2) normal_correlation_file\n\t3) disease_correlation_file\n\t4) edge_information_output_normal\n\t5) edge_information_output_disease\n\t6) node_assign_output_normal\n\t7) node_assign_output_disease \n\t8)bipartite_output");
	}
	
	char *ppi_file = argv[1];
	char *normal_correlation_file = argv[2];
	char *disease_correlation_file = argv[3];
	char *edge_information_output_normal = argv[4];
	char *edge_information_output_disease = argv[5];
	char *node_assign_output_normal = argv[6];
	char *node_assign_output_disease = argv[7];
	char *bipartite_output = argv[8];
	char filtered_ppi[] = "ppi/temp_ppi";
	//datamap stores significant edges of ppi in the given graph  
	std::map< string , char > dataMap_normal, dataMap_disease;
	//normal and disease graphs
	graph gn,gt;
       
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"getting the correlation values for normal network ...\n";
	getCorrelation(dataMap_normal, normal_correlation_file, ppi_file);
	cout<<"getting the correlation values for disease network ...\n";
	getCorrelation(dataMap_disease, disease_correlation_file, ppi_file);
	/*verify rest*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"removing useless edges from PPI (insignificant in both graphs)\n";
	ppi_file =  filter_ppi(ppi_file, filtered_ppi, dataMap_normal, dataMap_disease);
	cout<<"constructing normal graph...\n";
    createGraph(gn, ppi_file, edge_information_output_normal, dataMap_normal);
	cout<<"finding satisfactory assignment for the normal graph...\n";
	if(!gn.initBfs(1)) { //for normal case
		cout<<"\nERROR: Normal graph unsatisfiable";
		return 1;
	}
	cout<<"putting the satisfying assignment for normal network in file "<<node_assign_output_normal<<" ...\n";
	gn.printNodes(node_assign_output_normal);
	cout<<"number of unsatisfiable edges in normal graph = "<<gn.usatEdges<<endl; 
	cout<<"number of components in normal graph = "<<gn.component_count<<endl;	
	cout<<"putting the edge labels for normal network in file  "<<edge_information_output_normal<<" ...\n";
	//gn.printGraph(edge_information_output_normal);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"constructing disease graph...\n";
	createGraph(gt, ppi_file, edge_information_output_disease, dataMap_disease);
	cout<<"finding satisfactory assignment for the disease graph...\n";
	if(!gt.initBfs(1)) { //for disease case
		cout<<"\nERROR: disease graph unsatisfiable";
		return 1;
	}
	cout<<"putting the satisfying assignment for disease network in file "<<node_assign_output_disease<<" ...\n";
	gt.printNodes(node_assign_output_disease);
	cout<<"number of unsatisfiable edges in disease graph = "<<gt.usatEdges<<endl; 
	cout<<"number of components in disease graph = "<<gt.component_count<<endl;	
	cout<<"putting the edge labels for disease network in file  "<<edge_information_output_disease<<" ...\n";
	//gt.printGraph(edge_information_output_disease);
    
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"creating component graph...\n";
	int ***bipartite = new int**[gn.component_count];// cost_equal , 0-> unflipped nodes count,  cost_not_equal 1->flipped nodes count 
	for(int i(0); i<gn.component_count; i++)
	{
		bipartite[i] = new int*[gt.component_count]; 
		for(int j(0); 	j<gt.component_count; j++){
			bipartite[i][j] = new int[2];
			for(int k(0); k<2; k++)
				bipartite[i][j][k] = 0;
		}
		
	}
		cout<<"allocated space for bipartite\ncalculateing cost values...\n";
	for( graph::itNodes i= gn.nodes.begin(); i!=gn.nodes.end(); i++)
	{
		string pro = i->first;
		//cout<<pro<<" ";
		bool valN = i->second->val();
		int compN = i->second->component-1;
		graph::itNodes copyInDisease = gt.nodes.find(pro);
		if (copyInDisease == gt.nodes.end())
			//continue;
			printError("protien present in normal but absent in disease");
		bool valT = copyInDisease->second->val();
		int compT = copyInDisease->second->component-1;
		if (compN>=gn.component_count || compT>=gt.component_count)
		{
			cout<<"compN="<<compN<<" comptT="<<compT<<" gn.component_count="<<gn.component_count<<" gt.component_count= "<<gt.component_count<<endl;
			printError("compN or compN too large");
		}
		if(valN== valT)
			bipartite[compN][compT][0]++;
		else
			bipartite[compN][compT][1]++;
		  	  
	}
	cout<<"done\nprinting cost values to file "<<bipartite_output<<endl;
	ofstream fbp;
	fbp.open(bipartite_output);
	fbp<<"component_id_U\tcomponent_V\tcost_equal\tcost_not_equal\n";
	for(int i(0); i<gn.component_count; i++)
	{	
		for(int j(0); j<gt.component_count; j++)
		{
			if(!(bipartite[i][j][0]<=0 && bipartite[i][j][1]<=0))
				fbp<<i+1<<"\t"<<j+1<<"_\t"<<bipartite[i][j][0]<<"\t"<<bipartite[i][j][1]<<endl;
		}
	}
	fbp.close();			
    cout<<"success!\n";
	
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
void getCorrelation(std::map < string, char> &dataMap, char* correlation_file,char* ppi_file){
	ifstream fcor, fppi;
	std::map <string, float> temp_map;
	
	fcor.open(correlation_file);
	if(!fcor) 
		printError("could not open "+string(correlation_file)+"\n");
	/*get all significant correlations fron gene expression data*/
	while(fcor.good()){
		string pro1, pro2;
		float cor;
		fcor>>pro1>>pro2>>cor;
		if (pro1==pro2)
			continue;

		//these checks are not needed now as they have been taken care of during preprocessing ( filtering )
			/*
			if(pro1[0]  == '-' || pro2[0] == '-') continue;
			
			stringstream s;
			//getting rid of '/' in protein names
			s.str(pro1);
			getline(s,pro1,'/');
			if(edges.find(pro1) == edges.end()) continue;
			
			s.clear();
			s.str(pro2);
			getline(s,pro2,'/');
			if(edges.find(pro2) == edges.end()) continue;
		*/

		std::map< string, float> ::iterator it1 = temp_map.find(pro1+"|"+pro2);
		//std::map< string, std::map <string, float> >::iterator it2 = dataMap.find(pro2);
		
		if( it1 == temp_map.end() ){
			temp_map[pro1+"|"+pro2] = cor;
			temp_map[pro2+"|"+pro1] = cor;
		}

	}
	if (temp_map.size() == 0 )
		printError("correlation file not read properly / correlation file empty");
	fcor.close();
	int xor_count(0), 	nxor_count(0);
	fppi.open(ppi_file);
	while(fppi.good()){
		string pro1, pro2, confidence;
		fppi>>pro1>>pro2>>confidence;

		if(pro1 == pro2)
			continue;
		if(pro1[2] ==  '/' || pro2[2] == '/' )
			continue;
		if( (pro1[0] >= '0' && pro1[0] <= '9') || (pro2[0] >= '0' && pro2[0] <= '9') )
			continue;
		char label = getLabel(temp_map, pro1, pro2);
		if(label == DONT_CARE)
			continue;
		else if(label == XOR)
			xor_count++;
		else if(label == NXOR)
			nxor_count++;
		else
			printError("unknown lable found\n");
		dataMap[pro1+"|"+pro2] = label;
		dataMap[pro2+"|"+pro1] = label;
	}
	cout<<"number of positively correlated pairs in graph = "<<nxor_count<<endl;
	cout<<"number of negatively correlated pairs in graph  = "<<xor_count<<endl;
	fppi.close();		
}
void createGraph(graph &g, char* ppi_file, char* edgeLabels, std::map<string, char> & dataMap){
	cout<<dataMap.size()<<endl	;
	ifstream fp;
	fp.open(ppi_file); 
	if (!fp ) 
		printError("could not open "+string(ppi_file)+"\n");
	
	string protein1,protein2, garbage;
	char label;
	ofstream fLabel;
	fLabel.open(edgeLabels);
	int xorCount=0, nxorCount=0;
	while(fp.good()){
		//read edges, nodes
	   	fp>>protein1; 	fp>>protein2; fp>>garbage;
		if(protein1 == protein2)
			continue;
		/*
		These checks are not needed as they have been taking care of while filtering the ppi file
		if(protein1[2] ==  '/' || protein2[2] == '/' )
			continue;
		if( (protein1[0] >= '0' && protein1[0] <= '9') || (protein2[0] >= '0' && protein2[0] <= '9') )
			continue;
		*/
		//find edge label
		if (dataMap.find(protein1+"|"+protein2) == dataMap.end())
		{	
			label = DONT_CARE;
		}
		else 
			label = dataMap[protein1+"|"+protein2]; 
		//if ( label == DONT_CARE )
		//	printError("label should not have been dont care");
		if (label != XOR && label != NXOR && label != DONT_CARE)
				printError("undefined label");
		//add edge to graph only if nodes are significantly correlated
		bool added = false;
		g.addNode(protein1);	
		g.addNode(protein2);	
		if (label != DONT_CARE )
		{
			added = g.addEdge(protein1,protein2, label);
		}
		fLabel<<protein1<<"\t"<<protein2<<"\t"<<label<<endl;
	}
	fp.close();
	fLabel.close();
	cout<<protein1<<" "<<protein2<<" "<<label;
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
char* filter_ppi(char* ppi_file, char* filtered_ppi, std::map< string,char> &dataMap_normal, std::map< string,char> &dataMap_disease){
	ifstream fp;
	ofstream fout;
	; 
	string protein1,protein2, garbage;

	fp.open(ppi_file); 
	fout.open(filtered_ppi);
	if (!fp || !fout) 
		printError("could not open either "+string(ppi_file)+" or "+string(filtered_ppi)+"\n");
	while(fp.good()){
		//read edges, nodes
	   	fp>>protein1; 	fp>>protein2; fp>>garbage;
		if(protein1 == protein2)
			continue;
		if (dataMap_normal.find(protein1+"|"+protein2) == dataMap_normal.end() && 
			dataMap_disease.find(protein1+"|"+protein2) == dataMap_disease.end() )
			continue;	
		fout<<protein1<<"\t"<<protein2<<"\t"<<garbage<<endl;
	}
	fout.close();
	fp.close();
	return filtered_ppi;
}
	
/*void create_component_graph(char* ppi_file, char* comp_file, std::map< string , char > &dataMap, std::map < string ,int > &nodeCCList){
	//SEEMS THAT EDGES ARE NOT GETTING ADDED PROPERLY

	//for all edegs in graph, find the components that are connected by this graph, add an edge in component graph for these components
	ifstream  fin;
	cout<<component_counter<<endl;
	
	std::map<int, bool> components_matrix[component_counter];// =[component_counter][component_counter];
	//add edges in component graph
	graph g;
	fin.open(ppi_file);
	stringstream s;
			
	while(fin.good()){
		string protein1, protein2, garbage;
		fin>>protein1>>protein2>>garbage;
		//cout<<protein1<<" "<<protein2<<" "<<garbage<<endl;
		if (protein1 == protein2)
			continue;
		if (getLabel(dataMap, protein1, protein2) == DONT_CARE )
			continue;
		if (nodeCCList.find(protein1) == nodeCCList.end() || nodeCCList.find(protein2) == nodeCCList.end() ) 
			printError("A significantly interaction"+protein1+" "+protein2+" is absent from the connected components of flip-graph \n");
		int c1 = nodeCCList[protein1];
		int c2 = nodeCCList[protein2];
		if ( c1 >= component_counter || c2>= component_counter )
			printError("component id out of bounds\n"); 
		if( c1 != c2 ){
			string cc1, cc2;
			s<<c1;//>>" ">>c2;
			s>>cc1;//<<p2;
			s<<c2;//>>" ">>c2;
			s>>cc2;//<<p2;
			cout<<cc1<<" "<<cc2<<endl;
			g.addEdge(cc1, cc2, NXOR);
			s.clear();
		}
	}
	fin.close();
	char file[] = "testing_component_graph_normal.txt";
	g.printGraph(file);
	return;
	if (!g.initBfs(0)){
		printError("couldn't satisfy the component graph !somethign not right");
	}
	//output the compoennt graph adjacency matriux into file
	ofstream fout;
	fout.open(comp_file);

	fout<<"number of components in fip-graph= "<<g.component_count<<endl;
	cout<<"number of components in fip-graph= "<<g.component_count<<endl;
	for(graph::itNodes i= g.nodes.begin(); i != g.nodes.end(); i++){
		fout<<i->first<<" "<<i->second->component<<endl;
	}
	
	fout.close();
	
}*/
