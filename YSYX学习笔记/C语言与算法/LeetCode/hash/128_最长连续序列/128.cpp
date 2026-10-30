/*
给定一个未排序的整数数组 nums ，找出数字连续的最长序列（不要求序列元素在原数组中连续）的长度。

请你设计并实现时间复杂度为 O(n) 的算法解决此问题。

 

示例 1：

输入：nums = [100,4,200,1,3,2]
输出：4
解释：最长数字连续序列是 [1, 2, 3, 4]。它的长度为 4。
示例 2：

输入：nums = [0,3,7,2,5,8,4,6,0,1]
输出：9
示例 3：

输入：nums = [1,0,1,2]
输出：3
 

提示：

0 <= nums.length <= 105
-109 <= nums[i] <= 109

思路: 用哈希集合快速判断某个元素是否在集合中.
unordered_set：只存储 键 (key)，没有 value。相当于一个只有 key 的 unordered_map。它的底层实现也是哈希表，提供了平均 O(1) 的插入、删除和查找操作。
用来快速判断某个元素是否存在.
.find(key) 返回一个迭代器, 指向找到的元素, 不存在则返回end()
.count(key) 返回0/1表示在容器中是否存在.(因为set元素唯一)
*/

#include <vector>
#include <unordered_set>

class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> set(nums.begin(), nums.end()); //C++很多容器提供 `迭代器范围`构造函数. 从另一个容器的一段元素范围初始化集合. 同时这一步自动完成了去重.
        int longest = 0;
        for(int n : set) //int基础数据类型没必要&n按引用传递避免拷贝, 直接全部拷贝.
        {
            int current_count = 1;  //当前以n为开头的连续序列长度.一开始为1
            if(!set.count(n-1))//如果n-1不在集合中, 说明n是一个连续序列的起点.
            {
                while(set.count(n + current_count)){
                    current_count++;
                }
            }
            longest = (longest>current_count) ? longest : current_count;
        }
        return longest;
    }
};



