#include <vector>

class Solution {
public:
    void dfs(TreeNode *node, std::vector<int> &v)
    {
        if (!node)
            return;
        dfs(node->left, v);
        v.emplace_back(node->val);
        dfs(node->right, v);
    }
    
    std::vector<int> inorderTraversal(TreeNode* root) 
    {
        if (!root)
            return {};
        
        std::vector<int> v;
        dfs(root, v);
        return v;
    }
};

