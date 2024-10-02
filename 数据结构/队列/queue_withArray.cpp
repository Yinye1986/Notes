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

  int front;
  int rear = -1;
  __TYPE__ arr[__SIZE__];

private:
};

void Queue::enqueue(__TYPE__ arg) {
  try:
    if (rear == std::size(arr)) {
      throw std::runtime_error "";
    }
catch:
}
void Queue::dequeue() {}
__TYPE__ Queue::getFront() {}
__TYPE__ Queue::getRear() {}
bool Queue::isEmpty() {
  if (front == -1 && rear == -1) {
    return true;
  } else {
    return false;
  }
}

int main() {}
