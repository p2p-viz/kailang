#[derive(Debug, PartialEq, Eq)]
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

#[derive(Debug, PartialEq, Eq)]
pub enum Keywords {
    Import,
    Number,
    Enum,
    Char,
    Public,
    If,
    IO,
    Print
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