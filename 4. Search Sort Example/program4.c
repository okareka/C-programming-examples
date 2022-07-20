/* Author: Aftab Ahmad aftab@acm.org
The purpose of this program is to demonstrate the syntax for using search and sort
capabilities of the C language using the stdlib functions for comparing two entries
in an array and sorting/searching them.
The program is designed to be easily extended into making a multi-category employees
registration system.
A test database is created in the textfile named roster that is called master_roster
in the program and is passed as argv[1].
On gcc, if you use the following command:
$gcc -o ss searchsort.c
you get the executable ss. To run it, use
$./ss roster
Then you get seven choices in a user loop for doing various things to roster
including enrolling a new member, listing all records, searching
by first name, searching by last name, or first name, or ID and adding if ID
not found.
The program can easily be extended to make new rosters based on given criteria.
It has been tested on Ubuntu with gcc and Windows 10 with Code::Blocks
For sorting, qsort() is used and for searching lfind() is used.
A word of caution: if there is a buffer overflow error introduced,
it tends to reveal memory contents in Windows, perhaps not in Ubuntu Linux.
For more on search/sort in C libraries, see the following link:
https://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_chapter/libc_9.html */

#include <stdio.h>      /* Contains input/output functions */
#include <stdlib.h>     /* Needed for compare() for search/sort functions */
#include <search.h>     /* For search function lfind()*/
#include<string.h>      /* For various string functions */
#include<time.h>        /* For clock to randomize seed to generate random ID */

#define MAX_PEOPLE 10   /* To protect against memory abuse */
#define ID_LENGTH 5
#define NAME_SIZE 10

/* New type Person which is a struct */
typedef struct {
    char fname[NAME_SIZE];      /* This allocates fname NAME_SIZE bytes to avoid buffer overflow */
    char lname[NAME_SIZE];
    char status[NAME_SIZE];     /* Such as Student, Faculty, Staff */
    char level[NAME_SIZE];      /* Such as freshman, sophomore, etc. */
    char rank[NAME_SIZE];       /* Such as Assistant Professor, Associate Professor, Professor */
    float yearsTograduate;
    float yearsEmployed;
    char ID[ID_LENGTH];
} Person;

/* compare() and qsort() are defined in stdlib const void * can be replaced by variable types */
int compare_lname(const void *p1, const void *p2);
int compare_fname(const void *p1, const void *p2);
int compare_ID(const void *p1, const void *p2);

/* search functions call lfind() using compare, perform search of various string types */
void search_lname (const char *name, Person *p, long unsigned int lsize); /* search last name in list p of size lsize */
void search_fname (const char *name, Person *p, long unsigned int lsize); /* search first name in list p of size lsize */
void *search_ID (const char *name, Person *p, long unsigned int lsize);

/* User prompt to make choices */
void start ();

/* Following function was written at an intermediate stage to test Person*/
void testing123();

/* Enroll a new member for a database */
Person enroll();

/* Following function adds an entry to a roster, gets the total number, prints entry to file f */
void add_to_roster(Person entry, Person roster[], unsigned int *count, FILE *f);

/* Initialize a person, primarily designed as a 'constructor' for struct Person */
Person NewPerson();

/* Print to file f person's details with or without headers. For f = stdout, print to screen */
void print_Person(FILE *f, Person *p, char header);

/* Print array of Person to file f For f = stdout, print to screen */
void print_list(FILE *f, Person *p, unsigned int c);

/* generate a random ID */
char* id();

/* To print to the stdout */
void print_to_roster(Person *p, unsigned int count);

/* Following function reads roster saved in file f and returns a pointer to its array */
/* c is the pointer to the total count of the roster entries */
Person *read_roster (FILE *f, unsigned int *c);

/* Delete the given entry from an array of Person */
Person *delete(Person *p, char *ident, unsigned int *lsize);

/* The main() function */
int main (int argc, char** argv) {

    /* pass the roster file as argv[1] for read and write (at the end) */
    FILE *master_roster = fopen(argv[1], "a+");

    /* randomize the seed of rand() on each execution of the program */
    srand(time(NULL));

    unsigned int list_size = 0;     /* To return the size of list read from a file */
    char find_name[NAME_SIZE];      /* for name search */
    Person *list;                   /* For using as array of Person */
    Person member;                  /* To input output Person details */
    int answer = 0;                 /* To be used in user loop for various choices */
    char user_input = 'x', answer2;          /* To be used to exit user loop */

    start();
    scanf("%d", &answer);           /* Read one of the given choices */
    do {
        switch (answer) {

            case 1:                                                     /* Read file roster into list */
                list = read_roster(master_roster, &list_size);          /* array and return its size too */
                member = enroll();                                      /* Enroll a new member */
                add_to_roster(member, list, &list_size, master_roster); /* Add new member to master_roster*/
            break;

            case 2:
                list = read_roster(master_roster, &list_size);
                print_list(stdout, list, list_size);
            break;

            case 3: /* Sort using last name */
                list = read_roster(master_roster, &list_size);
                qsort(list, list_size, sizeof(Person), compare_lname);  /* available in <stdlin.h>*/
                print_list(stdout, list, list_size);
                break;

            case 4: /* Sort using first name */
                list = read_roster(master_roster, &list_size);
                qsort(list, list_size, sizeof(Person), compare_fname);
                print_list(stdout, list, list_size);
                break;

            case 5: /* Search entry using first name */
                printf("\nEnter the first name to search: ");
                scanf(" %s", find_name);
                list = read_roster(master_roster, &list_size);
                search_fname (find_name, list, list_size);
                break;

            case 6: /* Search entry using last name*/
                printf("\nEnter the last name to search: ");
                scanf(" %s", find_name);
                list = read_roster(master_roster, &list_size);
                search_lname (find_name, list, list_size);
                break;

            case 7: /* Search entry by ID, and add if not present */
                printf("\nEnter the ID to search: ");
                scanf(" %s", find_name);
                list = read_roster(master_roster, &list_size);
              //  search_ID (find_name, list, list_size);
                if (search_ID (find_name, list, list_size) == 0) {
                    printf("\nWould you like to add this to the roster? (y/n)");
                    scanf(" %c", &answer2);
                    if (answer2 == 'y') {
                        member = enroll();
                        add_to_roster(member, list, &list_size, master_roster);
                        printf("\nThe new roster is: \n");
                        list = read_roster(master_roster, &list_size);
                        print_list(stdout, list, list_size);
                    }
                }
                else {
                    printf("\nWould you like to remove member with ID = %c? (y/n)", answer2);
                    scanf(" %c", &answer2);
                    if (answer2 == 'y'){
                        list = delete(list, find_name, &list_size);
                        printf("\nThe remaining list is:\n");
                        print_list(stdout, list, list_size);
                        rewind(master_roster);
                        master_roster = fopen(argv[1], "w"); /* Write from the beginning */
                        print_list(master_roster, list, list_size);
                        fclose(master_roster); /* To print (flush) the new list into roster file */
                        master_roster = fopen(argv[1], "a+"); /* Go back to a+ mode to write at the end */
                    }
                }
                break;

            default:
                printf("\n\nSorry %d is not a valid choice!\n\n", answer);
            break;
        }
        printf("\nEnter x to exit, another key to continue: ");
        scanf(" %c", &user_input);
        if (user_input != 'x') {
            start();
            scanf("%d", &answer);
        }
    }   while(user_input != 'x');
    fclose(master_roster);
return (printf("\nAll done!\n"));
}

void testing123() {
    Person p1 = NewPerson();
    Person p2 = NewPerson();
    Person p3 = NewPerson();
    strcpy(p3.fname, "Tom");
    strcpy(p3.lname, "Hanks");
    strcpy(p2.fname, "Jane");
    strcpy(p2.lname, "Doe");
    print_Person(stdout, &p1, 'y'); /* this was to be used to print a header in print_Person() */
    print_Person(stdout, &p2, 'n');
    print_Person(stdout, &p3, 'n');
    fprintf(stdout, "\n");
}

void print_Person(FILE *f, Person *p, char header) { /* string is passed by reference*/
    /*if (header == 'y') {
    printf("\n%-18s%-12s%-12s%-12s%-12s%-15s%-12s\n","Name", "Status", "Level", "Rank", "Years2grad", "YearsEmployed", "ID");
    fprintf(f, "--------------------------------------------------------------------------------------\n");
    }   */
    fprintf(f, "%-9s%-9s%-12s%-12s%-12s%-11.2f%-14.2f%-12s\n", p->fname, p->lname, p->status,
        p->level, p->rank, p->yearsTograduate, p->yearsEmployed, p->ID);
}

Person NewPerson() {
    Person p = {"John", "Doe", "--", "--", "--", 0, 0, "00000"};
return p;
}

void add_to_roster(Person entry, Person roster[], unsigned int *count, FILE *f){
    roster[*count] = entry;
    (*count)++;
    print_Person(f, &entry, 'n');
    /* fwrite(&entry, sizeof(Person), 1, f);Obviously, fwrite() is not stable on all compilers. */
}

char* id() {
 char c[10] = "0123456789";
 static char new_id[ID_LENGTH];     /* By declaring it as static, it can be passed outside the function */
 for (int j = 0; j < ID_LENGTH; ++j)
    new_id[j] = c[rand()%10];
return (new_id);
}

void print_to_roster(Person *p, unsigned int count) {
    printf("\n%-9s%s%-12s%-12s%-12s%-12s%-15s%-12s\n","First Name", "Last Name",
           "Status", "Level", "Rank", "Years2grad", "YearsEmployed", "ID");
    printf("--------------------------------------------------------------------------------------\n");
    for (int j = 0; j < count; ++j)
        printf("%-9s%-9s%-12s%-12s%-12s%-11.2f%-14.2f%-12s\n", p[j].fname, p[j].lname,
            p[j].status, p[j].level, p[j].rank,
            p[j].yearsTograduate, p[j].yearsEmployed, p[j].ID);
}

Person enroll() {
   Person p;
    printf("\n Enter first name: ");
    scanf("%s", p.fname);
    printf("\n Enter last name: ");
    scanf("%s", p.lname);
    printf("\n Enter status (student, faculty, staff, other): ");
    scanf("%s", p.status);
    printf("\n Enter level if student (freshman, sophomore, junior, senior, other): ");
    scanf("%s", p.level);
    printf("\n Enter rank (Lecturer. etc.): ");
    scanf("%s", p.rank);;
    printf("\n (If student) Enter years to graduate or enter 0: ");
    scanf("%f", &p.yearsTograduate);
    printf("\n (If employee) Enter years of service or enter 0: ");
    scanf("%f", &p.yearsEmployed);
    printf("\n Enter 5-digit ID or n to use ID generated by the system: ");
    scanf(" %s", p.ID); /* it will read <enter> without the leading space */
    if (!strcmp(p.ID,"n"))
        strcpy(p.ID, id());

    return p;
}

void start () {
    printf("\nPlease enter:\n");
    printf("\n\t1\t To add a new record to text file 'roster' or to a specified file\n");
    printf("\n\t2\t To print the existing database from the specified file\n");
    printf("\n\t3\t To print the existing database from the specified file by last names\n");
    printf("\n\t4\t To print the existing database from the specified file by first names\n");
    printf("\n\t5\t To search for an entry in the given file by first name\n");
    printf("\n\t6\t To search for an entry in the given file by last name\n");
    printf("\n\t7\t To search for an entry in the given file by ID or add it if not found\n");
    printf("\n\nEnter your choice here: ");
}

Person *read_roster(FILE *f, unsigned int* c) {
    Person *p = malloc(MAX_PEOPLE*sizeof(Person));
    int count = 0;
    rewind(f);
    while (fscanf(f, "%s", p[count].fname)== 1) {
        fscanf(f, "%s", p[count].lname);
        fscanf(f, "%s", p[count].status);
        fscanf(f, "%s", p[count].level);
        fscanf(f, "%s", p[count].rank);
        fscanf(f, "%f", &p[count].yearsTograduate);
        fscanf(f, "%f", &p[count].yearsEmployed);
        fscanf(f, "%s\n", p[count].ID);
                ++count;
    }
    *c = count;
    return p;
}

void print_list(FILE *f, Person *p, unsigned int c){
    for (int j = 0; j < c; ++j)
        print_Person(f, &p[j], 'n');
}

int compare_lname(const void *p1, const void *p2) {
    Person *pp1 = (Person *)p1;
    Person *pp2 = (Person *)p2;
    return strcmp (pp1->lname, pp2->lname); /* 0 if equal, 1 if greater than, -1 if less than */
}

int compare_fname(const void *p1, const void *p2) {
    Person *pp1 = (Person *)p1;
    Person *pp2 = (Person *)p2;
    return strcmp (pp1->fname, pp2->fname);
}

int compare_ID(const void *p1, const void *p2) {
    Person *pp1 = (Person *)p1;
    Person *pp2 = (Person *)p2;
    return strcmp (pp1->ID, pp2->ID);
}

void search_lname (const char *name, Person *p, long unsigned int lsize) {
    Person target, *result;
    strcpy(target.lname, name);
    result = lfind (&target, p, &lsize, sizeof (Person), compare_lname);
    if (result)
        print_Person (stdout, result, 'n');
    else
        printf ("Couldn't find %s.\n", name);
}

void search_fname (const char *name, Person *p, long unsigned int lsize){
    Person target, *result;
    strcpy(target.fname, name);
    result = lfind (&target, p, &lsize, sizeof (Person), compare_fname);
    if (result)
        print_Person (stdout, result, 'n');
    else
        printf ("Couldn't find %s.\n", name);
}

void *search_ID (const char *name, Person *p, long unsigned int lsize){
    Person target, *result;
    strcpy(target.ID, name);
    result = lfind (&target, p, &lsize, sizeof (Person), compare_ID);
    if (result)
        print_Person (stdout, result, 'n');
    else
        printf ("Couldn't find %s.\n", name);
    return result;
  }

Person *delete(Person *p, char *ident, unsigned int *lsize){
    Person *pp = malloc(MAX_PEOPLE*sizeof(Person)); /* New list will be copied in this */
    int j,k;
    for (j = 0; j < *lsize; ++j)    /* Find the entry to remove using ID */
        if (strcmp(p[j].ID, ident))
            pp[j]= p[j];
    else {
        k = j;
        j++;    /* This will skip the entry from being copied into pp[], and thud removed */
        break;
    }
    while (j < *lsize) {            /* Copy the entries before and after the one to be deleted */
        pp[k] = p[j];
        k++;
        j++;
    }
    (*lsize)--; /* New size of the list after removal. Could be inferred instead of setting it */
    return pp;
}

