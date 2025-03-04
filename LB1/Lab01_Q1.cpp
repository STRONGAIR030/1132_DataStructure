class Solution {
   public:
    bool isMonotonic(vector<int> &nums) {
        if (nums.size() == 1) {
            // Comparison: Count = 1 (nums.size() == 1)
            return true;  // Count = 1 (return)
        }

        bool enq = true;  // Count = 1 (Assignment)
        bool inc = true;  // Count = 1 (Assignment)
        int i = 0;        // Count = 1 (Assignment)

        for (; i < nums.size() - 1; i++) {
            // Each iteration:
            // - Comparison: Count = 1 (i < arr.size())
            // - Increment: Count = 1 (i++)

            if (nums[i] != nums[i + 1]) {
                // Comparison: Count = 4 (nums[i] != nums[i + 1])
                enq = false;                  // Count = 1 (Assignment)
                inc = nums[i] < nums[i + 1];  // Count = 5 (Assignment and Comparison)
                break;
            }
        }
        for (; i < nums.size() - 1; i++) {
            // Each iteration:
            // - Comparison: Count = 1 (i < arr.size())
            // - Increment: Count = 1 (i++)
            if (inc && nums[i] > nums[i + 1]) {
                // Comparison: Count = 5 (inc && nums[i] > nums[i + 1])
                return false;  // Count = 1 (return)
            }

            if (!inc && nums[i + 1] > nums[i]) {
                // Comparison: Count = 6 (!inc && nums[i + 1] > nums[i])
                return false;  // Count = 1 (return)
            }
        }

        return true;  // Count = 1 (return)
    }
};