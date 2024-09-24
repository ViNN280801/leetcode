class Solution {
public:
    int kthFactor(int n, int k) {
        if (k < 1 || n < k || n > 1000)
            return -1;
        
        if (k == 1)
            return 1;
        
        int idx{1};
        for (int i{2}; i <= n; ++i)
        {
            if (n % i == 0)
                ++idx;
            if (idx == k)
                return i;
        }
        return -1;
    }
};