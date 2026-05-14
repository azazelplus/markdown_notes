/*
给你一个整数数组 nums ，判断是否存在三元组 [nums[i], nums[j], nums[k]] 满足 i != j、i != k 且 j != k ，同时还满足 nums[i] + nums[j] + nums[k] == 0 。请你返回所有和为 0 且不重复的三元组。

注意：答案中不可以包含重复的三元组。

 

 

示例 1：

输入：nums = [-1,0,1,2,-1,-4]
输出：[[-1,-1,2],[-1,0,1]]
解释：
nums[0] + nums[1] + nums[2] = (-1) + 0 + 1 = 0 。
nums[1] + nums[2] + nums[4] = 0 + 1 + (-1) = 0 。
nums[0] + nums[3] + nums[4] = (-1) + 2 + (-1) = 0 。
不同的三元组是 [-1,0,1] 和 [-1,-1,2] 。
注意，输出的顺序和三元组的顺序并不重要。
示例 2：

输入：nums = [0,1,1]
输出：[]
解释：唯一可能的三元组和不为 0 。
示例 3：

输入：nums = [0,0,0]
输出：[[0,0,0]]
解释：唯一可能的三元组和为 0 。

思路:
A: 联想两数之和查哈希表. 可以直接两层遍历（固定前两个数），然后第三个数查哈希表。 因为N数之和要固定前N-1个数，所以时间复杂度是O(n^(N-1)).

B: 排序(nlogn)+双指针(n^2): 
不同于1_两数之和返回下标, 本题不要求返回下标, 所以允许我们对原数组排序(简单的升序). 而对于排序好的数组, `两数之和`可以用双指针在O(n)时间内完成, 比哈希表方法更简洁!
*/



class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> res;

        sort(nums.begin(), nums.end());//对数组排序
        int len = nums.size();

        //遍历数组, 固定第一个数字
        for(int i=0; i<len-2; i++){

            //利用排好序的数组可以去重: 固定第一个数字的时候, 只选择多次出现的数字的最小下标那个. 后续两数之和问题, 从它右边所有数字里选择.
            if(i>0 && (nums[i-1]==nums[i])) continue;
            //双指针求两数之和问题.
            int left = i+1, right = len-1, target = 0 - nums[i];
            while(left<right){
                int sum = nums[left] + nums[right];
                if(sum<target){
                    left++;
                } else if(sum>target){
                    right--;
                } else {
                    //找到了一个两数之和
                    res.push_back({nums[i], nums[left], nums[right]});
                    //继续找, while跳过重复的数字
                    while(left<right && nums[left] == nums[left+1])     left++;                       
                    while(left<right && nums[right] == nums[right-1])   right--;   
                    left++;
                    right--;                   
                }
            }
        }
        return res;
    }
};
















