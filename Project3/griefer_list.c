#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Purpose: Data structure for each node in an AVL tree
 * char* username: unique identifier
 * int servers: number of servers user is banned from
 * struct node* left: points to left child
 * struct node* right: points to right child
 * int height: keeps track of the height of node */
typedef struct node{
    char* username;
    int servers;
    struct node* left;
    struct node* right;
    int height;
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
    return (1 + max(height(node->left), height(node->right)));
}

/* Purpose: Create a new node for AVL tree
 * char* username: the username assigned to new node
 * Return: New initialized/malloc'd node */
AVL_NODE* create_new_node(char* username){
    AVL_NODE* new_node = (AVL_NODE*) malloc(sizeof(AVL_NODE));
    new_node->username = (char*) malloc(sizeof(username));
    strcpy(new_node->username, username);
    new_node->servers = 1;
    new_node->left = new_node->right = NULL;
    new_node->height = 1;
    return(new_node);
}

/* Purpose: Calculates the balance of tree/subtree
 * AVL_NODE* node: root of tree or subtree
 * Return: balance value of tree/subtree */
int balance(AVL_NODE* node){
    if (node == NULL)
        return 0;
    return(height(node->left) - height(node->right));
}

/* Purpose: Find minimum value in subtree 
 * AVL_NODE* node: root of subtree
 * Return: smallest node of subtree */
AVL_NODE* successor_node(AVL_NODE* node){
    AVL_NODE* current = node;
    while(current->left != NULL)
        current = current-> left;
    return (current);
}

/* Purpose: Make a left rotation on subtree 
 * AVL_NODE* a: root of subtree
 * Return: new root of rotated subtree */
AVL_NODE* rotate_left(AVL_NODE* a){
    AVL_NODE* b = a->right; /* a's current right child will become a's parent */
    AVL_NODE* c = b->left; /* b's right child */
    b->left = a; /* a becomes child of b */
    a->right = c; /* c becomes right child of a */
    a->height = max(height(a->left), height(a->right)) + 1;
    b->height = max(height(b->left), height(b->right)) + 1;

    return (b); /* b is new root */    
}

/* Purpose: Make a right rotation on subtree 
 * AVL_NODE* c: root of subtree
 * Return: new root of rotated subtree */
AVL_NODE* rotate_right(AVL_NODE* c){
    AVL_NODE* b = c->left; /* b becomes parent of c */
    AVL_NODE* a = b->right; 
    b->right = c; /* c becomes right child of b */
    c->left = a; /* a becomes child of c */
    c->height = max(height(c->left), height(c->right)) + 1;
    b->height = max(height(b->left), height(b->right)) + 1;

    return (b); /* b is new root */    
}

/* Purpose: Insert node to AVL tree 
 * AVL_NODE* node: Root of tree/subtree
 * char* username: Unique identifier of new node
 * Return: Updated tree with newly inserted node */
AVL_NODE* insert(AVL_NODE* node, char* username){
    /* If tree/subtree is empty */
    if (node == NULL)
        return(create_new_node(username));

    /* If key is less than root */
    if (strcmp(username, node->username) < 0)
        node->left = insert(node->left, username);

    /* If key is greater than root */
    else if (strcmp(username, node->username) > 0)
        node->right = insert(node->right, username);

    /* Not a new user, just increase server count */
    else{
        node->servers++;
        return (node);
    }

    /* Get height of tree and check balance */
    node->height = max(height(node->left), height(node->right)) + 1;
    int node_balance = balance(node);

    /* Rebalance left side of tree */
    if (node_balance > 1){
        if (strcmp(username, node->left->username) < 0)
            return (rotate_right(node));
        if (strcmp(username, node->left->username) > 0){
            node->left = rotate_left(node->left);
            return (rotate_right(node));
        }
    }

    /* Rebalance right side of tree */
    if (node_balance < -1){
        if (strcmp(username, node->right->username) < 0)
            return (rotate_left(node));
        if (strcmp(username, node->right->username) > 0){
            node->right = rotate_right(node);
            return (rotate_left(node));
        }
    }

    return (node);
}

/* Purpose: Print AVL tree in order 
 * AVL_NODE* root: Root of tree/subtree
 * Return: Nothing, but will print tree contents and free nodes */
void print_tree(AVL_NODE* root){
    if (root != NULL){
        print_tree(root->left);
        printf("%s: %d ", root->username, root->servers);
        free(root->username);
        print_tree(root->right);
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
    if (argc == 2){
        /* Main Program*/
        printf("%s\n", argv[1]);
        AVL_NODE* root = create_new_node("Drew");
        root = insert(root, "Drew");
        root = insert(root, "Bob");
        root = insert(root, "Peter");
        print_tree(root);
        printf("\n");
        return (0);
    }
    else{
        /* Error Message */
        printf("Please specify tree type upon execution.\n");
        return (-1);
    }
}