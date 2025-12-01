#include <cstring>
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
  // void StackClass::reverseArr(char *arr);
  char *getReversedArr(char *arr);
  // void reverseStr(std::string &str);
  // std::string getReversedStr(std::string &str);

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

// void StackClass::reverseArr(char *arr) {
//   int i = 0;
//   int j = 0;
//   while (arr[i] != '\0') {
//     push(arr[i]);
//     i++;
//   }
//   while (!isEmpty()) {
//     arr[j] = getTop();
//     pop();
//     j++;
//   }
// }

char *StackClass::getReversedArr(char *arr) {
  for (size_t i = 0; arr[i] != '\0'; i++) {
    push(arr[i]);
  }
  size_t length = std::strlen(arr);
  char *a = new char[length + 1];
  for (size_t i = 0; i < length; i++) {
    a[i] = getTop();
    pop();
  }
  return a;
}

// void StackClass::reverseStr(std::string &str) {
//   for (char c : str) {
//     push(c);
//   }
//   for (size_t i = 0; i < str.length(); i++) {
//     str[i] = getTop();
//     pop();
//   }
// }

// std::string StackClass::getReversedStr(std::string &str) {
//   for (char c : str) {
//     push(c);
//   }
//   std::string a(str.length(), '\0');
//   for (size_t i = 0; i < str.length(); i++) {
//     a[i] = getTop();
//     pop();
//   }
//   return a;
// }

int main() {
  StackClass SC;
  // char arr[6] = {'a', 'b', 'c', 'd', 'e', '\0'};
  // SC.reverseArr(arr);
  // for (int i = 0; i < 6; i++) {
  //   std::cout << arr[i];
  // }

  char arr[] = "abcde";
  char *revStr = SC.getReversedArr(arr);
  std::cout << "Reversed Arr:" << revStr;
  delete revStr;

  // std::string strrr = "abcde";
  // SC.reverseStr(strrr);
  // std::cout << strrr;

  // std::string str = "abcde";
  // std::string revStr = SC.getReversedStr(str);
  // std::cout << "revStr:" << revStr << std::endl;
  // std::cout << "getReversedStr(str):" << SC.getReversedStr(str);
  return 0;
}
