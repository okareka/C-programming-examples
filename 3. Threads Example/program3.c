/* Author Aftab Ahmad aftab@acm.org */
/* This program demonstrates the use of threads from the C language */
/* pthread library. */
/* It reads 10 X 10 matrix from a file pointed to by argv[1] by using a thread for each row */
/* Then it writes the row transposed to the stdout */

/* HOW THIS WORKS? /
/* There are 10 row of decimal digits in the file program3.in. */
/* These rows have all 0's, all 1's, a;; 2's, and so on. This is the data, a 2D matrix */
/* The function read_Matrix() reads it into a 2D array and returns the array */
/* The thread function row() is passed the row number and the data matrix */
/* The function row() prints out the row number passed from the data matrix passed to standard output */
/* Ten threads print out ten rows as ten columns */
/* If you uncomment thread_join(), then a thread can be interrupted. */
/* In that case, the printing out of rows will not be uninterrupted */
/* and you may see a violation of the order of data. */
/* On Ubuntu Linux using the following compiling and execution commands work flawlessly"*/
/*  $gcc -o threads program3.c -pthread
    $./threads program3.in  */
/* On Windows 10, using Code::Blocks, after adding argv[1] and linking pthread */
/* the program runs but does not give correct output. It looks like the data is */
/* not passed from inside read_Matrix() even though the whole idea of defining */
/* Matrix was to avoid this problem. Passing the Matrix as a pointer does not work either */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

#define NUM_OF_ROWS	10
#define NUM_OF_COLS	10
#define NUM_OF_THREADS 10

/* By defining 2D array as a struct, it is easier to pass it in functions unlike arrays */
typedef struct {
    int input[NUM_OF_ROWS][NUM_OF_COLS];
}
Matrix;

/* Following type is defined for passing data matrix and row number to the thread function*/
typedef struct {
    Matrix data;
    int number;
}
Row_data;

/* read 2D data from file f and return it as Matrix*/
Matrix read_Matrix(FILE *f);

/* To be the thread function. Write row data transposed to the stdout*/
void *row(void *rowdata);

int main (int argc, char* argv[]) {

	FILE *input_file;
	input_file = fopen(argv[1], "r"); /* Open for reading the file passed as argv[1]*/

    pthread_t threads[NUM_OF_THREADS]; /* To return thread id by thread_create() */

    Row_data row_data; /* To pass data Matrix and row number to each thread */

	row_data.data = read_Matrix(input_file); /* Read data matrix to row_data.data from input file */

/* Print data Matrix to check the read_Matrix() worked fine */
    printf("\nHere is the data read from the input file: \n");
   	for (int k = 0; k < NUM_OF_ROWS; ++k) {
            for (int m = 0; m < NUM_OF_COLS; ++m)
                fprintf(stdout,"%d\t", row_data.data.input[k][m]); /* Tab-spaced row data*/
                fprintf(stdout,"\n"); /* Next row */
    	}
/* Create and run threads */
    printf("\nHere is the data transposed with each row as a thread with one sec pause: \n");
	for (int t = 0; t < NUM_OF_THREADS; ++t) {
		row_data.number = t; /* Threads correspond to Row numbers */
		pthread_create(&threads[t], NULL, row,&row_data);
		pthread_join(threads[t], NULL); /* It is the wait() equivalent. Try with and without this */
    }
//	pthread_exit(NULL); /* Try also by uncommenting this. What is the difference and why?*/
	fprintf(stdout,"\n\n That's all folks!\n\n");
	fclose(input_file);
	return 0;
}

Matrix read_Matrix(FILE *inf) {
	Matrix d;
    	for (int k = 0; k < NUM_OF_ROWS; ++k)
		for (int m = 0; m < NUM_OF_COLS; ++m)
	        	fscanf(inf,"%d[^\t]\n",&d.input[k][m]); /* read tabbed data from file *inf until \n*/
    return d;
}

void *row(void *rowdata) {

	Row_data *r_d = (Row_data *) rowdata; /* assign rowdata to local r_d struct*/

	sleep(1); /* 1 second */

	for (int k = 0; k < NUM_OF_COLS; ++k)
        fprintf(stdout, "%d\t",r_d->data.input[k][r_d->number]);
        fprintf(stdout, "\n");
}
