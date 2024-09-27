#include <iostream>
#include <stdexcept>

#define __MAX_SIZE__ 101

class StackClass {
public:
  StackClass();
  void push(int val);
  void pop();
  void print();
  bool isEmpty();
  int getTop();
  ~StackClass();

private:
  int top;
  int arr[__MAX_SIZE__];
};

StackClass::StackClass() : top(-1) {}
StackClass::~StackClass() {}

void StackClass::push(int val) {
  try {
    if (top > __MAX_SIZE__ - 1) {
      throw std::runtime_error("Stack is overflow!");
    } else {
      arr[++top] = val;
    }
  } catch (const std::runtime_error &e) {
    std::cerr << "捕获到异常:" << e.what() << std::endl;
  }
}

void StackClass::pop() {
  try {
    if (top == -1) {
      throw std::runtime_error("stack is empty");
    } else {
      top--;
    }
  } catch (const std::runtime_error &e) {
    std::cerr << "捕获到异常:" << e.what() << std::endl;
  }
}

void StackClass::print() {
  for (int i = 0; i <= top; i++) {
    std::cout << arr[i] << "\t";
  }
}

int StackClass::getTop() { return arr[top]; }

int main() {
  StackClass SC;
  int choice, value;
}
