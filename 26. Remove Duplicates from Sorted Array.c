int removeDuplicates(int *nums, int numsSize)
{
    // Base cases:
    // 1. Empty array
    // 2. Negative size
    if ((!nums || numsSize == 0) ||
        (numsSize < 0))
        return 0;

    // 2. Only 1 element
    if (numsSize == 1)
        return 1;

    // 3. Two different elements
    if (numsSize == 2 && nums[0] != nums[1])
        return 2;
    else if (numsSize == 2 && nums[0] == nums[1])
        return 1;

    // General case: Use two pointers to find unique elements
    // Time complexity: O(n)
    // Space complexity: O(1)
    int n = 0;
    for (int i = 0; i < numsSize; ++i)
        if (nums[i] != nums[n])
            nums[++n] = nums[i];
    return n + 1;
}

#include <assert.h>
#include <stdlib.h>

int main()
{
    assert(removeDuplicates(NULL, 0) == 0);
    assert(removeDuplicates(NULL, -1) == 0);
    assert(removeDuplicates(NULL, 1) == 0);

    int nums1[] = {1};
    assert(removeDuplicates(nums1, 1) == 1);

    int nums2[] = {1, 2};
    assert(removeDuplicates(nums2, 2) == 2);

    int nums3[] = {2, 2};
    assert(removeDuplicates(nums3, 2) == 1);

    int nums4[] = {1, 1, 2};
    assert(removeDuplicates(nums4, 3) == 2);

    int nums5[] = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    assert(removeDuplicates(nums5, 10) == 5);

    int nums6[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    assert(removeDuplicates(nums6, 10) == 1);

    int nums7[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    assert(removeDuplicates(nums7, 10) == 10);

    return EXIT_SUCCESS;
}
