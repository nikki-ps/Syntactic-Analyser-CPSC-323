// Assignment_2_ROUGH.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Lexer.h"
using namespace std;
string getToken();
string getLexeme();
void Display();
void DisplayandPop();
void Pop();
bool Identifier();
bool RealNumber();
bool Factor();
bool TermPrime();
bool Term();
bool ExpressionPrime();
bool Expression();
bool Statement();
bool Assign();
bool Declarative();
bool Type();
void ErrorCall(string errorType);

// Returns the token on the top of the stack
string getToken()
{
	Token temp = synStack.top();
	return temp.token;

}
// Returns the Lexeme on the top of the stack
string getLexeme()
{
	Token temp = synStack.top();
	return temp.lexeme;

}
// Displays the lexeme and token on the top of the stack
void Display()
{
	Token temp = synStack.top();
	cout << temp.lexeme << "     " << temp.token << endl;
}

//Pops the Token on the top of the stack then displays the next Token if it is not the EOF marker.
void DisplayandPop()
{
	synStack.pop();


	//if EOF marker has not been reached yet
	if (getLexeme() != "$$")
	{
		Token temp = synStack.top();
		cout << endl << endl << temp.lexeme << "     " << temp.token << endl;
	}
}
// Pops the Token on the top of the stack.
void Pop()
{
	synStack.pop();
	cout << endl;
}


bool Identifier()
{
	//tests if lexeme's respective token is an identifier, return true
	if (getToken() == "Identifier")
	{
		return true;
	}
	return false;
}

bool RealNumber()
{
	//tests if lexeme's respective token is a realNumber, return true
	if (getToken() == "RealNumber")
	{
		return true;
	}
	return false;

}

//FACTOR --> ( <EXPRESSION> ) ||
//FACTOR --> <ID> ||
//FACTOR --> <Num>
bool Factor()
{
	if (Identifier())
	{
		cout << "<Factor> -> <ID>" << endl;
		cout << "< ID > ->Identifier" << endl;

		//Pop the top element of stack
		//start working on next element

		DisplayandPop();


		return true;
	}
	else if (RealNumber())
	{
		cout << "<Factor> -> <Number> " << endl;

		// Pop the top element of stack
		//start working on next element

		DisplayandPop();

		return true;
	}
	else if (getLexeme() == "(")
	{
		//If left paranthesis is encountered, the paranthesis is popped and Expression will be ran.
		cout << "<Factor> -> (<Expression>)" << endl;
		DisplayandPop();
		if (Expression())
		{
			//Expression is called from within itself
			if (getLexeme() == ")")
			{
				//If Expression returns true and Right Paranthesis is encountered, it is popped and returns true.
				cout << "<Factor> -> (<Expression>)" << endl;
				DisplayandPop();
				return true;
			}

			ErrorCall("Invalid Factor: <Factor> -> (<Expression>) | <Identifier> | <RealNumber> ");
			return false;
		}
		ErrorCall("Invalid Factor: <Factor> -> (<Expression>) | <Identifier> | <RealNumber> ");
		return false;
	}
	else
	{
		ErrorCall("Invalid Factor");
		return false;
	}
}




//TERM PRIME --> * <FACTOR> <TERM PRIME> ||
//TERM PRIME --> / <FACTOR> <TERM PRIME> ||
//TERM PRIME-- > Epsilon
bool TermPrime()
{
	//check base case
	if (getLexeme() == "$$")
	{
		return true;
	}

	//check if * sign exists
	if (getLexeme() == "*")
	{
		cout << "<TermPrime> -> * <Factor> <TermPrime>" << endl;

		DisplayandPop();

		if (Factor())
		{
			if (TermPrime())
			{
				return true;
			}
			return false;
		}

		return false;
	}

	//check if / sign exists
	if (getLexeme() == "/")
	{
		cout << "<TermPrime> -> / <Factor> <TermPrime>" << endl;

		DisplayandPop();

		if (Factor())
		{
			if (TermPrime())
			{
				return true;
			}
			return false;
		}

		return false;
	}

	//in the case epsilon is reached
	//do nothing return true
	return true;
}





//TERM --> <FACTOR> <TERM PRIME>
bool Term()
{
	cout << "<Term> -> <Factor> <Term Prime>" << endl;
	if (Factor())
	{
		if (TermPrime())
		{
			return true;
		}
		return false;
	}
	return false;
}





//EXPRESSION PRIME --> +<TERM><EXPRESSION PRIME> ||
//EXPRESSION PRIME --> -<TERM><EXPRESSION PRIME> ||
//EXPRESSION PRIME --> Epsilon
bool ExpressionPrime()
{
	//check base case
	if (getLexeme() == "$$")
	{
		return true;
	}

	//check if + sign exists
	if (getLexeme() == "+")
	{
		//print results
		cout << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;

		DisplayandPop();

		if (Term())
		{
			if (ExpressionPrime())
			{
				return true;
			}
		}
	}
	//check if - sign exists
	else if (getLexeme() == "-")
	{
		//print results
		cout << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;

		DisplayandPop();

		if (Term())
		{
			if (ExpressionPrime())
			{
				return true;
			}
		}
	}

	//in the situation it is epsilon, 
	//I believe I would just do nothing and return true, ending the sequence
	else
	{
		cout << "<Empty> -> Epsilon" << endl;
		return true;
	}
}

// EXPRESSION --> <TERM><EXPRESSION PRIME>
bool Expression()
{

	cout << "<Expression> -> <Term> <Expression Prime>" << endl;
	if (Term())
	{
		if (ExpressionPrime())
		{
			return true;
		}
		return false;
	}
	return false;
}
//<Statement> -> <Assign>
//<Statement> -> <Declarative> 
bool Statement()
{
	//Displays the first token and lexeme
	Display();
	if (Assign())
	{
		if (getLexeme() == ";")
		{
			/*if the current lexeme on the top of the stack is a semicolon, indicating the end of a line of code, it is
			popped. Then if the next lexeme is not the EOF, indicating another line of code exists, "Statement" is ran
			again.*/
			Pop();
			if (getLexeme() != "$$")
			{
				if (Statement())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			ErrorCall("Invalid Assign: Expected ';'");
			return false;
		}
	}
	else if (Declarative())
	{
		if (getLexeme() == ";")
		{
			//Checking if next line of code exists like it does above.
			Pop();
			if (getLexeme() != "$$")
			{
				if (Statement())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			ErrorCall("Invalid Declarative: Expected ';'");
			return false;
		}
	}
	ErrorCall("Invalid Statement!");
	return false;
}

// <Assign> -> <ID> = <Expression>;
bool Assign()
{
	if (getToken() == "Identifier")
	{
		//if the current Token is an Identifier
		cout << "<Statement> -> <Assign> " << endl;
		cout << "<Assign> -> <ID> = <Expression>;" << endl;
		cout << "<ID> -> Identifier" << endl;
		DisplayandPop();

		if (getLexeme() == "=")
		{
			//if the current Lexeme is a "="
			DisplayandPop();

			if (Expression())
			{
				//runs expression again and returns true if expression is true.
				return true;
			}
			return false;
		}
		ErrorCall("Invalid Assign: Expected '=' ");
		return false;
	}
	return false;
}

//<Declarative> -> <Type> <ID> 
bool Declarative()
{
	if (Type())
	{
		//if type returns true, (Lexeme is an int, bool, or float.)
		if (Identifier())
		{
			//runs "Identifier" and if the Token is an identifier returns true.
			cout << "<ID> -> Identifier" << endl;
			DisplayandPop();
			return true;
		}
		ErrorCall("Invalid Declarative: Expected proper Identifier");
		return false;
	}
	return false;
}

//<Type> -> bool | float | int 
bool Type()
{
	//if the current Token is a bool, float, or int and a Keyword, returns true.
	if (getLexeme() == "int" && getToken() == "KeyWord   ")
	{
		cout << "<Declarative> -> <Type> <ID>" << endl;
		cout << "<Type> -> bool | float | int " << endl;
		DisplayandPop();
		return true;
	}
	else if (getLexeme() == "bool" && getToken() == "KeyWord   ")
	{
		cout << "<Declarative> -> <Type> <ID>" << endl;
		cout << "<Type> -> bool | float | int " << endl;
		DisplayandPop();
		return true;
	}
	else if (getLexeme() == "float" && getToken() == "KeyWord   ")
	{

		cout << "<Type> -> bool | float | int " << endl;
		DisplayandPop();
		return true;
	}
	return false;
}
void ErrorCall(string errorType)
{
	cout << "\nError Encountered:" << errorType << endl << endl;
}

