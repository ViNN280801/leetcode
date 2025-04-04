struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution
{
public:
    TreeNode *subtreeWithAllDeepest(TreeNode *root) noexcept { return _dfs(root).node; }

private:
    struct node_depth_t
    {
        TreeNode *node;
        int depth;
    };

    node_depth_t _dfs(TreeNode *node) noexcept
    {
        // 1. Base case - empty tree
        if (!node)
            return {nullptr, 0};

        // 1. Recursively traversing left subtree
        auto [leftnode, leftdepth]{_dfs(node->left)};

        // 2. Recursively traversing right subtree
        auto [rightnode, rightdepth]{_dfs(node->right)};

        // 3. If left depth == right depth, then the current node is the smallest subtree with all the deepest nodes
        if (leftdepth == rightdepth)
            return {node, leftdepth + 1};

        // 4. If the left depth is greater -> return the left node
        if (leftdepth > rightdepth)
            return {leftnode, leftdepth + 1};

        // 5. If the right depth is greater -> return the right node
        return {rightnode, rightdepth + 1};
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
    EXPECT_EQ(solution.subtreeWithAllDeepest(root)->val, 2);
}

TEST(Solution, test2)
{
    Solution solution;
    TreeNode *root = new TreeNode(1);
    EXPECT_EQ(solution.subtreeWithAllDeepest(root)->val, 1);
}

TEST(Solution, test3)
{
    Solution solution;
    TreeNode *root = new TreeNode(0);
    root->left = new TreeNode(1);
    root->right = new TreeNode(3);
    root->left->right = new TreeNode(2);
    EXPECT_EQ(solution.subtreeWithAllDeepest(root)->val, 2);
}

// Compiling commmand
// g++ -Wall -Wpedantic -Wextra -Werror 865.\ Smallest\ Subtree\ with\ all\ the\ Deepest\ Nodes.cpp -lgtest -lgtest_main
