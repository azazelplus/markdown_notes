




/**
You are given two non-empty linked lists representing two non-negative integers. The digits are stored in reverse order, and each of their nodes contains a single digit. Add the two numbers and return the sum as a linked list.
给定两个非空链表，表示两个非负整数。数字以逆序存储，并且它们的每个节点都包含一个数字。将两个数字相加，并以链表形式返回和。

You may assume the two numbers do not contain any leading zero, except the number 0 itself.
你可以假设这两个数字不包含任何前导零，除了数字 0 本身。

 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */



#include <vector>
#include <unordered_map>
using namespace std;


class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);  //创建一个虚拟节点头. 在它后面接整个答案链表. 它直接是栈变量, 因为我们不在意它出函数的死活
        ListNode* cur = &dummy;
        int carry = 0;  //进位

        while(l1||l2||carry){
            int x = l1 ? l1->val : 0; //如果l1不为空, x为l1的值, 否则为0
            int y = l2 ? l2->val : 0; //如果l2不为空, y为l2的值, 否则为0
            int sum = x + y + carry; //当前位的和
            carry = sum/10;
            cur->next = new ListNode(sum%10);
            cur = cur->next;
            if (l1) l1 = l1->next; //如果l1不为空, l1指向下一个节点
            if (l2) l2 = l2->next; //如果l2不为空, l2指向下一个节点
        }
        return dummy.next; //返回虚拟节点的下一个节点, 即答案链表的头
    }
};





















