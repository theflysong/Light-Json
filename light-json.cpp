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

struct Lexer {
	vector<Token> output;
	void lex(string code) {
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
};

int main() {
	return 0;
}
