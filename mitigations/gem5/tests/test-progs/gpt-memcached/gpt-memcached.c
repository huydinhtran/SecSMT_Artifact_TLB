#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

#define KEY_SIZE 30
#define VALUE_SIZE 200
#define TABLE_SIZE 131072  // 128K entries
#define TOTAL_ITEMS 1000
#define WARMUP_ITEMS 500

typedef struct {
    char key[KEY_SIZE + 1];
    char value[VALUE_SIZE + 1];
    struct Item *next;
} Item;

Item *hash_table[TABLE_SIZE];
pthread_mutex_t hash_table_mutex[TABLE_SIZE];

unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % TABLE_SIZE;
}

void set_item(const char *key, const char *value) {
    unsigned long index = hash(key);
    pthread_mutex_lock(&hash_table_mutex[index]);
    Item *new_item = malloc(sizeof(Item));
    strncpy(new_item->key, key, KEY_SIZE);
    strncpy(new_item->value, value, VALUE_SIZE);
    new_item->next = hash_table[index];
    hash_table[index] = new_item;
    pthread_mutex_unlock(&hash_table_mutex[index]);
}

const char* get_item(const char *key) {
    unsigned long index = hash(key);
    pthread_mutex_lock(&hash_table_mutex[index]);
    Item *item = hash_table[index];
    while (item != NULL) {
        if (strncmp(item->key, key, KEY_SIZE) == 0) {
            pthread_mutex_unlock(&hash_table_mutex[index]);
            return item->value;
        }
        item = item->next;
    }
    pthread_mutex_unlock(&hash_table_mutex[index]);
    return NULL;
}

void generate_random_string(char *str, size_t length) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (size_t i = 0; i < length; i++) {
        int key = rand() % (int)(sizeof(charset) - 1);
        str[i] = charset[key];
    }
    str[length] = '\0';
}

long long time_diff_ns(struct timespec *start, struct timespec *end) {
    return (end->tv_sec - start->tv_sec) * 1000000000LL + (end->tv_nsec - start->tv_nsec);
}

int compare(const void *a, const void *b) {
    return (*(long long*)a - *(long long*)b);
}

int main() {
    srand(time(NULL));
    for (int i = 0; i < TABLE_SIZE; i++) {
        pthread_mutex_init(&hash_table_mutex[i], NULL);
    }

    char (*keys)[KEY_SIZE + 1] = malloc(TOTAL_ITEMS * (KEY_SIZE + 1));
    char (*values)[VALUE_SIZE + 1] = malloc(TOTAL_ITEMS * (VALUE_SIZE + 1));
    if (keys == NULL || values == NULL) {
        perror("Failed to allocate memory");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < TOTAL_ITEMS; i++) {
        generate_random_string(keys[i], KEY_SIZE);
        generate_random_string(values[i], VALUE_SIZE);
    }

    for (int i = 0; i < WARMUP_ITEMS; i++) {
        set_item(keys[i], values[i]);
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    long long latencies[TOTAL_ITEMS];
    int operations_in_interval = 0;

    for (int i = 0; i < TOTAL_ITEMS; i++) {
        struct timespec op_start, op_end;
        clock_gettime(CLOCK_MONOTONIC, &op_start);
        get_item(keys[i]);
        clock_gettime(CLOCK_MONOTONIC, &op_end);
        latencies[i] = time_diff_ns(&op_start, &op_end);

        operations_in_interval++;
    }


    clock_gettime(CLOCK_MONOTONIC, &end);
    long long total_time_ns = time_diff_ns(&start, &end);
    double rps = (double)TOTAL_ITEMS / (total_time_ns / 1000000000.0);

    qsort(latencies, TOTAL_ITEMS, sizeof(long long), compare);
    long long p99_latency_ns = latencies[(int)(TOTAL_ITEMS * 0.99)];

    printf("Overall RPS: %.2f\n", rps);
    printf("99th percentile latency: %lld nanoseconds\n", p99_latency_ns);
    printf("Total execution time: %lld nanoseconds\n", total_time_ns);

    free(keys);
    free(values);

    return 0;
}
