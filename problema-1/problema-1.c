#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
typedef int bool;
#define MAXN 10

bool thread = FALSE;
bool process = FALSE;
int N = 1;
char filename[] = "";

FILE *input_file;

int lower_bounds[MAXN];
int upper_bounds[MAXN];

void validation_rutine();
void master();
void thread_primes();
void process_primes();
void compute_bounds(int file_lenght, int lower_out[N], int upper_out[N]);
int count_lines(FILE *file);

int main(int argc, char *argv[])
{
    int opt;
    char *options = "-tpn:";

    while ((opt = getopt(argc, argv, options)) != -1)
    {
        switch (opt)
        {
        case TRUE:
            strcpy(filename, optarg);
        case 't':
            thread = TRUE;
            break;
        case 'p':
            process = TRUE;
            break;
        case 'n':
            N = atoi(optarg);
            break;
        case '?':
            exit(1);
        case ':':
            exit(1);
        }
    }

    validation_rutine();
    master();

    return 0;
}

void validation_rutine()
{
    if (!thread && !process)
    {
        printf("Error: You must select either to use thread or process");
        exit(1);
    }

    if (thread && process)
    {
        printf("Error: You cannot select to use both thread and process");
        exit(1);
    }

    if (!(1 <= N && N <= MAXN))
    {
        printf("Error: N must satisfy 1 <= N <= 10");
        exit(1);
    }
}

void master()
{
    input_file = fopen(filename, "r");

    if (input_file == NULL) {
        printf("Could not open file");
        exit(1);
    }

    int file_length = count_lines(input_file);

    compute_bounds(file_length, lower_bounds, upper_bounds);

    if (thread)
    {
        thread_primes();
    }
    else
    {
        process_primes();
    }
}

void thread_primes()
{
    // do thread stuff
    printf("DEBUG: Working with threads");
}

void process_primes()
{
    // do process stuff
    printf("DEBUG: Working with process");
}

void compute_bounds(int file_lenght, int lower_out[N], int upper_out[N])
{
    int div = file_lenght / N;

    for (int i = 0; i < N; i++)
    {
        lower_out[i] = div * i;
        upper_out[i] = lower_out[i] + div;
    }

    upper_out[N - 1] = file_lenght;
}

void primes_worker(int lower_bound, int upper_bound)
{
    // Worker funtion
}

int count_lines(FILE *file) {
    int c, count = 0;

    for (c = getc(input_file); c != EOF; c = getc(input_file)) {
        if (c == '\n') {
            count++;
        }
    }

    rewind(input_file);

    return count;
}
