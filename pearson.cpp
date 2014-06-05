#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<map>
#include<cstdlib>
#include<cmath>

using namespace std;
float calcPearson( vector<float> &ExpArray1, vector<float> &ExpArray2, int SampleSize );
float threshold = 0.0f;
int main(int argc, char* argv[]){
	if (argc < 6 ){
		cout<<"usage - ./correlation gene_Expression_File_Name output_file number_of_samples threshold ppi_file\n";
		return 0;
	}
	char *gene_Expression_File_Name = argv[1];
	char *output_file = argv[2];
	char *number_of_samples = argv[3];
	char *threshold_string = argv[4];
	char *ppi_file = argv[5];
	
	threshold= atof(threshold_string);
	int SAMPLE_SIZE =atoi(number_of_samples);
	
	ifstream fp;
	fp.open(gene_Expression_File_Name);
	//read raw gene expression data into an array
	//cout<<"out";
	
	map <string, vector<float> > geneExp;
	
	stringstream temp;
	while(fp.good()){
		string proData, proName;
		float sample;
		vector<float> proSamples;
		
		fp>>proName;
		getline(fp, proData);
		if (proName=="") continue;
		
		temp<<proData;
		//temp>>proName;
		
		int count = 0;
		while(temp.good()){
			temp>>sample;
			//cout<<sample<<endl;
			proSamples.push_back(sample);
			count++;
			if (count == SAMPLE_SIZE)
				break; 
		}
		temp.clear();
		
		geneExp[proName] = proSamples;

	}
	fp.close();
	
	
	//calculate pearson correlation for every pair and store 
	//only if the value is either above threshold of below -threshold
	ofstream out;
	out.open(output_file);
	typedef map<string, vector<float> >::iterator iter;
	int relations = 0;
	/*for(iter i(geneExp.begin()); i!=geneExp.end(); i++){
		
		string pro1 = i->first;
		for(iter j(i); j!=geneExp.end(); j++){
						
			string pro2 = j->first;
			
			float c = calcPearson( i->second, j->second , SAMPLE_SIZE);
			if (  abs(c) >=threshold )
			{	
				out<<pro1<<" "<<pro2<<" "<<c<<endl;
				relations++;
			}
		}
		//cor[pro1] = proCor;
	}*/
	fp.open(ppi_file);
	int counter=0;
	while( fp.good()	){
		string pro1, pro2, confidence;
		fp>>pro1>>pro2>>confidence;
		if(pro1==pro2|| pro1=="")
			continue;
		if(geneExp.find(pro1) == geneExp.end()){
			cout<<"error: could find protein "<<pro1<<"in gene expression file\n";
			exit(1);
		}
		if(geneExp.find(pro2) == geneExp.end()){
			cout<<"error: could find protein "<<pro2<<"in gene expression file\n";
			exit(1);
		}
		float c = calcPearson( geneExp[pro1], geneExp[pro2] , SAMPLE_SIZE);
		if (  abs(c) >=threshold )
		{	
			out<<pro1<<" "<<pro2<<" "<<" "<<c<<endl;
			relations++;
		}
		counter++;
	}
	fp.close();
	out.close();	 
	cout<<"total number of interactions in ppi "<<counter <<"\nnumber of significant corelations found = "<<relations<<endl;
	return 0;
}

float calcPearson( vector<float> &ExpArray1, vector<float> &ExpArray2, int SampleSize ){//, float *PCC){ //Pearson correlation given two arrays of expressions.
  
  	if ( ExpArray1.size() <SampleSize || ExpArray2.size() <SampleSize ){
  		cout<< "sample size too much ";
  		//cout<< ExpArray1.size() <<" "<<SampleSize<<endl ;
  		return 0.0f;
  	} 
	//cout<<"helo";
    float mean1, mean2, r;
    int i;
    float sum1 = 0,sum2 = 0;
    float prod,prod1,prod2;
    float sumN = 0,sumD1 = 0,sumD2 = 0;
  
    float sample_weight[200];
    float sample_wt_sum = 0;
  
  
    for(i=0;i<200;i++)
        sample_weight[i] = 1;
  
  
  
    for(i=0;i<SampleSize;i++){
        sum1 = sum1 + (ExpArray1 [i]) * sample_weight[i];
        sum2 = sum2 + (ExpArray2 [i]) * sample_weight[i];
        sample_wt_sum = sample_wt_sum + sample_weight[i];
    }
  
  
    mean1 = sum1/sample_wt_sum;
    mean2 = sum2/sample_wt_sum;
  
    //printf("mean1: %lf mean2: %lf ",mean1,mean2);
  
    for(i=0;i<SampleSize;i++){
        prod = (ExpArray1[i] * sample_weight[i] - mean1) * ((ExpArray2[i]) * sample_weight[i] - mean2);
        sumN = sumN + prod;
    }
  
    for(i=0;i<SampleSize;i++){
        prod1 = ((ExpArray1[i]) * sample_weight[i] - mean1) * ((ExpArray1[i]) * sample_weight[i] - mean1);
        sumD1 = sumD1 + prod1;
  
        prod2 = ((ExpArray2[i]) * sample_weight[i] - mean2) * ((ExpArray2[i]) * sample_weight[i] - mean2);
        sumD2 = sumD2 + prod2;
  
        //printf("prod1: %lf prod2: %lf ",prod1,prod2);
    }
  
    //printf("sumN: %lf sumD1: %lf sumD2: %lf ",sumN,sumD1,sumD2);
  
    //sumD1 = sqrt(sumD1);
    //sumD2 = sqrt(sumD2);
  
    r = sumN/sqrt(sumD1*sumD2);
    //printf("r: %lf ",r);
  
  	return r;
    //*PCC = r;
    //return 1;
  
} 
