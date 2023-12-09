#include <stdlib.h>

void inorder(struct TreeNode *node, int *elems, int *sz)
{
    if (!node || !elems || !sz)
        return;
    inorder(node->left, elems, sz);
    elems[(*sz)++] = node->val;
    inorder(node->right, elems, sz);
}

int *inorderTraversal(struct TreeNode *root, int *sz) 
{
    int *elems = calloc(100ul, sizeof(int));
    if (!elems)
        return elems;

    *sz = 0;
    inorder(root, elems, sz);
    return elems;
}

