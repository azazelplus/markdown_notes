
/*给你一个字符串数组，请你将 字母异位词 组合在一起。可以按任意顺序(提示! 可以用unordered_map哈希表容器, 它的for迭代器循环是不可预测顺序的)返回结果列表。

 

示例 1:

输入: strs = ["eat", "tea", "tan", "ate", "nat", "bat"]

输出: [["bat"],["nat","tan"],["ate","eat","tea"]]

解释：

在 strs 中没有字符串可以通过重新排列来形成 "bat"。
字符串 "nat" 和 "tan" 是字母异位词，因为它们可以重新排列以形成彼此。
字符串 "ate" ，"eat" 和 "tea" 是字母异位词，因为它们可以重新排列以形成彼此。
示例 2:

输入: strs = [""]

输出: [[""]]

示例 3:

输入: strs = ["a"]

输出: [["a"]]

思路: 异位词排序后会得到相同字符串, 可以把排序后的字符串作为key, 对应的val是一个字符串数组, 遍历输入原字符串并填入哈希表, 最后输出哈希表即可.

 */

#include <vector>
#include <unordered_map>

class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {

        //创建哈希表, key是排序后的字符串, val是一个字符串数组, 存储所有排序后相同的字符串
        unordered_map<string, vector<string>> mp;
        for(string& s : strs){
            string key = s;
            sort(key.begin(), key.end());   //sort(某个容器的迭代器1, 某个容器的迭代器2) 对容器的迭代器范围内的元素进行排序
            mp[key].push_back(s);    //mp[key]返回的是对val的引用, 是一个vector<string>, push_back(s)来追加一个string           
        }

        //从哈希表里取出所有值, 组成result
        vector<vector<string>> res;
        for(auto& bucket: mp){
            res.push_back(bucket.second);
        }
        return res;
    }
};





