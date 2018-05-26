/* 046267 Computer Architecture - Spring 2017 - HW #3 */
/* Implementation (skeleton)  for the dataflow statistics calculator */

#include "dflow_calc.h"
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

typedef struct node{
    InstInfo inst;
    int latency;
    int depth;
    int depth_and_latency;
    int prog_depth;
    int src1_dep_idx;
    int src2_dep_idx;
} Node;

/////////////////// FUNC DECLAR /////////////////
Node* buildDependencies(InstInfo arr[], int size, const unsigned int opsLatency[]);
void printDependencies(Node* arr, int size);
int recursiveDepthCalc(Node* arr, unsigned int idx);
void depthCalcWrapper(Node* node, int size);
void setProgDepth(Node* arr, int size);
/////////////////// DECLAR END /////////////////


ProgCtx analyzeProg(const unsigned int opsLatency[],  InstInfo progTrace[], unsigned int numOfInsts) {

    if(numOfInsts <=0) return PROG_CTX_NULL;

    Node* dependencies = buildDependencies(progTrace,numOfInsts,opsLatency);

    // Calc the depths of each cell
    depthCalcWrapper(dependencies,numOfInsts);

    // Calc the prog depth
    setProgDepth(dependencies,numOfInsts);

    //printDependencies(dependencies,numOfInsts);

    return dependencies;
}

void freeProgCtx(ProgCtx ctx) {
    free(ctx);
}

int getInstDepth(ProgCtx ctx, unsigned int theInst) {
    if(theInst < 0 || ctx==NULL) return -1;
    Node* temp = (Node*)ctx;
    return temp[theInst].depth;
}

int getInstDeps(ProgCtx ctx, unsigned int theInst, int *src1DepInst, int *src2DepInst) {
    if(theInst < 0 || ctx==NULL) return -1;
    Node* temp = (Node*)ctx;
    *src1DepInst = temp[theInst].src1_dep_idx;
    *src2DepInst = temp[theInst].src2_dep_idx;
    return 0;
}

int getProgDepth(ProgCtx ctx) {
    if(ctx==NULL) return 0;
    Node* temp = (Node*)ctx;
    return temp[0].prog_depth;
}

// This is debug function.
void printDependencies(Node* arr, int size){
    for(int i=0; i<size ; i++){
        printf("%d | %d: %d %d %d | deps: %d %d | latency: %d | depth: %d | prog depth: %d | depth+lat:%d\n",i,arr[i].inst.opcode,
               arr[i].inst.dstIdx,arr[i].inst.src1Idx,arr[i].inst.src2Idx,
                arr[i].src1_dep_idx,arr[i].src2_dep_idx,arr[i].latency,arr[i].depth,
               arr[i].prog_depth,arr[i].depth_and_latency);
    }
    printf("\n");
}

// This function builds the array of all data.
Node* buildDependencies(InstInfo arr[], int size, const unsigned int opsLatency[]){
    Node* nodes_array = malloc(sizeof(Node)*size);
    assert(nodes_array != NULL);

    //Copy data
    for(int i=0;i<size;i++){
        nodes_array[i].inst.opcode = arr[i].opcode;
        nodes_array[i].inst.dstIdx = arr[i].dstIdx;
        nodes_array[i].inst.src1Idx = arr[i].src1Idx;
        nodes_array[i].inst.src2Idx = arr[i].src2Idx;
        nodes_array[i].src1_dep_idx = -1;
        nodes_array[i].src2_dep_idx = -1;
        nodes_array[i].depth = 0;
        nodes_array[i].prog_depth = 0;

        // add latency
        nodes_array[i].latency = opsLatency[nodes_array[i].inst.opcode];
    }

    //Go from the end to the beginning
    for(int i=size-1;i>0;i--){

        int src1 = nodes_array[i].inst.src1Idx;
        int src2 = nodes_array[i].inst.src2Idx;

        // Find deps for src1
        for(int j=i-1; j>=0; j--){
            int dst = nodes_array[j].inst.dstIdx;
            if(dst == src1){
                // src1 depends on dst in index j;
                nodes_array[i].src1_dep_idx = j;
                break;
            }

        }

        // Find deps for src2
        for(int j=i-1; j>=0; j--) {
            int dst = nodes_array[j].inst.dstIdx;
            if (dst == src2) {
                // src2 depends on dst in index j
                nodes_array[i].src2_dep_idx = j;
                break;
            }
        }
    }

    return nodes_array;
}

// Recursion wrapper
void depthCalcWrapper(Node* node, int size){
    for(int i=size-1;i>0;i--){
        (void)recursiveDepthCalc(node,i);
    }

}

// A recursive function which calculates a depth for each cell.
int recursiveDepthCalc(Node* arr, unsigned int idx){
    //stop condition
    if (idx == 0){
        //printf("stop condition was reached: return latency: %d\n",arr[idx].latency);
        return arr[idx].latency;
    }

    int current_time = arr[idx].latency;

    // Calculate time for src1 dependency:
    int src1_dep_index = arr[idx].src1_dep_idx;
    int time_src1_dep = 0;
    if (src1_dep_index != -1){
        time_src1_dep = recursiveDepthCalc(arr,src1_dep_index);
    }


    // Calculate time for src2 dependency:
    int src2_dep_index = arr[idx].src2_dep_idx;
    int time_src2_dep = 0;
    if (src2_dep_index != -1){
        time_src2_dep = recursiveDepthCalc(arr,src2_dep_index);
    }

    int result;

    // Return the longest time + current time
    if(time_src1_dep >= time_src2_dep && time_src1_dep != 0){
        //Update current depth
        arr[idx].depth = time_src1_dep;
        result = current_time+time_src1_dep;
    } else if(time_src1_dep < time_src2_dep && time_src2_dep != 0) {
        //Update current depth
        arr[idx].depth = time_src2_dep;
        result = current_time+time_src2_dep;
    } else { // No dependencies.
        result = current_time;
    }

    if(idx == 1){
        //printf("result for idx=1 is: %d, time_src1=%d, time_src2=%d \n",result,time_src1_dep,time_src2_dep);
    }

    return result;
}

// This function sets for each cell in array the sum of depth and latency.
// Then it updates the prog_depth filed for the samr maximum value we got.
void setProgDepth(Node* arr, int size){
    int max = 0;

    // Set Depth and latency for each cell.
    for(int i=0;i<size;i++) {
        arr[i].depth_and_latency = arr[i].depth + arr[i].latency;
    }

    // Calc max val
    for(int i=0;i<size;i++) {
        if(max < arr[i].depth_and_latency){
            max = arr[i].depth_and_latency;
        }
    }

    // Set for each cell the prog depth
    for(int i=0;i<size;i++) {
        arr[i].prog_depth = max;
    }
}