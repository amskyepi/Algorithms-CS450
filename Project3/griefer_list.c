#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Purpose: Data structure for each node in an AVL tree
 * char* username: unique identifier
 * int servers: number of servers user is banned from
 * long int recent_ban_time: most recent timestamp from most recent ban
 * int height: keeps track of the height of node
 * struct node* left: points to left child
 * struct node* right: points to right child */
typedef struct node{
    char* username;
    int servers;
    long int recent_ban_time;
    int height;
    struct node* left;
    struct node* right;
}AVL_NODE;

/* Purpose: Simple function that finds the maximum of two values */
int max(int a, int b){
    if (a > b)
        return (a);
    return(b);
}

/* Purpose: Calculate Height of given node in AVL tree 
 * AVL_NODE* node: given node
 * Return: height of given node */
int height(AVL_NODE* node){
    if (node == NULL)
        return (0);
    return (max(height(node->left), height(node->right)) + 1);
}

/* Purpose: Create a new node for AVL tree
 * char* username: the username assigned to new node
 * Return: New initialized/malloc'd node */
AVL_NODE* create_new_node(char* username, long int ban_timestamp){
    /* Allocate memory for new node */
    AVL_NODE* new_node = (AVL_NODE*) malloc(sizeof(AVL_NODE));
    new_node->username = (char*) malloc(sizeof(char) * strlen(username) + 1);
    /* Initialize new node */
    strcpy(new_node->username, username);
    new_node->servers = 1;
    new_node->recent_ban_time = ban_timestamp;
    new_node->left = new_node->right = NULL;
    new_node->height = 1;
    return(new_node);
}

/* Purpose: Calculates the balance of tree/subtree
 * AVL_NODE* node: root of tree or subtree
 * Return: balance value of tree/subtree */
int balance(AVL_NODE* node){
    if (node == NULL)
        return (0);
    return(height(node->left) - height(node->right));
}

/* Purpose: Make a left rotation on subtree 
 * AVL_NODE* a: root of subtree
 * Return: new root of rotated subtree */
AVL_NODE* rotate_left(AVL_NODE* a){
    AVL_NODE* b = a->right;
    AVL_NODE* c = b->left;
    b->left = a; /* a->right->left becomes a */
    a->right = c; /* a->right becomes a->right->left */
    /* Recalculate heights */
    a->height = height(a);
    b->height = height(b);
    return (b); /* b is new root */    
}

/* Purpose: Make a right rotation on subtree 
 * AVL_NODE* c: root of subtree
 * Return: new root of rotated subtree */
AVL_NODE* rotate_right(AVL_NODE* a){
    AVL_NODE* b = a->left; 
    AVL_NODE* c = b->right; 
    b->right = a; /* a->left->right becomes a */
    a->left = c; /* a->left becomes a->left->right */
    /* Recalculate heights */
    a->height = height(a);
    b->height = height(b);
    return (b); /* b is new root */    
}

/* Purpose: Insert node to AVL tree 
 * AVL_NODE* node: Root of tree/subtree
 * char* username: Unique identifier of new node
 * Return: Updated tree with newly inserted node */
AVL_NODE* insert(AVL_NODE* node, char* username, long int ban_timestamp){
    /* If tree/subtree is empty */
    if (node == NULL)
        return(create_new_node(username, ban_timestamp));

    /* If key is less than root */
    if (strcmp(username, node->username) < 0)
        node->left = insert(node->left, username, ban_timestamp);

    /* If key is greater than root */
    else if (strcmp(username, node->username) > 0)
        node->right = insert(node->right, username, ban_timestamp);

    /* Not a new user, just increase server count */
    else{
        node->servers++;
        if (ban_timestamp > node->recent_ban_time)
            node->recent_ban_time = ban_timestamp;
        return (node);
    }

    /* Get height of tree and check balance */
    node->height = height(node);
    int node_balance = balance(node);
    
    /* LL Rotate */
    if (node_balance > 1 && strcmp(username, node->left->username) < 0)
        return (rotate_right(node));

    /* LR Rotate */
    if (node_balance > 1 && strcmp(username, node->left->username) < 0){
        node->left = rotate_left(node->left);
        return (rotate_right(node));
    }

    /* RR Rotate */
    if (node_balance < -1 && strcmp(username, node->right->username) > 0)
        return (rotate_left(node));

    /* RL Rotate */
    if (node_balance < -1 && strcmp(username, node->right->username) > 0){
        node->right = rotate_right(node->right);
        return (rotate_left(node));
    }
    return (node);
}

/* Purpose: Search for user in database and print information.
 * AVL_NODE* node: Root of tree
 * char* username: Username which we want to search for in tree
 * Return: Nothing, but will print node contents of given username */
void search_node(AVL_NODE* node, char* username){
    if (node == NULL){
        printf("%s is not currently banned from any servers.\n", username);
        return;
    }
    if (strcmp(username, node->username) < 0)
        search_node(node->left, username);
    if (strcmp(username, node->username) > 0)
        search_node(node->right, username);
    if (strcmp(username, node->username) == 0){
        printf("%s was banned from %d servers. most recently on: %ld\n", 
            node->username, node->servers, node->recent_ban_time);
        return;
    }
}

/* Purpose: Print AVL tree in order 
 * AVL_NODE* root: Root of tree/subtree
 * Return: Nothing, but will print tree contents and free nodes */
void free_tree(AVL_NODE* root){
    if (root != NULL){
        free_tree(root->left);
        free(root->username);
        free_tree(root->right);
        free(root);
    }
    return;
}

/* Purpose: Ingests and assigns data from stdin to the tree type specified at command line.
 * The user will specify `avl` or `scapegoat` upon executing the program in the terminal.
 * 
 * int argc: Number of command line arguments provided upon execution.
 * char **argv: Char arrays containing command line arguments.
 * return: If 0, program executes successfully. If -1, the user did not specify either 
 * `avl` or `scapegoat`.
 */
int main(int argc, char** argv){
    if (argc == 3){
        /* Start timer */
        clock_t start = clock();
        int user_count = 0;
        char list_users[20][200];

        /* Ingest usernames from stdin */
        for (int i = 0; i < 11; i++){
            if (scanf("%s", list_users[i]) == 1)
                user_count++;
        }

        /* Open file to read data into database */
        FILE* file_ptr = fopen(argv[2], "r");
        /* Error Message */
        if (file_ptr == NULL){
            printf("Could not open file.\n");
            return (-1);
        }
        /* AVL Tree */
        if (strcmp(argv[1], "avl") == 0){
            char name[20];
            int server_id;
            long int timestamp;

            /* read in root of tree */
            fscanf(file_ptr, "%s %d %ld", name, &server_id, &timestamp);
            AVL_NODE* root = create_new_node(name, timestamp);
            
            /* read in remaining data */
            while (fscanf(file_ptr, "%s %d %ld", name, &server_id, &timestamp) == 3)
                root = insert(root, name, timestamp);

            /* Search for each user in the list and print information stored
             * in our database (tree) */
            for (int i = 0; i < user_count; i++)
                search_node(root, list_users[i]);
            free_tree(root);
        }
        /* Scapegoat Tree */
        else if (strcmp(argv[1], "scapegoat") == 0){
        }
        /* End timer and print out time in microseconds */
        clock_t end = clock();
        double cpu_time = (((double) (end - start)) / CLOCKS_PER_SEC) * 1000000;
        printf("total time in microseconds: %lf\n", cpu_time);
        fclose(file_ptr);
        return (0);
    }
    else{
        /* Error Message */
        printf("Please specify tree type upon execution.\n");
        return (-1);
    }
}