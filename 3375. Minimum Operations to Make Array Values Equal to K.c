/*
You are given an integer array nums and an integer k.

An integer h is called valid if all values in the array that are strictly
greater than h are identical.

For example, if nums = [10, 8, 10, 8], a valid integer is h = 9
because all nums[i] > 9 are equal to 10, but 5 is not a valid integer.

You are allowed to perform the following operation on nums:

- Select an integer h that is valid for the current values in nums.
- For each index i where nums[i] > h, set nums[i] to h.

Return the minimum number of operations required to make every element in nums
equal to k. If it is impossible to make all elements equal to k, return -1.


Example 1:

Input: nums = [5,2,5,4,5], k = 2
Output: 2
Explanation:
The operations can be performed in order using valid integers 4 and then 2.

Example 2:

Input: nums = [2,1,2], k = 2
Output: -1
Explanation:
It is impossible to make all the values equal to 2.

Example 3:

Input: nums = [9,7,5,3], k = 1
Output: 4
Explanation:
The operations can be performed using valid integers in the order 7, 5, 3,
and 1.

Constraints:

    1 <= nums.length <= 100
    1 <= nums[i] <= 100
    1 <= k <= 100
*/

#include <stdlib.h>
#include <uthash.h>

typedef struct {
  int key;
  UT_hash_handle hh;
} hash_item_t;

// Time complexity: O(1)
// Space complexity: O(n)
hash_item_t *hash_find_item(hash_item_t **hash_table, int key) {
  hash_item_t *p_entry = NULL;
  HASH_FIND_INT(*hash_table, &key, p_entry);
  return p_entry;
}

// Time complexity: O(1)
// Space complexity: O(n)
int hash_add_item(hash_item_t **obj, int key) {
  if (hash_find_item(obj, key)) {
    return -1;
  }
  hash_item_t *p_entry = (hash_item_t *)malloc(sizeof(hash_item_t));
  p_entry->key = key;
  HASH_ADD_INT(*obj, key, p_entry);
  return 0;
}

// Time complexity: O(n)
// Space complexity: O(1)
void hash_free(hash_item_t **obj) {
  hash_item_t *curr = NULL, *tmp = NULL;
  HASH_ITER(hh, *obj, curr, tmp) {
    HASH_DEL(*obj, curr);
    free(curr);
  }
}

// Time complexity: O(n)
// Space complexity: O(n)
int minOperations(int *nums, int numsSize, int k) {
  // Base cases:
  if (!nums || numsSize == 0)
    return -1;
  if (numsSize == 1 && nums[0] > k)
    return 1;

  // Time complexity: O(n) =>
  // for loop: O(numsSize)
  // hash_add_item: O(1)
  // hash_find_item: O(1)
  // hash_free: O(n)

  // Space complexity: O(n) =>
  // hash_item_t *st: O(n)
  hash_item_t *st = NULL;
  for (int i = 0; i < numsSize; i++) {
    if (nums[i] < k)
      return -1;
    else if (nums[i] > k)
      hash_add_item(&st, nums[i]);
  }
  int result = HASH_COUNT(st);
  hash_free(&st);
  return result;
}

#include <assert.h>
#include <stddef.h>

int main() {
  int nums[] = {5, 2, 5, 4, 5};
  int k = 2;
  int result = minOperations(nums, 5, k);
  assert(result == 2);

  int nums2[] = {2, 1, 2};
  k = 2;
  result = minOperations(nums2, 3, k);
  assert(result == -1);

  int nums3[] = {9, 7, 5, 3};
  k = 1;
  result = minOperations(nums3, 4, k);
  assert(result == 4);

  assert(minOperations(NULL, 0, 1) == -1);
  assert(minOperations((int[]){1}, 1, 1) == 0);
  assert(minOperations((int[]){2}, 1, 1) == 1);

  return 0;
}
