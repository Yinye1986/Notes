#include <iostream>

struct Node {
  int data;
  Node *prev;
  Node *next;
};

class LinkedList {
public:
  LinkedList() : head(nullptr), prev(nullptr) {}

  void addAtHead(int val);
  void addAtTail(int val);
  void insert_atAnywhere(int n, int val);
  void print();

  ~LinkedList() {
    Node *current = head;
    while (current != nullptr) {
      Node *nextNode = current->next;
      delete current;
      current = nextNode;
    }
  }

private:
  Node *head;
  Node *prev;
};

void LinkedList::addAtHead(int val) {
  if (head == nullptr) {
    Node *newNode = new Node{val, nullptr, nullptr};
    head = newNode;
  } else {
    Node *newNode = new Node{val, nullptr, head};
    head = newNode;
  }
};

void LinkedList::addAtTail(int val) {
  if (head == nullptr) {
    Node *newNode = new Node{val, nullptr, nullptr};
    head = newNode;
  } else {
    Node *temp = head;
    while (temp->next != nullptr) {
      temp = temp->next;
    }
    Node *newNode = new Node{val, temp, nullptr};
    temp->next = newNode;
  }
};

void LinkedList::insert_atAnywhere(int n, int val) {
  if (head == nullptr) {
    return;
  } else {
    if (n < 0) {
      return;
    } else if (n == 1) {
      Node *newNode = new Node{val, nullptr, head};
      head = newNode;
    } else {
      Node *temp = head;
      for (int i = 0; i < n - 2; i++) {
        temp = temp->next;
      }
      Node *newNode = new Node{val, temp, temp->next};
      temp->next->prev = newNode;
      temp->next = newNode;
    }
  }
}
// 好像没写好,但是会了

void LinkedList::print() {
  Node *temp = head;
  while (temp) {
    std::cout << temp->data << " ";
    temp = temp->next;
  }
  std::cout << std::endl;
}

int main() {
  LinkedList list;
  int choice, value;

  while (1) {
    std::cout << " 1. 添加节点到链表头部" << std::endl;
    std::cout << " 2. 头删" << std::endl;
    std::cout << " 3. 添加节点到链表尾部" << std::endl;
    std::cout << " 4. 尾删" << std::endl;
    std::cout << " 5. 添加节点到指定位置" << std::endl;
    std::cout << " 6. 指定删除" << std::endl;
    std::cout << " 7. 反转链表(迭代方法)" << std::endl;
    std::cout << " 8. 反转链表(递归方法)" << std::endl;
    std::cout << " 9. 打印链表" << std::endl;
    std::cout << "10. 打印链表除头结点外节点个数" << std::endl;
    std::cout << "11. 退出程序" << std::endl;
    std::cout << "请输入你的选择:";
    std::cin >> choice;
    switch (choice) {
    case 1:
      std::cout << "请输入要头插的值";
      std::cin >> value;
      list.addAtHead(value);
      break;
    case 2:
      break;
    case 3:
      std::cout << "请输入要尾插的值";
      std::cin >> value;
      list.addAtTail(value);
      break;
    case 9:
      list.print();
      break;
    default:
      std::cout << "无效的选择，请重新输入!" << std::endl;
      break;
    }
  }
}
