class Solution {
   public:
    int findLucky(vector<int> &arr) {
        vector<int> includeNums;  // to store the include numbers
        vector<int> luckNums;     // to store the lucky numbers

        // to find the include numbers
        for (int i = 0; i < arr.size(); i++) {
            bool notExit = true;  // to check the number is already in includeNums
            for (int j = 0; j < includeNums.size(); j++) {
                // if the number is already in the includeNums then break the loop
                if (includeNums[j] == arr[i]) {
                    notExit = false;  // if the number is already in includeNums then notExit = false
                    break;
                }
            }
            if (notExit)
                includeNums.push_back(arr[i]);  // push the number in includeNums that don't in includeNums
        }

        // to find the lucky numbers
        for (int i = 0; i < includeNums.size(); i++) {
            int count = 0;  // count the number of times the number is in the arr
            for (int j = 0; j < arr.size(); j++) {
                if (includeNums[i] == arr[j])
                    count++;
                if (count > includeNums[i])  // if the count is greater than the number then break the loop
                    break;
            }
            if (count == includeNums[i])  // if the count is equal to the number then push the number in the luckNums
                luckNums.push_back(includeNums[i]);
        }

        // to find the maximum number from the luckNums
        int max = -1;
        for (int i = 0; i < luckNums.size(); i++) {
            if (max < luckNums[i])  // if the max is less than the luckNums[i] then max = luckNums[i]
                max = luckNums[i];
        }

        // return max luck number or not fund
        if (luckNums.size()) {
            return max;
        } else {
            return -1;
        }
    }
};