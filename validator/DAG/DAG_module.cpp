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
int th_count=10;
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
    string inputs[10];
    int output_len;
    string outputs[10];
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
        return j;
    };



    void DAG_delete(int n)
    {
        DAG->remove_AU(DAG,n);
    };
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
	int_32 DAG_select(Geek* geek){ geek -> DAG_select(); }
	void DAG_delete(Geek* geek, int n){ geek -> DAG_delete(n); }
}
