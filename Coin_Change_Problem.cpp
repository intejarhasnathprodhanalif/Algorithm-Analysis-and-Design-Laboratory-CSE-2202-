/* Problem Statement for 1D dp

    You are given an array coins[] of distinct coin denominations and an integer amount.Each coin can be used an unlimited number of times.Find the minimum number of coins needed to make up amount.If it is impossible to make that amount,
    return -1.

           Example :

    coins = {1, 2, 5},
    amount = 11 → Output : 3(5 + 5 + 1)
        coins = {2},
    amount = 3 → Output : -1 */

    #include<bits/stdc++.h>

    using namespace std;

    int coinChange(vector<int>&coins, int amount)
    {
        const int INF=1e9;
        vector<int>dp(amount+1, INF);
        dp[0]=0;

        for(int i=1; i<=amount; i++)
        {
            for(int c : coins)
            {
                if(c<=i && dp[i-c]!=INF)
                {
                    dp[i]=min(dp[i], dp[i-c]+1);
                }
            }
        }

        return dp[amount] == INF ? -1 : dp[amount];
    }

    int main()
    {
        int n, amount;
        cout << "Enter number of coins: ";
        cin >> n;

        vector<int>coins(n);
        cout << "Enter value of the coins: ";
        for(int i=0; i<n; i++)
        {
            cin >> coins[i];
        }

        cout << "Enter the amount: ";
        cin >> amount;

        cout << "Minimum of coins needed: " << coinChange(coins, amount) << endl; 

        return 0;
    }

    /*Problem statement for 2D dp

You are given an array coins[] of n distinct coin denominations and an integer amount. You have an unlimited supply of each coin type. Find the minimum number of coins needed to make up exactly amount. If it is impossible to make that amount with the given coins, return -1.

Input
n: number of coin types
coins[0..n-1]: the coin denominations
amount: the target amount

Output
The minimum number of coins that sum to amount, or -1 if no combination works.
Example 1

Input:  coins = {1, 2, 5}, amount = 11
Output: 3
Explanation: 11 = 5 + 5 + 1

Example 2

Input:  coins = {2}, amount = 3
Output: -1
Explanation: 3 cannot be formed using only coins of value 2.

Example 3

Input:  coins = {1}, amount = 0
Output: 0
Explanation: Amount 0 needs no coins.*/

/*int coinChange(vector<int>&coins, int amount)
    {
        int n = coins.size();
        const int INF=1e9;
        vector<vector<int>>dp(n+1, vector<int>(amount+1, INF));
        
        for(int i=0; i<n; i++)
        {
            dp[i][0]=0;
        }

        for(int i=1; i<=n; i++)
        {
            for(int j=0; j<=amount; j++)
            {
                dp[i][j]=dp[i-1][j];

                if(coins[i-1]<=j && dp[i][j-coins[i-1]] != INF)
                {
                    dp[i][j] = min(dp[i][j], dp[i][j-coins[i-1]]+1);
                }
            }
        }

        return dp[n][amount] == INF ? -1 : dp[n][amount];
    }

    int main()
    {
        int n, amount;
        cout << "Enter number of coins: ";
        cin >> n;

        vector<int> coins(n);
        cout << "Enter value of the coins: ";
        for (int i = 0; i < n; i++)
        {
            cin >> coins[i];
        }

        cout << "Enter the amount: ";
        cin >> amount;

        cout << "Minimum of coins needed: " << coinChange(coins, amount) << endl;

        return 0;
    }*/