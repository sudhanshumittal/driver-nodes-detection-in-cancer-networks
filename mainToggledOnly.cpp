/*
description
create a new ppi file by considereing only toggled edges
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
#include <unordered_map>
using namespace std;

#define PEARSON_FACTOR 0.5
#define XOR 'x'
#define NXOR 'n'
#define TINY_VALUE 1e-20
#define DONT_CARE '-'
int min(int a, int b);
//char getLabel(string xstr, string ystr);
char getLabel (std::map<string, float>  &dataMap, string pro1, string pro2);
//double Correlation(const vector<double>& x, const vector<double>& y);
void printError(string a){
	/*ofstream fp;
	fp.open("log.txt", ios_base::app);
	fp<<a;
	fp.close();*/
	cout<<a;
	exit(1);
}
class node{
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
    bool visited_;  //if node is visited in bfs
    typedef  std::map<string, edge>::iterator it; //iterator over adj_ type

public:

    node(string s){
        name_ =s;
        visited_= false;
        value = 2;
        //value[0] = 2;
        //value[1] = 2;
        //label_ = DONT_CARE;
    }
    node(string s, char l){
        name_ = s;
        visited_= false;
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
        return visited_;
    }
    void visit(){
        visited_ =true;
    }
    void unvisit(){
        visited_ = false;
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
    void addNeighbour(string s, node *n, char label){
        it i= adj_.find(s);
        if(i == adj_.end() ) {
            //new neighbour
            edge e;
            e.label_ = label;
            e.n = n;
            adj_[s]= e;
        }
		else {
			//reset edge
			adj_[s].label_ = label;
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
int usatEdges;
class graph{
    std::map < string ,node* > nodes;
	typedef std::map<string ,node*>::iterator itNodes;
public:
    bool bfs(node * root, int init){
		if ( root == NULL ) return true;
		
		cout<<"new root is "<<root->name()<<endl; 
		queue< node* > q;
        root->setVal(init);
        root->visit();
        q.push(root);
        node* unvisited;
        while(!q.empty()){
            
            node* curr = q.front();
            q.pop();
            //cout<<curr->name()<<" ";
            
            std::map<string, node::edge> adj = curr->getNeighbours();
            if (curr->val() == 2){
	            for(std::map<string, node::edge>::iterator i = adj.begin(); i!=adj.end(); i++){
	            	node::edge next = i->second;
               		if(  next.label() != DONT_CARE ){
	                	if( next.n->val() != 2){
		                	curr->setVal( satisfy(next.n->val(), next.label() ) ); 	
	                		//if (satisfy(next.n->val(), next.label()) == 2 ) 
	                		//	cout<<"error\n";
	                		break;
	                	}
	    				else
	    					curr->setVal( init );
	                	
	                }

	            }
        	}
            for(std::map<string, node::edge>::iterator i = adj.begin(); i!=adj.end(); i++){
                node::edge next = i->second;
                //labeled edge and the node is dont_care then label it
                if( !next.n->visited()){
                	//label unvisited node
                  	next.n->setVal( satisfy(curr->val(), next.label()) );
                    q.push(next.n);
                }
                else{
                	//check correctness for labeled/visited nodes
                 	//if some edge is not getting satisfied count unsatisfied edge 
					int satisfyingVal = satisfy(curr->val(), next.label()) ;
					//if( satisfyingVal != 2 && version == 1) cout<<"\nfound one non dontcare node for abnormal";
					if ( satisfyingVal == 2 ) ; 
					else if( next.n->val() == 2){
						next.n->setVal( satisfyingVal);
						q.push(next.n);
					}
					else if(next.n->val() != satisfyingVal ){
							//cout<<curr->name()<<" "<<curr->val(version)<<"--"<<next.label()<<"-->"<<next.n->name()<<" "<<next.n->val(version);
							//cout<<endl<<satisfyingVal;
							usatEdges++;
							//resetVisit();
							//return false;
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
        node * root = nodes.begin()->second;
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
    void addEdge(string s1, string s2, char label){
        if (s1 == s2) return;
        addNode(s1);
        addNode(s2);
        nodes[s1]->addNeighbour(s2, nodes[s2], label);
        nodes[s2]->addNeighbour(s1, nodes[s1], label);
    }
    
	void printNodes(char* file){
		ofstream fp;
        fp.open(file);
		if( !fp ){
			printError("could not open output file for vertex labels");
		}
        for(itNodes i = nodes.begin(); i != nodes.end(); i++){
			fp<<i->second->name()<<"\t"<<i->second->val()<<endl;//"\t"<<i->second->val(1)<<endl;
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
void getCorrelation(std::map< string, float > &dataMap, std::map<string, string> & edges, char* file);
void createGraph(graph &g, char* ppi_file, char* edgeLabelFile, std::map< string, float> &dataMap);
void getNodes(std::map<string, string> &edges, char* );
void create_revised_ppi(char*, std::map< string, float> &dataMapN, std::map< string, float> &dataMapT);
	
int main(int argc, char* argv[])
{
	if(argc < 6){
		printError("usage ./a.out correlation_file_normal  ppi_file_name   edge_label_output_file   vertex_labels_file   correlation_file_cancer\n");
	}   
	
	//getting list of nodes from ppi file
	std::map<string, string> edges;
	
	getNodes(edges, argv[2]);

	cout<<"number of unique nodes is ="<<edges.size()<<endl;
	/*for ( std::map<string, string>::iterator i = edges.begin(); i!=edges.end(); i++)
		cout<<	i->first	<<" "<<		i->second	<<endl;*/
	cout<<"getting the correlation values...\n";
	std::map< string , float > dataMapN;
	std::map< string , float > dataMapT;
	
	getCorrelation(dataMapN, edges, argv[1]);
	getCorrelation(dataMapT, edges, argv[5]);
	create_revised_ppi(argv[2], dataMapN, dataMapT);
	return 0;
	/*
	//working on the ppi file now
	cout<<"creating graph...\n";
	graph g;
    createGraph(g, argv[2], argv[3], dataMap);
	
	cout<<"traversing the graph now...\n";
	
	if(!g.initBfs(1)) { //for normal case
		cout<<"\nERROR: Normal graph unsatisfiable";
		return 1;
	}
	cout<<"putting the results in file...\n";
	g.printNodes(argv[4]);
	cout<<"unsatisfiable = "<<usatEdges<<endl; 
	
	cout<<"success!\n";
    */

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
	//correlation file contains all significant correlation edges between every pair of gene
	//irrespective of weither it is present in the ppi or not
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

		//std::map< string, float> ::iterator it1 = dataMap.find(pro1+"|"+pro2);
		//std::map< string, std::map <string, float> >::iterator it2 = dataMap.find(pro2);
		
		//if( it1 == dataMap.end() ){
			dataMap[pro1+"|"+pro2] = cor;
			dataMap[pro2+"|"+pro1] = cor;
		//}

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
		if ( label == NXOR)
			nxorCount++;
		else if (label== XOR) 
			xorCount++;
		//add edge to graph
		g.addEdge(protein1,protein2, label);
		

		fLabel<<protein1<<"\t"<<protein2<<"\t"<<label<<endl;
		//cout<<protein1<<endl<<protein2<<endl;
			 
	}
	//cout<<"number of positively correlated pairs = "<<nxorCount<<endl;
	//cout<<"number of negatively correlated pairs = "<<xorCount<<endl;
	fp.close();
	fLabel.close();
}

char getLabel (std::map<string, float > &dataMap, string pro1, string pro2){
	
	if (dataMap.find(pro1+"|"+pro2) == dataMap.end())
		return DONT_CARE;
	float val = dataMap[pro1+"|"+pro2];
	/*float val2 = dataMap[pro2+"|"+pro1];
	if (val != val2)
		cout<<"error found in get Label"<<endl;
	*/
	if(val >=PEARSON_FACTOR) return NXOR;
	else if (val <=-PEARSON_FACTOR) return XOR;
	else return DONT_CARE;
}

void create_revised_ppi(char* ppi_file, std::map< string, float> &dataMapN, std::map< string, float> &dataMapT){
	
	ifstream fp;
	fp.open(ppi_file);
	ofstream newppi, isolatedNodesFile;
	newppi.open("revised_ppi.txt");
	isolatedNodesFile.open("isolated_nodes_in_flip_graph_file.txt");
	int count = 0;
	int edges_flipped_count = 0;
	int edges_unchanged_count = 0;
	int pos_cor_normal(0), neg_cor_normal(0);
	int pos_cor_tumor(0); int neg_cor_tumor(0);
	string pro1, pro2, garbage;
	unordered_map<string, bool> isIsolated;
	while(fp.good()){
		fp>>pro1>>pro2>>garbage;
		if ( pro1 == pro2 )
			continue;
		char label1 = getLabel(dataMapN, pro1, pro2);
		char label2 = getLabel(dataMapT, pro1, pro2);
		if (label1 == DONT_CARE && label2 == DONT_CARE ) //edge absent in both normal and tumor n/w
			continue;
		else if( (label1 != DONT_CARE && label2 == DONT_CARE) || (label1 == DONT_CARE && label2 != DONT_CARE) ){
			//addition or deletion case
			//these interacting nodes might be lost because of the removal of edges
			if ( isIsolated.find(pro1) != isIsolated.end() && isIsolated[pro1] == false )
				continue;
			else
				isIsolated[pro1] = true;
			if ( isIsolated.find(pro2) != isIsolated.end() && isIsolated[pro2] == false )
				continue;
			else
				isIsolated[pro2] = true;
		
		}
		if(label1 == NXOR)
			pos_cor_normal++;
		else 
			neg_cor_normal++;
		if(label2 == NXOR)
			pos_cor_tumor++;
		else 
			neg_cor_tumor++;
		if (label1 != label2)
			edges_flipped_count++;
		else
			edges_unchanged_count++;
		count++;
		isIsolated[pro1] = false;
		isIsolated[pro2] = false;
		newppi<<pro1<<"\t"<<pro2<<"\t"<<garbage<<endl;	
	}
	for(unordered_map<string, bool>::iterator i= isIsolated.begin(); i!= isIsolated.end(); i++)
		if (i->second == true)
			isolatedNodesFile<<i->first<<" ";
	isolatedNodesFile.close();

	cout<<"number of edges retained = "<<count<<endl;
	cout<<"number of edges that flipped = "<<edges_flipped_count<<endl;
	cout<<"number of edges that remained unchanged = "<<edges_unchanged_count<<endl;
	cout<<"under normal conditions:\n\tpositive correlations="<<pos_cor_normal<<"\n\tnegative correlations="<<neg_cor_normal<<endl;
	cout<<"under tumor conditions:\n\tpositive correlations="<<pos_cor_tumor<<"\n\tnegative correlations="<<neg_cor_tumor<<endl;
	fp.close();
	newppi.close();
}
