#ifndef __LIGHT_JSON_HEADER__ONCE_
#define __LIGHT_JSON_HEADER__ONCE_
#include <string>
#include <vector>
#include <map>
#include <cstdarg>
#include <cstdio>

namespace __JsonParser_Util {
	struct Token {
		enum Type {
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
			PAIR,
			EMPTY
		};
		Type type;
		std::string value;
	};

	struct ErrorHandler {
		static void error(const char* msg, ...);
		static void UnexpectError(const char except, const char current);
		static void UnexpectError(const char* except, const char current);
		static void UnexpectError(const std::string except, const std::string current);
		static void UnexpectError(const Token except, const Token current);
	};

	struct Lexer {
		std::string* words;
		int wordsNum;
		char* sign;
		int signNum;
		std::map<std::string, Token> tokenMap;
		Lexer(std::string words[], int wordsNum, char sign[], int signNum, std::map<std::string, Token> tokenMap);
		bool isSign(char ch);
		bool isWord(std::string word);
		Token signDo(char ch, std::string code, int& pos);
		std::vector<Token> lex(std::string code);
	};

	struct Parser {
		struct ASTNode {
			std::vector<ASTNode*> children;
			ASTNode* parent;
			Token value;

			ASTNode* addChildren(ASTNode* value);
			ASTNode* addChildren(Token value);
			~ASTNode();
		};

		ASTNode* parse(std::vector<Token> token, int beginPos = 0);
	};

	std::string toString(Token::Type type);
}
#ifndef LJSON_ASHEADER 
namespace __JsonParser_Util {
	std::string JsonKeyWords[] = {"true", "false"};
	int wordsNum = sizeof(JsonKeyWords) / sizeof(*JsonKeyWords);
	char JsonSign[] = {
		'\n', '\r', '\t', ' ', 
		'{', '}', '[', ']', ':', ',', '"', '\'',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	int signNum = sizeof(JsonSign) / sizeof(*JsonSign);
	std::map<std::string, Token> JsonTokenMap;
	void init() {
		JsonTokenMap["true"] = Token{Token::BOOLEAN, "true"};
		JsonTokenMap["false"] = Token{Token::BOOLEAN, "false"};
	}
	void ErrorHandler::error(const char* msg, ...) {
		va_list args;
		va_start(args, msg);
		vfprintf(stderr, msg, args);
		va_end(args);
	}
	void ErrorHandler::UnexpectError(const char except, const char current){
		error("Unexpect Char \'%c\'(ASCII:%d), expect\'%c\'(ASCII:%d).", current, current, except, except);
	}
	void ErrorHandler::UnexpectError(const char* except, const char current){
		error("Unexpect Char \'%c\'(ASCII:%d), expect %s", current, current, except);
	}
	void ErrorHandler::UnexpectError(const std::string except, const std::string current){
		error("Unexpect Word %s, expect %s", current.c_str(), except.c_str());
	}
	void ErrorHandler::UnexpectError(const Token except, const Token current){
		error("Unexpect Token {%s:%s}, expect {%s:%s}", toString(current.type).c_str(), current.value.c_str(), toString(except.type).c_str(), except.value.c_str());
	}
	Lexer::Lexer(std::string words[], int wordsNum, char sign[], int signNum, std::map<std::string, Token> tokenMap) {
		this->words = words;
		this->wordsNum = wordsNum;
		this->sign = sign;
		this->signNum = signNum;
		this->tokenMap = tokenMap;
	}
	std::vector<Token> Lexer::lex(std::string code) {
		std::vector<Token> output;
		std::string word = "";
		for (int i = 0 ; i < code.length() ; i ++) {
			if (! isSign(code[i])) {
				word += code[i];
			}
			else {
				if (! isWord(word)) {
					ErrorHandler::UnexpectError(word, "???");
				}
				else {
					output.push_back(tokenMap[word]);
				}
				word = "";
				output.push_back(signDo(code[i], code, i));
			}
		}
		return output;
	}
	bool Lexer::isSign(char ch) {
		for (int i = 0 ; i < signNum ; i ++) {
			if (sign[i] == ch) {
				return true;
			}
		}
		return false;
	}
	bool Lexer::isWord(std::string word) {
		for (int i = 0 ; i < wordsNum ; i ++) {
			if (words[i] == word) {
				return true;
			}
		}
	}
	Token Lexer::signDo(char ch, std::string code, int& pos) {
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
			break;
		}
	}
	}
	std::string toString(Token::Type type) {
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
	Parser::ASTNode::~ASTNode() {
		for (ASTNode* child : children) {
			delete child;
		}
		children.clear();
	}
	Parser::ASTNode* Parser::ASTNode::addChildren(ASTNode* node) {
		node->parent = this;
		this->children.push_back(node);
		return node;
	}
	Parser::ASTNode* Parser::ASTNode::addChildren(Token value) {
		ASTNode* node;
		node->value = value;
		return addChildren(node);
	}
	Parser::ASTNode* Parser::parse(std::vector<Token> token, int beginPos) {
		bool mode = token[beginPos].type == Token::CUR_BR;
		ASTNode* root = new ASTNode;
		bool isKeyed = false;
		if (mode) {
			root->value = Token{Token::OBJECT, "object"};
			ASTNode* cur = root;
			for (int i = beginPos + 1 ; i < token.size() ; i ++) {
				switch (token[i].type) {
					case Token::CUR_BR: {
						if (token[i].value == "{") {
							cur->addChildren(parse(token, i));
						}
						else {
							goto End;
						}
						break;
					}
					case Token::STRING: {
						break;
					}
					default:
						break;
				}
			}
		}
		else {
			root->value = Token{Token::ARRAY, "array"};
			for (int i = beginPos + 1 ; i < token.size() ; i ++) {
				
			}
		}
		End:
		return root;
	}
}


#define __DEBUG__LJSON_
#ifdef __DEBUG__LJSON_
#include <iostream>
int main() {
	using namespace __JsonParser_Util;
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