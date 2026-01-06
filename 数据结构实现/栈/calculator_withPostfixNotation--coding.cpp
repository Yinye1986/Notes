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
  int count();
  void reverseStr(std::string &str);

  int top = -1;
  char arr[__MAX_SIZE__];

private:
};

StackClass::StackClass() {}

void StackClass::push(char val) {
  try {
    if (top >= __MAX_SIZE__ - 1) {
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

int StackClass::count() { return top + 1; }

char StackClass::getTop() { return arr[top]; }

void StackClass::reverseStr(std::string &str) {
  for (char c : str) {
    push(c);
  }
  for (size_t i = 0; i < str.length(); i++) {
    str[i] = getTop();
    pop();
  }
}

std::string getPostfixExpr(std::string expr) {
  class operat : public StackClass {
  public:
    void push(char val) {
      try {
        if (top >= __MAX_SIZE__ - 1) {
          throw std::runtime_error("Stack is overflow!");
        } else {
          arr[++top] = val;
        }
      } catch (const std::runtime_error &e) {
        std::cerr << "operator类捕获到异常:" << e.what() << std::endl;
      }
    }
    void pop() {
      try {
        if (top == -1) {
          throw std::runtime_error("stack is empty");
        } else {
          top--;
        }
      } catch (const std::runtime_error &e) {
        std::cerr << "operator类捕获到异常:" << e.what() << std::endl;
      }
    };
  };
  class postfixExpr : public StackClass {
  public:
    void push(char val) {
      try {
        if (top >= __MAX_SIZE__ - 1) {
          throw std::runtime_error("Stack is overflow!");
        } else {
          arr[++top] = val;
        }
      } catch (const std::runtime_error &e) {
        std::cerr << "postfixExpr类捕获到异常:" << e.what() << std::endl;
      }
    }
    void pop() {
      try {
        if (top == -1) {
          throw std::runtime_error("stack is empty");
        } else {
          top--;
        }
      } catch (const std::runtime_error &e) {
        std::cerr << "postfixExpr类捕获到异常:" << e.what() << std::endl;
      }
    };
  };

  operat operato;
  postfixExpr postfixExpr;
  StackClass rev;
  for (char c : expr) {
    if (c != '*' && c != '/' && c != '-' && c != '+' && c != '(' && c != ')') {
      postfixExpr.push(c);
    } else {
      if (operato.isEmpty()) {
        operato.push(c);
      } else if (c == '(') {
        operato.push(c);
      } else if (c == ')') {
        while (operato.getTop() != '(') {
          postfixExpr.push(operato.getTop());
          operato.pop();
        }
        operato.pop();
      } else if ((c == '*' || c == '/') &&
                 (operato.getTop() == '+' || operato.getTop() == '-')) {
        operato.push(c);
      } else if ((c == '*' || c == '/') &&
                 (operato.getTop() == '*' || operato.getTop() == '/')) {
        postfixExpr.push(operato.getTop());
        operato.pop();
        operato.push(c);
      } else if (c == '+' || c == '-') {
        while ((operato.getTop() != '(') && (operato.isEmpty() == false)) {
          postfixExpr.push(operato.getTop());
          operato.pop();
        }
        operato.push(c);
      }
    }
  }
  std::string str(postfixExpr.count(), '\0');
  for (size_t i = postfixExpr.count() - 1; i >= 0; i--) {
    str[i] = postfixExpr.getTop();
    postfixExpr.pop();
  }
  rev.reverseStr(str);
  return str;
}

int main() {
  std::string expr = "a+b-c*2/2";
  std::cout << "postfix expr:" << getPostfixExpr(expr);
}
