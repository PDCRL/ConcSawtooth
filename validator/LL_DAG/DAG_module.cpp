#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <pthread.h>
#include <atomic>
#include <chrono>
#include "Graph.cpp"
#include <map>

using namespace std;
using namespace std::this_thread; 
using namespace std::chrono; 
typedef int int_32;
int th_count=54;
std::map<string, int> txn_dict;
std::atomic<int> atomic_count{0}, AU{0}, AU2{0};
std::atomic<bool> Mminer(false); //Flag to identify inaccurate
int Total_txns, thcount;
static int duration_tot[56];
struct node
{
	Graph::Graph_Node *txnnode;
	struct node *next;
};
struct Garbagecoll
{
	struct node *head;
	struct node *tail;
}Garcolls[56];
// Creating an graph object
Graph *DAG= new Graph();
Graph *DAG2= new Graph();


struct transaction 
{
    string txn_id;
    int txn_no;
    int input_len;
    string inputs[4];
    int output_len;
    string outputs[4];
    bool flag=false;
    string pred;
};
struct Address
{
	string address;
	atomic<int> Cread;
	atomic<int> Cwrite;
};

static Address addresses[1500];
map<string, int> addMap;

static vector<transaction> curr_txns,empty_txns;

class Geek{
    public:

    static void add_nodes(int PID) 
    {
	    int txn,bat_no,i,j,k,inp_lo,inp_ex,out_lo,out_ex,miner=0;
	    Graph::Graph_Node *A= new Graph::Graph_Node;
	    Graph::Graph_Node *B= new Graph::Graph_Node;
	    Garcolls[PID].head= new node;
	    Garcolls[PID].tail= new node;
	    Garcolls[PID].head->txnnode= NULL;
	    Garcolls[PID].head->next=Garcolls[PID].tail;
	    Garcolls[PID].tail->txnnode= NULL;
	    Garcolls[PID].tail->next=NULL;
	    auto start = high_resolution_clock::now();
	    while(1)
	    {
	        txn=atomic_count++;
	        if(txn>=Total_txns) 
	        {
	        atomic_count--; 
	        duration_tot[PID]=miner;
	        return;
	        }
	        DAG->add_node(curr_txns[txn].txn_no,curr_txns[txn].txn_no,&A);
	        DAG2->add_node(curr_txns[txn].txn_no,curr_txns[txn].txn_no,&B);
	        node* temp = new node;
	        temp->next=Garcolls[PID].head->next;
	        temp->txnnode=A;
	        Garcolls[PID].head->next= temp;
          	inp_lo= curr_txns[txn].input_len;
          	out_lo= curr_txns[txn].output_len;

	        for (int i = 0; i < txn; i++) 
	        {	
		        if(curr_txns[txn].txn_no != curr_txns[i].txn_no){

		        inp_ex= curr_txns[i].input_len;
          		out_ex= curr_txns[i].output_len;
		        for (int j = 0; j < inp_lo; j++) {
		        for (int k = 0; k < out_ex; k++) {
		        
		        if(curr_txns[txn].inputs[j]==curr_txns[i].outputs[k]) { 
		            DAG->add_edge(curr_txns[i].txn_no, curr_txns[txn].txn_no,curr_txns[i].txn_no, curr_txns[txn].txn_no);
		            DAG2->add_edge(curr_txns[i].txn_no,curr_txns[txn].txn_no, curr_txns[i].txn_no,      curr_txns[txn].txn_no);
		            }
		        }
		        }
		        for (int j = 0; j < out_lo; j++) {
		        for (int k = 0; k < inp_ex; k++) {
		        
		        if(curr_txns[txn].outputs[j]==curr_txns[i].inputs[k]) { 
		            DAG->add_edge(curr_txns[i].txn_no,curr_txns[txn].txn_no, curr_txns[i].txn_no, curr_txns[txn].txn_no);
		            DAG2->add_edge(curr_txns[i].txn_no,curr_txns[txn].txn_no, curr_txns[i].txn_no, curr_txns[txn].txn_no);
		            }
		        }
		        }
		        for (int j = 0; j < out_lo; j++) {
		        for (int k = 0; k < out_ex; k++) {
		        
		        if(curr_txns[txn].outputs[j]==curr_txns[i].outputs[k]) { 
		        DAG->add_edge(curr_txns[i].txn_no,curr_txns[txn].txn_no, curr_txns[i].txn_no, curr_txns[txn].txn_no);
		        DAG2->add_edge(curr_txns[i].txn_no,curr_txns[txn].txn_no, curr_txns[i].txn_no, curr_txns[txn].txn_no);
		        }
		        }
		        }
	        }}
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        unsigned int dwDuration = duration.count();
        miner=miner+dwDuration;
	        
	    }

	    };


    static void predecessor(int PID) 
    {

	    int txn;
	    Graph::Graph_Node *A= new Graph::Graph_Node;	
	    while(1)
	    {
	        txn=AU2++;
	        if(txn>=Total_txns) {AU2--; return;}
	        string pred_list= DAG->find_pred(DAG,txn);
	        curr_txns[txn].pred=pred_list;
	    }
   };



    static void destructor(int PID) 
	    {
	    node* temp = new node;

	    while(Garcolls[PID].head->next != Garcolls[PID].tail)
		    {
		        temp=Garcolls[PID].head->next;
		        Garcolls[PID].head->next=temp->next;
		        Graph::Graph_Node *txn= temp->txnnode;
		        delete txn;
		        txn= NULL;
		        delete temp;
		        temp=NULL;
		    }

	    };

    static void Extra_edges(int PID) 
    {
	    int txn,bat_no,i,j,k,inp_lo,inp_ex,out_lo,out_ex;
	    bool flag=false;
	    while(true)
	    {
	        txn=atomic_count++;
	        if(txn>=Total_txns) {atomic_count--; return;}
          	inp_lo= curr_txns[txn].input_len;
          	out_lo= curr_txns[txn].output_len;

	        for (int i = 0; i < txn; i++) 
	        {	
		        if(curr_txns[txn].txn_no != curr_txns[i].txn_no)
		        {
		            inp_ex= curr_txns[i].input_len;
              		out_ex= curr_txns[i].output_len;
		            for (int j = 0; j < inp_lo; j++) {
		            for (int k = 0; k < out_ex; k++) {  
		            if(curr_txns[txn].inputs[j]==curr_txns[i].outputs[k]) {flag=true;}
		            }
		            }
		            for (int j = 0; j < out_lo; j++) {
		            for (int k = 0; k < inp_ex; k++) {
		            if(curr_txns[txn].outputs[j]==curr_txns[i].inputs[k]) {flag=true;}
		            }
		            }
		            for (int j = 0; j < out_lo; j++) {
		            for (int k = 0; k < out_ex; k++) { 
		            if(curr_txns[txn].outputs[j]==curr_txns[i].outputs[k])  {flag=true;}
		            }
		            }
	            }
	        }
	    }
    };


    static void Missing_edges(int PID) 
	    {
	    int txn,k;
	    
	    string adrs;
	    //Local map to keep track of local addresses
        int LmapSize=0,i=0,Lpos, Gpos,inp_lo,out_lo,deg=-1;
        struct Address localAdd[10];
            while(true){//!Mminer){
                txn=atomic_count++;
                LmapSize=0;
                map<string, int> localMap;
                if( txn>=Total_txns) {return;}     
                while(deg!=0)
                {
                    deg=DAG->findInDeg(DAG,txn);
                }
                inp_lo= curr_txns[txn].input_len;
          	    out_lo= curr_txns[txn].output_len;
          	    
          	    for (int i = 0; i < inp_lo; i++) 
          	        {
          	        adrs= curr_txns[txn].inputs[i];
          	        localMap.insert(pair<string,int>(adrs, LmapSize));
                         if(LmapSize<localMap.size())
                            {
                            localAdd[LmapSize].address=adrs;
                            localAdd[LmapSize].Cread=0;
                            localAdd[LmapSize].Cwrite=0;
                            LmapSize++;
                            }
                    }
          	    for (int i = 0; i < out_lo; i++) {
          	        adrs= curr_txns[txn].outputs[i];
          	        localMap.insert(pair<string,int>(adrs, LmapSize));
                         if(LmapSize<localMap.size()){
                            localAdd[LmapSize].address=adrs;
                            localAdd[LmapSize].Cread=0;
                            localAdd[LmapSize].Cwrite=0;
                            LmapSize++;
                            }
            	}
          	    for (int i = 0; i < inp_lo; i++) {
          	        adrs= curr_txns[txn].inputs[i];
          	        Lpos=localMap.at(adrs);
                    Gpos=addMap.at(adrs);
                    if(addresses[Gpos].Cwrite != localAdd[Lpos].Cwrite){Mminer=true;}
                    localAdd[Lpos].Cread++;
                    addresses[Gpos].Cread++;
                }
          	    for (int i = 0; i < out_lo; i++) {
          	        adrs= curr_txns[txn].outputs[i];
          	        Lpos=localMap.at(adrs);
                    Gpos=addMap.at(adrs);
                    if(addresses[Gpos].Cwrite != localAdd[Lpos].Cwrite){Mminer=true;}
                    if(addresses[Gpos].Cread != localAdd[Lpos].Cread){Mminer=true;}
                    localAdd[Lpos].Cwrite++;
                    addresses[Gpos].Cwrite++;
               }
          	    for (int i = 0; i < out_lo; i++) {
          	        adrs= curr_txns[txn].outputs[i];
          	        Lpos=localMap.at(adrs);
                    Gpos=addMap.at(adrs);
                    addresses[Gpos].Cread.fetch_sub(localAdd[Lpos].Cread);

                }
          	    for (int i = 0; i < inp_lo; i++) {
          	        adrs= curr_txns[txn].inputs[i];
          	        Lpos=localMap.at(adrs);
                    Gpos=addMap.at(adrs);
                    addresses[Gpos].Cwrite.fetch_sub(localAdd[Lpos].Cwrite);
                }

      
	        //cout<<"R"<<txn<<endl;
	    }

    };



    void DAG_prune()
    {
	    thread threads[th_count];
	    for(int i=0;i<th_count;i++)	{threads[i]= thread(destructor,i); }//starting writer threads
	    for(int i=0;i<th_count;i++){threads[i].join(); }//joining writer threads
	    //delete DAG;
	    Graph *DAG_new= new Graph();
	    atomic_count=0, 
	    AU=0;
	    AU2=0;
	    Total_txns=0;
	    DAG=DAG_new;
	    curr_txns= empty_txns;
    };


    int_32 DAG_select()
    {
        int i= DAG->inDeg_zero(DAG);
        int_32 j;

        if(i!= -1)
        { 
            j=curr_txns[i].txn_no;

        }
        else 
        {
            j=-1;
        }
//        cout<<"------"<<j<<"-------"<<endl;                
        return j;

    };



    void DAG_delete(int n)
    {
        DAG->remove_AU(DAG,n);
    };

    void DAG_create2()
    {
        //DAG->print_BG(DAG);
        int in_deg= DAG->print_grpah();
	    fstream batch_file;  
	    batch_file.open("DAG/in_deg.txt",ios::out);
	    batch_file<<"indegree:"<<in_deg<<endl;
    };

    void Smart_Validator(){
        int mapSize=0,inp_lo,out_lo,i;
        thread threads[th_count];
        struct Address add_temp;
        string adrs;
        for (int i = 0; i < Total_txns; i++) 
	    {
	        inp_lo= curr_txns[i].input_len;
          	out_lo= curr_txns[i].output_len;
          	for (int j = 0; j < inp_lo; j++) 
          	{
          	    adrs= curr_txns[i].inputs[j];
          	    addMap.insert(pair<string,int>(adrs, mapSize));
          	     if(mapSize<addMap.size())
          	     {
          	        addresses[mapSize].address=adrs;
          	        addresses[mapSize].Cread=0;
          	        addresses[mapSize].Cwrite=0;
          	        mapSize++;
          	     }
      	    }
          	for (int j = 0; j < out_lo; j++) 
          	{
          	    adrs= curr_txns[i].outputs[j];
          	    addMap.insert(pair<string,int>(adrs, mapSize));
          	     if(mapSize<addMap.size())
          	     {
          	        addresses[mapSize].address=adrs;
          	        addresses[mapSize].Cread=0;
          	        addresses[mapSize].Cwrite=0;
          	        mapSize++;
          	     }
      	    }
	    }
	    atomic_count=0;
	    //for(i=0;i<th_count;i++)	{threads[i]= thread(Missing_edges,i); }//starting writer threads
	    //for(i=0;i<th_count;i++){threads[i].join(); }//joining writer threads
	    atomic_count=0;
	    for(i=0;i<th_count;i++)	{threads[i]= thread(Extra_edges,i); }//starting writer threads
	    for(i=0;i<th_count;i++){threads[i].join(); }//joining writer threads

        //return Mminer;

    }

    void DAG_create()
    {
	    
	    int i,au_no,last_count,miner_tot=0;
	    last_count=Total_txns;
	    thread threads[th_count];
	    fstream batch_file,miner_file; 
	    struct transaction txn; 
	    miner_file.open("DAG/miner_timing.txt",ios::app);
	    batch_file.open("DAG/batch_for_DAG.txt",ios::in); 
	    if (batch_file.is_open())
	    {   
	        string tp;
		    while(getline(batch_file, tp))
		    {

			    txn.txn_id= tp;

			    au_no=AU;
			    AU=AU+1;
			    txn.txn_no=au_no;
			    txn.pred="";
			    getline(batch_file, tp);
			    txn.input_len= stoi(tp);
			    for(i=0;i<txn.input_len;i++) { getline(batch_file, tp);
							    txn.inputs[i]=tp;	}
			    getline(batch_file, tp);
			    txn.output_len= stoi(tp);
			    for(i=0;i<txn.output_len;i++) { getline(batch_file, tp);
							    txn.outputs[i]=tp;	}
			    curr_txns.push_back(txn);		
		    }
	    }
	    batch_file.close(); 
	    Total_txns=curr_txns.size();

	    for(i=0;i<th_count;i++)	{threads[i]= thread(add_nodes,i); }//starting writer threads
	    for(i=0;i<th_count;i++){threads[i].join(); }//joining writer threads
    }
};
int main()
{


    // Creating an object
    Geek t; 
  
    // Calling function
    t.DAG_create();  
    return 0;

}

extern "C" 
{

	Geek* Geek_new(){ return new Geek(); }
	void DAG_prune(Geek* geek){ geek -> DAG_prune(); }
	void DAG_create(Geek* geek){ geek -> DAG_create(); }
	void DAG_create2(Geek* geek){ geek -> DAG_create2(); }
	int_32 DAG_select(Geek* geek){ geek -> DAG_select(); }
	void Smart_Validator(Geek* geek){ geek -> Smart_Validator(); }
	void DAG_delete(Geek* geek, int n){ geek -> DAG_delete(n); }
}
