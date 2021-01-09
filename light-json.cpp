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
		OBJECT,
		ARRAY,
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
		case ':': {
			output.push_back(Token{Token::COLON, ":"});
			break;
		}
		case ',': {
			output.push_back(Token{Token::COMMA, ","});
			break;
		}
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': {
			string v = "";
			while (isdigit(code[i])) v += code[i ++];
			i --;
			output.push_back(Token{Token::NUMBER, v});
			break;
		}
		case '"': {
			string v = ""; i ++;
			while (code[i] != '"') v += code[i ++]; 
			output.push_back(Token{Token::STRING, v});
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
	case Token::OBJECT:
		return "Object";
	case Token::ARRAY:
		return "Array";
	case Token::PAIR:
		return "Pair";
	case Token::EMPTY:
		return "Empty";
	default:
		return "Unknow";
	}
}

#define __DEBUG__LJSON_
#ifdef __DEBUG__LJSON_
int main() {
	Lexer lexer;
	lexer.lex(
		"{\n\
			\"a\":233,\n\
			\"c\": {\"arr\": [\"222\", \"777\"]}\n\
		}"
		);
	for(Token token : lexer.output) {
		cout << toString(token.type) << ":" << token.value << endl;
	}
	return 0;
}
#endif
#endif
#endif