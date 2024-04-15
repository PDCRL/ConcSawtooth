class Adj_DAG
{
public:
    int txn_count;
    int txn_adj[1000][1000]={0};
    atomic_int in_deg[1000];

    Adj_DAG(int txn_count){
        //this->txn_count = txn_count;
        //txn_adj= new int*[txn_count];
        //in_deg= new atomic_int[txn_count];
    }
    void add_edge (int from_id,int to_id){
        printf("%d,%d \n",from_id,to_id);
          txn_adj[from_id][to_id]=1;
//        in_deg[to_id]++;
    }
    int Select_Txn()
    {
        int count,var_zero=0;
        atomic<int> test;
        bool flag=false;
        int completed=0;
        for (int i = 0; i < txn_count; i++){
            if(in_deg[i]==0)
            {
                
                if(in_deg[i].compare_exchange_strong(var_zero,-1))
                {
                    flag=true;
                    break;
                }
            }
            if(in_deg[i] == -1)
                completed++;

            if(flag)
            {
                return i;
            }
        }

        if(completed == txn_count)
            return -1; //signifies all complete

        //sleep_until(std::chrono::time_point<std::chrono::system_clock>::max());
           

    }
    void update_edges(int txn_id)
    {
        for (int i=0;i<txn_count;i++) {
            if (txn_adj[txn_id][i] == 1) {
                in_deg[i]--;

                
            }
        }
    }
    int in_deg_txn(int txn_id)
    {
        int row_numbers = sizeof(txn_adj)/sizeof(txn_adj[0]);
        int sum = 0;
        for(int i=0;i<row_numbers;i++)
        {
            sum  = sum + txn_adj[i][txn_id];
        }
        
    }
};
