class Solution {
   public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        vector<int> sortArray;                  // 放結果
        int i = 0;                              // nums1 的 index
        int j = 0;                              // nums2 的 index
        while (i < m || j < n) {                // 如果還沒sort完
            if (j >= n) {                       // 如果nums2都放進結果了
                sortArray.push_back(nums1[i]);  // 放入nums1最上面的值
                i++;                            // nums1 的 index++
            } else if (i >= m) {                // 如果nums1都放進結果了
                sortArray.push_back(nums2[j]);  // 放入nums2最上面的值
                j++;                            // nums2 的 index++
            } else if (nums1[i] <= nums2[j]) {  // 如果nums1[i] <= nums2[j]
                sortArray.push_back(nums1[i]);  // 放入nums1最上面的值
                i++;                            // nums1 的 index++
            } else {                            // 如果nums1[i] > nums2[j]
                sortArray.push_back(nums2[j]);  // 放入nums1最上面的值
                j++;                            // nums2 的 index++
            }
        }
        nums1 = sortArray;  // 將結果放回nums1
    }
};