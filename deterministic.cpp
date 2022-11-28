#include <bits/stdc++.h>
using namespace std;

double maxn = 1000000.0;

int n, h;
vector<double> mn;
vector<double> mx;

double production(vector<int> &arr, int ind, vector<double> &noLoad, vector<double> &inc, int p, vector<vector<double>> &dp)
{

    if (ind < 0)
    {
        if (p == 0)
            return 0;
        else
            return maxn;
    }

    if (p == 0)
        return maxn;

    int unit = arr[ind];

    if (dp[ind][p] != -1.0)
        return dp[ind][p];

    double ans = maxn;

    for (int x = mn[unit]; x <= min(mx[unit], p * 1.0); x++)
        ans = min(ans, noLoad[unit] + (inc[unit] * x) + production(arr, ind - 1, noLoad, inc, p - x, dp));

    dp[ind][p] = ans;

    return ans;
}

double calculate(int mask1, int mask2, vector<double> &cold)
{
    double ans = 0.0;

    int n = cold.size();

    for (int i = 0; i < n; i++)
        if ((mask2 & (1 << i)) == 0)
        {
            if (mask1 & (1 << i))
                ans = ans + cold[i];
        }

    return ans;
}

int main()
{

    cin >> n >> h;

    vector<vector<double>> pro(1 << n, vector<double>(h + 1, maxn));

    vector<int> power(h + 1, 0);
    vector<double> noLoad(n, 0);
    vector<double> inc(n, 0);
    vector<double> avgCost(n, 0);
    vector<double> cold(n, 0);
    mn.clear();
    mx.clear();
    mn.resize(n, 0);
    mx.resize(n, 0);

    set<int> states[h + 1];

    for (int i = 0; i < n; i++)
        cin >> noLoad[i] >> inc[i] >> mn[i] >> mx[i] >> avgCost[i] >> cold[i];

    for (int i = 1; i <= h; i++)
        cin >> power[i];

    for (int k = 1; k <= h; k++)
        for (int mask = 0; mask < (1 << n); mask++)
        {
            double sum1 = 0.0;
            double sum2 = 0.0;

            for (int j = 0; j < n; j++)
                if (mask & (1 << j))
                {
                    sum1 = sum1 + mn[j];
                    sum2 = sum2 + mx[j];
                }

            if ((sum2 >= power[k]) and (sum1 <= power[k]))
                states[k].insert(mask);
        }

    for (int k = 1; k <= h; k++)
        for (int mask : states[k])
        {
            vector<int> arr;

            for (int i = 0; i < n; i++)
                if (mask & (1 << i))
                    arr.push_back(i);

            int m = arr.size();

            vector<vector<double>> dp(m, vector<double>(800, -1.0));

            double z = production(arr, m - 1, noLoad, inc, power[k], dp);

            pro[mask][k] = z;
        }

    vector<pair<int, int>> arr;

    for (int i = 0; i < n; i++)
        arr.push_back({avgCost[i], i});

    sort(arr.begin(), arr.end());

    vector<int> priorMasks;
    int tMask = 0;

    for (int i = 0; i < arr.size(); i++)
    {
        int x = arr[i].second;
        tMask = tMask | (1 << x);

        priorMasks.push_back(tMask);
    }

    int mask = (1 << 2) + (1 << 1) + (1 << 3);

    priorMasks.push_back(mask);

    vector<int> finalState[h + 1];

    for (int k = 1; k <= h; k++)
        for (int mask : priorMasks)
            if (states[k].find(mask) != states[k].end())
                finalState[k].push_back(mask);

    vector<vector<double>> dp(h + 1, vector<double>(1 << n, maxn));

    double mn = maxn;
    int x = 0;

    for (int mask : states[1])
        if (mn > pro[mask][1])
        {
            mn = pro[mask][1];
            x = mask;
        }

    dp[1][x] = pro[x][1];

    for (int mask : finalState[1])
        if (mask != x)
            dp[1][mask] = calculate(mask, x, cold) + pro[mask][1];

    for (int k = 2; k <= h; k++)
        for (int mask : finalState[k])
            for (int prev : finalState[k - 1])
                dp[k][mask] = min(dp[k][mask], dp[k - 1][prev] + pro[mask][k] + calculate(mask, prev, cold));

    vector<double> hourCost(h + 1, 0.0);
    vector<int> hourMask(h + 1, 0);

    for (int k = 1; k <= h; k++)
    {
        int tans = 1e9;
        int tMask = 0;

        for (int mask : finalState[k])
            if (tans > dp[k][mask])
            {
                tans = dp[k][mask];
                tMask = mask;
            }

        hourCost[k] = tans;
        hourMask[k] = tMask;
    }

    vector<vector<int>> table(n, vector<int>(h + 1, 0));

    for (int k = 1; k <= h; k++)
        for (int i = 0; i < n; i++)
            if (hourMask[k] & (1 << i))
                table[i][k] = 1;

    for (int i = 0; i < n; i++)
    {
        cout << "  ";
        for (int j = 1; j <= h; j++)
            cout << table[i][j] << "       ";

        cout << endl;
    }

    cout << endl;
    cout << " ";
    for (int i = 1; i <= h; i++)
        cout << hourCost[i] << "   ";
    cout << endl;

    return 0;
}
