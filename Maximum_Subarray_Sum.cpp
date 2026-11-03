/*Problem Statement

You are given an array arr[] of n integers (positive, negative, or zero). Find the maximum sum of a contiguous, non-empty subarray.

Examples:

arr = {-2, 1, -3, 4, -1, 2, 1, -5, 4} → Output: 6 (subarray {4, -1, 2, 1})
arr = {-3, -1, -2} → Output: -1 (the subarray must be non-empty, so the best is the single element -1)
arr = {5} → Output: 5
*/

#include <bits/stdc++.h>
using namespace std;

// Max sum of a subarray that crosses mid
int maxCrossingSum(vector<int> &arr, int low, int mid, int high)
{
    // Best sum ending at mid, extending left
    int sum = 0;
    int leftBest = INT_MIN;
    for (int i = mid; i >= low; i--)
    {
        sum += arr[i];
        leftBest = max(leftBest, sum);
    }

    // Best sum starting at mid+1, extending right
    sum = 0;
    int rightBest = INT_MIN;
    for (int i = mid + 1; i <= high; i++)
    {
        sum += arr[i];
        rightBest = max(rightBest, sum);
    }

    return leftBest + rightBest;
}

int maxSubarray(vector<int> &arr, int low, int high)
{
    // Base case: one element
    if (low == high)
        return arr[low];

    int mid = low + (high - low) / 2;

    int leftMax = maxSubarray(arr, low, mid);
    int rightMax = maxSubarray(arr, mid + 1, high);
    int crossMax = maxCrossingSum(arr, low, mid, high);

    return max({leftMax, rightMax, crossMax});
}

int main()
{
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter elements: ";
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    cout << "Maximum subarray sum: " << maxSubarray(arr, 0, n - 1) << endl;
    return 0;
}