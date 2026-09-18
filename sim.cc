#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim.h"
#include <vector>
#include <cmath>
#include <string>
#include <cstdint>
#include <inttypes.h>
#include <sstream> 
#include <algorithm>
#include <math.h>
#include <iostream>
#include <iomanip>  // For setw()
//#include "sim.h"
using namespace std;

int correct_pred=0;
int total_pred=0;
int miss_pred=0;
float misspred_rate=0;
float misspred_percentage;


struct bimodal {
    
    unsigned long int branch_pred;
    unsigned long int index;

    bimodal() : branch_pred(2),index(0) {}

};

struct gshare {
    //unsigned long int bhr;
    unsigned long int branch_pred;
    unsigned long int index;

    gshare(): branch_pred(2), index(0) {}
};

struct hybrid {
    unsigned long int chooser;
    unsigned long int branch_pred;
    unsigned long int index;

    hybrid(): chooser(1), branch_pred(2), index(0) {}
};

class Branch_Pred
{
    public:
    Branch_Pred(unsigned long int k, unsigned long int M1, unsigned long int M2,
     unsigned long int N, char* Name)
        : k(k), M1(M1), M2(M2), N(N),Name(Name), bhr(0) 
        {
        if(M2!=0)
        {
            sets_bi=pow(2,M2);
            bimodalCounter.resize(sets_bi);
        }

        if(M1!=0)
        {
            sets_gs=pow(2,M1);
            gshareCounter.resize(sets_gs);
        }

        if(k!=0)
        {
            sets_hy=pow(2,k);
            hybridCounter.resize(sets_hy);
        }


    }

    void check(unsigned long int addr, char outcome)
    {
        if(strcmp(Name, "bimodal") == 0)
        {
            index_bi=getbiIndex(addr);
            for(int i=0;i<sets_bi;i++)
            {
               if(i==index_bi)
               {
                if(outcome=='t')
                {
                    if(bimodalCounter[i].branch_pred==2 || bimodalCounter[i].branch_pred==3)
                    {
                        correct_pred++;
                        IncrementCounter_bi(i);
                        
                    }
                    else
                    {
                        miss_pred++;
                        IncrementCounter_bi(i);
                    }
                }
                else
                {
                    if(bimodalCounter[i].branch_pred==0 || bimodalCounter[i].branch_pred==1)
                    {
                        correct_pred++;
                        DecrementCounter_bi(i);
                    }
                    else
                    {
                        miss_pred++;
                        DecrementCounter_bi(i);
                    }
                }
               } 
            }
        }
        else if(strcmp(Name, "gshare") == 0)
        {
            int t=0;
            index_gs=getgsIndex(addr);
            //index_bi=getbiIndex(addr);
            for(int i=0;i<sets_gs;i++)
            {
               if(i==index_gs)
               {
                if(outcome=='t')
                {
                    t=1;
                    if(gshareCounter[i].branch_pred==2 || gshareCounter[i].branch_pred==3)
                    {
                        correct_pred++;
                        IncrementCounter_gs(i);
                        
                    }
                    else
                    {
                        miss_pred++;
                        IncrementCounter_gs(i);
                    }
                }
                else
                {
                    t=0;
                    if(gshareCounter[i].branch_pred==0 || gshareCounter[i].branch_pred==1)
                    {
                        correct_pred++;
                        DecrementCounter_gs(i);
                    }
                    else
                    {
                        miss_pred++;
                        DecrementCounter_gs(i);
                    }
                }
                updateBHR(t);
               } 
            }

        }
        else if(strcmp(Name, "hybrid") == 0)
        {
            int bi=0;
            int gs=0;
            int t_gs=0;
            int t_bi=0;
            unsigned long int in_bi;
            unsigned long int in_gs;

            index_gs=getgsIndex(addr);
                        for(int j=0;j<sets_gs;j++)
                        {
                        if(j==index_gs)
                        {
                            in_gs=j;
                            if(outcome=='t')
                            {
                                t_gs=1;
                                if(gshareCounter[j].branch_pred==2 || gshareCounter[j].branch_pred==3)
                                {
                                    gs=1;
                                    
                                }
                                else
                                {
                                    gs=0;
                                }
                            }
                            else
                            {
                                t_gs=0;
                                if(gshareCounter[j].branch_pred==0 || gshareCounter[j].branch_pred==1)
                                {
                                    gs=1;
                                }
                                else
                                {
                                    gs=0;
                                }
                            }
                        } 
                        }

            index_bi=getbiIndex(addr);
                        for(int p=0;p<sets_bi;p++)
                        {
                        if(p==index_bi)
                        {
                            in_bi=p;
                            if(outcome=='t')
                            {
                                t_bi=1;
                                if(bimodalCounter[p].branch_pred==2 || bimodalCounter[p].branch_pred==3)
                                {
                                    bi=1;
                                    
                                }
                                else
                                {
                                    bi=0;
                                }
                            }
                            else
                            {
                                t_bi=0;
                                if(bimodalCounter[p].branch_pred==0 || bimodalCounter[p].branch_pred==1)
                                {
                                    bi=1;
                                }
                                else
                                {
                                    bi=0;
                                }
                            }
                        } 
                        }

            index_hy=gethyIndex(addr);
            for(int i=0;i<sets_hy;i++)
            {
                if(i==index_hy)
                {
                    if(hybridCounter[i].chooser>=2)
                    {
                        if(gs==0)
                        {
                            miss_pred++;
                            if(t_gs==1)
                            {
                                IncrementCounter_gs(in_gs);
                            }
                            else
                            {
                                DecrementCounter_gs(in_gs);
                            }
                            updateBHR(t_gs);
                        }
                        else
                        {
                            correct_pred++;
                            if(t_gs==1)
                            {
                                IncrementCounter_gs(in_gs);
                            }
                            else
                            {
                                DecrementCounter_gs(in_gs);
                            }
                            updateBHR(t_gs);
                        }
                    }
                    else
                    {
                        if(bi==0)
                        {
                            miss_pred++;
                            if(t_bi==1)
                            {
                                IncrementCounter_bi(in_bi);
                            }
                            else
                            {
                                DecrementCounter_bi(in_bi);
                            }
                            updateBHR(t_bi);
                        }
                        else
                        {
                            correct_pred++;
                            if(t_bi==1)
                            {
                                IncrementCounter_bi(in_bi);
                            }
                            else
                            {
                                DecrementCounter_bi(in_bi);
                            }
                            updateBHR(t_bi);
                        }
                    }

                    updateChooser(i,gs,bi);


                }
            }
        }
    }

    void updateChooser(unsigned long int index, int gs, int bi)
    {
        if(gs==1 && bi==0)
        {
            if(hybridCounter[index].chooser<3)
            {
                hybridCounter[index].chooser++;
            } 
        }
        else if(gs==0 && bi==1)
        {
            if(hybridCounter[index].chooser>0)
            {
                hybridCounter[index].chooser=hybridCounter[index].chooser-1;
            }
        }
    }

    void IncrementCounter_bi(unsigned long int index)
    {
        if(bimodalCounter[index].branch_pred<3)
        {
            bimodalCounter[index].branch_pred++;
        }
    }

    void DecrementCounter_bi(unsigned long int index)
    {
        if(bimodalCounter[index].branch_pred>0)
        {
            bimodalCounter[index].branch_pred=bimodalCounter[index].branch_pred -1;
        }
    }

    void IncrementCounter_gs(unsigned long int index)
    {
        if(gshareCounter[index].branch_pred<3)
        {
            gshareCounter[index].branch_pred++;
        }
    }

    void DecrementCounter_gs(unsigned long int index)
    {
        if(gshareCounter[index].branch_pred>0)
        {
            gshareCounter[index].branch_pred=gshareCounter[index].branch_pred -1;
        }
    }

    void updateBHR(int t)
    {
        bhr= (bhr >> 1) | (t << (N - 1));
    }

    void DisplayBi()
    {
        cout<<left<<"FINAL BIMODAL CONTENTS"<<endl;
        for(int i=0;i<sets_bi;i++)
        {
            cout<<" "<<i<<"\t"<<bimodalCounter[i].branch_pred<<endl;
        }
    }

    void DisplayGS()
    {
        cout<<left<<"FINAL GSHARE CONTENTS"<<endl;
        for(int i=0;i<sets_gs;i++)
        {
            cout<<" "<<i<<"\t"<<gshareCounter[i].branch_pred<<endl;
        }
    }

    void DisplayHy()
    {
        cout<<left<<"FINAL CHOOSER CONTENTS"<<endl;
        for(int i=0;i<sets_hy;i++)
        {
            cout<<" "<<i<<"\t"<<hybridCounter[i].chooser<<endl;
        }
    }



    unsigned long int getbiIndex(unsigned long int addr)
    {
        return (addr >> 2) & ((1 << M2) - 1);
    }

    unsigned long int getgsIndex(unsigned long int addr)
    {
        unsigned long int index=(addr >> 2) & ((1 << M1) - 1);
        unsigned long int upperNbits= (index >> (M1 - N)) & ((1 << N) - 1);
        unsigned long int xorindex= upperNbits ^ (bhr & (1 << N) - 1);
        return  (index & ((1 << (M1 - N)) - 1)) | (xorindex << (M1 - N));
    }

    unsigned long int gethyIndex(unsigned long int addr)
    {
        return (addr >> 2) & ((1 << k) - 1);
    }

    public:
    unsigned long int k;
    unsigned long int M1;
    unsigned long int M2;
    unsigned long int N;
    char* Name;

    unsigned long int sets_bi;
    unsigned long int sets_gs;
    unsigned long int sets_hy;

    unsigned long int index_bi;
    unsigned long int index_gs;
    unsigned long int index_hy;

    unsigned long int bhr;

    /*vector<vector<CacheBlock>> cache;
    vector<StreamBuffer> MultiBuffers;*/
    vector<bimodal> bimodalCounter;
    vector<gshare> gshareCounter;
    vector<hybrid> hybridCounter;


};

/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim bimodal 6 gcc_trace.txt
    argc = 4
    argv[0] = "sim"
    argv[1] = "bimodal"
    argv[2] = "6"
    ... and so on
*/
int main (int argc, char* argv[])
{
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    bp_params params;       // look at sim_bp.h header file for the the definition of struct bp_params
    char outcome;           // Variable holds branch outcome
    unsigned long int addr; // Variable holds the address read from input file
    
    if (!(argc == 4 || argc == 5 || argc == 7))
    {
        printf("Error: Wrong number of inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }
    
    params.bp_name  = argv[1];
    
    // strtoul() converts char* to unsigned long. It is included in <stdlib.h>
    if(strcmp(params.bp_name, "bimodal") == 0)              // Bimodal
    {
        if(argc != 4)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.M2       = strtoul(argv[2], NULL, 10);
        trace_file      = argv[3];

        //make rest zero
        params.M1=0;
        params.K=0;
        params.N=0;

        printf("COMMAND\n%s %s %lu %s\n", argv[0], params.bp_name, params.M2, trace_file);
    }
    else if(strcmp(params.bp_name, "gshare") == 0)          // Gshare
    {
        if(argc != 5)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.M1       = strtoul(argv[2], NULL, 10);
        params.N        = strtoul(argv[3], NULL, 10);
        trace_file      = argv[4];

        //make rest zero
        params.M2=0;
        params.K=0;
        //params.N=0;

        printf("COMMAND\n%s %s %lu %lu %s\n", argv[0], params.bp_name, params.M1, params.N, trace_file);

    }
    else if(strcmp(params.bp_name, "hybrid") == 0)          // Hybrid
    {
        if(argc != 7)
        {
            printf("Error: %s wrong number of inputs:%d\n", params.bp_name, argc-1);
            exit(EXIT_FAILURE);
        }
        params.K        = strtoul(argv[2], NULL, 10);
        params.M1       = strtoul(argv[3], NULL, 10);
        params.N        = strtoul(argv[4], NULL, 10);
        params.M2       = strtoul(argv[5], NULL, 10);
        trace_file      = argv[6];
        printf("COMMAND\n%s %s %lu %lu %lu %lu %s\n", argv[0], params.bp_name, params.K, params.M1, params.N, params.M2, trace_file);

    }
    else
    {
        printf("Error: Wrong branch predictor name:%s\n", params.bp_name);
        exit(EXIT_FAILURE);
    }
    
    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }
    
    Branch_Pred predTable(params.K,  params.M1, params.M2,
     params.N, params.bp_name);

    char str[2];
    while(fscanf(FP, "%lx %s", &addr, str) != EOF)
    {
        
        outcome = str[0];

       total_pred++;
       predTable.check(addr,outcome); 

    }


    misspred_rate=static_cast<float>(miss_pred)/total_pred;
    misspred_percentage=misspred_rate*100;

    cout<<"OUTPUT"<<endl;
    cout << left; // Left align the text
    cout << setw(27) << " number of predictions:" <<left<< total_pred << endl;
    cout << setw(27) << " number of mispredictions:" <<left<< miss_pred << endl;
    cout << setw(27) << " misprediction rate:" <<left<< fixed << setprecision(2) << misspred_percentage << "%" << endl;

    if(strcmp(params.bp_name, "bimodal") == 0)
    {
        predTable.DisplayBi();
    }
    else if(strcmp(params.bp_name, "gshare") == 0)
    {
        predTable.DisplayGS();
    }
    else if(strcmp(params.bp_name, "hybrid") == 0)
    {
        predTable.DisplayHy();
        predTable.DisplayGS();
        predTable.DisplayBi();
    }

    return 0;
}
