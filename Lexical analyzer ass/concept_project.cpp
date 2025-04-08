#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>  // For strcpy_s
using namespace std;

enum class CharClass { LETTER, DIGIT, UNKNOWN, END_OF_FILE };
enum class Token {
    INT_LIT = 10, IDENT = 11,
    ASSIGN_OP = 20, ADD_OP = 21, SUB_OP = 22,
    MULT_OP = 23, DIV_OP = 24,
    LEFT_PAREN = 25, RIGHT_PAREN = 26,
    END_OF_FILE = -1
};

CharClass charClass;
char lexeme[100];
char nextChar;
int lexLen;
Token nextToken;
ifstream in_fp;

void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';
    }
    else {
        cout << "Error - lexeme too long\n";
    }
}

void getChar() {
    if (in_fp.get(nextChar)) {
        if (isalpha(nextChar)) charClass = CharClass::LETTER;
        else if (isdigit(nextChar)) charClass = CharClass::DIGIT;
        else charClass = CharClass::UNKNOWN;
    }
    else {
        charClass = CharClass::END_OF_FILE;
    }
}

void getNonBlank() {
    while (isspace(nextChar)) getChar();
}

Token lookup(char ch) {
    switch (ch) {
    case '(': nextToken = Token::LEFT_PAREN; break;
    case ')': nextToken = Token::RIGHT_PAREN; break;
    case '+': nextToken = Token::ADD_OP; break;
    case '-': nextToken = Token::SUB_OP; break;
    case '*': nextToken = Token::MULT_OP; break;
    case '/': nextToken = Token::DIV_OP; break;
    default: nextToken = Token::END_OF_FILE; break;
    }
    addChar();
    return nextToken;
}

Token lex() {
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
    case CharClass::LETTER:
        do { addChar(); getChar(); } while (charClass == CharClass::LETTER || charClass == CharClass::DIGIT);
        nextToken = Token::IDENT;
        break;

    case CharClass::DIGIT:
        do { addChar(); getChar(); } while (charClass == CharClass::DIGIT);
        nextToken = Token::INT_LIT;
        break;

    case CharClass::UNKNOWN:
        lookup(nextChar);
        getChar();
        break;

    case CharClass::END_OF_FILE:
        nextToken = Token::END_OF_FILE;
        strcpy_s(lexeme, "EOF");
        break;
    }

    cout << "Next token: " << static_cast<int>(nextToken)
        << ", Next lexeme: " << lexeme << endl;
    return nextToken;
}

int main() {
    in_fp.open("front.in");
    if (!in_fp.is_open()) {
        cerr << "ERROR: Cannot open front.in\n";
        return 1;
    }

    getChar();
    do {
        lex();
    } while (nextToken != Token::END_OF_FILE);

    in_fp.close();
    return 0;
}