/* Author: Aftab Ahmad aftab@acm.org
/* This program codes the following types of hello world
1. Hello world! using printf() function
2. Hello world using fprintf() function with stdout as the outputfile (same result as 1.
3. Hello wrold, where hello world is the argv[1] string, such as the following
prompt$./a.out "hello world!"
4. Hello world from a file pointed to by inputfile with filename program1.in
5. Same as 4 except that the output is printed in a file program1.out pointed to by outputfile
*/
/* On Ubuntu Linux with gcc, everything works fine. Here are example command lines
    $gcc -o hw program1.c
    $./hw "Y'ello!"
*/
/* On Windows 10 with Code::Blocks, everything except printing out what is read
    from program.in works fine. On Windows, string passing has issues.
    It might be the reason why Windows has its own getche() instead of promoting
    the use of getc() or getchar()/putchar().
*/

#include<stdio.h>

int main (int argc, char** argv) {
	printf("\n");
	printf("Hello world! (well, using printf())\n\n");
	fprintf(stdout, "Hello world, again! (using fprintf with stdout as output file)\
	\n\n");
	fprintf(stdout,"Using argv[1] as string: %s\n\n", argv[1]);

	FILE *inputfile;
	char message;
	inputfile = fopen("program1.in", "r");

	fprintf(stdout, "Here is the message in program1.in file\n\n\n");
	while ((message = fgetc(inputfile)) != EOF)
		putchar(message);
	fprintf(stdout, "\n\n");

	rewind(inputfile);
	FILE *outputfile;
	outputfile = fopen("program1.out", "w");
	fprintf(stdout, "A message has been sent to program1.out by program1.in. Please\
	check!\n\n");
	while ((message = fgetc(inputfile)) != EOF)
		fputc(message, outputfile);

	fclose(inputfile);
	fclose(outputfile);
}
