// Yunjia Xi 517030910102
// Part A  simulates the behavior of a cache memory
#include "cachelab.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>	// for uintN_t 
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>


struct cache_line{
    int vaild;        //flag whether this line is valid
    uint64_t tag;     //identifier of block stored
    uint64_t LRUtime; //evict the block who has the min time
};

typedef struct cache_line* cache_set;
cache_set *cache;    
int verbose = 0;      //flag whether switch to verbose mode
uint64_t clock = 0;   //record time
uint64_t hitNum;      //total number of hit
uint64_t missNum;     //total number of miss
uint64_t evictionNum; //total number of eviction

//detect hit, miss and eviction
void Detect(uint64_t setIdx, uint64_t tag, uint64_t lineNum) 
{
    cache_set set = cache[setIdx];
    int hit = 0;
    uint64_t oldestBlock;
    uint64_t minTime = UINT64_MAX;

    clock++; //clock incerese, youngest block has largest clock time
    
    for (uint64_t i = 0; i < lineNum; i++)
        if (set[i].vaild && set[i].tag == tag){ //check hit
            hit = 1;
            set[i].LRUtime = clock; //update LRUtime
            hitNum++;
            if (verbose)
                printf(" hit ");
            break;
        }

    if(!hit)
    {
        missNum++;
        if (verbose)
            printf(" miss ");

        // find the oldest block(block with min time)
        // invaild blocks are also oldest for they must be 0
        for(uint64_t i = 0; i < lineNum; i++) 
            if(set[i].LRUtime < minTime){
                minTime = set[i].LRUtime;
                oldestBlock = i;
            }

        //check weather it's an eviction or invaild block
        if (!set[oldestBlock].vaild){  // invaild block exists
            set[oldestBlock].vaild = 1; 
        } 
        else{                          // evict a block
            evictionNum++;
            if (verbose)
                printf(" eviction");
        }

        //update tag and LRUtime   
        set[oldestBlock].tag = tag;
        set[oldestBlock].LRUtime = clock;
    }
}


int main(int argc, char** argv){
    int opt, b, s;
    uint64_t S, E;
    FILE* traceFile;
    char* helpMessage = "Usage: \"program name\" [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n";

    //get parameter
    while(-1 != (opt = getopt(argc, argv, "hvE:b:s:t:"))){	
        // determine which argument it’s processing 
        switch(opt) { 
            case 'h':                                   
                printf("%s\n", helpMessage);            // print help message
				break;
            case 'v':
                verbose = 1;                            //set verbose flag
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 's':
                s = atoi(optarg);
				S = 1 << s;                             // S is the number of set
                break;
            case 't':
                if ((traceFile = fopen(optarg, "r")) == NULL) { //open file for reading
					printf("Failed to open tracefile");
					exit(0);
				}   
                break;
            default:
                printf("wrong argument\n %s",helpMessage);
                break;
        }
    }

    //allocate memory for cache
    //use calloc for it can initialize to 0
    if ((cache = calloc(S, sizeof(cache_set))) == NULL) {// initialize the sets 
        printf("Failed to calloc set");
        exit(0);
    }
    for(int i = 0; i < S; i++){                          // initialize the lines in sets 
        if ((cache[i] = calloc(E, sizeof(struct cache_line))) == NULL){
            printf("Failed to calloc line in sets");
            exit(0);
        }
    }

    //read and detect
    char operation;
    uint64_t address;  //store hexadecimal address
    int size;
    uint64_t tag;
    uint64_t setIdx;   //set index

    while(fscanf(traceFile," %c %lx,%d", &operation, &address, &size)>0) //use lx for hexadecimal address
    {
	    if (operation != 'I'){              //omit 'I' operation
            setIdx = (address >> b) & (S - 1); 
            tag = address >> (s + b);
            if (verbose)    
                printf("%c %lx ", operation, address);
            Detect(setIdx, tag, E);         //detection for L/M/S
            if (operation == 'M')
                Detect(setIdx, tag, E);     //second detection for M
            if (verbose)
                printf("\n");
        }
    }
    fclose(traceFile);                      //close file when done


    //free memory
    for (int i = 0; i < S; ++i){
        free(cache[i]);
    }
    free(cache);

    printSummary(hitNum, missNum, evictionNum);
    return 0;
}
