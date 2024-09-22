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
    head->prev = newNode;
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
      head->prev = newNode;
    } else {
      Node *temp = head;
      for (int i = 0; i < n - 2; i++) {
        temp = temp->next;
      }
      Node *newNode = new Node{val,temp,temp->next};
      temp->next->prev = newNode;
      temp->next = newNode;
    }
  }
}

