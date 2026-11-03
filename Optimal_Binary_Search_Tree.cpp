/*Problem Statement for 2D dp

You are given n keys in sorted order, k1 < k2 < ... < kn, and for each key a search frequency freq[i] (how often that key is searched). Build a binary search tree from these keys so that the total search cost is minimum.

The cost of searching a key is its level in the tree (root is level 1). So:

Total cost = sum of ( freq[i] * level of key i )

Output: the minimum possible total cost.

Example:

keys = {10, 12, 20}, freq = {34, 8, 50} → Output: 142*/

#include<bits/stdc++.h>

using namespace std;

int optimalBST(vector<int>&freq)
{
    int n = freq.size();
    const int INF = 1e9;

    vector<int>pre(n+1, 0);
    for(int i=1; i<=n; i++)
    {
        pre[i]=pre[i-1]+freq[i-1];
    }

    vector<vector<int>> dp(n+2, vector<int>(n+1, 0));
    for(int len=1; len<=n; len++)
    {
        for(int i=1; i+len-1<=n; i++)
        {
            int j=i+len-1;
            dp[i][j]=INF;
            int sum=pre[j]-pre[i-1];

            for(int r=i; r<=j; r++)
            {
                int cost = dp[i][r-1]+dp[r+1][j]+sum;
                dp[i][j]=min(dp[i][j], cost);
            }
        }
    }

    return dp[1][n];
}

int main()
{
    int n;
    cout << "Enter number of keys: ";
    cin >> n;

    vector<int> keys(n);
    cout << "Enter the keys(ascending order): ";
    for(int i=0; i<n; i++)
    {
        cin >> keys[i];
    }

    vector<int> freq(n);
    cout << "Enter frequencies:";
    for(int i=0; i<n; i++)
    {
        cin >> freq[i];
    }

    cout << "Minimum Search Cost: " << optimalBST(freq) << endl;

    return 0;

}

/*Problem Statement for 3D dp

You are given n keys in sorted order with search frequencies freq[1..n], and an integer H. Build a BST whose height is at most H (a single node has height 1) so that the total search cost is minimum:

Total cost = sum of ( freq[i] * level of key i )

Output: the minimum cost, or -1 if no BST of height at most H can hold all n keys.

Examples (freq = {34, 8, 50}):

H = 3 → 142 (same as the unrestricted answer)
H = 2 → 176 (the tree must be balanced)
H = 1 → -1 (one level holds only one key)

#include <bits/stdc++.h>
using namespace std;

int optimalBSTHeight(vector<int>& freq, int H) {
    int n = freq.size();
    const int INF = 1e9;

    vector<int> pre(n + 1, 0);
    for (int i = 1; i <= n; i++) pre[i] = pre[i - 1] + freq[i - 1];

    // dp[h][i][j]
    vector<vector<vector<int>>> dp(
        H + 1, vector<vector<int>>(n + 2, vector<int>(n + 1, INF)));

    // Base case: empty ranges cost 0 for every height
    for (int h = 0; h <= H; h++)
        for (int i = 1; i <= n + 1; i++)
            dp[h][i][i - 1] = 0;

    for (int h = 1; h <= H; h++) {
        for (int len = 1; len <= n; len++) {
            for (int i = 1; i + len - 1 <= n; i++) {
                int j = i + len - 1;
                int sum = pre[j] - pre[i - 1];

                for (int r = i; r <= j; r++) {
                    int left  = dp[h - 1][i][r - 1];
                    int right = dp[h - 1][r + 1][j];
                    if (left != INF && right != INF) {
                        dp[h][i][j] = min(dp[h][i][j], left + right + sum);
                    }
                }
            }
        }
    }

    return dp[H][1][n] == INF ? -1 : dp[H][1][n];
}

int main() {
    int n, H;
    cout << "Enter number of keys: ";
    cin >> n;

    vector<int> freq(n);
    cout << "Enter frequencies (keys in sorted order): ";
    for (int i = 0; i < n; i++) cin >> freq[i];

    cout << "Enter maximum height H: ";
    cin >> H;

    cout << "Minimum search cost: " << optimalBSTHeight(freq, H) << endl;
    return 0;
}
    */