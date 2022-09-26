#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define num_players 100

int partition(int arr[], int low, int high){
    int temp;
    int pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element
    int *large_arr;
    int large_count = 0;
 
    for (int j = low; j <= high- 1; j++){
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] >= pivot){
            i++;    // increment index of smaller element
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}
 
/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(int arr[], int low, int high){
    if (low < high){
        /* pi is partitioning index, arr[p] is now
           at right place */
        int p_index = partition(arr, low, high);
 
        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, p_index - 1);
        quickSort(arr, p_index + 1, high);
    }
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)b - *(int*)a );
}

double to_sort(int score_array[], char *score_list_name, int sort_alg_type){
    clock_t start, end;
    double cpu_time;
    printf("%s\n", score_list_name);

    start = clock();
    if (sort_alg_type == 1)
        qsort(score_array, num_players, sizeof(int), cmpfunc);
    else
        quickSort(score_array, 0, num_players - 1);
    end = clock();

    cpu_time = (((double) (end - start)) / CLOCKS_PER_SEC) * 1000000;
    for (int i = 0; i < num_players; i++)
        printf("%d\n", score_array[i]);
    printf("\ntime taken: %lf\n\n", cpu_time);
    return (cpu_time);
}
int main(int argc, char **argv){
    if (argc == 2){
        char* list_names[6] = 
        {"SKILL_BREAKDANCING",
        "SKILL_APICULTURE",
        "SKILL_BASKET",
        "SKILL_XBASKET",
        "SKILL_SWORD",
        "TOTAL_XP"};

        /* score_array[i] corresponds to list_names[i] */
        int score_arrays[6][num_players];

        /* Ingest player scores into corrosponding arrays */
        for (int i = 0; i < num_players; i++)
            scanf("%d %d %d %d %d", &score_arrays[0][i],
            &score_arrays[1][i],
            &score_arrays[2][i],
            &score_arrays[3][i],
            &score_arrays[4][i]);

        /* Calculate Total_XP */
        for (int i = 0; i < num_players; i++){
            score_arrays[5][i] = 
            score_arrays[0][i] +
            score_arrays[1][i] +
            score_arrays[2][i] +
            score_arrays[3][i] +
            score_arrays[4][i];
        }

        clock_t start, end;
        int sort_alg_type; /* 0 = custom, 1 = standard */
        double cpu_time, sum_cpu_time = 0;

        /* Time to sort! */
        if (!strcmp(argv[1], "standard")) 
            sort_alg_type = 1;
        if (!strcmp(argv[1], "custom"))
            sort_alg_type = 0;

        for (int i = 0; i < 5; i++){
            cpu_time = to_sort(score_arrays[i], list_names[i], sort_alg_type);
            sum_cpu_time += cpu_time;
        }
        printf("total time taken: %lf\n", sum_cpu_time);
    }
    else
        printf("Please specify either 'standard' or 'custom' sorting upon execution.\n");

    return 0;
}