use std::{env, io::Write};

mod parser;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        println!("Usage: {} <file>", args[0]);
        return;
    }
    let file = std::fs::read_to_string(args[1].clone()).unwrap();
    let tokens = parser::lexer(file);
    
    let mut out = std::fs::File::create("tokens.txt").unwrap();
    // for token in tokens.clone() {
    //    out.write_fmt(format_args!("{} ", token)).unwrap();
    // }
    out.write_fmt(format_args!("{:?}", tokens)).unwrap();
    println!("Tokens generated successfully!");
    let res = parser::parser(tokens);
    match res {
        Ok(res) => {
            println!("Parser: {:?}", res);
        }
        Err(e) => {
            println!("Parser Error: {:?}", e);
        }
    }
}
