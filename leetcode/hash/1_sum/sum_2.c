/*
哈希表, 及防止碰撞方法: 开放地址OA法(最简单的线性探测法)
*/

#include <stdio.h>
#include <stdlib.h>

// hash table
typedef struct {
    
    int * keys, *vals;  //键值对
    bool *occupied;  // 标记这个桶是否被占用
    int size;   // 桶的数量.
}HashMapOA;

// hash func. expect key as non-negative integer
int hash(int key, int size){
    return ((key % size) + size) % size;  //最简单的hash函数, 直接%哈希表长度
}


// 构造函数和析构函数
HashMapOA* createHashMapOA(int size){
    HashMapOA *map = (HashMapOA*)malloc(sizeof(HashMapOA));
    map->size = size;
    map->keys = (int*)calloc(size, sizeof(int));
    map->vals = (int*)calloc(size, sizeof(int));
    map->occupied = (bool*)calloc(size, sizeof(bool));
    return map; 
}

void freeHashMapOA(HashMapOA *map){
    free(map->keys);
    free(map->vals);
    free(map->occupied);
    free(map);
}


// 向哈希表中插入一个键值对. 
int put(HashMapOA *map, int key, int val){
    int hash_index = hash(key, map->size);
    // 检查该位置是否已被占用
    if(map->occupied[hash_index]){
        // 线性探测
        while(map->occupied[hash_index] && map->keys[hash_index] != key){
            hash_index = (hash_index + 1) % map->size;
        }
        if(map->keys[hash_index] == key){
            // 键已存在，更新值
            map->vals[hash_index] = val;
            return 0;
        }
    }
    // 插入新键值对
    map->keys[hash_index] = key;
    map->vals[hash_index] = val;
    map->occupied[hash_index] = true;

    return 0;
} 

// 查找 key，返回 value，不存在返回 -1
int getOA(HashMapOA *map, int key) {
    int h = hashOA(key, map->size);
    int i = 0;
    int index = h;
    while (map->occupied[index]) {
        if (map->keys[index] == key) {
            return map->values[index];
        }
        i++;
        index = (h + i) % map->size;
        // 如果探测完一圈还没找到，说明不存在（实际上不会，因为有空位）
    }
    return -1;
}

// 解题函数, 参数nums, numsSize, target, returnSize已经给出可以直接用
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    
    int bucketSize = numsSize * 2 + 1;  // 表大小取数组长度的 2 倍+1，保证不会满（开放地址法必须有多余空位）
    HashMapOA *map = createHashMapOA(bucketSize);

    for (int i = 0; i < numsSize; i++) {
        int complement = target - nums[i];
        int j = getOA(map, complement);
        if (j != -1) {
            int *ans = (int*)malloc(2 * sizeof(int));
            ans[0] = j;
            ans[1] = i;
            *returnSize = 2;
            freeHashMapOA(map);
            return ans;
        }
        putOA(map, nums[i], i);
    }

    freeHashMapOA(map);
    *returnSize = 0;
    return NULL;
}

// 测试函数. 不过leetcode自动调用twoSum不需要main
int main(){
    res = twoSum(nums, numsSize, target, returnSize);
    printf("res: [%d, %d]\n", res[0], res[1]);
    return 0;
}















