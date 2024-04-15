#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <atomic>
#include <vector>
#include <map>
#include "hash.cpp"

using namespace std;

constexpr int th_count = 1;

std::map<string, int> txn_dict;
std::atomic<int> atomic_count{0}, AU{0};


int Total_txns;
int txn_adj[1200][1200] = {0};
atomic_int in_deg[1200];
atomic_int pos{0};

struct transaction
{
    string txn_id;
    int txn_no;
    int input_len;
    string inputs[10];
    int output_len;
    string outputs[10];
    bool flag = false;
};

static vector<transaction> curr_txns, empty_txns;

class Geek
{
public:
	static int select_txn()
	{
	    int var_zero = 0;
        int var_one = 1;
	    for (int i = pos; i < Total_txns; i++)
	    {
                if (in_deg[i] == 0 && in_deg[i].compare_exchange_strong(var_zero, -1))
                {
                    return i;
                }
            
        }
	    for (int i = 0; i < pos; i++)
	    {
        
            if (in_deg[i] == 0 && in_deg[i].compare_exchange_strong(var_zero, -1))
            {
                pos = i;
                return i;
            }
        }
	    // for (int i = 0; i < Total_txns; i++)
	    // {
        //     var_one=in_deg[i];
        //     if (in_deg[i] > 0 && in_deg[i].compare_exchange_strong(var_one, -1))
        //     {
        //         curr_txns[i].flag=false;
        //         return i;
        //     }
	        
        // }
	    return -1;
	}

static void add_nodes(int PID)
{
    int txn, inp_lo, inp_ex, out_lo, out_ex;
    int i = 0,txn2;
    while (true)
    {
        int edge_txn;
        bool flag = false;
        txn = atomic_count.fetch_add(1);
        if (txn >= Total_txns)
        {
            atomic_count.fetch_sub(1);
            return;
        }

        inp_lo = curr_txns[txn].input_len;
        out_lo = curr_txns[txn].output_len;
        
        for (int j = 0; j < inp_lo; j++)
        {
            for (i = txn - 1; i >= 0 && !flag; i--)
            {
                if (in_deg[i] > -1)
                {
                    int inp_ex = curr_txns[i].input_len;
                    int out_ex = curr_txns[i].output_len;

                    for (int k = 0; k < out_ex; k++)
                    {
                        if (curr_txns[txn].inputs[j] == curr_txns[i].outputs[k])
                        {
                            flag = true;
                            txn2=i;
                            break;
                        }
                    }
                }
            }

            if (flag)
            {
                if (txn_adj[curr_txns[txn2].txn_no][txn] == 0)
                {
                    txn_adj[curr_txns[txn2].txn_no][txn] = 1;
                    in_deg[txn]++;
                }

                flag = false;  // Reset flag here
            }
        }

        for (int j = 0; j < out_lo; j++)
        {
            for (i = txn - 1; i >= 0 && !flag; i--)
            {
                if (in_deg[i] > -1)
                {
                    int inp_ex = curr_txns[i].input_len;
                    int out_ex = curr_txns[i].output_len;

                    for (int k = 0; k < out_ex; k++)
                    {
                        if (curr_txns[txn].outputs[j] == curr_txns[i].outputs[k])
                        {
                            flag = true;
                            txn2=i;
                            break;
                        }
                    }

                    for (int k = 0; k < inp_ex; k++)
                    {
                        if (curr_txns[txn].outputs[j] == curr_txns[i].inputs[k])
                        {
                            flag = true;
                            break;
                        }
                    }
                }
            }

            if (flag)
            {
                if (txn_adj[curr_txns[txn2].txn_no][txn] == 0)
                {
                    txn_adj[curr_txns[txn2].txn_no][txn] = 1;
                    in_deg[txn]++;
                }

                flag = false;  // Reset flag here
            }
        }
    }
}


void DAG_prune()
{
    for (int i = 0; i < Total_txns; i++)
    {
        fill_n(txn_adj[i], Total_txns, 0);
        in_deg[i] = 0;
    }

    atomic_count = 0;
    AU = 0;
    Total_txns = 0;
    curr_txns.clear();
}
int DAG_select()
{
    int i = select_txn();
    if(i!= -1) {DAG_write();}
    return (i != -1) ? curr_txns[i].txn_no : -1;
}

void DAG_undo(int n)
{
    in_deg[n]=0;   
}


void DAG_write()
{
    ofstream outfile("matrix.txt", ios::in);

    if (!outfile.is_open())  // Check if the file is successfully opened
    {
        cout << "Error opening file." << endl;
        return;
    }
    outfile << "-------------------------------------------\n";
    for (int i = 0; i < Total_txns; i++)
    {
        for (int j = 0; j < Total_txns; j++)
        {
            outfile << txn_adj[i][j] << ' ';
        }
        outfile << '\n';
    }


    outfile << "-------------------------------------------\n";

    for (int j = 0; j < Total_txns; j++)
    {
        outfile << in_deg[j] << ' ';
    }
    outfile << '\n';

    outfile << "-------------------------------------------\n";


    outfile.close();  // Close the file stream when done
}


 void DAG_create2()
{
    // for (int i = 0; i < Total_txns; i++)
    // {
    //     for (int j = 0; j < Total_txns; j++)
    //     {
    //         cout << txn_adj[i][j] << ' ';
    //     }
    //     cout << '\n';
    // }

    cout << "-------------------------------------------\n";
    
    for (int j = 0; j < Total_txns; j++)
    {
        cout << in_deg[j] << ' ';
    }
    cout << '\n';
    
    cout << "-------------------------------------------\n";
    
    for (int j = 0; j < Total_txns; j++)
    {
        cout << curr_txns[j].txn_no << ' ';
    }
    cout << '\n';
}

void DAG_delete(int n)
{
    for (int i = 0; i < Total_txns; i++)
    {
        if (txn_adj[n][i] == 1 && in_deg[i] > 0)
        { --in_deg[i];}
    }
    --in_deg[n];



}


int DAG_create()
{
    fstream batch_file("DAG/batch_for_DAG.txt",  ios::in);
    int i,init=AU;
    if (!batch_file.is_open())
    {
        cerr << "Error: Unable to open file." << endl;
        return -1;
    }
    string tp;
    struct transaction txn;

    while (getline(batch_file, tp))
    {
        txn.txn_id = tp;
        txn.txn_no = AU++;
        getline(batch_file, tp);
        txn.input_len = stoi(tp);
        for (int i = 0; i < txn.input_len; i++)
        {
            getline(batch_file, tp);
            txn.inputs[i] = tp;
        }

        getline(batch_file, tp);
        txn.output_len = stoi(tp);
        for (int i = 0; i < txn.output_len; i++)
        {
            getline(batch_file, tp);
            txn.outputs[i] = tp;
        }

        curr_txns.push_back(txn);
        int pos=curr_txns.size();
        
    }

    batch_file.close();
    Total_txns = curr_txns.size();

    thread threads[th_count];
	for(i=0;i<th_count;i++)	{threads[i]= thread(add_nodes,i); }//starting writer threads
	for(i=0;i<th_count;i++){threads[i].join(); }//joining writer threads
    DAG_write();
    return curr_txns.size();

}
};
int main()
{
    Geek t;
    int k= t.DAG_create();
    return 0;
}

extern "C"
{
    Geek *Geek_new() { return new Geek(); }
    void DAG_prune(Geek *geek) { geek->DAG_prune(); }
    int DAG_create(Geek *geek) { geek->DAG_create(); }
    int DAG_select(Geek *geek) { geek->DAG_select(); }
    void DAG_create2(Geek *geek) { geek->DAG_create2(); }
    void DAG_delete(Geek *geek, int n) { geek->DAG_delete(n);}
    void DAG_undo(Geek *geek, int n) { geek->DAG_undo(n); }
}

