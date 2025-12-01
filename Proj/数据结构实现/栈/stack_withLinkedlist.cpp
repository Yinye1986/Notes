#include <iostream>

struct Node {
  int data;
  Node *next;
};

class StackByLinkedlist {
public:
  StackByLinkedlist() : top(nullptr) {} // top是一个指针

  void push(int value) {
    Node *newNode = new Node{value, top}; // newNode指向top所指向的
    top = newNode;                        // top指向newNode
  }

  void pop() {
    Node *temp = top;
    top = top->next;
    delete temp;
  }

  int getTop() { return top->data; }

  bool isEmpty() {
    if (top == nullptr) {
      return 1;
    } else {
      return 0;
    }
  }

  void print() {
    Node *temp = top;
    while (temp) {
      std::cout << temp->data << " ";
      temp = temp->next;
    }
    std::cout << std::endl;
  }

  int countNodes() {
    int count = 0;
    Node *temp = top;
    while (temp) {
      count++;
      temp = temp->next;
    }
    return count;
  }

  ~StackByLinkedlist() {
    Node *current = top;
    while (current != nullptr) {
      Node *nextNode = current->next;
      delete current;
      current = nextNode;
    }
  }

private:
  Node *top;
};
