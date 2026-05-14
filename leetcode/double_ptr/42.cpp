/*
给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。

 

示例 1：



输入：height = [0,1,0,2,1,0,1,3,2,1,2,1]
输出：6
解释：上面是由数组 [0,1,0,2,1,0,1,3,2,1,2,1] 表示的高度图，在这种情况下，可以接 6 个单位的雨水（蓝色部分表示雨水）。 
示例 2：

输入：height = [4,2,0,3,2,5]
输出：9
 
思路:
对于每个位置，它左边最高的墙和右边最高的墙，决定了这里能存多少水。
取两边较矮的那个墙，减去当前位置的地面高度，就是水的深度。
如果这个值是负数，说明存不了水（水会流走），取0。

A: 可以直接暴力求. 遍历每个位置, 向左右看找最高的柱子. 时间复杂度O(n^2).

B: 动态规划预处理.
*/


/*A*/
// 暴力法. 超出时间限制...
class Solution {
public:
    int trap(vector<int>& height) {
        int res = 0;
        int n = height.size();
        for(int i=1; i < n - 1; i++){
            int left_max=0, right_max=0;
            for(int j = 0; j<i; j++) left_max=max(height[j], left_max);
            for(int k = i+1; k<n; k++) right_max=max(height[k], right_max);
            int water = min(left_max, right_max) - height[i];
            if(water<0) water=0;
            res += water;
        }
        return res;
    }
};

/*
B
DP动态规划: 提前算出每个位置的left_max和right_max
其实就是注意到left_max一定往左走是递增的, 每个位置只需要比较右边位置的left_max和右边位置高度的最大值. 所以可以在一次遍历中O(n)地找到所有位置的left_max.
*/
class Solution {
public:
    int trap(vector<int>& height) {
        int n = height.size();
        if(n==0) return 0;

        //每个位置的left_max和right_max数组
        vector<int> left_max(n), right_max(n);

        //预计算left_max
        left_max[0] = 0;
        for(int i = 1; i<n; i++)
            left_max[i] = max(left_max[i-1], height[i-1]);

        //right_max同理
        right_max[n-1] = 0;
        for(int i = n-2; i>-1; i--)
            right_max[i] = max(right_max[i+1], height[i+1]);
        //计算每个位置的水量
        int res = 0;
        for(int i=1; i<n-1; i++){
            int water = min(left_max[i], right_max[i]) - height[i];
            if(water<0) water=0;
            res += water;
        }
        return res;
    }
};

/*
C
进一步优化: 双指针, 将空间复杂度从O(n)降为O(1).
不用提前计算所有位置的left_max和right_max.
用两个指针 left 和 right 分别指向数组的左右两端, 做一次向中间移动的遍历. (时间O(n))

维护两个变量 left_max 和 right_max，分别表示从 最左边 到 当前left 的最大值、从 最右边 到 当前right 的最大值。
如果 height[left] < height[right]，则 left 位置的雨水量由 left_max 决定（因为右边有更高的墙保证水不会从右边流出），此时计算 left 位置的水量并移动 left++；否则对称处理 right。
*/
class Solution {
public:
    int trap(vector<int>& height) {
        int res = 0;
        int n = height.size();
        int left = 0, right = n-1;
        int left_max = 0, right_max = 0;

        while(left < right){
            if (height[left] < height[right]){
                // 左边矮，左边决定水位
                left_max = max(left_max, height[left]); //更新left_max
                int water = left_max - height[left];
                if(water<0) water=0;
                res += water;
                left++;
            }else{
                // 右边矮，右边决定水位
                right_max = max(right_max, height[right]); //更新right_max
                int water = right_max - height[right];
                if(water<0) water=0;
                res += water;
                right--;
            }
        }
        return res;
    }
};




