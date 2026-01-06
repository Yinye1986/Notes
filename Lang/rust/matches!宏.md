# 应用场景

例如, 有一个动态数组, 里面存有以下枚举:

```rust
enum MyEnum {
    Foo,
    Bar
}
fn main() {
    let v = vec![MyEnum::Foo,MyEnum::Bar,MyEnum::Foo];
}
```

现在如果想对 v 进行过滤, 只保留类型是 MyEnum::Foo 的元素, 可能会这么写:

```rust
v.iter().filter(|x| x == MyEnum::Foo);
```

v.iter().filter(|x| matches!(x, MyEnum::Foo));

```rust
matches!(expression, pattern)
```

基本上等价于

```rust
match expression {
    pattern => true,
    _ => false,
}
```
