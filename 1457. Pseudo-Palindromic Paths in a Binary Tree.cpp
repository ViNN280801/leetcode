/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution final {
private:
    constexpr bool isPseudoPalindrome(int n) const { return (n & (n - 1)) == 0; }

    int countPseudoPalindromicPaths(TreeNode *node, int n) const
    {
        if (!node)
            return 0;

        n ^= 1 << (node->val - 1);

        if (!node->left && !node->right)
            return isPseudoPalindrome(n) ? 1 : 0;
        return countPseudoPalindromicPaths(node->left, n) + countPseudoPalindromicPaths(node->right, n);
    }
public:
    constexpr int pseudoPalindromicPaths (TreeNode* root) const { return countPseudoPalindromicPaths(root, 0); }
};
