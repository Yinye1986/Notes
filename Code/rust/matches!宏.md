```rust
matches!(expression, pattern)
```
基本上等价于1

```rust
match expression {
    pattern => true,
    _ => false,
}
```
