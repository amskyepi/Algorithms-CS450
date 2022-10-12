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