# 基本資料

姓名：陳乙銜
學號：113310205

---

# Lab01-Ex1.

## Briefly describe the problem. (題目簡述)

eg. Count operations in Find Maximum code as comments.

## code

```c++
int findMax(const vector<int>& arr) {
    int max = arr[0];  // Count = 2 (Assignment and Array access)
    for (int i = 1; i < arr.size(); i++) {
        // Loop initialization: Count = 1 (Assignment i = 0)
        // Each iteration:
        // - Comparison: Count = 1 (i < arr.size())
        // - Increment: Count = 1 (i++)

        if (arr[i] > max) {
            // Comparison: Count = 2 (arr[i] > max)

            max = arr[i];  // Count = 2 (Assignment and Array access)
        }

        // Loop operations = 2 + 2 + 2 = 6
    }

    return max;  // Count = 1 (return)
}

// Total operations:
// 3 (initial) + n * 6 (loop operations) + 1 (return)
// = 4 + 6n operations
// Therefore, O(n) complexity
```

---

# Lab01-Ex2.

## Briefly describe the problem. (題目簡述)

eg. Count operations in Find Maximum code as comments.

## code

```c++
void printPairs(const vector<int>& arr) {
    int count = 0;
    // Loop initialization
    count++;  // i = 0
    for (int i = 0; i < arr.size(); i++) {
        // Loop comparison
        count++;  // i < arr.size()

        // Loop initialization
        count++;  // i + 1
        count++;  // j = i + 1

        for (int j = i + 1; j < arr.size(); j++) {
            // Loop comparison
            count++;  // j < arr.size()

            // printing operation
            count++;  // arr[i]
            count++;  // arr[j]

            cout << arr[i] << "," << arr[j] << endl;

            count++;  // j++
        }
        count++;  // (i++)
    }

    cout << "Total operations: " << count << endl;
}
```

---

# Discussion Section

1. Why don't we count operations like arr.size()?
   A: arr.size() 是 O(1)，即使執行了 n 次，總共也是 O(1) \* n = O(n)，但我們只關注影響主要運算的部分，如果你把 arr.size()放在迴圈裡他也要 count operations.
2. Why do we drop constants in Big-O notation?

- If an algorithm takes 3n+2 operations, why do we say O(n)?
  A: 在 n 足夠大時 2 這格常數可以忽略不計。
- What's the practical importance of this?
  A: Big O 可以讓我們知道演算法的執行次數的成長速度。

3. Looking at the examples above:

- Which has more actual operations: findNumber or sumArray?
  A: findNumber
- Does this change their Big O classification? Why or why not?
  A: 不會，兩個都是 O(n)，因為 Big O 只是在描述執行次數的成長速度。

---

# Lab01-Q1.

## code

```c++
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

```

## Discussion

1. count step:
   if n == 1 => 2,
   if nums all element are same => 3 + 2n + 1 = 4 + 2n
   else => 3 + 10 + 2n + (2 + 5 + 6) \* n + 1 = 13 + 15n

2. Big O:
   這個只要考慮 n 足夠大的時候，且是在描述成長速度所以 4 + 2n = 13 + 15n => O(n)
