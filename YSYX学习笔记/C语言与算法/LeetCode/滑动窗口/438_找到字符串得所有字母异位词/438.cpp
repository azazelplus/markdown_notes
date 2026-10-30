/*
给定两个字符串 s 和 p，找到 s 中所有 p 的 异位词 的子串，返回这些子串的起始索引。不考虑答案输出的顺序。

 

示例 1:

输入: s = "cbaebabacd", p = "abc"
输出: [0,6]
解释:
起始索引等于 0 的子串是 "cba", 它是 "abc" 的异位词。
起始索引等于 6 的子串是 "bac", 它是 "abc" 的异位词。
 示例 2:

输入: s = "abab", p = "ab"
输出: [0,1,2]
解释:
起始索引等于 0 的子串是 "ab", 它是 "ab" 的异位词。
起始索引等于 1 的子串是 "ba", 它是 "ab" 的异位词。
起始索引等于 2 的子串是 "ab", 它是 "ab" 的异位词。

思路: 也可以用滑动窗口解决. 窗口大小固定为p.size(), 维护一个表记录窗口内字符的频次, 右边界右移, 包含新字符, 更新频次表. 如果窗口内的字符频次和p的频次表相同了, 就记录当前左边界的位置.
维护一个字符频次表（长度为26的数组），记录当前窗口中每个字母出现了多少次。每次窗口向右滑动一步：
去掉左边离开窗口的字符
加入右边进入窗口的字符
比较当前窗口的频次表和 p 的频次表是否相等
窗口大小固定为 p.length()，所以不需要像“无重复字符的最长子串”那样动态调整大小。
*/








class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int> res; // 答案. 
        int ns = s.size(), np = p.size();
        if (ns < np) return res; // 如果s的长度小于p, 那么不可能有异位词, 直接返回空结果.   
        
        
        vector<int> pCount(26, 0);  // p的26个字母频次表. 窗口要不断和它对比
        vector<int> windowCount(26, 0); // 当前窗口的频次表. 每次右移都要更新它.

        // 先得到p的频次表
        for(char c: p) pCount[c-'a']++;

        // 初始化第一个窗口并检查是否匹配
        for(int i = 0; i<np; i++){
            windowCount[s[i]-'a']++;
        }
        if(windowCount == pCount) res.push_back(0); // 如果第一个窗口就匹配了, 记录索引0

        // 滑动窗口
        for(int right = np; right < ns; right++){
            int left = right - np; // 窗口的左边界位置

            // 右边界右移, 包含新字符
            windowCount[s[right]-'a']++;
            // 左边界右移, 去掉离开窗口的字符
            windowCount[s[left]-'a']--;

            // 检查当前窗口是否匹配p的频次表
            if(windowCount == pCount){
                res.push_back(right - np + 1); // 记录当前左边界的位置
            }
        }
        return res; // 返回所有匹配的起始索引
    }
};







