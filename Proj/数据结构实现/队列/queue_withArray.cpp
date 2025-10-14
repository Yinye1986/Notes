#include <iostream>

#define __TYPE__ int
#define __SIZE__ 100

class Queue {
public:
  Queue() {}
  void enqueue(__TYPE__);
  void dequeue();
  __TYPE__ getFront();
  __TYPE__ getRear();
  bool isEmpty();
  bool isFull();
  void print(char args);

  int front = -1;
  int rear = -1;
  __TYPE__ arr[__SIZE__];

private:
};

void Queue::enqueue(__TYPE__ arg) {
  if (isFull())
    return;
  if (isEmpty()) {
    arr[++rear] = arg;
    front++;
  } else {
    arr[++rear] = arg;
  }
}

void Queue::dequeue() {
  if (isEmpty())
    return;
  if (front == rear) {
    --front;
    --rear;
  }
  // 删除索引即可无需清除数组数值
  front++;
}

bool Queue::isEmpty() {
  if (front == -1 && rear == -1) {
    return true;
  } else {
    return false;
  }
}

bool Queue::isFull() {
  if (rear == __SIZE__ - 1) {
    return true;
  } else {
    return false;
  }
}

void Queue::print(char args) { // p or n 正反输出
  int temp;
  if (args == 'p') {
    temp = front;
    while (temp <= rear) {
      std::cout << arr[temp++];
    }
  } else {
    temp = rear;
    while (temp >= front) {
      std::cout << arr[temp--];
    }
  }
}

int main() {
  Queue q;
  q.enqueue(1);
  q.enqueue(3);
  q.enqueue(5);
  q.enqueue(6);
  q.enqueue(7);
  q.dequeue();
  q.print('p');
  std::cout << std::endl;
  q.print('n');
}
