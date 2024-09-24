class Solution:
    def kthFactor(self, n: int, k: int) -> int:
        if k < 1 or n < k or n > 1000:
            return -1

        if k == 1:
            return 1

        idx = 1
        for x in range(2, n + 1):
            if n % x == 0:
                idx += 1
            if idx == k:
                return x
        return -1
