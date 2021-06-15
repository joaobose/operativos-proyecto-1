#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0
#define MAXN 10

int N = 1;
char filename[] = "";

FILE *input_file;

int lower_bounds[MAXN];
int upper_bounds[MAXN];

int *numbers;

int count = 0;
pthread_mutex_t lock;

void validation_rutine();
void master();
void thread_primes();
void compute_bounds(int file_lenght, int lower_out[N], int upper_out[N]);
int count_lines(FILE *file);
void primes_worker(void *param);
bool is_prime(int number);
void load_numbers(int file_length);

int main(int argc, char *argv[])
{
    int opt;
    char *options = "-n:";

    while ((opt = getopt(argc, argv, options)) != -1)
    {
        switch (opt)
        {
        case TRUE:
            strcpy(filename, optarg);
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

    thread_primes();
}

void thread_primes()
{

    pthread_t threads[N];

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("Error: Failed to create lock mutex");
        exit(1);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_create(&threads[i], NULL, (void *)primes_worker, (void *)i);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    printf("%d primes found.\n", count);
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
        bool is = is_prime(numbers[j]);
        if (is)
        {
            pthread_mutex_lock(&lock);
            count++;
            pthread_mutex_unlock(&lock);
        }

        fprintf(output_file, "%d %d\n", numbers[j], is);
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
