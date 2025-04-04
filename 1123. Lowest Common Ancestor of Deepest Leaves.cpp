struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

/*
Given the root of a binary tree, return the lowest common ancestor of its deepest leaves.

Recall that:
- The node of a binary tree is a leaf if and only if it has no children
- The depth of the root of the tree is 0. if the depth of a node is d, the depth
  of each of its children is d + 1.
- The lowest common ancestor of a set S of nodes, is the node A with the largest depth
  such that every node in S is in the subtree with root A.
*/

namespace utils
{
    inline constexpr int max(int a, int b) noexcept { return a > b ? a : b; }
}

class Solution
{
private:
    struct node_depth_t
    {
        TreeNode *node;
        int depth;
    };

    // Assumes that the node is not nullptr.
    // If the node is nullptr -> undefined behavior.
    // Time complexity: O(1)
    // Space complexity: O(1)
    // Reason: We only check if the node is a leaf.
    constexpr bool _isLeaf(TreeNode *node) { return !node->left && !node->right; }

    // Time complexity: O(n)
    // Reason: We traverse the tree once.
    // Space complexity: O(n)
    // Reason: We use a stack to store the nodes.
    node_depth_t _dfs(TreeNode *node)
    {
        if (!node)
            return {nullptr, 0};
        auto [leftLCA, leftDepth]{_dfs(node->left)};
        auto [rightLCA, rightDepth]{_dfs(node->right)};

        // Case 1. Left depth == right depth
        if (leftDepth == rightDepth)
            return {node, leftDepth + 1};

        // Case 2. Left depth > right depth
        if (leftDepth > rightDepth)
            return {leftLCA, leftDepth + 1};

        // Case 3. Right depth > left depth
        return {rightLCA, rightDepth + 1};
    }

public:
    TreeNode *lcaDeepestLeaves(TreeNode *root)
    {
        /* ================== Edge cases: ================== */
        // 1. Empty tree
        if (!root)
            return nullptr;

        // 2. Single node
        if (_isLeaf(root))
            return root;
        /* ================== ============ ================== */

        // All the remaining cases
        return _dfs(root).node;
    }
};

#include <gtest/gtest.h>

TEST(Solution, test1)
{
    Solution solution;
    TreeNode *root = new TreeNode(3);
    root->left = new TreeNode(5);
    root->right = new TreeNode(1);
    root->left->left = new TreeNode(6);
    root->left->right = new TreeNode(2);
    root->right->left = new TreeNode(0);
    root->right->right = new TreeNode(8);
    root->left->right->left = new TreeNode(7);
    root->left->right->right = new TreeNode(4);
    EXPECT_EQ(solution.lcaDeepestLeaves(root)->val, 2);
}

TEST(Solution, test2)
{
    Solution solution;
    TreeNode *root = new TreeNode(1);
    EXPECT_EQ(solution.lcaDeepestLeaves(root)->val, 1);
}

TEST(Solution, test3)
{
    Solution solution;
    TreeNode *root = new TreeNode(0);
    root->left = new TreeNode(1);
    root->right = new TreeNode(3);
    root->left->right = new TreeNode(2);
    EXPECT_EQ(solution.lcaDeepestLeaves(root)->val, 2);
}

// Compiling commmand
// g++ -Wall -Wpedantic -Wextra -Werror 1123.\ Lowest\ Common\ Ancestor\ of\ Deepest\ Leaves.cpp -lgtest -lgtest_main
