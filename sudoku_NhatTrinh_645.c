#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>

// the minimum number of threads is 11
// this program uses 11 worker threads
// this program uses 1 parent thread to spawn 11 worker threads
const int NUM_THREADS = 11;
// hard coded 9x9 sudoku grid
const int grid[9][9] = {
        {6, 5, 3, 1, 2, 8, 7, 9, 4},
        {1, 7, 4, 3, 5, 9, 6, 8, 2},
        {9, 2, 8, 4, 6, 7, 5, 3, 1},
        {2, 8, 6, 5, 1, 4, 3, 7, 9},
        {3, 9, 1, 7, 8, 2, 4, 5, 6},
        {5, 4, 7, 6, 9, 3, 2, 1, 8},
        {8, 6, 5, 2, 3, 1, 9, 4, 7},
        {4, 1, 2, 9, 7, 5, 8, 6, 3},
        {7, 3, 9, 8, 4, 6, 1, 2, 5}};

// data structure to pass in data to threads
typedef struct {
    int row;
    int column;
} parameters;

// function belonging to worker threads
void * worker_run(void * arg) {
    // parameters passed in
    parameters * p;
    p = (parameters * ) arg;
    
    // variables to take care of status code returning
    // success is 1, failure is 2
    void * passed;
    int success = 1;
    passed = malloc(sizeof(success));
    
    // check if parameters passed in are valid for 9x9 grid
    if (p->row > 8 || p->row < 0 || p->column > 8 || p->column < 0){
        free(arg);
        success = 3;
        passed = &success;
        return passed;
    }
    
    // loop for worker threads assigned with checking 9 segments of sudoku
    if (!(p->row==8  && p->column==0) && !(p->row==8 && p->column==8)){
        // array to check off which number has been seen
        int seen [10] = {1,0,0,0,0,0,0,0,0};
        // this worker thread will traverse 3x3 grid to check sudoku
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                // if number in sudoku is not seen, switch index in seen array to 1
                if (seen[grid[i + p->row][j + p->column]] == 0)
                    seen[grid[i + p->row][j + p->column]] = 1;
                // else if number is already seen within this segment
                // set code to 2 for failure
                else{
                    success = 2;
                }
            }
        }
    }
    
    // code to for two worker threads that check columns and rows
    else {
        // starting indexes for this worker thread
        int row_start, column_start = 0;
        
        // check if data passed in is valid for this 9x9 sudoku grid
        if (p->row==8  && p->column==0 || p->row==8 && p->column==8){
            row_start = p->row;
            column_start = p->column;
        }
        // else this function will return 2 for failure of sudoku
        else
            success = 2;
        
        // this code segment is for worker thread that checks vertically
        if (p->column == 0){
            // same strategy as other worker thread loops
            // except this loop will check vertically
            /* this loop will check the sudoku from bottom left
             to top right
             */
            for (int a = row_start; a >= 0; a--){
                int seen[10] = {1,0,0,0,0,0,0,0,0,0};
                for (int b = column_start; b < 9; b++){
                    if (seen[grid[a][b]] == 0)
                        seen[grid[a][b]] = 1;
                    else
                        success = 2;
                }
            }
        }
        else{
            /* this worker thread will check the sudoku horizontally
             from the bottom right to top left of the sudoku grid
             */
            for (int a = row_start; a >= 0; a--){
                int seen[10] = {1,0,0,0,0,0,0,0,0,0};
                for (int b = column_start; b >= 0; b--){
                    if (seen[grid[a][b]] == 0 && grid[a][b] > 0 && grid[a][b] <= 9){
                        seen[grid[a][b]] = 1;
                    }
                    else{
                        success = 2;
                }
            }
        }
        }}
    // free the memory when the action is done
    free(arg);
    // passed is pointing to the status code of the function
    passed = &success;
    return passed;
}

// function for parent thread
void * parent_run(void * arg) {
    
    // initialize an array of worker threads
    pthread_t worker_threads[NUM_THREADS];
    
    // walkers for column start and row start
    int column = 0;
    int row = 0;
    
    // return value representing status code for parent thread
    void * passed;
    // count the number of worker threads that return 1 (success)
    int total_passed = 0;
    
    // variable to check the right dimensions of the grid
    int testgrid[9][9];
    // return 3 if grid size invalid
    if (sizeof(grid) != sizeof(testgrid)){
        int invalid = 3;
        passed = &invalid;
        return passed;
    }
    
    // loop to create NUM_THREADS (11) - 2 because ..
    // .. these threads check 9 segments of sudoku
    // last two threads will check horizontal rows ..
    // .. and vertical columns
    for (int i = 0; i < NUM_THREADS; i++) {
        // passing parameters to each thread
        parameters * data = (parameters *) malloc(sizeof (parameters));
        
        // for the 9 remaining worker threads that check 9 segments
        if (i > 1) {
            // the parameters passed into the function are stored in a local variable
            data->column = column;
            data->row = row;
            // create the worker thread
            pthread_create(&worker_threads[i], NULL, worker_run, data);
            // the status of the worker thread is stored in the void pointer passed
            pthread_join(worker_threads[i],&passed);
            // if passed is set to 1 then worker passed, increment the total threads passed
            if (*(int*)passed == 1)
                total_passed++;
            // if the column is still under the range of the next 6, increment by 3
            if (column < 6)
                column += 3;
            // if the column reached the end, reset it to 0th index
            else if (column == 6)
                column = 0;
            // if the row is under 6 increment it by 3
            // row can never exceed 6
            if (row < 6)
                row += 3;
        }
        // for setting starting point of worker thread 2 that checks columns
        else if (i == 1) {
            // start at bottom right of sudoku
            data->column = 8;
            data->row = 8;
            pthread_create(&worker_threads[i], NULL, worker_run, data);
            pthread_join(worker_threads[i],&passed);
            // if worker thread passed increase the count
            if (*(int*)passed == 1)
                total_passed++;
        } 
        // for the first worker thread checking the rows
        else if (i == 0) {
            // this thread starts at bottom left of sudoku
            data->column = 0;
            data->row = 8;
            pthread_create(&worker_threads[i], NULL, worker_run, data);
            pthread_join(worker_threads[i],&passed);
            // if pthread returns 1 then success and increment total passed variable
            if (*(int*)passed == 1)
                total_passed++;
        }
    }
    // if the number of threads passed does not equal to number of threads created, then fail
    // 2 is the code for sudoku failure
    if (total_passed != NUM_THREADS){
        int failure = 2;
        passed = &failure;
    }
    return passed;
}

// function to print sudoku at the end of the program
void print_sudoku(){
    printf("\n");
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            printf(" %i",grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char** argv) {
    // greet the user
    printf("Sudoku Checker from Nhat Trinh.\n");
    
    // variable for parent thread
    pthread_t parent_thread;
    // variable to take care of parent thread return status code
    void * passed;
    
    pthread_create(&parent_thread, NULL, parent_run, NULL);
    pthread_join(parent_thread,&passed);
    
    //print the sudoku
    print_sudoku();
    
    // if parent thread returns 1 sudoku has passed test
    if (*(int*)passed == 1)
        printf("Your sudoku is valid.\n");
    // if parent thread returns 2 sudoku invalid
    else if (*(int*)passed == 2)
        printf("Your sudoku has invalid.\n");
    
    return 0;
}
