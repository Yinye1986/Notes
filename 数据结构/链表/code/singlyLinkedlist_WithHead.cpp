#include <iostream>

struct Node {
    int data;
    Node* next;
};

class LinkedList {
public:
    LinkedList() : head(new Node{1,nullptr}) {}

    void addAtHead(int value) {
        Node* newNode = new Node{value, head->next}; // newNode指向原首元结点        
        head->next = newNode;                        // head指向newNode
        head->data++;
    }

    void delAtHead() {}

    void addAtTail(int value) {
        Node* newNode = new Node{value, nullptr};
        // 1. 如果空,直接head指向newNode
        if(head == nullptr) {
            head->next = newNode;
            head->data++;
            return;
        }
        // 2. 如果不空,则遍历链表到最后一个,在将最后的指向newNode
        Node* temp = head;
        while(temp->next != nullptr) { // 如果temp不等于nullptr(即不是尾)
            temp = temp->next;         // 则迭代遍历下一个
        }
        temp->next = newNode;
        head->data++;
    }

    void insert_atAnywhere(int value, int a) {
        Node* newNode = new Node{value, nullptr};
        if(a<0) return;
        if(a==1) {
            newNode->next = head->next;
            head->next = newNode;
            head->data++;
        }
        // 1. 迭代遍历到n
        int n = 1;
        Node* temp = head;
        
        while (n < a) {
            temp = temp->next;
            n++;
        }
        temp->next = newNode;
        head->data++;
    }

    void print() {
        Node* temp = head;
        while (temp) {
            std::cout << temp->data << " ";
            temp = temp->next;
        }
        std::cout << std::endl;
    }

    int countNodes() {
        int count = 0;
        Node* temp = head;
        while (temp) {
            count++;
            temp = temp->next;
        }
        return count;
    }

    // 析构函数，释放链表内存
    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

private:
    Node* head;
};

int main() {
    LinkedList list;
    int choice, value, a;

    while (1) {
        std::cout << " 1. 添加节点到链表头部" << std::endl;
        std::cout << " 2. 头删" << std::endl;
        std::cout << " 3. 添加节点到链表尾部" << std::endl;
        std::cout << " 4. 尾删" << std::endl;
        std::cout << " 5. 添加节点到指定位置" << std::endl;
        std::cout << " 6. 指定删除" << std::endl;
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
                std::cout << "请输入要添加到链表尾部的值:";
                std::cin >> value;
                list.addAtTail(value);
                break;

            case 4:
                std::cout << "请输入要添加到链表尾部的值:";
                std::cin >> value;
                list.addAtTail(value);
                break;

            case 5:
                std::cout << "请输入值与要插入的制定位置:";
                std::cin >> value >> a;
                list.insert_atAnywhere(value, a);
                break;

            case 6:
                std::cout << "请输入要添加到链表尾部的值:";
                std::cin >> value;
                list.addAtTail(value);
                break;

            case 7:
                std::cout << "链表中的节点个数是: " << list.countNodes() << std::endl;
                break;

            case 8:
                std::cout << "请输入要添加到链表尾部的值:";
                std::cin >> value;
                list.addAtTail(value);
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
