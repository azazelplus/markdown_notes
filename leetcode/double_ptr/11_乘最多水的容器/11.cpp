

/*
给定一个长度为 n 的整数数组 height 。有 n 条垂线，第 i 条线的两个端点是 (i, 0) 和 (i, height[i]) 。

找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。

返回容器可以储存的最大水量。

说明：你不能倾斜容器.

示例 1：



输入：[1,8,6,2,5,4,8,3,7]
输出：49 
解释：图中垂直线代表输入数组 [1,8,6,2,5,4,8,3,7]。在此情况下，容器能够容纳水（表示为蓝色部分）的最大值为 49。
示例 2：

输入：height = [1,1]
输出：1

思路: 双指针贪心移动...
S = min(height[i], height[j]) * (j - i)
我们只要能正确遍历S即可. 这需要两个指针i和j.
先让i和j指向两端.
我们每次只向中间移动数较小的那个指针, 因为可以断言: 如果我们向中间移动任意步数较大的那个指针, 那么S一定不会变大(因为S的值由较小的那个数决定了).
即:
    假设当前 i 和 j，且 height[i] < height[j]。
    立即有断言：对于所有以 i 为左边界，且右边界 k 满足 i < k <= j 的容器 (i, k)，其面积都不会超过当前面积 S(i, j)。
    以 i 为左边界的所有其他容器, 都不可能比 S(i, j) 更大了. 所以我们可以让 i 向中间移动一步.

所以这是一个贪心算法: 在当前状态下，可以选择一个方向移动，并证明被跳过的所有状态都不可能是最优解.


假设存在最大的S(i,j), 我们可以保证i左边和j右边的数都小于i和j处的数(不然S就更大了). 
*/







class Solution {
public:
    int maxArea(vector<int>& height) {
        int left = 0, right = height.size()-1;
        int maxwater = 0;
        while(left < right){
            int water = min(height[left], height[right]) * (right-left);
            maxwater = max(maxwater, water);
            if(height[left] < height[right])
                left++;
            else
                right--;    
        }  
        return maxwater;    
    }  
};

















