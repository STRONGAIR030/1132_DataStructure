class Solution
{
public:
    int findLucky(vector<int> &arr)
    {
        vector<int> includeNums;
        vector<int> luckNums;
        for (int i = 0; i < arr.size(); i++)
        {
            bool notExit = true;
            for (int j = 0; j < includeNums.size(); j++)
            {
                if (includeNums[j] == arr[i])
                {
                    notExit = false;
                    break;
                }
            }
            if (notExit)
                includeNums.push_back(arr[i]);
        }
        for (int i = 0; i < includeNums.size(); i++)
        {
            int count = 0;
            for (int j = 0; j < arr.size(); j++)
            {
                if (includeNums[i] == arr[j])
                    count++;
                if (count > includeNums[i])
                    break;
            }
            if (count == includeNums[i])
                luckNums.push_back(includeNums[i]);
        }
        int max = -1;
        for (int i = 0; i < luckNums.size(); i++)
        {
            if (max < luckNums[i])
                max = luckNums[i];
        }
        if (luckNums.size())
        {
            return max;
        }
        else
        {
            return -1;
        }
    }
};