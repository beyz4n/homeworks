/*
    150120077 - Beyza Nur KAYA
    C program to implement the AVL and Splay trees.
    At the end of this code, we have costs to compare.
*/

// including the headers/libraries to use
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// creation of global variables to compare avl and splay trees
int numberOfRotationsAVL = 0;
int numberOfComparisonAVL = 0;

int numberOfRotationsSplay = 0; 
int numberOfComparisonSplay = 0;

// creating the node for avl trees and typedef it to certain name
typedef struct avlNode
{
    // it will have key, height and frequency properties with two possible children(left and right)
    int key;
    int height; // we need this to check the balance property.
    int frequency;
    struct avlNode *right;
    struct avlNode *left;
} AVLNode;

// creating the node for splay trees and typedef it to certain name
typedef struct splayNode
{
    // it will have key and frequency properties with two possible children(left and right)
    int key;
    int frequency;
    struct splayNode *right;
    struct splayNode *left;
} SplayNode;

// typedef the pointer to avl nodes
typedef AVLNode *AVLNodePtr;

typedef SplayNode *SplayNodePtr;

// this method checks the balance property in avl trees and returns the height difference
int isBalanced(AVLNodePtr tree)
{
    // if the tree does not exists, returns 0
    if (tree == NULL)
        return 0;
    // if the tree does not have a right child, returns left - (-1) = left + 1
    else if (tree->left != NULL && tree->right == NULL)
        return tree->left->height + 1;
    // if the tree does not have a left child, returns -1 - right
    else if (tree->right != NULL && tree->left == NULL)
        return -1 - tree->right->height;
    // if the tree does have both childs, returns the difference between left and right
    else if (tree->right != NULL && tree->left != NULL)
        return tree->left->height - tree->right->height;
}

// this method does the left-left rotation for AVL Trees
AVLNodePtr left_left_rotation(AVLNodePtr tree)
{
    // rotation number is incremented
    numberOfRotationsAVL++;
    // left sub tree and left subtree's right child are pointed by pointers
    AVLNodePtr rootLeft = tree->left;
    AVLNodePtr rootLeftCRightC = rootLeft->right;

    // put the tree to the root's left's right
    rootLeft->right = tree;
    // the blank tree's left now will be the root's left subtree's right child
    tree->left = rootLeftCRightC;

    // change their height, I made this equation from an example
    rootLeft->right->height = rootLeft->right->height - 2;
    
    // after the rotations are done and height is updated, return the new tree root as root's left
    return rootLeft;
}

// this method does the right-right rotation for AVL Trees
AVLNodePtr right_right_rotation(AVLNodePtr tree)
{
    // rotation number is incremented
    numberOfRotationsAVL++;
    // right subtree and right subtree's left child are pointed by pointers
    AVLNodePtr rootRight = tree->right;
    AVLNodePtr rootRightCLeftC = rootRight->left;

    // put the tree to the root's rigth's left
    rootRight->left = tree;
    // the blank tree's rigth now will be the root's right subtree's left child
    tree->right = rootRightCLeftC;

    // change their height, I made this equation from an example
    rootRight->left->height =  rootRight->left->height - 2;

    // after the rotations are done and height is updated, return the new tree root as root's right
    return rootRight;
}
// this method will insert the key to the tree
AVLNodePtr insertToAVL(AVLNodePtr tree, int key)
{
    // if tree is null
    if (tree == NULL)
    {
        // allocate memory for the tree
        tree = malloc(sizeof(AVLNode));
        // if the memory allocated propperly
        if (tree != NULL)
        {
            // put the values to the node and return the node created
            tree->height = 0;
            tree->frequency = 1;
            tree->key = key;
            tree->left = NULL;
            tree->right = NULL;
            return tree;
        }
        // if not properly allocated
        else
        {
            // print the message and return null
            printf("The memory cannot be allocted.");
            return NULL;
        }
    }
    // now we will find the place to insert the key
    // if key is larger than the tree's
    if (tree->key < key)
    {
        // since one more comparison is done, incremented 
        numberOfComparisonAVL++;
        // since this key should be in the tree's right, we will call the method to recursively find the right place
        tree->right = insertToAVL(tree->right, key);
        // after the insertion is done, update the heights
        // if the tree does not have a left child, update it to the right + 1
        if (tree->left == NULL && tree->right != NULL)
            tree->height = tree->right->height + 1;
        // if the tree does not have a right child, update it to the left + 1
        else if (tree->right == NULL && tree->left != NULL)
            tree->height = tree->left->height + 1;
        // if both children exists, update it to the maximum of the right and left + 1 
        else if (tree->left != NULL && tree->right != NULL)
        {
            // if the left height is more, update it to the left + 1
            if(tree->left->height > tree->right->height)
                tree->height = tree->left->height + 1;
            // if the right height is more, update it to the right + 1 (or when they are the same)
            else if(tree->left->height <= tree->right->height)
                tree->height = tree->right->height + 1;
        }

        // when the left side has more height, which is being more than 1 and the key which causes the problem
        // alpha node is the left left of the key
        if (isBalanced(tree) > 1 && tree->left->key > key)
        {
            // perform the LL rotation and assign it to tree
            tree = left_left_rotation(tree);
            // return it
            return tree;
        }
        // when the right side has more height, which is being less than -1 and the key which causes the problem
        // alpha node is the right rigth of the key
        if (isBalanced(tree) < -1 && tree->right->key < key)
        {
            // perform the RR rotation and assign it to tree
            tree = right_right_rotation(tree);
            // return it
            return tree;
        }
        // when the left side has more height but this time alpha node is in the left-right of the key
        if (isBalanced(tree) > 1 && tree->left->key < key)
        {
            // we will rotate this twice since it is a double rotation RL
            // tree's left will be the result of right right rotation that means one of the 
            // rotation is done
            tree->left = right_right_rotation(tree->left);
            // then we need to rotate it with left left once more so the node in the bottom(out of 3) will go up
            tree = left_left_rotation(tree);
            // then increase the heights of the tree's we rotated after the rotations 
            tree->height++;
            tree->left->height++;
            // return the tree
            return tree;

        }
        // when the right side has more height but this time alpha node is in the right-left of the key
        if (isBalanced(tree) < -1 && tree->right->key > key)
        {
            // we will rotate this twice since it is a double rotation LR
            // tree's right will be the result of left left rotation that means one of the 
            // rotation is done
            tree->right = left_left_rotation(tree->right);
            // then we need to rotate it with right right once more so the node in the bottom(out of 3) will go up
            tree = right_right_rotation(tree);
            // then increase the heights of the tree's we rotated after the rotations 
            tree->height++;
            tree->right->height++;
            // return the tree
            return tree;
        }
    }
    // the same procedure applies here as well
    // this time the key is in left subtree
    else if (tree->key > key)
    {
        // since one more comparison is done, incremented 
        numberOfComparisonAVL++;
        // since this key should be in the tree's left, we will call the method to recursively find the right place
        tree->left = insertToAVL(tree->left, key);

        // after the insertion is done, update the heights
        // if the tree does not have a left child, update it to the right + 1
        if (tree->left == NULL && tree->right != NULL)
            tree->height = tree->right->height + 1;
        // if the tree does not have a right child, update it to the left + 1
        else if (tree->right == NULL && tree->left != NULL)
            tree->height = tree->left->height + 1;
        // if both children exists, update it to the maximum of the right and left + 1 
        else if (tree->left != NULL && tree->right != NULL)
        {
            // if the left height is more, update it to the left + 1
            if(tree->left->height > tree->right->height)
                tree->height = tree->left->height + 1;
            // if the right height is more, update it to the right + 1 (or when they are the same)
            else if(tree->left->height <= tree->right->height)
                tree->height = tree->right->height + 1;
        }

        // when the left side has more height, which is being more than 1 and the key which causes the problem
        // alpha node is the left left of the key
        if (isBalanced(tree) > 1 && tree->left->key > key)
        {
            // perform the LL rotation and assign it to tree
            tree = left_left_rotation(tree);
            // return it
            return tree;
        }
        // when the right side has more height, which is being less than -1 and the key which causes the problem
        // alpha node is the right rigth of the key
        if (isBalanced(tree) < -1 && tree->right->key < key)
        {
            // perform the RR rotation and assign it to tree
            tree = right_right_rotation(tree);
            // return it
            return tree;
        }
        // when the left side has more height but this time alpha node is in the left-right of the key
        if (isBalanced(tree) > 1 && tree->left->key < key)
        {
            // we will rotate this twice since it is a double rotation RL
            // tree's left will be the result of right right rotation that means one of the 
            // rotation is done
            tree->left = right_right_rotation(tree->left);
            // then we need to rotate it with left left once more so the node in the bottom(out of 3) will go up
            tree = left_left_rotation(tree);
            // then increase the heights of the tree's we rotated after the rotations 
            tree->height++;
            tree->left->height++;
            // return the tree
            return tree;

        }
        // when the right side has more height but this time alpha node is in the right-left of the key
        if (isBalanced(tree) < -1 && tree->right->key > key)
        {
            // we will rotate this twice since it is a double rotation LR
            // tree's right will be the result of left left rotation that means one of the 
            // rotation is done
            tree->right = left_left_rotation(tree->right);
            // then we need to rotate it with right right once more so the node in the bottom(out of 3) will go up
            tree = right_right_rotation(tree);
            // then increase the heights of the tree's we rotated after the rotations 
            tree->height++;
            tree->right->height++;
            // return the tree
            return tree;
        }
    }
    // if the key exists in the tree
    else
    {
        // increment comparisons by one
        numberOfComparisonAVL++;
        // increment the frequency
        tree->frequency++;
        // return that tree so that it can go out from recursion
        return tree;
    }
    // return the tree incase
    return tree;
}

// this method actual does bst insertion for splay before rotating to splay
SplayNodePtr insertSplay(SplayNodePtr tree, int key)
{
    // if tree is null
    if (tree == NULL) 
    {
        // then allocate memory for tree
        tree = malloc(sizeof(SplayNode));
        // if it does not allocated properly
        if (tree == NULL){
            // give the error message and return null
            printf("The memory cannot be allocated.");
            return NULL;
        }
        // otherwise put the given data to the node
        else
        {
            tree->key = key;
            tree->frequency = 1;
            tree->left = NULL;
            tree->right = NULL;
            return tree;
        }
    }
    // now if it is not null then we will search for the place to insert
    // if it is in the left
    else if(key < tree->key)
    {
        // increment the comparison number
        numberOfComparisonSplay++;
        // make the tree's left assigned to the recursive function in the left
        tree->left = insertSplay(tree->left, key);
    }
    // if it is in the right
    else if (key > tree->key)
    {
        // increment comparison number
        numberOfComparisonSplay++;
        // make the tree's right assigned to the recursive function in the right
        tree->right = insertSplay(tree->right, key);
    }
    // if it exists in the tree
    else if(tree->key == key)
    {
        // increment the comparison number
        numberOfComparisonSplay++;
        // increment the frequency
        tree->frequency++;
        // return the tree
        return tree;
    }
    // return the tree to continue recursion
    return tree;
}

// same methods as avl without the height
// this method makes (x-r) rotation in splay 
SplayNodePtr left_left_rotationSplay(SplayNodePtr tree)
{
    // rotation number is incremented
    numberOfRotationsSplay++;
    // left sub tree and left subtree's right child are pointed by pointers
    SplayNodePtr rootLeft = tree->left;
    SplayNodePtr rootLeftCRightC = rootLeft->right;
    // put the tree to the root's left's right
    rootLeft->right = tree;
    // the blank tree's left now will be the root's left subtree's right child
    tree->left = rootLeftCRightC;
    // return after the rotation
    return rootLeft;
}
// this method makes (x-r) rotation in splay 
SplayNodePtr right_right_rotationSplay(SplayNodePtr tree)
{
    // rotation number is incremented
    numberOfRotationsSplay++;
    // right sub tree and right subtree's left child are pointed by pointers
    SplayNodePtr rootRight = tree->right;
    SplayNodePtr rootRightCLeftC = rootRight->left;
    // put the tree to the root's rigth's left
    rootRight->left = tree;
    // the blank tree's right now will be the root's right subtree's left child
    tree->right = rootRightCLeftC;
    // return after the rotation
    return rootRight;
}

// this method splays the key given - an outline for all rotations
SplayNodePtr splay(SplayNodePtr tree, int key, int rootkey)
{
    // first we find the key calling the splay method recursively
    if(key > tree->key)
    {
        tree->right = splay(tree->right, key, rootkey);
    }
    else if(key < tree->key)
    {
        tree->left = splay(tree->left,key,rootkey);
    }
    else
        return tree;
    // if it is null then return null
    if(tree == NULL)
        return NULL;
    
    // x - r rotations
    // checks if the right of the tree is not null and it is actually the root by checking the 
    // rootkey equals to tree's key and also tree's right key will be equal to key
    else if(tree->right != NULL && tree->right->key == key && rootkey == tree->key)
    {
        // then do one x-r rotation in right situation
        // call right_right rotation for that (same as the x-r right)
        tree = right_right_rotationSplay(tree);
        return tree;
    }
    // checks if the left of the tree is not null and it is actually the root by checking the 
    // rootkey equals to tree's key and also tree's left key will be equal to key
    else if(tree->left != NULL && tree->left->key == key && rootkey == tree->key)
    {
        // then do one x-r rotation in left situation
        // call left_left rotation for that (same as the x-r left)
        tree = left_left_rotationSplay(tree);
        return tree;
    }
    // zigzig rotations
    // checks if the tree's right and right right exists and do the zig zig right
    // if the key is equal to the tree's right right's key
    else if(tree->right != NULL && tree->right->right != NULL && tree->right->right->key == key)
    {
        // then this is two times right right rotation (two x-r rotation)
        tree = right_right_rotationSplay(tree);
        tree = right_right_rotationSplay(tree);
        return tree;
    }
    // checks if the tree's left and left left exists and do the zig zig left
    // if the key is equal to the tree's left left's key
    else if(tree->left != NULL && tree->left->left != NULL && tree->left->left->key == key)
    {
        // then this is two times left left rotation (two x-r rotation)
        tree = left_left_rotationSplay(tree);
        tree = left_left_rotationSplay(tree);
        return tree;
    }
    // zigzag rotations
    // checks if the tree's right and right left exists and do the zig zag right
    // if the key is equal to the tree's right left's key
    else if(tree->right != NULL && tree->right->left != NULL && tree->right->left->key == key)
    {
        // like RL and LR double rotation
        // we need to put the bottom to the top but it is 2 buttom instead of 1 buttom
        // so 2 rotations
        // tree's right will be assigned to left_left rotation of itself
        tree->right = left_left_rotationSplay(tree->right);
        // then rotate the whole tree right_right
        tree = right_right_rotationSplay(tree);
        return tree;
    }
    // checks if the tree's left and left right exists and do the zig zag left
    // if the key is equal to the tree's left right's key
    else if(tree->left != NULL && tree->left->right != NULL && tree->left->right->key == key)
    {
        // like RL and LR double rotation
        // we need to put the bottom to the top but it is 2 buttom instead of 1 buttom
        // so 2 rotations
        // tree's left will be assigned to right_right rotation of itself
        tree->left = right_right_rotationSplay(tree->left);
        // then rotate the whole tree left_left
        tree = left_left_rotationSplay(tree);
        return tree;
    }
    // no case then return tree to continue recursion
    else
    {
    return tree;
    }

}

// prints preorder
void printPreOrderAVL(AVLNodePtr *tree)
{
    // if *tree is not null
    if ((*tree) != NULL)
    {
        // print itself
        printf("%d ", (*tree)->key);
        // call to print left recursively
        printPreOrderAVL(&((*tree)->left));
        // call to print right recursively
        printPreOrderAVL(&((*tree)->right));
    }
    // if null then return
    else
        return;
}

// prints preorder
void printPreOrderSplay(SplayNodePtr *tree)
{
    // if *tree is not null
    if ((*tree) != NULL)
    {
        // print itself
        printf("%d ", (*tree)->key);
        // call to print left recursively
        printPreOrderSplay(&((*tree)->left));
        // call to print right recursively
        printPreOrderSplay(&((*tree)->right));
    }
    // if null then return
    else
        return;
}

// main method
int main(int argc, char **argv)
{
    // initialize a number to read
    int number = 0;
    // initialize the AVL tree pointer null
    AVLNodePtr treeAVL = NULL;
    // open the file for avl
    FILE *textFileAVL = fopen(argv[1],"r");
    // print avl tree:
    printf("AVL tree:\n");
    // read the file with fscanf
    while (fscanf(textFileAVL, "%d ", &number) == 1)
    {
        // call the insertion method and assign its return value to treeAVL
        treeAVL = insertToAVL(treeAVL, number);
    }
    // after reading print preorder
    printPreOrderAVL(&treeAVL);
    // print the cost for avl
    printf("\nTotal cost: %d\n", numberOfComparisonAVL + numberOfRotationsAVL);
    // close for avl 
    fclose(textFileAVL);

    // now initialize the Splay tree pointer null
    SplayNodePtr treeSplay = NULL;
    // open file for splay this time
    FILE *textFileSplay = fopen(argv[1], "r");
    // print splay tree:
    printf("Splay tree:\n");
    // read the numbers using fscanf
    while (fscanf(textFileSplay, "%d ", &number) == 1)
    {
        // call the insertion method and assign its return value to treeSplay
        treeSplay = insertSplay(treeSplay, number);
        // call the splay method and assign its return value to treeSplay
        treeSplay = splay(treeSplay, number, treeSplay->key);
    }
    // after reading print preorder
    printPreOrderSplay(&treeSplay);
    // print the cost for splay
    printf("\nTotal cost: %d", numberOfComparisonSplay + numberOfRotationsSplay);
    // close for splay
    fclose(textFileSplay);
    // return the main method
    return 0;
}
