/*
Given an array of integers nums and an integer target, return indices of the two numbers such that they add up to target.
给定一个整数数组 nums 和一个整数 target ，返回和为 target 的两个数的索引。

You may assume that each input would have exactly one solution, and you may not use the same element twice.
你可以假设每个输入只有一个解，并且不能使用同一个元素两次。

You can return the answer in any order.
你可以以任何顺序返回答案。

Example 1:  示例 1：

Input: nums = [2,7,11,15], target = 9
Output: [0,1]
Explanation: Because nums[0] + nums[1] == 9, we return [0, 1].


哈希表, 及防止碰撞方法: 链地址法
*/

#include <stdio.h>
#include <stdlib.h>

// hash table node
typedef struct Node{
    int key;
    int val;
    struct Node *next;
} Node;

// hash table
typedef struct {
    Node **buckets;
    int size;   // 桶的数量.
}HashMap;

// hash func. expect key as non-negative integer
int hash(int key, int size){
    return ((key % size) + size) % size;  //最简单的hash函数, 直接%哈希表长度
}


// 构造函数和析构函数
HashMap* createHashMap(int size){
    HashMap *map = (HashMap*)malloc(sizeof(HashMap));
    map->size = size;
    map->buckets = (Node**)calloc(size, sizeof(Node*));
    return map; 
}

void freeHashMap(HashMap *map){
    for(int i = 0; i<map->size; i++){
        Node *cur = map->buckets[i]; 
        while(cur){
            Node *temp = cur;
            cur = cur->next;
            free(temp);
        }
    }
    free(map->buckets);
    free(map);
}

// 向哈希表中插入一个键值对. 头插法。
int put(HashMap *map, int key, int val){
    int hash_index = hash(key, map->size);
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->val = val;

    // 头插法
    newNode->next = map->buckets[hash_index];   
    map->buckets[hash_index] = newNode;

    return 0;
} 

// 通过遍历key对应的哈希桶, 从哈希表中查找key对应的val. 没有则返回-1
int get(HashMap *map, int key){
    int hash_index = hash(key, map->size);

    //遍历哈希桶(遍历链表)
    Node *cur = map->buckets[hash_index];
    while(cur){
        if(cur->key == key) 
            return cur->val;
        cur = cur->next;
    }
    return -1;  // 没有找到
}

// 解题函数, 参数nums, numsSize, target, returnSize已经给出可以直接用
int *twoSum(int* nums, int numsSize, int target, int* returnSize){

    int bucketSize = numsSize * 2+1; //哈希桶数量取组数大小*2+1, 减少碰撞
    HashMap *map = createHashMap(bucketSize);
    for(int i = 0; i<numsSize; i++){
        int complement = target - nums[i];
        //哈希表里有没有元素等于complement?
        int res = get(map, complement);
        //没找到: 将自己加入哈希表.
        if(res == -1){
            put(map, nums[i], i);
        }
        //找到了 
        else{
            freeHashMap(map);
            int *ans = (int*)malloc(2*sizeof(int));
            ans[0] = res; ans[1] = i;

            //返回
            *returnSize = 2;
            return ans;
        }
    }
    freeHashMap(map);
    return NULL;
}

// 测试函数. 不过leetcode自动调用twoSum不需要main
int main(){
    res = twoSum(nums, numsSize, target, returnSize);
    printf("res: [%d, %d]\n", res[0], res[1]);
    return 0;
}















