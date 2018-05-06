#include "stdafx.h"
#include "Calculator.h"
#include "Lexer.h"
#include "Parser.h"
#include <vector>
#include <stack>
#include <istream>
#include <ostream>
#include <cmath> //power
#include <string>

namespace calc {

	using namespace std;
	using namespace parser;

	/* encapsulate values needed when evaluating */
	class EvaluationContext {
	private: 
		/* current number of symbol processed (1-indexed)*/
		int symbolNo;
		/* stack used to evaluate according to RPN*/
		std::stack<double> outStack;
		/* (x) variable's value */
		double variableValue;
	public:
		EvaluationContext(double variableValue) 
			: symbolNo(1), variableValue(variableValue) {
				;
		}

		/* move forward by 1 symbol*/
		void inc() {
			++symbolNo;
		}

		double getVariableValue() {
			return variableValue;
		}

		/* put output of evaluation to the stack*/
		void pushOutput(double d) {
			outStack.push(d);
		}

		/* pop one value from the stack*/
		double popOutput() {
			if (outStack.empty()) {
				throw StatementException(symbolNo);
			}
			double el = outStack.top();
			outStack.pop();
			return el;
		}

		/* It is called only after evaluation ends; returns theresult of evaluation*/
		double getResult() {
			if (outStack.size() != 1) {
				throw StatementException(symbolNo);
			}
			return outStack.top();
		}
	};

	/* Element of a stack created to represent
	Reverse Polish Notation.
	Each subclass represents specialized element type. I chose such
	approach, because I want to process inputs in a polymorphic way
	and avoid having if(dynamic_casts<>)-like statements everywhere*/
	class RPNElement {
	public:
		RPNElement() {;}
		/* evaluate this operation */
		virtual void evaluate(EvaluationContext& ctx) = 0;
		/* save to stream */
		virtual void toStream(ostream& o) = 0;
	};

	/* A literal floating-point value. No operands */
	class RPNValueElement : public RPNElement {
	private:
		double value;
	public:
		RPNValueElement(double value) 
			: RPNElement(),
			value(value) {
				;
		}

		virtual void evaluate(EvaluationContext& ctx) {
			//input floating-point values are just stacked in the output
			ctx.pushOutput(value);
		}

		virtual void toStream(ostream& o) {
			o << value;
		}
	};

	class RPNFunction1ArgElement : public RPNElement {
	private:
		string name;
		Function1Arg* func;
	public:
		RPNFunction1ArgElement(string name, Function1Arg* func) 
			: name(name), func(func) {;}

		virtual void evaluate(EvaluationContext& ctx) {
			//1. pop function arg
			//2. execute operation(operand1)
			//3. push back the result
			double arg1 = ctx.popOutput();
			ctx.pushOutput(func->eval(arg1));
		}

		virtual void toStream(ostream& o) {
			o << name;
		}
	};

	/* Unary operator */
	class RPNUnaryOperatorElement : public RPNElement  {
	public:
		RPNUnaryOperatorElement() : RPNElement() {
		}
		virtual void evaluate(EvaluationContext& ctx) {
			//unary operation:
			//1. pop operand1
			//2. execute operation(operand1)
			//3. push back the result
			double operand = ctx.popOutput();
			ctx.pushOutput(operation(operand));
		}

		/* GoF template method; inheriting classes implement just
		the pure operation */
		virtual double operation(double operand) = 0;
	};

	class RPNUnaryNegationElement : public RPNUnaryOperatorElement {
	public:
		RPNUnaryNegationElement() :RPNUnaryOperatorElement(){
			;
		}

		virtual double operation(double operand) {
			return -operand;
		}

		virtual void toStream(ostream& o) {
			o << '~';
		}
	};

	/* Binary operator */
	class RPNBinaryOperatorElement : public RPNElement  {
	public:
		virtual void evaluate(EvaluationContext& ctx) {
			//binary operation:
			//1. pop operand2
			//2. pop operand1
			//3. execute operation(operand1, operand2)
			//4. push back the result
			double operand2 = ctx.popOutput();
			double operand1 = ctx.popOutput();
			ctx.pushOutput(operation(operand1, operand2));
		}
		/* GoF template method; inheriting classes implement just
		the pure operation */
		virtual double operation(double operand1, double operand2) = 0;
	};

	class RPNPlusElement : public RPNBinaryOperatorElement {
	public:
		RPNPlusElement() {;}
		virtual double operation(double operand1, double operand2) {
			return operand1+operand2;
		}

		virtual void toStream(ostream& o) {
			o << '+';
		}
	};

	class RPNMinusElement : public RPNBinaryOperatorElement {
	public:
		RPNMinusElement() {;}
		virtual double operation(double operand1, double operand2) {
			return operand1-operand2;
		}

		virtual void toStream(ostream& o) {
			o << '-';
		}
	};

	class RPNMulElement : public RPNBinaryOperatorElement {
	public:
		RPNMulElement() {;}
		virtual double operation(double operand1, double operand2) {
			return operand1*operand2;
		}

		virtual void toStream(ostream& o) {
			o << '*';
		}
	};

	class RPNDivElement : public RPNBinaryOperatorElement {
	public:
		RPNDivElement() {;}
		virtual double operation(double operand1, double operand2) {
			return operand1/operand2;
		}

		virtual void toStream(ostream& o) {
			o << '/';
		}
	};

	class RPNPowElement : public RPNBinaryOperatorElement {
	public:
		RPNPowElement() {;}
		virtual double operation(double operand1, double operand2) {
			return pow(operand1, operand2);
		}

		virtual void toStream(ostream& o) {
			o << '^';
		}
	};

	/* A variable which may be evaluated */
	class RPNVariableElement : public RPNElement {
	private:
		string varName;
	public:
		RPNVariableElement(string varName)
			: varName(varName) {
				;
		}

		virtual void evaluate(EvaluationContext& ctx) {
			//input variable - evaluate variable's value and stack in the output
			ctx.pushOutput(ctx.getVariableValue());
		}

		virtual void toStream(ostream& o) {
			o << varName;
		}
	};


	/* Translate input from Lexer into a series RPNElements.*/
	class Lexem2SymbolVisitor : public LexemVisitor {
	private:
		/* context value */
		string variableName;
		/* context value */
		parser::FunctionLookupTable* functionLookupTable;
		/* context value */
		parser::ConstantLookupTable* constantLookupTable;
		/* count symbols */
		int symbolCounter;
		/* result */
		vector<RPNElement*> rpnSymbols;
	public:
		Lexem2SymbolVisitor(			
			string variableName,
			parser::FunctionLookupTable* functionLookupTable,
			parser::ConstantLookupTable* constantLookupTable) 
			: 
		variableName(variableName),
			functionLookupTable(functionLookupTable),
			constantLookupTable(constantLookupTable),	
			symbolCounter(1) {
				;
		}

		vector<RPNElement*> getSymbols() {
			return rpnSymbols;
		}

		virtual void visit(MinusLexem& minusLexem) {
			rpnSymbols.push_back(new RPNMinusElement());
		}

		virtual void visit(PlusLexem& plusLexem) {
			rpnSymbols.push_back(new RPNPlusElement());
		}

		virtual void visit(MulLexem& mulLexem) {
			rpnSymbols.push_back(new RPNMulElement());
		}

		virtual void visit(DivLexem& divLexem) {
			rpnSymbols.push_back(new RPNDivElement());
		}

		virtual void visit(CParenLexem& cParenLexem) {
			throw StatementException("symbol not supported for RPN");
		}

		virtual void visit(OParenLexem& oParenLexem) {
			throw StatementException("symbol not supported for RPN");
		}

		virtual void visit(TildeLexem& tildeLexem) {
			rpnSymbols.push_back(new RPNUnaryNegationElement());
		}

		/* ~ 'tilde' is used to represent the unary negation */
		virtual void visit(FloatLexem& floatLexem) {
			rpnSymbols.push_back(new RPNValueElement(floatLexem.getValue()));
		}


		virtual void visit(DashLexem& floatLexem) {
			rpnSymbols.push_back(new RPNPowElement());
		}

		virtual void visit(IdentifierLexem& identifierLexem) {
			string id = identifierLexem.toString();
			if (id == variableName) {
				//the identifier represents simply the variable 
				rpnSymbols.push_back(new RPNVariableElement(variableName));
			} else {
				RPNElement* el = NULL;
				if (functionLookupTable != NULL
					&& functionLookupTable->exists(id)) {
						//if identifier corresponds to a function name
						el = new RPNFunction1ArgElement(
							id,
							functionLookupTable->lookup(id));
				}
				if (el == NULL && constantLookupTable != NULL
					&& constantLookupTable->exists(id)) {
						//constant are at this stage translated to numerical values
						el = new RPNValueElement(constantLookupTable->lookup(id));
				}
				if (el != NULL) {
					rpnSymbols.push_back(el);
				} else {
					throw StatementException("illegal symbol. not a function, constant or variable");
					//throw "illegal symbol. not a function, constant or variable";
				}
			} 
		}
	};

	/* Translate AST tree into a series of RPNElements (each element
	represents one symbol in the Reverse Polish Notation)*/
	class Ast2RPNVisitor : public AstVisitor {
	private:
		/* context value */
		string variableName;
		/* context value */
		parser::FunctionLookupTable* functionLookupTable;
		/* context value */
		parser::ConstantLookupTable* constantLookupTable;
		/* count symbols */
		int symbolCounter;
		/* result */
		vector<RPNElement*> rpnSymbols;
	public:
		Ast2RPNVisitor(			
			string variableName,
			parser::FunctionLookupTable* functionLookupTable,
			parser::ConstantLookupTable* constantLookupTable) 
			: 
		variableName(variableName),
			functionLookupTable(functionLookupTable),
			constantLookupTable(constantLookupTable),	
			symbolCounter(1) {
				;
		}

		vector<RPNElement*> getSymbols() {
			return rpnSymbols;
		}

		virtual void visit(FloatLiteralAstNode& floatLiteralNode) {
			rpnSymbols.push_back(new RPNValueElement(floatLiteralNode.getLiteralValue()));
		}

		virtual void visit(VariableAstNode& variableNode) {
			if (variableNode.getVarIdentifier() != variableName) {
				throw StatementException(symbolCounter, string("unknown variable name"));
			}
			rpnSymbols.push_back(new RPNVariableElement(variableName));
		}

		virtual void visit(UnaryNegationAstNode& unaryNegationNode) {
			rpnSymbols.push_back(new RPNUnaryNegationElement());
		}

		virtual void visit(AddOperatorAstNode& addOperatorNode) {
			rpnSymbols.push_back(new RPNPlusElement());
		}

		virtual void visit(SubOperatorAstNode& subOperatorNode) {
			rpnSymbols.push_back(new RPNMinusElement());
		}

		virtual void visit(MulOperatorAstNode& mulOperatorNode) {
			rpnSymbols.push_back(new RPNMulElement());
		}

		virtual void visit(DivOperatorAstNode& divOperatorNode) {
			rpnSymbols.push_back(new RPNDivElement());
		}

		virtual void visit(PowerOperatorAstNode& powerOperatorNode) {
			rpnSymbols.push_back(new RPNPowElement());
		}

		virtual void visit(FunctionCall1ArgAstNode& funcCallNode) {
			string funcName = funcCallNode.getFunctionName();
			if (functionLookupTable->exists(funcName)) {
				//check if function is defined
				Function1Arg* func = functionLookupTable->lookup(funcName);
				rpnSymbols.push_back(new RPNFunction1ArgElement(funcName, func));
			} else {
				throw StatementException(symbolCounter, string("unknown function " + funcName));
			}
		}

		virtual void visit(ConstantAstNode& constantNode) {
			rpnSymbols.push_back(new RPNValueElement(constantNode.getValue()));
		}
	};

	Calculator::Calculator(
		string variableName,
		FunctionLookupTable* functionLookupTable,
		ConstantLookupTable* constantLookupTable,
		istream& inputStream)
		:
	variableName(variableName),
		functionLookupTable(functionLookupTable),
		constantLookupTable(constantLookupTable) {

			constructFromStream(inputStream);
	}

	Calculator::Calculator(
		string variableName,
		FunctionLookupTable* functionLookupTable,
		ConstantLookupTable* constantLookupTable,
		parser::AstNode* ast) 
		:
	variableName(variableName),
		functionLookupTable(functionLookupTable),
		constantLookupTable(constantLookupTable) {

			Ast2RPNVisitor visitor(variableName, 
				functionLookupTable, 
				constantLookupTable);

			ast->visitPostOrder(visitor);

			input = visitor.getSymbols();
	}

	Calculator::~Calculator() {
		for (auto it = input.begin(); it != input.end(); ++it) {
			delete (*it);
		}
	}

	void Calculator::constructFromStream(std::istream& inputStream) {
		Lexer lexer;
		auto_ptr<Lexem> lexem;
		Lexem2SymbolVisitor l2sVisitor(
			this->variableName,
			this->functionLookupTable,
			this->constantLookupTable);

		int symbolNo = 1;
		//transform symbols into reverse-polish-notation objects
		while ((lexem = lexer.next(inputStream)).get() != NULL) {
			try {
				lexem->accept(l2sVisitor);
				symbolNo++;
			} catch (StatementException& e) {
				throw StatementException(symbolNo, e.whatStr());
			}
		}

		//object-oriented representation of the chain of symbols
		//in Reverse Polish Notation
		//in the order left->right
		this->input = l2sVisitor.getSymbols();


	}

	void Calculator::save(std::ostream& outputStream) {
		int i = 0;
		for (auto it = input.begin(); it != input.end(); ++it, ++i) {
			//for each symbol, output to stream and separate using spaces
			if (i > 0) {
				outputStream << " ";
			}
			(*it)->toStream(outputStream);
		}
	}


	double Calculator::calculate(double varValue) {
		if (input.empty()) {
			return 0.0;
		}
		EvaluationContext ctx(varValue);
		/* for each symbol, call evaluation method, which is polymorphically
		executed on each RPN-element (symbol) in a different manner*/
		for (auto it = input.begin(); it != input.end(); ++it) {
			(*it)->evaluate(ctx);
			ctx.inc();
		}
		return ctx.getResult();
	}


	
	/*** Some basic functions ***/

	double FunctionIdentity::eval(double in) {
		return in;
	}

	double FunctionSin::eval(double in) {
		return sin(in);
	}

	double FunctionCos::eval(double in) {
		return cos(in);
	}

	double FunctionExp::eval(double in) {
		return exp(in);
	}

	double FunctionLog::eval(double in) {
		return log(in);
	}

	/*** Standard lookup tables ***/

	StdConstantLookupTable::StdConstantLookupTable() 
	: ConstantLookupTable() {
		add(string("ONE"), 1.0f);
		add(string("ZERO"), 0.0f);
		add(string("PI"), 3.1415926535897932384626433832795f);
		add(string("E"), 2.7182818284590452353602874713527f);
	}

	StdFunctionLookupTable::StdFunctionLookupTable()
		: FunctionLookupTable() {
			add(string("sin"), new FunctionSin());
			add(string("cos"), new FunctionCos());
			add(string("exp"), new FunctionExp());
			add(string("log"), new FunctionLog());
	}

	/*** End of Some basic functions ***/

	StatementException::StatementException(string s) {
		s = "Invalid statement: " + s; 
	}

	StatementException::StatementException(int symbolNo) {
		stringstream sb;
		sb << "Invalid statement at symbol no " << symbolNo; 
		this->s = sb.str();
	}

	StatementException::StatementException(int symbolNo, string s) {
		stringstream sb;
		sb << "Invalid statement at symbol no " << symbolNo 
			<< " " << s;
		this->s = sb.str();
	}

	const char *StatementException::what() const {
		return s.c_str();
	}

	std::string StatementException::whatStr() {
		return s;
	}
}

