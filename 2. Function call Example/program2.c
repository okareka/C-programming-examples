/* Author: Aftab Ahmad aftab@acm.org
This program demonstrates the use of functions in C for the uninitiated.
The program reads integer data as an array of integers from an inputfile program2.in,
rotates the data with a random amount and prints the resulting data to
the stdout and an output file program2.out.
It works in a loop until the user decides to not continue.
On Ubuntu linux, here is an example of compiling and using it
    $gcc -o fn program2.c
    $./fc
On Windows 10, using the same (maybe older version) compiler, GCC,
it does not do its job well. I tested on Code::Blocks
*/

#include<stdio.h>
#include<stdlib.h>
#include <time.h>

#define MAX_DATA_SIZE 100 /* To protect against buffer overflow */
#define MAX_PLAYS 100

/* function prototypes */

void printarray(int n, int d[]); /* print out the element values of integer array d[] of size n */
int* read_file_data(FILE* in, int *d_size, int d[]); /* read data of size d_size from file in */
int* rotate(int array_size, int in_array[], int out_array[]); /* randomly rotate the elements of array */
void write_file_data(FILE *out, int d_size, int data[]); /* write into file 'out' data from array data[] */

/*main function body */

int main (int argc, char** argv) {

	FILE *input_file = fopen("program2.in", "r"); /* File program2.in must be there or you get an error */
	FILE *output_file = fopen("program2.out", "w"); /* File program2.out does not have to be there */
	int data_size; /* The number of integers read from the data file */
	int *data_in, *data_out; /* pointers to data arrays to be used to pass arrays in functions */
	int data[MAX_DATA_SIZE] = {0}; /* array where data will be saved after reading it from the input file */

    srand(time(0)); /* set the seed of rand() to current time */
	data_in = read_file_data(input_file, &data_size, data); /* Function call to read_file()*/
	printf("\nHere is the original data\n");
	printarray(data_size, data); /* would also work if data is replaced with data_in */

    printf("\nHere is the randomly rotated data\n");
	char ans = 'y';
	do { /* user play loop starts here */
            /* Following are function calls to rotate(), printarray(), write_file() */
        data_out = rotate(data_size, data, data_out);/* C has this problem hard to pass arrays */
        printarray(data_size, data_out);
        write_file_data(output_file, data_size, data_out);
        fprintf(output_file, "\n");
        printf("\nEnter y to rotate again or any other key to exit.\n");
        scanf(" %c", &ans); /* space before %c needed to skip carriage return for subsequent runs */
	} while (ans == 'y');

    printf("\nThat's all folks!\n");
    fprintf(output_file, "\nThat's all folks!\n");

    /* closing files */
	fclose(input_file);
    fclose(output_file);

return 0; /* End of main () */
}

/* function definitions */
int* read_file_data(FILE *in, int *d_size, int d[]) {
    int j = 0;

    while (fscanf(in, "%d" , &d[j])== 1)/* As long as there is something to scan from file 'in'*/
        ++j;
        *d_size = j; /* return pointer to data size*/
        return d; /* return data array */
}
int* rotate (int array_size, int in_array[], int out_array[]) {

    int bias = rand() % array_size; /* rotate this much */
    int index = 0; /* index of the output array */
    static int out[MAX_DATA_SIZE]; /* static will allow it to be returned because it is not on stack*/

    for (int j = 0; j < array_size; ++j) {
            index = (bias + j) % array_size;
            out[index] = in_array[j];/*in_array[0] replaced by in_array[index] and so on */
            out_array = out;
    }
    return out_array;
}
void write_file_data(FILE* out, int d_size, int* data) {
    for (int j = 0; j < d_size; ++j)
    fprintf(out, "%d ", data[j]);
}
void printarray(int n, int d[]) {
    for (int j = 0; j < n; ++j)
        printf("%d ", d[j]);
        printf("\n");
}
