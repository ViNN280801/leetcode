/*
You are given two positive integers low and high.

An integer x consisting of 2 * n digits is symmetric if the sum of the first n
digits of x is equal to the sum of the last n digits of x. Numbers with an
odd number of digits are never symmetric.

Return the number of symmetric integers in the range [low, high].

Example 1:

Input: low = 1, high = 100
Output: 9
Explanation: There are 9 symmetric integers between 1 and 100: 11, 22, 33, 44,
55, 66, 77, 88, and 99.

Example 2:

Input: low = 1200, high = 1230
Output: 4
Explanation: There are 4 symmetric integers between 1200 and 1230: 1203, 1212,
1221, and 1230.

Constraints:
1 <= low <= high <= 10^4
*/

#include <math.h>
#include <stdlib.h>

// Assumes only unsigned integers, but keep signed for return -1 in failure
int _count_digits(int num)
{
	// Base case:
	if (num < 0)
		return -1;

	// Time complexity: O(log10(num)), because num is divided by 10 in each iteration
	// Space complexity: O(1), because only a single variable is used
	int count = 0;
	while (num > 0)
	{
		count++;
		num /= 10;
	}
	return count;
}

int *num_to_array(int num)
{
	// Base case:
	if (num < 0)
		return NULL;

	// Counting digits in number
	// Time complexity: O(log10(num))
	// Space complexity: O(1)
	int count_of_digits = _count_digits(num);
	if (count_of_digits % 2 != 0)
		return NULL;

	// Allocating memory in heap and checking for failure
	// Time complexity of calloc: O(n), because it initializes all elements to 0
	// Space complexity: O(n), because we allocate memory for n elements
	int *arr = (int *)calloc(count_of_digits, sizeof(int));
	if (!arr)
		return NULL;

	// Important: Array will be in reverse order!
	// But now we exactly know, that we have even number of digits in the array
	// Time complexity: O(n), where n is the number of digits in the number
	// Space complexity: O(n), where n is the number of digits in the number
	for (int i = 0; i < count_of_digits; i++)
	{
		arr[i] = num % 10;
		num /= 10;
	}

	return arr;

	// Complexity resume:
	// Time complexity: O(log10(num)) + O(n) + O(n) = O(n)
	// Space complexity: O(1) + O(n) + O(n) = O(n)
}

// Returns 1 if symmetric, 0 if not, -1 in case of error
int _is_symmetric(int *arr, int size)
{
	// Base case:
	if (!arr || size <= 0)
		return -1;

	// Where:
	// sum1 = arr[size - 1] + arr[size - 2] + ... + arr[size / 2]
	// sum2 = arr[size / 2] + arr[size / 2 - 1] + ... + arr[0]
	// Because num_to_array() will return in reverse order, but there is no sense
	// how to sum, because by rearranging the places of the terms, the sum does not change
	int sum1 = 0, sum2 = 0;
	for (int i = 0; i < size; ++i)
	{
		if (i < size / 2)
			sum1 += arr[i];
		else
			sum2 += arr[i];
	}

	return (sum1 == sum2) ? 1 : 0;

	// Complexity resume:
	// Time complexity: O(n), because we iterate through the array once
	// Space complexity: O(1), because we use a constant amount of extra space for the 2 sums
}

int countSymmetricIntegers(int low, int high)
{
	/* ====== Base cases: ====== */
	// 1. low or high is negative
	// 2. low > high
	if ((low < 0 || high < 0) || (low > high))
		return -1;

	// 3. low == high and count of digits is odd
	if (low == high && _count_digits(low) % 2 != 0)
		return 0;

	// Base cases 1,2,3 are best cases:
	// Time complexity: O(1)
	// Space complexity: O(1)

	// 3. low == high
	// we can check only one number immediately
	// This case is the middle case:
	// Time complexity: O(log10(num)) + O(n) + O(n) = O(n)
	// Space complexity: O(1) + O(n) + O(1) = O(n)
	if (low == high)
		return _is_symmetric(num_to_array(low), _count_digits(low));
	/* ========================== */

	/* ====== Counting symmetric integers: ====== */
	// Worst case:
	// Time complexity: O(n/2), because we skip half of the numbers
	// Space complexity: O(1), because we don't use any extra space (in for loop)
	int count = 0;
	for (int i = low; i <= high; ++i)
	{
		// For optimization, we can skip entire ranges of numbers.
		// Let's say we have a range [1;1'000'000]
		// So, there are the following ranges:
		// 1) [1;9] - here the number of digits is 1, so we can skip the entire range
		// 2) [10;99] - here the number of digits is 2, so we check the range
		// 3) [100;999] - here the number of digits is 3, so we skip the range
		// 4) [1000;9999] - here the number of digits is 4, so we check the range
		// 5) [10'000;99'999] - here the number of digits is 5, so we skip the range
		// 6) [100'000;999'999] - here the number of digits is 6, so we check the range
		// 7) [1'000'000;1'000'000] - here the number of digits is 7, so we skip the range
		// How to skip ranges?
		// Checking the number of digits of the current number (i)
		// - If the number of digits is odd, then we raise 10 to the power of i
		// - If the number of digits is even, then we don't skip anything
		// Example:
		// i = 1, number of digits = 1, odd => i = 10^i => i = 10^1 = 10
		// i = 10, number of digits = 2, even => nothing to skip
		// i = 100, number of digits = 3, odd => i = 10^i => i = 10^3 = 1000
		// i = 1000, number of digits = 4, even => nothing to skip
		// i = 10000, number of digits = 5, odd => i = 10^i => i = 10^5 = 100000
		// i = 100000, number of digits = 6, even => nothing to skip
		// i = 1000000, number of digits = 7, odd => i = 10^i => i = 10^7 = 10000000

		// Time complexity: O(log10(num)) => _count_digits()
		// Space complexity: O(1) => _count_digits()
		int digits = _count_digits(i);
		if (digits % 2 != 0)
		{
			// Time complexity: O(log10(num))
			// Space complexity: O(1)
			i = (int)pow(10, digits);
			if (i > high)
				break;
		}

		// Time complexity: O(n) + O(n) + O(n) = O(n) => else
		// Space complexity: O(n) + O(1) + O(1) = O(n) => else
		else
		{
			// Time complexity: O(n) => num_to_array()
			// Space complexity: O(n) => num_to_array()
			int *num_arr = num_to_array(i);

			// Time complexity: O(n) => _is_symmetric()
			// Space complexity: O(1) => _is_symmetric()
			if (_is_symmetric(num_arr, digits) == 1)
				count++;

			// Time complexity: O(n) => free()
			// Space complexity: O(1) => free()
			free(num_arr);
		}
	}
	/* ========================================== */

	return count;

	// Complexity resume:
	// Time complexity: O(n/2) * [O(log10(num)) or O(n)] = O(n)
	// Space complexity: O(1) * [O(1) or O(n)] = O(n)
}

#include <assert.h>

int main()
{
	assert(countSymmetricIntegers(-1, 100) == -1);
	assert(countSymmetricIntegers(100, -1) == -1);
	assert(countSymmetricIntegers(100, 100) == 0);
	assert(countSymmetricIntegers(1000, 1000) == 0);
	assert(countSymmetricIntegers(101, 101) == 0);
	assert(countSymmetricIntegers(1001, 1001) == 1);
	assert(countSymmetricIntegers(2, 1) == -1);
	assert(countSymmetricIntegers(1, 100) == 9);
	assert(countSymmetricIntegers(1200, 1230) == 4);
	return 0;
}
