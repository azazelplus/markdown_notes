
/*
给定一个数组 nums，编写一个函数将所有 0 移动到数组的末尾，同时保持非零元素的相对顺序。

请注意 ，必须在不复制数组的情况下原地对数组进行操作。

示例 1:

输入: nums = [0,1,0,3,12]
输出: [1,3,12,0,0]
示例 2:

输入: nums = [0]
输出: [0]
 

提示:

1 <= nums.length <= 104
-231 <= nums[i] <= 231 - 1
 

思路: 维护i和lastNonZeroFoundAt两个整数索引(作为指针), 直接在引用传递的原数组上修改.
i遍历数组, lastNonZeroFoundAt记录当前非零元素应该放置的位置. 
当i遇到非零元素时, 将其放置在lastNonZeroFoundAt位置, 并将lastNonZeroFoundAt向前移动一位. 最后, lastNonZeroFoundAt之前的元素都是非零元素, 之后的元素都是0.
i遍历数组的for循环过程:
 i=0
 ↓
[0,1,0,3,12]
 ↑
 L=0

   i=1
   ↓
[0,1,0,3,12]
 ↑
 L=0

     i=2
     ↓
[1,1,0,3,12]
   ↑
   L=1

       i=3
       ↓
[1,1,0,3,12]
   ↑
   L=1

         i=4
         ↓
[1,3,0,3,12]
     ↑
     L=2

            i=5
            ↓
[1,3,12,3,12]
       ↑
       L=3

然后L后面全部替换为0
[1,3,12,0,0]
完成.

*/




class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int lastNonZeroFoundAt = 0;
        for(int i = 0; i < nums.size(); i++){
            if(nums[i] != 0)
                nums[lastNonZeroFoundAt++] = nums[i];
        }
        // 将lastNonZeroFoundAt之后的元素设置为0
        for(int i = lastNonZeroFoundAt; i < nums.size(); i++){
            nums[i] = 0;
        }
    }
};



