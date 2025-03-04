class Solution {
   public:
    vector<int> runningSum(vector<int>& nums) {
        int n = nums.size();    // array length
        vector<int> result(n);  // store the result
        result[0] = nums[0];    // the first element of the result is the first element of the nums

        // calculate the result array
        for (int i = 1; i < n; i++) {
            result[i] = result[i - 1] + nums[i];  // the result[i] is the sum of the result[i - 1] and currnet element of the nums
        }

        // return the result
        return result;
    }
};