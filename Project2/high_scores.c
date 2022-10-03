#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define num_players 1000000

/* Purpose: Used to enable qsort() function.
 *
 * Using code from: https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
 */
int cmpfunc (const void * a, const void * b){
   return ( *(int*)b - *(int*)a );
}

/* Purpose: Sorting algorithm using a combination of counting sort and qsort().
 *
 * int* array: Data to be sorted.
 * return: Nothing. But upon using this function, the data in `array` will be sorted in descending order.
 */
void custom_sort(int* array) {
    int count[10000] = {0};
    int l_array_size = 0;

    /* Find number of elements within range (0,9999) */
    for (int i = 0; i < num_players; i++){
        if (array[i] < 10000)
            l_array_size++;
    }

    /* Initialize new arrays */
    int g_array_size = num_players - l_array_size,
        l_element = 0, 
        g_element = 0;

    int less_array[l_array_size],
        greater_array[g_array_size];

    /* Store elements less than 10000 in one array, 
     * Store others in another array. */
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
    qsort(greater_array, g_array_size, sizeof(int), cmpfunc);

    /* Store the count of each element */
    for (int i = 0; i < l_array_size; i++)
        count[less_array[i]]++;

    /* Use count array to replace values in less_array */
    l_element = 0;
    for (int i = 9999; i >= 0; i--){
        while(count[i] > 0){
            less_array[l_element++] = i;
            count[i]--;
        }
    }

    /* Copy the sorted elements into original array */
    l_element = 0,
    g_element = 0;
    for (int i = 0; i < num_players; i++){
        if ( i < g_array_size)
        array[i] = greater_array[g_element++];
        else
        array[i] = less_array[l_element++];
    }    
}

/* Purpose: Enables the specified sorting algorithm, times it, and prints the sorted output.
 *
 * int* score_array: Data to be sorted and timed.
 * char* score_list_name: The name associated with the specified score_array.
 * int sort_alg_type: Indicates whether we use standard (1) or custom (0) sorting algorithm.
 * return: The number of ms it took to sort the specified data.
 */
double to_sort(int* score_array, char* score_list_name, int sort_alg_type){
    printf("%s\n", score_list_name);

    clock_t start = clock();
    if (sort_alg_type == 1)
        qsort(score_array, num_players, sizeof(int), cmpfunc);
    else
        custom_sort(score_array);
    clock_t end = clock();

    /* Print sorted output */
    for (int i = 0; i < num_players; i++)
            printf("%d\n", score_array[i]);

    double cpu_time = (((double) (end - start)) / CLOCKS_PER_SEC) * 1000000;
    printf("\ntime taken: %lf\n\n", cpu_time);
    return (cpu_time);
}

/* Purpose: Ingests data from stdin and allocates it to a specified array for sorting.
 * The main purpose of the program is to take in 5 columns of player data, create an array
 * containing the total_xp for each player, and sort a total of 6 arrays of data. The user
 * specifies `standard` or `custom` upon executing the program in the terminal.
 * 
 * int argc: Number of command line arguments provided upon execution.
 * char **argv: Char arrays containing command line arguments.
 * return: If 0, program executes successfully. If -1, the user did not specify either 
 * `standard` or `custom`.
 */
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
            scanf("%d %d %d %d %d", 
            &score_arrays[0][i],
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

        /* Set sorting algorithm to be used */
        if (!strcmp(argv[1], "standard")) 
            sort_alg_type = 1;
        if (!strcmp(argv[1], "custom"))
            sort_alg_type = 0;

        /* Sort all arrays */
        for (int i = 0; i < 6; i++){
            cpu_time = to_sort(score_arrays[i], list_names[i], sort_alg_type);
            sum_cpu_time += cpu_time;
        }
        printf("total time taken: %lf\n", sum_cpu_time);
    }
    else{
        printf("Please specify either 'standard' or 'custom' sorting upon execution.\n");
        return (-1);
    }
    return 0;
}