// Method 1
class Solution {
   public:
    vector<int> smallerNumbersThanCurrent(vector<int>& nums) {
        int countStep = 0;
        countStep += 101;      // count arry initialization
        int count[101] = {0};  // 計數陣列,範圍 0-100

        countStep++;         // n = nums.size()
        int n = num.size();  // 陣列長度

        countStep += n;         // result initialization
        vector<int> result(n);  // vector<int>是動態陣列,在執行分配記憶體時, 可以根據 num.size() 自動調整大小

        // 計算每個數字的出現次數,此迴圈遍歷 nums,並統計每個數字 num 出現的次數

        for (int num : nums) {
            countStep++;     // for loop
            countStep += 2;  // count[num]++
            count[num]++;
        }
        // 計算比當前數字小的數量
        for (int i = 1; i < 101; i++) {
            countStep++;     // for loop
            countStep += 5;  // count[i] += count[i - 1]
            count[i] += count[i - 1];
        }
        // 計算 result 陣列
        for (int i = 0; i < n; i++) {
            countStep++;     // for loop
            countStep += 4;  // result[i] = (nums[i] == 0) ? 0 : count[nums[i] - 1];
            result[i] = (nums[i] == 0) ? 0 : count[nums[i] - 1];
            // count[nums[i]-1]代表比 nums[i]小的數的數量,nums[i]==0 時則直接返回 0
        }

        countStep++;  // return result
        return result;
    }

    // CountStep = 101 + 1 + n + 3n(count num appear) + 101 * (5 + 1) + 5n + 1= 9n + 709
    // Time complexity: O(n)