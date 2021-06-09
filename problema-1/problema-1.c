#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
typedef int bool;

bool thread = FALSE;
bool process = FALSE;
int N = 1;
char file[] = "";

void validation_rutine();
void master();
void thread_primes();
void process_primes();

int main(int argc, char *argv[])
{
    int opt;
    char *options = "-tpn:";

    while ((opt = getopt(argc, argv, options)) != -1)
    {
        switch (opt)
        {
        case TRUE:
            strcpy(file, optarg);
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

    if (!(1 <= N && N <= 10))
    {
        printf("Error: N must satisfy 1 <= N <= 10");
        exit(1);
    }
}

void master()
{
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

    int lower_bounds[N];
    int upper_bounds[N];
}

void process_primes()
{
    // do process stuff
    printf("DEBUG: Working with process");

    int lower_bounds[N];
    int upper_bounds[N];
}

void compute_bounds(int file_lenght, int *lower_out[N], int *upper_out[N])
{
    int div = file_lenght / N;

    for (int i = 0; i < N; i++)
    {
        *(lower_out[i]) = div * i;
        *(upper_out[i]) = *(lower_out[i]) + div;
    }

    *(upper_out[N - 1]) = file_lenght;
}

void primes_worker(int lower_bound, int upper_bound)
{
    // Worker funtion
}
