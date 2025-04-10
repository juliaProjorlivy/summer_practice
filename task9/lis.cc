#include <algorithm>
#include <iostream>
#include <climits>
#include <vector>


template <typename T>
std::vector<T> get_lis_from_dp(std::vector<T> arr1, std::vector<T> arr2, std::vector<std::vector<T>> dp)
{
    int sz_x = dp[0].size();
    int sz_y = dp.size();

    int i_x = sz_x - 1;
    int i_y = sz_y - 1;

    std::vector<T> rez;
    while(i_x > 0 && i_y > 0)
    {
        // Go diagonal
        if(arr1[i_x - 1] == arr2[i_y - 1])
        {
            rez.push_back(arr1[i_x - 1]);
            --i_x;
            --i_y;
        }
        // Go up
        else if(dp[i_y - 1][i_x] > dp[i_y][i_x - 1])
        {
            --i_y;
        }
        else if(dp[i_y - 1][i_x] == dp[i_y][i_x - 1])
        {
            // Go where the number is larger
            if(arr1[i_x - 1] > arr2[i_y - 1])
            {
                --i_y;
            }
            else
            {
                --i_x;
            }
        }
        // Go left
        else
        {
            --i_x;
        }
    }

    std::reverse(rez.begin(), rez.end());
    return rez;
}

template <typename T>
std::vector<std::vector<T>> find_lcs(std::vector<T> arr1, std::vector<T> arr2)
{
    std::vector<std::vector<T>> dp(arr2.size() + 1, std::vector<T>(arr1.size() + 1, 0));
    for(int i = 1; i < arr2.size() + 1; ++i)
    {
        for(int j = 1; j < arr1.size() + 1; ++j)
        {
            if(arr2[i - 1] == arr1[j - 1])
            {
                dp[i][j] = dp[j - 1][i - 1] + 1;
            }
            else
            {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp;
}

// Get longest increasing subsequnce through longest common subsequence of a given array and sorted array
template <typename T>
std::vector<T> get_lis(std::vector<T> arr)
{
    // Get sorted array from given
    std::vector<T> sorted_arr(arr.begin(), arr.end());
    std::sort(sorted_arr.begin(), sorted_arr.end());

    // Get dp table
    std::vector<std::vector<T>> dp = find_lcs(sorted_arr, arr);
    // std::cout << "dp is " << std::endl;
    // std::for_each(dp.begin(), dp.end(), [](std::vector<T> x){std::for_each(x.begin(), x.end(),[](T y) {std::cout << y << " ";}); std::cout << std::endl;});
    // Calculate a subsequnce from the dp table
    std::vector<T> rez = get_lis_from_dp(sorted_arr, arr, dp);

    return rez;
}

int main()
{
    // Number of elements in array
    int n_elem = 0;
    std::cin >> n_elem;
    std::vector<int> arr(n_elem);
    // Get input data
    std::for_each(arr.begin(), arr.end(), [](int &x) {std::cin >> x;});

    std::vector<int> lis = get_lis(arr);

    std::for_each(lis.begin(), lis.end(), [](int x) {std::cout << x << ' ';});
    return 0;

}

