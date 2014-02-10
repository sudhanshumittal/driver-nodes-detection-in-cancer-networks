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
	if (argc < 5 ){
		cout<<"usage - ./pearson  gene_Expression_File_Name  output file number_of_samples threshold\n";
		return 0;
	}
	threshold= atof(argv[4]);
	int SAMPLE_SIZE =atoi(argv[3]);
	
	ifstream fp;
	fp.open(argv[1]);
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
	out.open(argv[2]);
	typedef map<string, vector<float> >::iterator iter;
	int relations = 0;
	for(iter i(geneExp.begin()); i!=geneExp.end(); i++){
		
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
	}
	out.close();	 
	cout<<"number of relations found = "<<relations<<endl;
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
