# 结构体

## TYPE 1

```C
typedef struct {
    int chinese;
    int math;
    int eng;
    void (*printScore)(struct )
} score1;

score1 score_of_hung;
```

解析：
```typedef```在此处**声明并定义**了一个**数据类型**，所以可以直接用```score1```声明一个**结构体变量**（类似实例化）

## TYPE 2

```C
void printScore(struct *) {}

struct score2 {
    int chinese;
    int math;
    int english;          // 这四个都是成员变量
    void (*printScore)(struct)  // 结构体中不允许声明函数，但是可以有函数指针
};

struct score2 t1,t2[],*t3 // 通过此语句声明一个结构体变量
```

解析：
- ```score2```是结构体的标签,可以通过```struct <tag>```去声明一个结构体变量

### TYPE 2.1

```C
struct score1 {
    int chinese;
    int math;
    int english;
    void (*printScore)();
} t1; // 直接在声明结构体的同时声明一个结构体变量
```

不推荐此格式

## ...
