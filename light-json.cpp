#ifndef __LIGHT_JSON_HEADER__ONCE_
#define __LIGHT_JSON_HEADER__ONCE_
#include <string>
#include <vector>
#include <map>
#include <cstdarg>
#include <cstdio>

#define INCOMPLETE
#define COMPLETE
#define DEPRECATED
#define PARSER_COMPONENT
#define DEBUG_UTIL 

COMPLETE PARSER_COMPONENT namespace __JsonParser_Component {
	COMPLETE PARSER_COMPONENT struct Token {
		COMPLETE PARSER_COMPONENT enum Type {
			EMPTY,
			NUMBER,
			STRING,
			COMMA,
			COLON,
			CUR_BR,
			MID_BR,
			BOOLEAN,
			/*These In ASTTree*/
			OBJECT,
			ARRAY,
			PAIR
		};
		Type type;
		std::string value;
		bool operator<(Token a) const {
			return true;
		}
	};

	COMPLETE PARSER_COMPONENT struct ErrorHandler {
		COMPLETE PARSER_COMPONENT static void error(const char* msg, ...);
		COMPLETE PARSER_COMPONENT static void UnexpectError(const char except, const char current);
		COMPLETE PARSER_COMPONENT static void UnexpectError(const char* except, const char current);
		COMPLETE PARSER_COMPONENT static void UnexpectError(const std::string except, const std::string current);
		COMPLETE PARSER_COMPONENT static void UnexpectError(const Token except, const Token current);
	};

	COMPLETE PARSER_COMPONENT struct Lexer {
		std::string* words;
		int wordsNum;
		char* sign;
		int signNum;
		std::map<std::string, Token> tokenMap;
		COMPLETE PARSER_COMPONENT Lexer(std::string words[], int wordsNum, char sign[], int signNum, std::map<std::string, Token> tokenMap);
		COMPLETE PARSER_COMPONENT bool isSign(char ch);
		COMPLETE PARSER_COMPONENT bool isWord(std::string word);
		COMPLETE PARSER_COMPONENT Token signDo(char ch, std::string code, int& pos);
		COMPLETE PARSER_COMPONENT std::vector<Token> lex(std::string code);
	};

	INCOMPLETE PARSER_COMPONENT struct Parser {
		COMPLETE PARSER_COMPONENT struct ASTNode {
			std::vector<ASTNode*> children;
			ASTNode* parent;
			Token value;

			COMPLETE PARSER_COMPONENT ASTNode* addChildren(ASTNode* value);
			COMPLETE PARSER_COMPONENT ASTNode* addChildren(Token value);
			COMPLETE PARSER_COMPONENT ~ASTNode();
		};
		Token *begins;
		std::map<Token, Token> ends;
		COMPLETE PARSER_COMPONENT Parser(Token *begins, std::map<Token, Token> ends);
		INCOMPLETE PARSER_COMPONENT ASTNode* parse(std::vector<Token> token, int beginPos = 0);
	};

	COMPLETE DEBUG_UTIL std::string toString(Token::Type type);
}
#ifndef LJSON_ASHEADER 
COMPLETE PARSER_COMPONENT namespace __JsonParser_Component {
	PARSER_COMPONENT std::string JsonKeyWords[] = {"true", "false", ""};
	PARSER_COMPONENT int wordsNum = sizeof(JsonKeyWords) / sizeof(*JsonKeyWords);
	PARSER_COMPONENT char JsonSign[] = {
		'\n', '\r', '\t', ' ', 
		'{', '}', '[', ']', ':', ',', '"', '\'',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	PARSER_COMPONENT int signNum = sizeof(JsonSign) / sizeof(*JsonSign);
	PARSER_COMPONENT Token JsonBegins[] = {
		{Token::CUR_BR, "{"}, {Token::MID_BR, "["}
	};
	PARSER_COMPONENT int beginsNum = sizeof(JsonBegins) / sizeof(*JsonBegins);
	PARSER_COMPONENT std::map<Token, Token> JsonEnds;
	PARSER_COMPONENT std::map<std::string, Token> JsonTokenMap;
	COMPLETE PARSER_COMPONENT void init() {
		JsonTokenMap["true"] = Token{Token::BOOLEAN, "true"};
		JsonTokenMap["false"] = Token{Token::BOOLEAN, "false"};
		JsonTokenMap[""] = Token{Token::EMPTY, ""};
		JsonEnds[{Token::CUR_BR, "{"}] = {Token::CUR_BR, "}"};
		JsonEnds[{Token::MID_BR, "["}] = {Token::MID_BR, "]"};
	}
	COMPLETE PARSER_COMPONENT void ErrorHandler::error(const char* msg, ...) {
		va_list args;
		va_start(args, msg);
		vfprintf(stderr, msg, args);
		va_end(args);
	}
	COMPLETE PARSER_COMPONENT void ErrorHandler::UnexpectError(const char except, const char current){
		error("Unexpect Char \'%c\'(ASCII:%d), expect\'%c\'(ASCII:%d).", current, current, except, except);
	}
	COMPLETE PARSER_COMPONENT void ErrorHandler::UnexpectError(const char* except, const char current){
		error("Unexpect Char \'%c\'(ASCII:%d), expect %s", current, current, except);
	}
	COMPLETE PARSER_COMPONENT void ErrorHandler::UnexpectError(const std::string except, const std::string current){
		error("Unexpect Word %s, expect %s", current.c_str(), except.c_str());
	}
	COMPLETE PARSER_COMPONENT void ErrorHandler::UnexpectError(const Token except, const Token current){
		error("Unexpect Token {%s:%s}, expect {%s:%s}", toString(current.type).c_str(), current.value.c_str(), toString(except.type).c_str(), except.value.c_str());
	}
	COMPLETE PARSER_COMPONENT Lexer::Lexer(std::string words[], int wordsNum, char sign[], int signNum, std::map<std::string, Token> tokenMap) {
		this->words = words;
		this->wordsNum = wordsNum;
		this->sign = sign;
		this->signNum = signNum;
		this->tokenMap = tokenMap;
	}
	COMPLETE PARSER_COMPONENT std::vector<Token> Lexer::lex(std::string code) {
		std::vector<Token> output;
		std::string word = "";
		for (int i = 0 ; i < code.length() ; i ++) {
			if (! isSign(code[i])) {
				word += code[i];
			}
			else {
				if (! isWord(word)) {
					ErrorHandler::UnexpectError("???", word);
				}
				else {
					if (tokenMap[word].type != Token::EMPTY)
						output.push_back(tokenMap[word]);
				}
				word = "";
				Token tok = signDo(code[i], code, i);
				if (tok.type != Token::EMPTY)
					output.push_back(tok);
			}
		}
		return output;
	}
	COMPLETE PARSER_COMPONENT bool Lexer::isSign(char ch) {
		for (int i = 0 ; i < signNum ; i ++) {
			if (sign[i] == ch) {
				return true;
			}
		}
		return false;
	}
	COMPLETE PARSER_COMPONENT bool Lexer::isWord(std::string word) {
		for (int i = 0 ; i < wordsNum ; i ++) {
			if (words[i] == word) {
				return true;
			}
		}
	}
	COMPLETE PARSER_COMPONENT Token Lexer::signDo(char ch, std::string code, int& pos) {
	switch (ch) {
		case ' ':
		case '\n':
		case '\r':
		case '\t':
			return Token{Token::EMPTY, ""};
		case '{': {
			return Token{Token::CUR_BR, "{"};
		}
		case '}': {
			return Token{Token::CUR_BR, "}"};
		}
		case '[': {
			return Token{Token::MID_BR, "["};
		}
		case ']': {
			return Token{Token::MID_BR, "]"};
		}
		case ':': {
			return Token{Token::COLON, ":"};
		}
		case ',': {
			return Token{Token::COMMA, ","};
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
			std::string v = "";
			while (isdigit(code[pos])) v += code[pos ++];
			pos --;
			return Token{Token::NUMBER, v};
			break;
		}
		case '"': {
			std::string v = ""; pos ++;
			while (code[pos] != '"') v += code[pos ++]; 
			return Token{Token::STRING, v};
		}
		default: {
			return Token{Token::EMPTY, ""};
		}
	}
	}
	COMPLETE DEBUG_UTIL std::string toString(Token::Type type) {
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
		case Token::BOOLEAN:
			return "Boolean";
		default:
			return "Unknow";
		}
	}
	COMPLETE PARSER_COMPONENT Parser::ASTNode::~ASTNode() {
		for (ASTNode* child : children) {
			delete child;
		}
		children.clear();
	}
	COMPLETE PARSER_COMPONENT Parser::ASTNode* Parser::ASTNode::addChildren(ASTNode* node) {
		node->parent = this;
		this->children.push_back(node);
		return node;
	}
	COMPLETE PARSER_COMPONENT Parser::ASTNode* Parser::ASTNode::addChildren(Token value) {
		ASTNode* node;
		node->value = value;
		return addChildren(node);
	}
	COMPLETE PARSER_COMPONENT Parser::Parser(Token *begins, std::map<Token, Token> ends) {
		this->begins = begins;
		this->ends = ends;
	}
	INCOMPLETE PARSER_COMPONENT Parser::ASTNode* Parser::parse(std::vector<Token> token, int beginPos) {
		return NULL;
	}
}


//#define __DEBUG__LJSON_
#ifdef __DEBUG__LJSON_
#include <iostream>
DEBUG_UTIL int main() {
	using namespace __JsonParser_Component;
	init();
	Lexer lexer(JsonKeyWords, wordsNum, JsonSign, signNum, JsonTokenMap);
	std::vector<Token> output = lexer.lex(
		"{\n\
			\"a\":233,\n\
			\"c\": {\"arr\": [\"222\", \"777\"]}\n\
			\"bool\": [true, false, true, false]\n\
		}"
		);
	for(Token token : output) {
		std::cout << toString(token.type) << ":" << token.value << std::endl;
	}
	return 0;
}
#endif
#endif
#endif
