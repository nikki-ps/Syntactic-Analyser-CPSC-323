#pragma once
#include <fstream>
#include <cctype>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <stack>

using namespace std;

bool isKeyword(string kWord)
{
	if (kWord == "int" || kWord == "float" || kWord == "bool" ||
		kWord == "true" || kWord == "false" || kWord == "if" ||
		kWord == "else" || kWord == "then" || kWord == "endif" ||
		kWord == "while" || kWord == "whileend" || kWord == "do" ||
		kWord == "doend" || kWord == "for" || kWord == "forend" ||
		kWord == "input" || kWord == "output" || kWord == "and" ||
		kWord == "or" || kWord == "not")
	{
		return true;
	}
	return false;
}

bool isSeperator(char s)
{
	if (s == '\'' || s == '(' || s == ')' || s == '{' || s == '}' ||
		s == '[' || s == ']' || s == ',' || s == '.' || s == ':' ||
		s == ';')
	{
		return true;
	}
	return false;
}

bool isOperator(char o)
{
	if (o == '*' || o == '+' || o == '-' || o == '=' || o == '/' || o == '>' ||
		o == '<' || o == '%')
	{
		return true;
	}
	return false;
}

//iterates through string char by char to check if there is a decimal point
//if decimal point is found, it is a real number
bool isRealNumber(string temp)
{
	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i] == '.')
		{
			return true;
		}
	}
	return false;
}
//structure to store lexeme and it's token type
struct Token
{
	string token;
	string lexeme;
};


stack<Token> synStack;
void stackBuilder(vector<Token> tokens)
{

	//creating $ to keep track of end of loop
	Token temp;
	temp.token = "$$";
	temp.lexeme = "$$";
	synStack.push(temp);
	for (int i = tokens.size() - 1; i >= 0; i--)
	{
		synStack.push(tokens[i]);
	}
}



//prints token vector to main, and also prints to a text file, 
//"sample.txt"
void print(vector<Token> tokens)
{
	cout << "TOKEN" << setw(20) << "LEXEMES" << endl;
	for (int i = 0; i < tokens.size(); i++)
	{
		cout << tokens[i].token << setw(5) << "=" << setw(9) << tokens[i].lexeme << endl;
	}
	fstream file;
	file.open("output3.txt", ios::out);
	if (!file) {
		cout << "Error creating file" << endl;
	}
	file << "TOKEN" << setw(20) << "LEXEMES" << endl;
	for (int i = 0; i < tokens.size(); i++)
	{
		file << tokens[i].token << setw(5) << "=" << setw(9) << tokens[i].lexeme << endl;
	}
	file.close();
}

//determines whether incoming char is a Separator or Operator
//and pushes back the input as either a separator or operator
void inputCheck(char& input, vector<Token>& tokens)
{
	if (isOperator(input))
	{
		string temp = "";
		temp += input;
		tokens.push_back({ "Operator  ", temp });
	}
	else if (isSeperator(input))
	{
		string temp = "";
		temp += input;
		tokens.push_back({ "Seperator ", temp });
	}
}
//main function
void analyzer(const char* file)
{
	ifstream fin(file);
	char input;
	vector<Token> tokens;
	int currentState = 0;
	int row = 0;
	int col = 0;
	string currentToken = "";

	int stateTable[20][20] = {
		//character, number, ., seperator, space, operator , $ , !
		{2,3,9,8,1,9,12,10}, // initital
		{2,2,5,5,4,5,2,5}, //keyword/identifier
		{12,3,3,7,6,7,12,7},// real number/integer
		{1,1,1,1,1,1,1,1}, //complete keyword/identifier
		{1,1,1,1,1,1,1,1}, //complete keyword/identifier but Sep or op encountered
		{1,1,1,1,1,1,1,1}, //complete number
		{1,1,1,1,1,1,1,1}, // complete number but Sep or op encountered
		{1,1,1,1,1,1,1,1}, // seperator
		{1,1,1,1,1,1,1,1}, //operator
		{10,10,10,10,10,10,10,11}, //comment start
		{1,1,1,1,1,1,1,1}, //comment complete
		{1,1,1,1,1,1,1,1}, //error
	};
	//reads in input from file and assigns it corresponding column
	//value in state table
	while (fin.get(input))
	{
		if (isalpha(input))
		{
			col = 0;
		}
		else if (isdigit(input))
		{
			col = 1;
		}
		else if (input == '.')
		{
			col = 2;
		}
		else if (isSeperator(input))
		{
			col = 3;
		}
		else if (input == ' ' || input == '\n')
		{
			col = 4;
		}
		else if (isOperator(input))
		{
			col = 5;
		}
		else if (input == '$')
		{
			col = 6;
		}
		else if (input == '!')
		{
			col = 7;
		}
		//using row and column value assigns the state table value to
		//currentState to be used in the switch statement.
		currentState = stateTable[row][col];

		switch (currentState)
		{
		case 1:                                                 //Initial State
			break;
		case 2:                                                 //Keyword/Identifier State
			currentToken += input;
			row = currentState - 1;
			break;
		case 3:                                                 //RealNumber/Integer State
			currentToken += input;
			row = currentState - 1;
			break;
		case 4:                                                 //Kw/Identifier complete
			if (isKeyword(currentToken))
			{
				tokens.push_back({ "KeyWord   ",currentToken });
			}
			else
			{
				tokens.push_back({ "Identifier",currentToken });
			}
			currentToken = "";
			currentState = 0;
			row = 0;
			break;
		case 5:                                                 //Kw/Identifier complete but S/O encountered
			if (isKeyword(currentToken))
			{
				tokens.push_back({ "KeyWord   ",currentToken });
			}
			else
			{
				tokens.push_back({ "Identifier",currentToken });
			}
			inputCheck(input, tokens);
			currentToken = "";
			currentState = 0;
			row = 0;
			break;
		case 6:                                                 //RealNumber/Integer complete
			if (isRealNumber(currentToken))
			{
				tokens.push_back({ "RealNumber", currentToken });
			}
			else
			{
				tokens.push_back({ "Integer   ", currentToken });
			}
			currentToken = "";
			currentState = 0;
			row = 0;
			break;
		case 7:                                                 //RealNumber/Integer complete but S/O encountered
			if (isRealNumber(currentToken))
			{
				tokens.push_back({ "RealNumber", currentToken });
			}
			else
			{
				tokens.push_back({ "Integer   ", currentToken });
			}
			inputCheck(input, tokens);
			currentToken = "";
			currentState = 0;
			row = 0;
			break;
		case 8:                                                 //Separator encountered
			currentToken = input;
			tokens.push_back({ "Seperator ", currentToken });
			currentToken = "";
			currentState = 0;
			row = 0;
			break;
		case 9:                                                 //Operator encountered
			currentToken = input;
			tokens.push_back({ "Operator  ", currentToken });
			currentToken = "";
			currentState = 0;
			row = 0;
			break;
		case 10:                                                //comment block encountered
			row = currentState - 1;
			break;
		case 11:                                                //comment block complete
			currentToken = "";
			currentState = 0;
			row = 0;
			break;
		case 12:                                                //Error Handler
			tokens.push_back({ "Error     ", currentToken });
			currentToken = "";
			currentState = 0;
			row = 0;
			break;
		}
	}
	/*
  if the last entry is a character or digit
  essentially the final state, takes the remainder of anything inside
  the current lexeme variable and tests its token type
  */
	if (currentToken != "")
	{
		if (isKeyword(currentToken))
		{
			tokens.push_back({ "KeyWord    ",currentToken });
		}
		else if (isdigit(currentToken[0]))
		{
			if (isRealNumber(currentToken))
			{
				tokens.push_back({ "RealNumber", currentToken });
			}
			else
			{
				tokens.push_back({ "Integer   ", currentToken });
			}
		}
		else
		{
			tokens.push_back({ "Identifier",currentToken });
		}
	}
	fin.close();
	stackBuilder(tokens);
}