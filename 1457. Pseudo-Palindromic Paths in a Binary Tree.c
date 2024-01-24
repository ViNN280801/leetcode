int isPseudoPalindrome(int pathCount) { return (pathCount & (pathCount - 1)) == 0; }

int countPseudoPalindromicPaths(struct TreeNode *node, int pathCount)
{
    if (!node)
        return 0;
    
    pathCount ^= 1 << (node->val - 1);

    if (!node->left && !node->right)
        return isPseudoPalindrome(pathCount) ? 1 : 0;

    return countPseudoPalindromicPaths(node->left, pathCount) + countPseudoPalindromicPaths(node->right, pathCount);
}

int pseudoPalindromicPaths(struct TreeNode *root) { return countPseudoPalindromicPaths(root, 0); }

