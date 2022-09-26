#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <string.h>

#define num_players 100

int partition(int arr[], int low, int high){
    int temp;
    int pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element
 
    for (int j = low; j <= high- 1; j++){
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot){
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

double use_standard(int score_array[], char *score_list_name){
    clock_t start, end;
    double cpu_time;
    printf("%s\n", score_list_name);
    start = clock();
    qsort(score_array, num_players, sizeof(int), cmpfunc);
    end = clock();
    cpu_time = (((double) (end - start)) / CLOCKS_PER_SEC) * 1000000;
    for (int i = 0; i < num_players; i++)
        printf("%d\n", score_array[i]);
    printf("\ntime taken: %lf\n\n", cpu_time);
    return (cpu_time);
}

double use_custom(int score_array[], char *score_list_name){
    clock_t start, end;
    double cpu_time;
    printf("%s\n", score_list_name);
    start = clock();
    quickSort(score_array, 0, num_players -1);
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

        int breakdancing_scores[num_players],
        underwater_apiculture_scores[num_players],
        basket_weaving_scores[num_players],
        extreme_basket_weaving_scores[num_players],
        longsword_scores[num_players],
        total_xp[num_players];

        /* Ingest player scores into corrosponding arrays */
        for (int i = 0; i < num_players; i++)
            scanf("%d %d %d %d %d", &breakdancing_scores[i],
            &underwater_apiculture_scores[i],
            &basket_weaving_scores[i],
            &extreme_basket_weaving_scores[i],
            &longsword_scores[i]);

        /* Calculate Total_XP */
        for (int i = 0; i < num_players; i++){
            total_xp[i] = breakdancing_scores[i] +
            underwater_apiculture_scores[i] +
            basket_weaving_scores[i] +
            extreme_basket_weaving_scores[i] +
            longsword_scores[i];
        }

        clock_t start, end;
        double cpu_time1, 
        cpu_time2, 
        cpu_time3, 
        cpu_time4, 
        cpu_time5, 
        cpu_time6;

        if (!strcmp(argv[1], "standard")){ 
            cpu_time1 = use_standard(breakdancing_scores, list_names[0]),
            cpu_time2 = use_standard(underwater_apiculture_scores, list_names[1]),
            cpu_time3 = use_standard(basket_weaving_scores, list_names[2]),
            cpu_time4 = use_standard(extreme_basket_weaving_scores, list_names[3]),
            cpu_time5 = use_standard(longsword_scores, list_names[4]),
            cpu_time6 = use_standard(total_xp, list_names[5]);

            printf("total time taken: %lf\n", 
            cpu_time1 +
            cpu_time2 +
            cpu_time3 +
            cpu_time4 +
            cpu_time5 +
            cpu_time6);
        }

        if (!strcmp(argv[1], "custom")){
            cpu_time1 = use_custom(breakdancing_scores, list_names[0]),
            cpu_time2 = use_custom(underwater_apiculture_scores, list_names[1]),
            cpu_time3 = use_custom(basket_weaving_scores, list_names[2]),
            cpu_time4 = use_custom(extreme_basket_weaving_scores, list_names[3]),
            cpu_time5 = use_custom(longsword_scores, list_names[4]),
            cpu_time6 = use_custom(total_xp, list_names[5]);

            printf("total time taken: %lf\n", 
            cpu_time1 +
            cpu_time2 +
            cpu_time3 +
            cpu_time4 +
            cpu_time5 +
            cpu_time6);
        }
    }
    else
        printf("Please specify either 'standard' or 'custom' sorting upon execution.\n");

    return 0;
}