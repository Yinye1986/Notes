# 指针

- 指针类型：
  - 普通指针  ：指向**基本数据类型**
  - 指针指针  ：指向**指针**
  - 函数指针  ：指向**函数**
  - 结构体指针：指向**结构体变量**
- 特殊
  - 指针数组：数组元素为指针
  - 指针函数：返回值为一个指针
## 注意事项

- 养成给指针赋初值的好习惯
- ...

## 指针类型

### 普通指针

```C
int a;
int* pointer; // 注意此处的int不是指针变量的类型,而是指针指向的变量的类型

pointer = &a; // 此处 &a 就得到了a的地址,所以此时 pointer 的值等于 a 的地址
```

解析：
```pointer```的值是a的地址
```*pointer```是a的值

### 指针指针

```C
int a;
int *pointer1;
int **pointer2;

pointer1 = &a;
pointer2 = &pointer1;
```

解析：
```pointer1```的值是a的地址
```*pointer1```是a的值
```pointer2```是pointer1的地址
```**pointer2```是pointer1的值（a的地址）

### 函数指针

```C
void returnPrintf(int dat) {
    printf("%d,\n",dat);
}
void (*pointer)(int) = returnPrintf; // 函数指针的返回类型要和原型函数相同，参数类型要和原型函数的参数类型一致
int main() {
  pointer(dat);
}
```

解析：
- 函数指针的返回类型要和**原型函数**一致
- 函数指针的形式参数类型要和原型函数一致
  - 类型一致
  - 同时为变量或指针
- 为指针赋值时不用”取函数地址“，直接赋原型函数名
- 调用时用函数指针名，且要传参

### 结构体指针

```C
typedef struct {
    int chinese;
    int math;
    int engn;
} score;
score class_one = {95,95,95};
score *pointer = &class_one; // 注意此处用定义的结构体类型声明结构体指针，并&<实例名>来取地址
```

解析：
- 结构体指针类型就是结构体变量的类型
- 调用成员变量时要用```<指针变量名>-><成员变量名>```例如```pointer->chinese```

### 指针数组

```C
int a = 1;
int b = 2;
int c = 3,
int *ptr_arr[];
ptr_arr[1] = &a;
```
