#include <cstdlib>
#include <stdio.h>

using namespace std;

typedef struct{
    int row;
    int column;
    } parameters;

int main(int argc, char** argv) {
    // hard coded sudoku grid
    int grid[9][9] = {{6,5,3,1,2,8,7,9,4},
    {1,7,4,3,5,9,6,8,2},{9,2,8,4,6,7,5,3,1},
    {2,8,6,5,1,4,3,7,9},{3,9,1,7,8,2,4,5,6},
    {5,4,7,6,9,3,2,1,8},{8,6,5,2,3,1,9,4,7},
    {4,1,2,9,7,5,8,6,3},{7,3,9,8,4,6,1,2,5}};
    
    // passing parameters to each thread
    parameters * data = (parameters *) malloc(sizeof(parameters));
    data->row = 1;
    data->column = 1;
    
    return 0;
}

