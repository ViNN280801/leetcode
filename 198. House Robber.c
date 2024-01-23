#include <stdlib.h>

int rob(int const *const nums, int numsSize)
{
    if (!nums || numsSize == 0)
        return 0;
    if (numsSize == 1 && nums)
        return nums[0];
    if (numsSize == 2 && nums)
        return (nums[0] < nums[1]) ? nums[1] : nums[0];

    int *tmp = calloc(numsSize, sizeof(int));
    if (!tmp)
        return 0;

    tmp[0] = nums[0];
    tmp[1] = (nums[0] < nums[1]) ? nums[1] : nums[0];

    for (size_t i = 2ul; i < numsSize; ++i)
        tmp[i] = ((nums[i] + tmp[i - 2]) < tmp[i - 1]) ? tmp[i - 1] : (nums[i] + tmp[i - 2]);

    int result = tmp[numsSize - 1];
    free(tmp);
    return result;
}

