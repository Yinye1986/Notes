#include <iostream>
#include <stdexcept>

#define __MAX_SIZE__ 101

class StackClass {
public:
  StackClass();
  void push(char val);
  void pop();
  void print();
  bool isEmpty();
  char getTop();
  char rewrite_AsPrifixExpr(std::string &str);

private:
  int top = -1;
  char arr[__MAX_SIZE__];
};

StackClass::StackClass() {}

void StackClass::push(char val) {
  try {
    if (top > __MAX_SIZE__ - 1) {
      throw std::runtime_error("Stack is overflow!");
    } else {
      arr[++top] = val;
    }
  } catch (const std::runtime_error &e) {
    std::cerr << "捕获到异常:" << e.what() << std::endl;
  }
};

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
};

void StackClass::print() {
  for (int i = 0; i <= top; i++) {
    std::cout << arr[i] << "\t";
  }
}
bool StackClass::isEmpty() {
  if (top == -1) {
    return 1;
  } else {
    return 0;
  }
}

char StackClass::getTop() { return arr[top]; }

char StackClass::rewrite_AsPrifixExpr(std::string &str) {
    for (char c : str) {
        if (c == '+' || c=='-' || c=='*' || c == '/')

    }
  int b;
}


int main() {
  StackClass SC;
  int a = 1, b = 2, c = 2;
  std::string expr = "a+b-c*2/2";
  return 0;
}
