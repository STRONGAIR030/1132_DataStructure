// Method 2
class Solution {
   public:
    vector<int> smallerNumbersThanCurrent(vector<int>& nums) {
        int countStep = 0;
        vector<int> result;  // 存結果
        int count;           // 計數器

        // 外層迴圈遍歷 nums 陣列中的每個元素
        for (int i = 0; i < nums.size(); i++) {
            countStep++;  // for loop

            countStep++;  // count = 0
            count = 0;    // 每次重新計算當前元素的較小數量

            // 內層迴圈再次遍歷 nums 陣列,統計比 nums[i] 小的元素數量
            for (int j = 0; j < nums.size(); j++) {
                countStep++;  // for loop

                countStep += 3;  // nums[j] < nums[i]
                if (nums[j] < nums[i]) {
                    countStep++;  // count++
                    count++;
                }  // 如果 nums[j] 比nums[i] 小,則計數器 +1
            }

            countStep++;              // result.push_back(count)
            result.push_back(count);  // 將計算出的數量存入 result
        }

        countStep++;  // return result
        return result;
    }
};

// CountStep = n[1 + 1 + n(1 + 3) + 1] + 1 = 4n^2 + 3n + 1
// Time complexity: O(n^2)