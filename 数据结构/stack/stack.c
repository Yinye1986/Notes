#include <stdio.h>

#define bool char // 定义bool类型（C中没有）
#define true 1
#define false 0

#define MAX_SIZE 50
#define INITIAL_VALUE -1

typedef struct {
  int data[MAX_SIZE]; // 用数组存放数据，MAX_SIZE作为栈满条件
  int top;            // 栈顶指针
} stack;

void initStack(stack *arg) {
  arg->top = -1; // 初始化栈顶指针
}

bool isEmpty(stack arg) { // 栈的判空
  if (arg.top == -1) {
    return true;
  } else {
    return false;
  }
}

bool pushVal(stack *arg0, int targetValue) { // 压栈
  if (arg0->top == MAX_SIZE - 1) {
    return false;
  } else {
    arg0->data[++arg0->top] = targetValue;
    return true;
  }
}

bool pullVal(stack *arg0) {
  if (isEmpty(*arg0)) {
    return false;
  } else {
    arg0->top--;
    return true;
  }
}

bool getTopValue(stack targetStack, int *targetValue) {
  if (isEmpty(targetStack)) {
    return false;
  } else {
    *targetValue = targetStack.data[targetStack.top];
    return true;
  }
}

int main() {
  stack example_stack;
  initStack(&example_stack);
  for (int i = 1; i < 9; i++) {
    pushVal(&example_stack, i); // 将1～8压栈
  }
  int topVal;
  int *arg = &topVal;
  for (int i = 1; i < 9; i++) {
    getTopValue(example_stack, arg); // getTopValue
    pullVal(&example_stack); // 先进后出原则，将8～1按序出栈
    printf("%d\t", i); // 为了便于检验，不从0而从1开始编号输出循环轮数
    printf("%d\n", *arg); // 为了便于检验，从栈顶输出栈中元素
  }
}
