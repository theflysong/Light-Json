#ifndef __LIGHT_JSON_HEADER__ONCE_
#define __LIGHT_JSON_HEADER__ONCE_
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Token {
	enum Type {
		NUMBER,
		STRING,
		COMMA,
		COLON,
		CUR_BR,
		MID_BR,
		/*These In ASTTree*/
		NEXT_LEVEL,
		PAIR,
		EMPTY
	};
	Type type;
	string value;
};


struct Lexer {
	vector<Token> output;
	void lex(string code);
};

string toString(Token::Type type);

#ifndef LJSON_ASHEADER 

void Lexer::lex(string code) {
	for (int i = 0 ; i < code.length() ; i ++) {
	switch (code[i]) {
		case ' ':
		case '\n':
		case '\r':
		case '\t':
				break;
		case '{': {
				output.push_back(Token{Token::CUR_BR, "{"});
				break;
			}
		case '}': {
				output.push_back(Token{Token::CUR_BR, "}"});
				break;
			}
		case '[': {
				output.push_back(Token{Token::MID_BR, "["});
				break;
			}
		case ']': {
				output.push_back(Token{Token::MID_BR, "]"});
				break;
			}
			default: {
				break;
			}
		}
	}
}

string toString(Token::Type type) {
	switch (type) {
	case Token::NUMBER:
		return "Number";
	case Token::STRING:
		return "String";
	case Token::COMMA:
		return "Comma";
	case Token::COLON:
		return "Colon";
	case Token::CUR_BR:
		return "Curly Brace";
	case Token::MID_BR:
		return "Middle Brace";
	case Token::NEXT_LEVEL:
		return "Next";
	case Token::PAIR:
		return "Key-Value Pair";
	case Token::EMPTY:
		return "Empty";
	default:
		return "Unknow";
	}
}

//#define __DEBUG__LJSON_
#ifdef __DEBUG__LJSON_
int main() {
	return 0;
}
#endif
#endif
#endif