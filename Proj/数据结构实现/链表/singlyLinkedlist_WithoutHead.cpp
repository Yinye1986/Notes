#include <iostream>

struct Node {
  int data;
  Node *next;
};

class LinkedList {
public:
  LinkedList() : head(nullptr) {} // head是一个指针

  void addAtHead(int value) {
    Node *newNode = new Node{value, head}; // newNode指向head所指向的
    head = newNode;                        // head指向newNode
  }

  void addAtTail(int value) {
    Node *newNode = new Node{value, nullptr};
    // 1. 如果空,直接head指向newNode
    if (head == nullptr) {
      head = newNode;
      return;
    }
    // 2. 如果不空,则遍历链表到最后一个,在将最后的指向newNode
    Node *temp = head;
    while (temp->next != nullptr) { // 如果temp不等于nullptr(即不是尾)
      temp = temp->next;            // 则迭代遍历下一个
    }
    temp->next = newNode; // 此时temp是尾,让其指向新结点即可
  }

  void delAtHead() {
    if (head == nullptr) {
      return;
    }
    Node *temp = head;
    head = head->next;
    delete temp;
  }

  void delAtTail() {
    if (head == nullptr)
      return; // 空链表直接返回

    // 使用双指针统一处理所有情况
    Node *prev = nullptr;
    Node *curr = head;

    while (curr->next != nullptr) { // 找到最后一个节点
      prev = curr;
      curr = curr->next;
    }

    if (prev == nullptr) { // 单节点情况
      delete head;
      head = nullptr;
    } else { // 多节点情况
      prev->next = nullptr;
      delete curr;
    }
  }

  void insert_atAnywhere(int value, int n) { // 插到n的前面
    Node *newNode = new Node{value, nullptr};
    if (n < 0)
      return;
    if (n == 1) {
      newNode->next = head;
      head = newNode;
      return;
    }
    // 1. 迭代遍历到n
    int a = 1;
    Node *temp = head; // 此时temp初始化为Node1
    while (a < n - 1) {
      temp = temp->next; // 第1次结束,temp指向3,a=1;
      a++;
    }
    // n-1轮,temp指向n
    newNode->next = temp->next;
    temp->next = newNode;
  }

  void delete_atAnywhere(int n) {
    // 空链表或无效位置直接返回
    if (head == nullptr || n < 1)
      return;
    if (n == 1) {
      delAtHead();
      return;
    }
    Node *prev = nullptr;
    Node *curr = head;
    int counter = 1; // 位置从 1 开始计数
    // 遍历直到找到第 n 个节点或链表末尾
    while (curr != nullptr && counter < n) {
      prev = curr;
      curr = curr->next;
      counter++;
    }
    if (curr == nullptr)
      return;

    if (prev) { // 中间节点或尾节点
      prev->next = curr->next;
      if (curr->next == nullptr) {
        // 隐含处理尾节点，但无需特殊操作
      }
      delete curr;
    } else {       // 理论上不会触发此分支
      delAtHead(); // 冗余安全措施
    }
  }

  void reverse_byIterativeMethod() {
    Node *prev, *current, *next;
    prev = nullptr;
    current = head;
    while (current->next != nullptr) {
      next = current->next;
      current->next = prev;
      prev = current;
      current = next;
    }
    current->next = prev;
    head = current;
  }

  void reverse_byRecursiveMethod(Node head) {}

  void print() {
    Node *temp = head;
    while (temp) {
      std::cout << temp->data << " ";
      temp = temp->next;
    }
    std::cout << std::endl;
  }

  int countNodes() {
    int count = 0;
    Node *temp = head;
    while (temp) {
      count++;
      temp = temp->next;
    }
    return count;
  }

  // 析构函数，释放链表内存
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
};

int main() {
  LinkedList list;
  int choice, value, a;

  while (1) {
    std::cout << " 1. 添加节点到链表头部" << std::endl;
    std::cout << " 2. 添加节点到链表尾部" << std::endl;
    std::cout << " 3. 添加节点到指定位置" << std::endl;
    std::cout << " 4. 删除首元节点" << std::endl;
    std::cout << " 5. 删除尾部节点" << std::endl;
    std::cout << " 6. 指定位置删除" << std::endl;
    std::cout << " 7. 反转链表(迭代方法)" << std::endl;
    std::cout << " 8. 反转链表(递归方法)" << std::endl;
    std::cout << " 9. 打印链表" << std::endl;
    std::cout << "10. 打印链表除头结点外节点个数" << std::endl;
    std::cout << "11. 退出程序" << std::endl;
    std::cout << "请输入你的选择:";
    std::cin >> choice;

    switch (choice) {
    case 1:
      std::cout << "请输入要添加到链表头部的值:";
      std::cin >> value;
      list.addAtHead(value);
      break;

    case 2:
      std::cout << "请输入要添加到链表尾部的值:";
      std::cin >> value;
      list.addAtTail(value);
      break;

    case 3:
      std::cout << "请输入值与要插入的指定位置:";
      std::cin >> value >> a;
      list.insert_atAnywhere(value, a);
      break;

    case 4:
      list.delAtHead();
      break;

    case 5:
      list.delAtTail();
      break;

    case 6:
      std::cout << "请输入要删除的值的位置:";
      std::cin >> a;
      list.delete_atAnywhere(a);
      break;

    case 7:
      std::cout << "已反转";
      list.reverse_byIterativeMethod();
      break;

    case 8:
      std::cout << "已反转";
      list.reverse_byIterativeMethod();
      // list.head = list.reverse_byRecursiveMethod(head);
      break;

    case 9:
      std::cout << "链表的内容是: ";
      list.print();
      break;

    case 10:
      std::cout << "请输入要添加到链表尾部的值:";
      std::cin >> value;
      list.addAtTail(value);
      break;

    case 11:
      std::cout << "退出程序..." << std::endl;
      return 0;

    default:
      std::cout << "无效的选择，请重新输入!" << std::endl;
      break;
    }
  }

  return 0;
}
