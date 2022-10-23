#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int scape_node_count = 1;
double log32 = 0.5849625007;

/* Purpose: Data structure for each node in an AVL tree
 * char* username: unique identifier
 * int servers: number of servers user is banned from
 * int recent_ban_time: most recent timestamp from most recent ban
 * int height: keeps track of the height of node
 * struct node* left: points to left child
 * struct node* right: points to right child */
typedef struct avl_node{
    char* username;
    int servers;
    int recent_ban_time;
    int height;
    struct avl_node* left;
    struct avl_node* right;
}AVL_NODE;

/* Purpose: Data structure for each node in a Scapegoat tree
 * char* username: unique identifier
 * int servers: number of servers user is banned from
 * int recent_ban_time: most recent timestamp from most recent ban
 * struct node* left: points to left child
 * struct node* right: points to right child
 * struct node* parent: points to node's parent */
typedef struct scape_node{
    char* username;
    int servers;
    int recent_ban_time;
    struct scape_node* left;
    struct scape_node* right;
    struct scape_node* parent;
}SCAPE_NODE;

/* Purpose: Simple function that finds the maximum of two values */
int max(int a, int b){
    if (a > b)
        return (a);
    return(b);
}

/* Purpose: Calculate Height of given node in AVL tree 
 * AVL_NODE* node: given node
 * Return: height of given node */
int avl_height(AVL_NODE* node){
    if (node == NULL)
        return (0);
    return (max(avl_height(node->left), avl_height(node->right)) + 1);
}

/* Purpose: Create a new node for AVL tree
 * char* username: the username assigned to new node
 * int ban_timestamp: the timestamp associated with most recent user ban
 * Return: New initialized/malloc'd node */
AVL_NODE* create_new_avl_node(char* username, int ban_timestamp){
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

/* Purpose: Calculates the balance of avl tree/subtree
 * AVL_NODE* node: root of tree or subtree
 * Return: balance value of tree/subtree */
int avl_balance(AVL_NODE* node){
    if (node == NULL)
        return (0);
    return(avl_height(node->left) - avl_height(node->right));
}

/* Purpose: Make a left rotation on subtree 
 * AVL_NODE* a: root of subtree
 * Return: new root of rotated subtree */
AVL_NODE* avl_rotate_left(AVL_NODE* a){
    AVL_NODE* b = a->right;
    AVL_NODE* c = b->left;
    b->left = a; /* a->right->left becomes a */
    a->right = c; /* a->right becomes a->right->left */
    /* Recalculate heights */
    a->height = avl_height(a);
    b->height = avl_height(b);
    return (b); /* b is new root */    
}

/* Purpose: Make a right rotation on subtree 
 * AVL_NODE* c: root of subtree
 * Return: new root of rotated subtree */
AVL_NODE* avl_rotate_right(AVL_NODE* a){
    AVL_NODE* b = a->left; 
    AVL_NODE* c = b->right; 
    b->right = a; /* a->left->right becomes a */
    a->left = c; /* a->left becomes a->left->right */
    /* Recalculate heights */
    a->height = avl_height(a);
    b->height = avl_height(b);
    return (b); /* b is new root */    
}

/* Purpose: Insert node to AVL tree 
 * AVL_NODE* node: Root of tree/subtree
 * char* username: Unique identifier of new node
 * int ban_timestamp: the timestamp associated with most recent user ban
 * Return: Updated tree with newly inserted node */
AVL_NODE* avl_insert(AVL_NODE* node, char* username, int ban_timestamp){
    /* If tree/subtree is empty */
    if (node == NULL)
        return(create_new_avl_node(username, ban_timestamp));

    /* If key is less than root */
    if (strcmp(username, node->username) < 0)
        node->left = avl_insert(node->left, username, ban_timestamp);

    /* If key is greater than root */
    else if (strcmp(username, node->username) > 0)
        node->right = avl_insert(node->right, username, ban_timestamp);

    /* Not a new user, just increase server count */
    else{
        node->servers++;
        if (ban_timestamp > node->recent_ban_time)
            node->recent_ban_time = ban_timestamp;
        return (node);
    }

    /* Get height of tree and check balance */
    node->height = avl_height(node);
    int node_balance = avl_balance(node);
    
    /* LL Rotate */
    if (node_balance > 1 && strcmp(username, node->left->username) < 0)
        return (avl_rotate_right(node));

    /* LR Rotate */
    if (node_balance > 1 && strcmp(username, node->left->username) < 0){
        node->left = avl_rotate_left(node->left);
        return (avl_rotate_right(node));
    }

    /* RR Rotate */
    if (node_balance < -1 && strcmp(username, node->right->username) > 0)
        return (avl_rotate_left(node));

    /* RL Rotate */
    if (node_balance < -1 && strcmp(username, node->right->username) > 0){
        node->right = avl_rotate_right(node->right);
        return (avl_rotate_left(node));
    }
    return (node);
}

/* Purpose: Search for user in database and print information.
 * AVL_NODE* node: Root of tree
 * char* username: Username which we want to search for in tree
 * Return: Nothing, but will print node contents of given username */
void search_avl(AVL_NODE* node, char* username){
    if (node == NULL){
        printf("%s is not currently banned from any servers.\n", username);
        return;
    }
    if (strcmp(username, node->username) < 0)
        search_avl(node->left, username);
    if (strcmp(username, node->username) > 0)
        search_avl(node->right, username);
    if (strcmp(username, node->username) == 0){
        printf("%s was banned from %d servers. most recently on: %ld\n", 
            node->username, node->servers, node->recent_ban_time);
        return;
    }
}

/* Purpose: Print AVL tree in order 
 * AVL_NODE* root: Root of tree/subtree
 * Return: Nothing, but will print tree contents and free nodes */
void free_avl_tree(AVL_NODE* root){
    if (root != NULL){
        free_avl_tree(root->left);
        free(root->username);
        free_avl_tree(root->right);
        free(root);
    }
    return;
}

/* Purpose: Create a new node for Scapegoat tree
 * char* username: the username assigned to new node
 * int ban_timestamp: the timestamp associated with most recent user ban
 * Return: New initialized/malloc'd node */
SCAPE_NODE* create_new_scape_node(char* username, int ban_timestamp){
    /* Allocate memory for new node */
    SCAPE_NODE* new_node = (SCAPE_NODE*) malloc(sizeof(SCAPE_NODE));
    new_node->username = (char*) malloc(sizeof(char) * strlen(username) + 1);
    /* Initialize new node */
    strcpy(new_node->username, username);
    new_node->servers = 1;
    new_node->recent_ban_time = ban_timestamp;
    new_node->left = new_node->right = NULL;
    return(new_node);
}

/* Purpose: Insert node to scapegoat tree 
 * SCAPE_NODE* node: Root of tree/subtree
 * char* username: Unique identifier of new node
 * int ban_timestamp: the timestamp associated with most recent user ban
 * Return: Updated tree with newly inserted node */
SCAPE_NODE* scape_insert(SCAPE_NODE* root, char* username, int ban_timestamp){
    /* If tree/subtree is empty */
    if (root == NULL){
        scape_node_count++;
        return(create_new_scape_node(username, ban_timestamp));
    }

    /* If key is less than root */
    if (strcmp(username, root->username) < 0)
        root->left = scape_insert(root->left, username, ban_timestamp);

    /* If key is greater than root */
    else if (strcmp(username, root->username) > 0)
        root->right = scape_insert(root->right, username, ban_timestamp);

    /* Not a new user, just increase server count */
    else{
        root->servers++;
        if (ban_timestamp > root->recent_ban_time)
            root->recent_ban_time = ban_timestamp;
        return (root);
    }
    
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
        char name[20];
        int server_id;
        long int timestamp;
        /* AVL Tree */
        if (strcmp(argv[1], "avl") == 0){
            /* read in root of tree */
            fscanf(file_ptr, "%s %d %ld", name, &server_id, &timestamp);
            AVL_NODE* root = create_new_avl_node(name, timestamp);
            
            /* read in remaining data */
            while (fscanf(file_ptr, "%s %d %ld", name, &server_id, &timestamp) == 3)
                root = avl_insert(root, name, timestamp);

            /* Search for each user in the list and print information stored
             * in our database (tree) */
            for (int i = 0; i < user_count; i++)
                search_avl(root, list_users[i]);
            free_avl_tree(root);
        }
        /* Scapegoat Tree */
        else if (strcmp(argv[1], "scapegoat") == 0){
            /* read in root of tree */
            fscanf(file_ptr, "%s %d %ld", name, &server_id, &timestamp);
            SCAPE_NODE* root = create_new_scape_node(name, timestamp);
            
            /* read in remaining data */
            while (fscanf(file_ptr, "%s %d %ld", name, &server_id, &timestamp) == 3)
                root = scape_insert(root, name, timestamp);

            printf("There are %d nodes\n", scape_node_count);
            printf("Alpha height = %d\n", (int)(log2(scape_node_count)/log32));
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