#[derive(Debug, PartialEq, Eq, Clone)]
pub enum Token {
    Number(i32),
    Plus,
    Minus,
    Multiply,
    Divide,
    GreaterThan,
    LessThan,
    LParen,
    RParen,
    LSqBracket,
    RSqBracket,
    Binding,
    RightArrow,
    LeftArrow,
    Identifier(String),
    String(String),
    Function,
    Dot,
    Keyword(Keywords),
    NewLine,
    Comma,
    EOF,
}

#[derive(Debug, PartialEq, Eq, Clone)]
pub enum Keywords {
    Import,
    Number,
    Enum,
    Char,
    Public,
    If,
    IO,
    Print,
}

pub fn lexer(src: String) -> Vec<Token> {
    println!("Lexer: \n{:?}", src);
    let mut tokens = Vec::new();
    let mut src = src.chars().peekable();
    while let Some(&c) = src.peek() {
        match c {
            '0'..='9' => {
                let mut num = String::new();
                while let Some(&c) = src.peek() {
                    match c {
                        '0'..='9' => {
                            num.push(c);
                            src.next();
                        }
                        _ => break,
                    }
                }
                tokens.push(Token::Number(num.parse().unwrap()));
            }
            '+' => {
                tokens.push(Token::Plus);
                src.next();
            }
            '-' => {
                // Check if next token is > then it's a right arrow
                src.next();
                if let Some(&'>') = src.peek() {
                    tokens.push(Token::RightArrow);
                    src.next();
                } else {
                    tokens.push(Token::Minus);
                }
            }
            '*' => {
                tokens.push(Token::Multiply);
                src.next();
            }
            '/' => {
                tokens.push(Token::Divide);
                src.next();
            }
            '>' => {
                tokens.push(Token::GreaterThan);
                src.next();
            }
            '<' => {
                src.next();
                if let Some(&'-') = src.peek() {
                    tokens.push(Token::LeftArrow);
                    src.next();
                }
                tokens.push(Token::LessThan);
            }
            '(' => {
                tokens.push(Token::LParen);
                src.next();
            }
            ')' => {
                tokens.push(Token::RParen);
                src.next();
            }
            '[' => {
                tokens.push(Token::LSqBracket);
                src.next();
            }
            ']' => {
                tokens.push(Token::RSqBracket);
                src.next();
            }
            '.' => {
                tokens.push(Token::Dot);
                src.next();
            }
            ',' => {
                tokens.push(Token::Comma);
                src.next();
            }
            '=' => {
                tokens.push(Token::Binding);
                src.next();
            }
            ':' => {
                tokens.push(Token::Function);
                src.next();
            }
            '"' => {
                let mut string = String::new();
                src.next();
                while let Some(&c) = src.peek() {
                    match c {
                        '"' => {
                            src.next();
                            break;
                        }
                        _ => {
                            string.push(c);
                            src.next();
                        }
                    }
                }
                tokens.push(Token::String(string));
            }
            'a'..='z' | 'A'..='Z' => {
                let mut id = String::new();
                while let Some(&c) = src.peek() {
                    match c {
                        'a'..='z' | 'A'..='Z' => {
                            id.push(c);
                            src.next();
                        }
                        _ => break,
                    }
                }
                match id.as_str() {
                    "import" => tokens.push(Token::Keyword(Keywords::Import)),
                    "number" => tokens.push(Token::Keyword(Keywords::Number)),
                    "enum" => tokens.push(Token::Keyword(Keywords::Enum)),
                    "char" => tokens.push(Token::Keyword(Keywords::Char)),
                    "pub" => tokens.push(Token::Keyword(Keywords::Public)),
                    "if" => tokens.push(Token::Keyword(Keywords::If)),
                    "IO" => tokens.push(Token::Keyword(Keywords::IO)),
                    "print" => tokens.push(Token::Keyword(Keywords::Print)),
                    _ => tokens.push(Token::Identifier(id)),
                }
            }
            '\n' => {
                tokens.push(Token::NewLine);
                src.next();
            }
            _ => {
                src.next();
            }
        }
    }
    tokens.push(Token::EOF);
    tokens
}

impl Token {
    pub fn is_eof(&self) -> bool {
        match self {
            Token::EOF => true,
            _ => false,
        }
    }
}

impl std::fmt::Display for Token {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Token::Number(n) => write!(f, "{}", n),
            Token::Plus => write!(f, "+"),
            Token::Minus => write!(f, "-"),
            Token::Multiply => write!(f, "*"),
            Token::Divide => write!(f, "/"),
            Token::GreaterThan => write!(f, ">"),
            Token::LessThan => write!(f, "<"),
            Token::LParen => write!(f, "("),
            Token::RParen => write!(f, ")"),
            Token::LSqBracket => write!(f, "["),
            Token::RSqBracket => write!(f, "]"),
            Token::Binding => write!(f, "="),
            Token::RightArrow => write!(f, "->"),
            Token::LeftArrow => write!(f, "<-"),
            Token::Identifier(id) => write!(f, "{}", id),
            Token::String(s) => write!(f, "\"{}\"", s),
            Token::Function => write!(f, ":"),
            Token::Dot => write!(f, "."),
            Token::Comma => write!(f, ","),
            Token::Keyword(k) => write!(f, "{}", k),
            Token::NewLine => write!(f, "\n"),
            Token::EOF => write!(f, "EOF"),
        }
    }
}

impl std::fmt::Display for Keywords {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Keywords::Import => write!(f, "import"),
            Keywords::Number => write!(f, "number"),
            Keywords::Enum => write!(f, "enum"),
            Keywords::Char => write!(f, "char"),
            Keywords::Public => write!(f, "pub"),
            Keywords::If => write!(f, "if"),
            Keywords::IO => write!(f, "IO"),
            Keywords::Print => write!(f, "print"),
        }
    }
}

#[derive(Debug)]
pub struct Program {
    imports: Vec<String>,
    bindings: Vec<Binding>,
}

#[derive(Debug)]
struct Binding {
    name: String,
    value: Expr,
}

#[derive(Debug)]
enum Expr {
    Number(f64),
    String(String),
    Identifier(String),
    Function(Box<Function>),
}

#[derive(Debug)]
struct Function {
    param: String,
    param_type: Type,
    body: Expr,
}

#[derive(Debug)]
enum Type {
    Number,
    String,
    Function(Box<Type>, Box<Type>),
    Map(Vec<(String, Type)>),
}

#[derive(Debug)]
pub enum Error {
    UnexpectedToken(Token),
    UnexpectedEOF,
}

pub fn parser(tokens: Vec<Token>) -> Result<Program, Error> {
    let mut tokens = tokens.iter().peekable();
    let mut program = Program {
        imports: Vec::new(),
        bindings: Vec::new(),
    };
    while let Some(token) = tokens.next() {
        match token {
            Token::Keyword(Keywords::Import) => {
                let mut path = String::new();
                loop {
                    if let Some(Token::Identifier(s)) = tokens.next() {
                        path.push_str(&s);
                    } else {
                        return Err(Error::UnexpectedToken(token.clone()));
                    }
                    if let Some(Token::Dot) = tokens.peek() {
                        path.push_str(".");
                    } else {
                        break;
                    }
                }
                program.imports.push(path);
            }
            Token::Identifier(name) => {
                if let Some(Token::Binding) = tokens.next() {
                    let value = match tokens.next() {
                        Some(Token::Number(n)) => Expr::Number(n.clone() as f64),
                        Some(Token::String(s)) => Expr::String(s.to_string()),
                        _ => return Err(Error::UnexpectedEOF)
                    };
                    program.bindings.push(Binding { name: name.to_string(), value });
                } else {
                    return Err(Error::UnexpectedToken(token.clone()));
                }
            }
            _ => continue,
        }
    }
    Ok(program)
}
