use std::env;
use std::fs;

fn main() {
    let args: Vec<String> = env::args().collect(); // 读取终端输入, 只支持Unicode
    let query = &args[1];
    let file_path = &args[2];

    println!("Searching for {}", query);
    println!("In file {}", file_path);
    let contents = fs::read_to_string(file_path).expect("Should have been able to read the file");

    println!("With text:\n{contents}");
}
