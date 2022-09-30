#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define num_players 100

int cmpfunc (const void * a, const void * b){
   return ( *(int*)b - *(int*)a );
}

void custom_sort(int array[]) {
    int count[10000] = {0};
    int array_size = 0;

    /* Find number of elements within range (0,9999) */
    for (int i = 0; i < num_players; i++){
        if (array[i] < 10000)
            array_size++;
    }

    /* Initialize new arrays */
    int less_array[array_size];
    int greater_array[num_players - array_size];
    int l_element = 0, g_element = 0;

    /* Store elements less than 10000 in new array */
    for (int i = 0; i < num_players; i++){
        if (array[i] < 10000){
            less_array[l_element] = array[i];
            l_element++;
        }
        else{
            greater_array[g_element] = array[i];
            g_element++;
        }
    }

    /* Sort values greater than 9999 */
    qsort(greater_array, num_players - array_size, sizeof(int), cmpfunc);

    /* Store the count of each element */
    for (int i = 0; i < array_size; i++)
        count[less_array[i]]++;

    /* Use count array to replace values in less_array */
    l_element = 0;
    for (int i = 9999; i >= 0; i--){
        while(count[i] > 0){
            less_array[l_element++] = i;
            count[i]--;
        }
    }

    l_element = 0;
    g_element = 0;

    /* Copy the sorted elements into original array */
    for (int i = num_players - 1; i >= 0; i--){
        while(l_element < array_size){
            array[i] = less_array[l_element++];
        }
        while(l_element < (num_players - array_size)){
            array[i] = greater_array[g_element++];
        }
    }
}

double to_sort(int score_array[], char *score_list_name, int sort_alg_type){
    clock_t start, end;
    double cpu_time;
    printf("%s\n", score_list_name);

    start = clock();
    if (sort_alg_type == 1){
        qsort(score_array, num_players, sizeof(int), cmpfunc);
    }
    else
        custom_sort(score_array);
    end = clock();
    for (int i = 0; i < num_players; i++)
            printf("%d\n", score_array[i]);
    cpu_time = (((double) (end - start)) / CLOCKS_PER_SEC) * 1000000;
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
        static int score_arrays[6][num_players];

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

        /*printf("Standard ");
        clock_t start = clock();
        qsort(score_arrays[0], num_players, sizeof(int), cmpfunc);
        clock_t end = clock();
        printf("took %lf ms\n", (((double) (end - start)) / CLOCKS_PER_SEC) * 1000000);
        printf("Custom ");
        start = clock();
        custom_sort(score_arrays[0]);
        end = clock();
        printf("took %lf ms\n", (((double) (end - start)) / CLOCKS_PER_SEC) * 1000000);*/
    }
    else
        printf("Please specify either 'standard' or 'custom' sorting upon execution.\n");

    return 0;
}