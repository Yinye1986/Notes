#include <stdio.h>
#include <stdlib.h>

#define bool char
#define true 1
#define false 0

typedef struct Lnode Lnode;

struct Lnode {
  int data;
  Lnode *next_ptr;
};

// 第一种初始化方式：先创建,在使用初始化函数为数据域和指针域赋值；另有一种直接返回一个初始化过的头结点，各有各的好处
void initLinkedlist(Lnode *head_node) { // 单链表的初始化其实就是头结点的初始化
  head_node->data = 0;
  head_node->next_ptr = NULL;
}

// 头插法
Lnode insertLinkedlist(Lnode *head_node, int new_data) {
  Lnode next_node;
  next_node.next_ptr = head_node->next_ptr;
  // 将新结点指向原首元结点（头结点原指向就是原首元结点）
  head_node->next_ptr = &next_node;
  // 将头结点指向新首元结点（即新插入的结点）
  next_node.data = new_data;
  return next_node;
}

int main() {
  Lnode *head_node = (Lnode *)malloc(sizeof(Lnode));
  initLinkedlist(head_node);
  // 至此我们得到了头结点且数据域所表示的链长为0，指针为空
  Lnode *next_node1;
}
