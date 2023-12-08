#include <string>

class Solution {
public:
    void preorderTraverse(TreeNode *root, std::string &s)
    {
        if (!root)
            return;
        
        s += std::to_string(root->val);
        if (!root->left && !root->right)
            return;
        s += '(';
        preorderTraverse(root->left, s);
        s += ')';
        if (root->right)
        {
            s += '(';
            preorderTraverse(root->right, s);
            s += ')';
        }
    }

    std::string tree2str(TreeNode* root)
    {
        if (!root)
            return "";

        std::string res;
        preorderTraverse(root, res);
        return res;
    }
};

