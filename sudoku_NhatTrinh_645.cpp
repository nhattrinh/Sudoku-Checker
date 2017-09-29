#include <cstdlib>
#include <stdio.h>
#include <pthread.h>

using namespace std;

const int NUM_THREADS = 11;

typedef struct{
    int row;
    int column;
    } parameters;
    
    
    void run(void * arg){
        
        
        // free the memory when the action is done
        free(arg);
    }
    
int main(int argc, char** argv) {
    // hard coded sudoku grid
    int grid[9][9] = {{6,5,3,1,2,8,7,9,4},
    {1,7,4,3,5,9,6,8,2},{9,2,8,4,6,7,5,3,1},
    {2,8,6,5,1,4,3,7,9},{3,9,1,7,8,2,4,5,6},
    {5,4,7,6,9,3,2,1,8},{8,6,5,2,3,1,9,4,7},
    {4,1,2,9,7,5,8,6,3},{7,3,9,8,4,6,1,2,5}};
    
    // passing parameters to each thread
    parameters * data [NUM_THREADS] = (parameters *) malloc(sizeof(parameters) * NUM_THREADS);
    
    int row = 0;
    int column = 0;
    
    pthread_t threads[NUM_THREADS];
    
    for (int i = 0; i < NUM_THREADS; i++){
        if (i > 1){
            data[i]->column=column;
            data[i]->row=row;
            pthread_create(thread[i], NULL, data[i]);
            if (column < 6)
                column += 3;
            else if (column == 6)
                column = 0;
            if (row < 6)
                row += 3;
        }
        else if (i == 1){
            data[1]->column=8;
            data[1]->row=8;
            pthread_create(threads[i], NULL, data[i]);
        }
        else if (i == 0){
            data[0]->column=0;
            data[0]->row=8;
            pthread_create(threads[i], NULL, data[i]);
        }
    }
    
    
    return 0;
}

