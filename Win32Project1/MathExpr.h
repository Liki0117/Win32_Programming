#ifndef MATHEXPRPARSER_H
#define MATHEXPRPARSER_H
#include <iostream>
#include <stack>
//#include <math.h>
#include <stdlib.h>

/*
#define ERROR_01 "Error: Empty Expression"
#define ERROR_02 "Error: Bad Operand"
#define ERROR_03 "Error: Bad Operator"
#define ERROR_04 "Error: Missing Operand"
#define ERROR_05 "Error: Divide by 0"
#define ERROR_06 "Error: Exponential Base is 0"*/

namespace MathExprParser {
	enum Operator {subtract, add, mult, div, power};
	
	Operator charToOp(char c) {
		switch (c) {
			case '-':
				return subtract;
			case '+':
				return add;
			case '*':
				return mult;
			case '/':
				return div;
			case '^':
				return power;
		}
	}
	
	//DEBUGGING USE
	char opToChar(Operator op) {
		switch (op) {
			case subtract:
				return '-';
			case add:
				return '+';
			case mult:
				return '*';
			case div:
				return '/';
			case power:
				return '^';
		}
	}
	
	/*-1 represents a < b
	0 represents a == b
	1 represents a > b
	*/
	int getPrecedence (Operator a, Operator b) {
		if (((a == subtract && b == add) || (a == add && b == subtract)) ||
				((a == mult && b == div) || (a == div && b == mult))) {
			return 0;
		}
		else if (a > b) {
			return 1;
		}
		else {
			return -1;
		}
	}
	
	bool validOperandChar(char c) {
		return (c >= '0' && c <= '9');
	}
	
	bool validOperator(char c) {
		switch (c) {
			case '-':
			case '+':
			case '*':
			case '/':
			case '^':
				return true;
			default:
				return false;
		}
	}
	
	//Calculates (number operator number)
	double calculateBasicExp(double a, double b, Operator op) {
		switch (op) {
			case subtract:
				return a - b;
			case add:
				return a + b;
			case mult:
				return a * b;
			case div:
				if (b == 0)
				//	throw ERROR_05;
				return a / b;
			case power:
				if (a == 0) {
				//	throw ERROR_06;
				}
				return pow(a, b);
		}
	}
	
	void skipSpaces(char*& s) {
		while (*s == ' ') {*s++;}
	}
	
	void parseOperand(char*& s, std::stack<double>& operandStack, std::stack<Operator>& operatorStack) {
		bool valid = false; //Ensures there is at least one digit before or after decimal
		double value = 0.0;
		
		while (validOperandChar(*s)) {
			valid = true;
			value = value * 10 + *s - '0';
			*s++;
		}
		if (*s == '.') {
			double divisor = 10.0;
			*s++;
			while(validOperandChar(*s)) {
				valid = true;
				value = value + (*s - '0') / divisor;
				divisor *= 10;
				*s++;
			}
		}
		if (!valid) {
			//throw ERROR_02;
		}
		else {
			operandStack.push(value);
		}
	}
	
	void evaluateStack(std::stack<double>& operandStack, std::stack<Operator>& operatorStack) {
		double a, b, result;
		Operator o;
		b = operandStack.top();
		operandStack.pop();
		a = operandStack.top();
		operandStack.pop();
		o = operatorStack.top();
		operatorStack.pop();
		//std::cout << a << opToChar(o) << b << std::endl;
		result = calculateBasicExp(a, b, o);
		operandStack.push(result);
	}
	
	void shortenStack(Operator op, std::stack<double>& operandStack, std::stack<Operator>& operatorStack) {
		if (getPrecedence(operatorStack.top(), op) != -1) {
			evaluateStack(operandStack, operatorStack);
		}
	}
	
	void parseExpression(char*& s, std::stack<double>& operandStack, std::stack<Operator>& operatorStack,
			char terminatingChar, int operandStackSize) {
		while (*s != terminatingChar) {
			skipSpaces(s);
			//Skip pluses because numbers are naturally positive
			while (*s == '+') {
				*s++;
				skipSpaces(s);
			}
			//For negative numbers
			while (*s == '-') {
				operandStack.push(-1);
				operatorStack.push(mult);
				*s++;
				skipSpaces(s);
			}
			//If there are brackets, recursion is used to evaluate the inner expression before the outer
			if (*s == '(') {
				*s++;
				parseExpression(s, operandStack, operatorStack, ')', operandStack.size());
				*s++;
			}
			else {
				//Parse Operand
				parseOperand(s, operandStack, operatorStack);
			}
			skipSpaces(s);
			//Parse Operator
			if (*s != terminatingChar) {
				if (validOperator(*s)) {
					if (operandStack.size() > operandStackSize + 1)
						shortenStack(charToOp(*s), operandStack, operatorStack);
					operatorStack.push(charToOp(*s));
				}
				else {
					//throw ERROR_03;
				}
			}
			skipSpaces(s);
			if (*s != terminatingChar) {
				*s++;
			}
		}
		while (operandStack.size() > max(operandStackSize + 1, 1)) {
			evaluateStack(operandStack, operatorStack);
		}
	}
	
	double parse(char* s) {
		std::stack<double> operandStack;
		std::stack<Operator> operatorStack;
		
		if (*s == '\0') { //Empty string is an invalid input
		//	throw ERROR_01;
		}
		
		parseExpression(s, operandStack, operatorStack, '\0', 0);
		
		if (operatorStack.size() == operandStack.size()) {
		//	throw ERROR_04;
		}
			
		return operandStack.top();
	}
}
#endif
