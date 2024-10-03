use std::io::Write;

mod parser;

fn main() {
    // Load test.fx file
    let file = std::fs::read_to_string("test.fx").unwrap();
    let tokens = parser::lexer(file);
    
    let mut out = std::fs::File::create("out.txt").unwrap();
    for token in tokens {
       out.write_fmt(format_args!("{} ", token)).unwrap();
    }
    println!("Tokens generated successfully!");
}
