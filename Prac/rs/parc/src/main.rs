fn main() {
    let mut s: String = "Hello".to_string();
    let ptr = &mut s;
    s.push_str("World");
    println!("{}\n", s);
    ptr.push('!');
    println!("{}", s);
}
