#include <stdio.h>
#include <stdlib.h>

/* Purpose: Data structure for each node in an AVL tree
 * int key: unique identifier
 * struct node* left: points to left child
 * struct node* right: points to right child
 * int height: keeps track of the height of node */
typedef struct node{
    int key;
    struct node* left;
    struct node* right;
    int height;
}AVL_NODE;

/* Purpose: Calculate Height of given node in AVL tree 
 * AVL_NODE* node: given node
 * Return: height of given node */
int height(AVL_NODE* node){
    if (node == NULL)
        return (0);
    return (node->height);
}

int max(int a, int b){
    if (a > b)
        return (a);
    return(b);
}

/* Purpose: Create a new node for AVL tree
 * int key: the key assigned to new node
 * Return: New initialized/malloc'd node */
AVL_NODE* create_new_node(int key){
    AVL_NODE* new_node = (AVL_NODE*) malloc(sizeof(AVL_NODE));
    new_node->key = key;
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
    a->right = c; /* c becomes child of a */
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
    b->right = c; /* c becomes child of b */
    c->left = a; /* a becomes child of c */
    c->height = max(height(c->left), height(c->right)) + 1;
    b->height = max(height(b->left), height(b->right)) + 1;

    return (b); /* b is new root */    
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
        return (0);
    }
    else{
        /* Error Message */
        printf("Please specify tree type upon execution.\n");
        return (-1);
    }
}