/*
author: sudhanshu mittal
description : final graph construction and satisfying assignment calculation program be it toggled only 
or something else
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

#define PEARSON_FACTOR 0.5
#define XOR 'x'
#define NXOR 'n'
#define TINY_VALUE 1e-20
#define DONT_CARE '-'

    
float usatEdges= 0; //number of unsatisfied edges

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
	        //value[0] = 2;
	        //value[1] = 2;
	        //label_ = DONT_CARE;
	    }
	    node(string s, char l){
	        name_ = s;
	        visited_= 0;
	        value = 2;
	        //value[0] = 2;
	        //value[1] = 2;
	        //label_ = l;
	    }
	    /*int val(){
	        return valNormal_;
	    }*/
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
	    	/*for(it i = adj_.begin(); i != adj_.end(); i++){
				if (i->second.n->name() == s) return true; 
			} 
			return false;*/
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
	 	graph(){
			component_count = 0;
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
	        root = unvisitedNode(); //serch for unlabeled node
	        if ( root == NULL ) //all nodes have been visited
	            return true;
			return bfs(root, init);
	    }
	    bool initBfs(int init){
	        node * root = nodes["SUMO2"];
	        return bfs(root, init);
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
	    /*void resetVisit( int version ){
	        //return;
	        for(itNodes i = nodes.begin(); i != nodes.end(); i++){
	            i->second->unvisit(version);
	            //unlabel all edges to dont Care
	            std::map<string, node::edge> adj = i->second->getNeighbours();
	            for ( std::map< string, node::edge >::iterator j  = adj.begin(); j != adj.end(); j++){
	            	j->second.setLabel(DONT_CARE);
	            }
	        }
	        //cout<<"reset visit called";
	        //exit(0);
	    }*/
	    void addNode(string s){
	        if (nodes.find(s)== nodes.end())
	        	nodes[s] = new node(s);
	    }
	    bool addEdge(string s1, string s2, char label){
	        if (s1 == s2) return false; /*no self loops*/
	        addNode(s1);
	        addNode(s2);
	        bool success = nodes[s1]->addNeighbour(s2, nodes[s2], label);
	        success &= nodes[s2]->addNeighbour(s1, nodes[s1], label);
	    	return success;
	    }
	    
		void printNodes(char* file){
			ofstream fp;
	        fp.open(file);
			if( !fp ){
				printError("could not open output file for vertex labels");
			}
	        fp<<"number_of_components "<<component_count<<endl;
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
	    /*void printGraph(){
	        ofstream fp;
	        fp.open("graphData.txt");
	        for(itNodes i = nodes.begin(); i != nodes.end(); i++){
	            fp<<endl<<i->first<<" ==> ";
	            fp<<i->second->printNeighbours();
	        }
	        fp.close();
	    }
	    void findMinFlips(){
	    	//simple version assuming single connected component
	    	int minFlips =0;
	    	for(itNodes i = nodes.begin(); i != nodes.end(); i++){
	    		if( i->second->val(0) == 2 || i->second->val(1) ==2 ) continue; 
	    		if(i->second->val(0) != i->second->val(1) ) minFlips++;
	    	}
	    	cout<<"minimum flips required are "<<min(minFlips, nodes.size()- minFlips);
	    }

	    char getLabel( string s1, string s2){
	    	return nodes[s2]->getLabel(s1);
	    }*/
};

int min(int a, int b);
//char getLabel(string xstr, string ystr);
char getLabel (std::map<string, float>  &dataMap, string pro1, string pro2);
//double Correlation(const vector<double>& x, const vector<double>& y);
void getCorrelation(std::map< string, float > &dataMap, std::map<string, string> & edges, char* file);
void createGraph(graph &g, char* ppi_file, char* edgeLabelFile, std::map< string, float> &dataMap);
void getNodes(std::map<string, string> &edges, char* );
void create_component_graph(char* ppi_file, char* comp_file,  std::map< string , float > &dataMap, std::map < string ,int > &nodeCCList);
int component_counter;
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
int main(int argc, char* argv[])
{
	if(argc < 8){
		printError("usage ./a.out correlation_file ppi_file_name edge_label_output_file vertex_labels_file component_graph_file filtered_ppi_with_flipped_only_edges isolated_nodes_file\n");
	}   
	/*stores connected component of each protein in flip-only graph (isloated nodes included)*/
	std::map < string ,int > nodeCCList = find_connected_components_flip_only(argv[6], argv[1], argv[7]); 
	/*for(std::map < string ,int >::iterator i = nodeCCList.begin(); i!= nodeCCList.end(); i++)
		cout<<i->first<<" "<<i->second<<endl;
	*/
	//getting list of nodes from ppi file
	std::map<string, string> edges;
	getNodes(edges, argv[2]);
	cout<<"number of unique nodes is ="<<edges.size()<<endl;
	/*
		for ( std::map<string, string>::iterator i = edges.begin(); i!=edges.end(); i++)
		cout<<	i->first	<<" "<<		i->second	<<endl;
	*/
	cout<<"getting the correlation values...\n";
	std::map< string , float > dataMap;
	getCorrelation(dataMap, edges, argv[1]);
	/*
	cout<<"creating graph...\n";
	graph g;
    createGraph(g, argv[2], argv[3], dataMap);
	
	cout<<"traversing the graph now...\n";
	if(!g.initBfs(1)) { //for normal case
		cout<<"\nERROR: Normal graph unsatisfiable";
		return 1;
	}
	
	cout<<"putting the satisfying assignment results in file..."<<argv[4]<<"\n";
	g.printNodes(argv[4]);
	cout<<"unsatisfiable = "<<usatEdges<<endl; 
	cout<<"number of components = "<<g.component_count<<endl;	
	
	//g.printGraph((char*)"test_edges.txt");
    */
    cout<<"creating component graph now\n";
    create_component_graph(argv[2], argv[5], dataMap, nodeCCList);
    
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
		if( (pro1[0] >= '0' && pro1[0] <= '9') || (pro2[0] >= '0' && pro2[0] <= '9') ){
			continue;
		}
		//cout<<pro1<<"\t"<<pro2<<endl;
		edges[pro1] = pro2;
		edges[pro2] = pro1;
		//fp>>pro1;		
	}
}
void getCorrelation(std::map < string, float> &dataMap, std::map<string, string>&edges, char* correlation_file){
	ifstream fp;
	fp.open(correlation_file);
	if(!fp) 
		printError("could not open "+string(correlation_file)+"\n");
	
	std::map <string, float> temp_map;
	while(fp.good()){
		string pro1, pro2;
		float cor;
		fp>>pro1;		fp>>pro2;		fp>>cor;
		
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

		std::map< string, float> ::iterator it1 = dataMap.find(pro1+"|"+pro2);
		//std::map< string, std::map <string, float> >::iterator it2 = dataMap.find(pro2);
		
		if( it1 == dataMap.end() ){
			dataMap[pro1+"|"+pro2] = cor;
			dataMap[pro2+"|"+pro1] = cor;
		}

	}
	fp.close();
			
}
void createGraph(graph &g, char* ppi_file, char* edgeLabels, std::map<string, float> & dataMap){
	ifstream fp;
	fp.open(ppi_file); 
	if (!fp ) 
		printError("could not open "+string(ppi_file)+"\n");
	
	string protein1,protein2, garbage;
	ofstream fLabel;
	fLabel.open(edgeLabels);
	int xorCount=0, nxorCount=0;
	while(fp.good()){
		//read edges, nodes
		
	   	fp>>protein1; 	fp>>protein2; fp>>garbage;
	   
		if(protein1[2] ==  '/' || protein2[2] == '/' ) continue;
		//find edge label
		
		char label=getLabel( dataMap, protein1, protein2 ); 
		if ( label == DONT_CARE )
				continue;
		
		//add edge to graph
		bool added = g.addEdge(protein1,protein2, label);
		
		if (added){
			fLabel<<protein1<<"\t"<<protein2<<"\t"<<label<<endl;
			if ( label == NXOR)
				nxorCount++;
			else if (label== XOR) 
				xorCount++;
		}
		//cout<<protein1<<endl<<protein2<<endl;
			 
	}
	cout<<"number of positively correlated pairs in graph created  = "<<nxorCount<<endl;
	cout<<"number of negatively correlated pairs in created graph  = "<<xorCount<<endl;
	fp.close();
	fLabel.close();
}

char getLabel (std::map<string, float > &dataMap, string pro1, string pro2){
		
	float val = dataMap[pro1+"|"+pro2];
	float val2 = dataMap[pro2+"|"+pro1];
	if (val != val2)
		cout<<"error found in get Label"<<endl;
	if(val >=PEARSON_FACTOR) return NXOR;
	else if (val <=-PEARSON_FACTOR) return XOR;
	else return DONT_CARE;
}
	
void create_component_graph(char* ppi_file, char* comp_file, std::map< string , float > &dataMap, std::map < string ,int > &nodeCCList){
	/*SEEMS THAT EDGES ARE NOT GETTING ADDED PROPERLY*/

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
	/*
	for (int i(0); i<component_counter; i++)
	{
		fout<<i<<" ";
		for (std::map<int, bool>::iterator j = components_matrix[i].begin()  ; j != components_matrix[i].end(); j++)
			fout<<j->first<<" ";
		fout<<endl;
	}
	*/
	fout.close();
	
}
