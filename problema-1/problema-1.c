#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0
#define MAXN 10

bool thread = FALSE;
bool process = FALSE;
int N = 1;
char filename[] = "";

FILE *input_file;

int lower_bounds[MAXN];
int upper_bounds[MAXN];

int *numbers;

void validation_rutine();
void master();
void thread_primes();
void process_primes();
void compute_bounds(int file_lenght, int lower_out[N], int upper_out[N]);
int count_lines(FILE *file);
void primes_worker(void *param);
bool is_prime(int number);
void load_numbers(int file_length);

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
            break;
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

    if (input_file == NULL)
    {
        printf("Error: Could not open file");
        exit(1);
    }

    int file_length = count_lines(input_file);

    load_numbers(file_length);

    fclose(input_file);

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

    pthread_t threads[N];

    for (int i = 0; i < N; i++)
    {
        pthread_create(&threads[i], NULL, (void *)primes_worker, (void *)i);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

void process_primes()
{
    pid_t pids[N];

    for (int i = 0; i < N; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            primes_worker((void *)i);
            return;
        }

        pids[i] = pid;
    }

    for (int i = 0; i < N; i++)
    {
        waitpid(pids[i], NULL, 0);
    }
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

void primes_worker(void *param)
{
    int i = (int)param;

    char output_filename[10];

    sprintf(output_filename, "./%d.txt", i);

    FILE *output_file = fopen(output_filename, "w");

    int l = lower_bounds[i];
    int u = upper_bounds[i];

    for (int j = l; j < u; j++)
    {
        fprintf(output_file, "%d %d\n", numbers[j], is_prime(numbers[j]));
    }

    fclose(output_file);
}

int count_lines(FILE *file)
{
    int c, count = 0;

    for (c = getc(input_file); c != EOF; c = getc(input_file))
    {
        if (c == '\n')
        {
            count++;
        }
    }

    rewind(input_file);

    return count;
}

void load_numbers(int file_length)
{
    numbers = malloc(sizeof(int) * file_length);

    for (int i = 0; i < file_length; i++)
    {
        fscanf(input_file, "%d", &numbers[i]);
    }
}

bool is_prime(int number)
{
    if (number < 2)
        return false; // Prime numbers are greater than 1

    // Check for each number smaller than sqrt(number) if a divisor exists
    for (int div = 2; div * div <= number; div++)
    {
        if (number % div == 0)
        {
            return false;
        }
    }

    return true; // If no divisor exists, the number is prime
}
