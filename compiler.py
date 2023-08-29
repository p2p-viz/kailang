import sys
import os
import subprocess
import shutil
import time
import string
import re
import tokenize

def print_help_and_exit(message):
    """
    Just Print the message passed to it and exit in case of invalid invocation
    """
    if message:
        print(message)
    
    print('Usage: python compiler.py <file> [options]')
    print('Options include: -r Run, -i Start in Interactive mode')


def parse_expression(source: str, node: dict) -> dict:
    return {}


def parse_to_ast(sourceFile) -> dict:
    """
    The Frontend
    """

    context = {}
    context["tokens"] = list(tokenize.tokenize(sourceFile.readline))[1:]
    context["current_line"] = 0

    
    # print("|".join(map(lambda x: x.string, context["tokens"])))

    

    return {}

def compile_to_c(ast: dict) -> str:
    """
    The C Codegen Backend
    """
    return "Hi"

def main():
    """
    The main function
    """
    if len(sys.argv) < 2:
        print_help_and_exit('No input file specified')
        return
        
    fileName = sys.argv[1]
    options = []
    if len(sys.argv) > 2:
        options = sys.argv[2:]
   
    
    if not os.path.isfile(fileName):
        print_help_and_exit(f'File not found {fileName}')
        return
    
    with open(fileName, "rb") as f:
        ast = parse_to_ast(f)
        cCode = compile_to_c(ast)
        with open(fileName + ".c", "w+") as f2:
            f2.write(cCode)

    print("Done.")        
        

if __name__ == '__main__':
    main()