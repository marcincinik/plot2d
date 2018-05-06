#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "Parser.h"
#include <istream>
#include <ostream>
#include <vector>
#include <exception>
#include <string>
#include <exception>

namespace calc {


	/* report all calculation exceptions */
	class StatementException : public std::exception {
	private:
		int symbolNo;
		std::string s;
	public:
		StatementException(std::string s);
		StatementException(int symbolNo);
		StatementException(int symbolNo, std::string s);
		virtual const char *what() const;
		std::string whatStr();
	};

	/* forward declaration */
	class RPNElement;

	/* Calculator to evaluate expressions using the Reverse Polish Notation.
	Instance of this class is either created using the RPN Notation (string)
	or using AST tree resulting from parsing.*/
	class Calculator {
	private:
			std::vector<RPNElement*> input;
			std::string variableName;
			parser::FunctionLookupTable* functionLookupTable;
			parser::ConstantLookupTable* constantLookupTable;
			void constructFromStream(std::istream& inputStream);
	public:
		/* create from AST*/
		Calculator(
			std::string variableName,
			parser::FunctionLookupTable* functionLookupTable,
			parser::ConstantLookupTable* constantLookupTable,
			parser::AstNode* ast);
		/* read the stream in the RPN notation and create*/
		Calculator(
			std::string variableName,
			parser::FunctionLookupTable* functionLookupTable,
			parser::ConstantLookupTable* constantLookupTable,
			std::istream& inputStream);
		virtual ~Calculator();
		/* Save current input as RPN in the stream*/
		void save(std::ostream& outputStream);
		double calculate(double varValue);
	};

	/*** Some basic functions ***/

	/* identity function */
	class FunctionIdentity : public parser::Function1Arg {
	public:
		virtual double eval(double in);
	};

	/* sin(x) */
	class FunctionSin : public parser::Function1Arg {
	public:
		virtual double eval(double in);
	};

	/* cos(x) */
	class FunctionCos : public parser::Function1Arg {
	public:
		virtual double eval(double in);
	};

	/* exp(x) */
	class FunctionExp : public parser::Function1Arg {
	public:
		virtual double eval(double in);
	};

	/* log(x) */
	class FunctionLog : public parser::Function1Arg {
	public:
		virtual double eval(double in);
	};

	/** standard constant's lookup table**/
	class StdConstantLookupTable : public parser::ConstantLookupTable {
	public:
		StdConstantLookupTable(); 
	};

	/** standard function's lookup table**/
	class StdFunctionLookupTable : public parser::FunctionLookupTable {
	public:
		StdFunctionLookupTable(); 
	};

}

#endif
