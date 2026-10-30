



#include <vector>
#include <unordered_map>
using namespace std;

// unordered_map 是一个哈希表封装. 内部使用链地址法处理碰撞. 提供方法:
// find(key). 不直接返回val, 而是返回一个迭代器(可以看成指向该键值对的指针, 用->second获取val, 用->first获取key). 若没找到, 返回
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> map; // key: 数字, value: 索引
        for (int i = 0; i< nums.size(); ++i){
            int complement = target - nums[i];
            // 查找complement是否在哈希表中
            if (map.find(complement) != map.end()){
                return {map[complement], i}; // 找到, 返回索引和i构成的vector
            } 

            //没找到, 将当前nums[i]和索引i存入哈希表
            map[nums[i]] = i;
        }
        // 如果遍历整个nums都没找到
        return {};
    }
};





















