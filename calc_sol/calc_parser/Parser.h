#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include <string>
#include <memory>
#include <vector>
#include <map>


namespace parser {

	using namespace parser;

	/* forward declaration */
	class AstNode;

	/* forward declaration */
	class VariableAstNode;

	/* forward declaration */
	class ConstantLookupTable;

	/* forward declaration */
	class FunctionLookupTable;

	/* Exception class used by parser
	to report syntax errors */
	class SyntaxException : public std::exception {
	private:
		int lineNo;
		int charNo;
		std::string s;
	public:
		SyntaxException(int lineNo, int charNo);
		SyntaxException(int lineNo, int charNo, std::string text);
		virtual const char *what() const;
	};

	/* Parser to parse texts according to the grammar

	expr                      ::= add_expr
	add_expr                  ::= mul_expr { ( Plus | Minus ) mul_expr }
	mul_expr                  ::= pow_expr { ( Mul | Div ) pow_expr }
	pow_expr                  ::= factor { Dash factor }
	factor                    ::= [ Minus ] (
	Float
	| func_call_expr
	| OParen expr CParen )
	//only 1-arg functions allowed
	func_call_expr            ::= variable [ OParen expr CParen ] 
	variable				  ::= Identifier

	See the "Lexer" class for tokens.

	Every method of this class represents one grammar rule. Such
	method i parsing according to that rule (and dependent rules).

	Methods are creating abstract syntax tree. The tree must be 
	deallocated by the client. Deallocation occurs recursively
	automatically
	*/
	class Parser {

	private:
		/* underlying lexer to tokenize input stream*/
		Lexer lexer;

		/* the stream - source of program */
		std::istream& inputStream;

		/* currently read lexem (token)*/
		std::auto_ptr<Lexem> currentLexem;

		/* advance automata by 1 token */
		void readNextSymbol();

		/* Accept optional symbol.
		Accept lexem of type indicated by LexemType (one of Lexem subclasses).
		If current state of automata doesn't allow to accept that symbol,
		return NULL auto_ptr, otherwise return auto_ptr for accepted symbol*/
		template <class LexemType>
		std::auto_ptr<LexemType> accept();

		/* Accept mandatory symbol.
		Accept lexem of type indicated by LexemType (one of Lexem subclasses).
		If current state of automata doesn't allow to accept that symbol,
		throw SyntaxException auto_ptr, otherwise return auto_ptr for accepted symbol*/
		template <class LexemType>
		std::auto_ptr<LexemType> expect();

		/* Check current symbol.
		Check if automata is in state indicated by LexemType (one of Lexem subclasses).
		Returns: true if current state is in Lexem indicated by LexemType, otherwise return false*/
		template <class LexemType>
		bool symbol();

		/* a table to lookup constant by name */
		ConstantLookupTable* constantLookupTable;

		/* a table to lookup function by name */
		FunctionLookupTable* functionLookupTable;

	public:

		Parser(std::istream& inputStream)
			: inputStream(inputStream), constantLookupTable(NULL), functionLookupTable(NULL) {
		}

		Parser(std::istream& inputStream, 
			ConstantLookupTable* constantLookupTable, FunctionLookupTable* functionLookupTable)
			: inputStream(inputStream)
			, constantLookupTable(constantLookupTable)
			, functionLookupTable(functionLookupTable) {
		}

		Parser& begin();

		/* parse expression */
		AstNode* expr();

		/* parse additive expression */
		AstNode* addExpr();

		/* parse multiplicative expression */
		AstNode* mulExpr();

		/* parse power expression */
		AstNode* powExpr();

		/* parse basic factor */
		AstNode* factor();

		/* parse function call*/
		AstNode* funcCall();

		/* parse variable */
		VariableAstNode* variable();

	};

	/* forward declaration */
	class AstVisitor;

	/* Abstract Syntaxt Tree - base class
	Used to represent parsed program.
	Structures of this type implement GoF composite
	design pattern. The visitor pattern is applied
	to traverse the composite*/
	class AstNode {
	public:
		/* Traverse sub-tree rooted in this node.
		Traversal is 'post-order': 
		1. Visit all sub-trees 
		2. Visit this node
		*/
		virtual void visitPostOrder(AstVisitor& visitor) = 0;
	};

	/* Abstract representation of literal */
	template <class T>
	class LiteralAstNode : public AstNode {
	private:
		//literal value
		T value;
	protected:
		LiteralAstNode(T t) : value(t) {}
	public:
		T getLiteralValue() {
			return value;
		}
	};

	/* floating-point literals */
	class FloatLiteralAstNode : public LiteralAstNode<double> {
	public: 
		FloatLiteralAstNode(double d) 
			: LiteralAstNode(d) {
		}
		virtual void visitPostOrder(AstVisitor& visitor);	
	};

	/* Node representing variable in calculation*/
	class VariableAstNode : public AstNode {
	private:
		//variable name
		std::string varName;
	public:
		VariableAstNode(std::string varName) : varName(varName) {}

		std::string getVarIdentifier() {
			return varName;
		}

		virtual void visitPostOrder(AstVisitor& visitor);
	};

	/* Abstraction for unary operators */
	class UnaryOperatorAstNode : public AstNode {
	private:
		//operator's symbol
		std::string symbol;
		//argument of the operator - expression
		AstNode* exprAst;
	protected:
		UnaryOperatorAstNode(std::string symbol, AstNode* exprAst)
			:symbol(symbol), exprAst(exprAst) {
		}

		virtual ~UnaryOperatorAstNode() {
			delete exprAst;
		}

		AstNode* getArgument() {
			return exprAst;
		}
	public:
		std::string getSymbol() {
			return symbol;
		}
	};

	/* unary negation '- expr '*/
	class UnaryNegationAstNode : public UnaryOperatorAstNode {
	public:
		UnaryNegationAstNode(AstNode* exprAst)
			: UnaryOperatorAstNode(std::string("-"), exprAst) {
				;
		}

		virtual void visitPostOrder(AstVisitor& visitor);
	};

	/* abstraction of binary operators */
	class BinaryOperatorAstNode : public AstNode {
	private:
		//operator's symbol
		std::string symbol;
		//left argument of the operator - expression
		AstNode* leftAst;
		//right argument of the operator - expression
		AstNode* rightAst;
	public:
		BinaryOperatorAstNode(
			std::string symbol,
			AstNode* leftAst,
			AstNode* rightAst) 
			: symbol(symbol), leftAst(leftAst), rightAst(rightAst) {
		}

		virtual ~BinaryOperatorAstNode() {
			delete leftAst;
			delete rightAst;
		}

		std::string getSymbol() {
			return symbol;
		}

		AstNode* getLeft() {
			return leftAst;
		}

		AstNode* getRight() {
			return rightAst;
		}
	};

	/* binary operator: addition */
	class AddOperatorAstNode : public BinaryOperatorAstNode {
	public:
		AddOperatorAstNode(
			AstNode* leftAst,
			AstNode* rightAst) 
			: BinaryOperatorAstNode(std::string("+"), leftAst, rightAst) {
		}

		virtual void visitPostOrder(AstVisitor& visitor);
	};

	/* binary operator: subtraction */
	class SubOperatorAstNode : public BinaryOperatorAstNode {
	public:
		SubOperatorAstNode(
			AstNode* leftAst,
			AstNode* rightAst) 
			: BinaryOperatorAstNode(std::string("-"), leftAst, rightAst) {
		}

		virtual void visitPostOrder(AstVisitor& visitor);
	};

	/* binary operator: multiplication */
	class MulOperatorAstNode : public BinaryOperatorAstNode {
	public:
		MulOperatorAstNode(
			AstNode* leftAst,
			AstNode* rightAst) 
			: BinaryOperatorAstNode(std::string("*"), leftAst, rightAst) {
		}

		virtual void visitPostOrder(AstVisitor& visitor);
	};

	/* binary operator: division */
	class DivOperatorAstNode : public BinaryOperatorAstNode {
	public:
		DivOperatorAstNode(
			AstNode* leftAst,
			AstNode* rightAst) 
			: BinaryOperatorAstNode(std::string("/"), leftAst, rightAst) {
		}


		virtual void visitPostOrder(AstVisitor& visitor);
	};

	/* binary operator: power */
	class PowerOperatorAstNode : public BinaryOperatorAstNode {
	public:
		PowerOperatorAstNode(
			AstNode* leftAst,
			AstNode* rightAst) 
			: BinaryOperatorAstNode(std::string("^"), leftAst, rightAst) {
		}

		virtual void visitPostOrder(AstVisitor& visitor);

	};

	/* function evaluator for 1-arg functions*/
	class Function1Arg {
	public:
		/* evaluate function's value */
		virtual double eval(double in) = 0;
	};

	/* function call - 1 arg */
	class FunctionCall1ArgAstNode : public AstNode {
	private:
		/* function name*/
		std::string funcName;
		/* argument of the function*/
		AstNode* arg1;
		/* function value evaluator - result of function lookup */
		Function1Arg* func;
	public:
		FunctionCall1ArgAstNode(std::string funcName, Function1Arg* func, AstNode* arg1) 
			: funcName(funcName), arg1(arg1), func(func) {
				;
		}
		virtual ~FunctionCall1ArgAstNode() {
			//function is not deleted - it belongs to the lookup table
			delete arg1;
		}

		virtual void visitPostOrder(AstVisitor& visitor);

		AstNode* getArg1() {
			return arg1;
		}

		std::string getFunctionName() {
			return funcName;
		}
	};

	/* constant value */
	class ConstantAstNode : public VariableAstNode {
	private:
		/* constant's value - result of lookup*/
		double value;
	public:
		ConstantAstNode(std::string constName, double value)
			: VariableAstNode(constName), value(value) {
				;
		}

		double getValue() {
			return value;
		}

		virtual void visitPostOrder(AstVisitor& visitor);
	};

	/* Generic implementation of symbol lookup table.
	Used for constant and funcion identifier lookup*/
	template <class T>
	class LookupTable {
	protected:
		std::map<std::string, T> table;
	protected:
		LookupTable() {
			;
		}

	public:
		void add(std::string key, T element) {
			if (exists(key)) {
				throw "illegal state";
			}
			table[key] = element;
		}

		bool exists(std::string key) {
			return table.count(key) == 1;
		}

		T lookup(std::string key) {
			if (exists(key)) {
				return table[key];
			} else {
				throw "illegal state";
			}
		}
	};

	/* lookup table for constants */
	class ConstantLookupTable : public LookupTable<double> {
	public:
		ConstantLookupTable() : LookupTable() {
			;
		}
	};

	/* Lookup table for functions 
	This table takes ownership of contained elements.
	It dealocates all functions at destruction time*/
	class FunctionLookupTable : public LookupTable<Function1Arg*> {
	public:
		FunctionLookupTable() : LookupTable() {
			;
		}
		~FunctionLookupTable() {
			for (std::map<std::string,Function1Arg*>::iterator it = table.begin(); it != table.end(); ++it) {
				delete it->second;
			}
		}

	};

	/* GoF Visitor Design pattern:
	The visitor class - a callback when traversing
	tree of NodeAst (and subclasses)*/
	class AstVisitor {
	public:
		virtual void visit(FloatLiteralAstNode& floatLiteralNode) = 0;

		virtual void visit(VariableAstNode& variableNode) = 0;

		virtual void visit(UnaryNegationAstNode& unaryNegationNode) = 0;

		virtual void visit(AddOperatorAstNode& addOperatorNode) = 0;

		virtual void visit(SubOperatorAstNode& subOperatorNode) = 0;

		virtual void visit(MulOperatorAstNode& mulOperatorNode) = 0;

		virtual void visit(DivOperatorAstNode& divOperatorNode) = 0;

		virtual void visit(PowerOperatorAstNode& powerOperatorNode) = 0;

		virtual void visit(FunctionCall1ArgAstNode& funcCallNode) = 0;

		virtual void visit(ConstantAstNode& constantNode) = 0;
	};

	/* helper visitor to transform AST into textual representation
	if reverse polish notation */
	class RPNTextVisitor : public AstVisitor {
	private:
		int tokens;
		std::stringstream b;
		/* add space between tokens */
		void space();
	public:
		RPNTextVisitor() : tokens(0) {
			;
		}
		/* return result - AST tree in reverse polish notation*/
		std::string getRPNText();

		virtual void visit(FloatLiteralAstNode& floatLiteralNode);

		virtual void visit(VariableAstNode& variableNode);

		virtual void visit(UnaryNegationAstNode& unaryNegationNode);

		virtual void visit(AddOperatorAstNode& addOperatorNode);

		virtual void visit(SubOperatorAstNode& subOperatorNode);

		virtual void visit(MulOperatorAstNode& mulOperatorNode);

		virtual void visit(DivOperatorAstNode& divOperatorNode);

		virtual void visit(PowerOperatorAstNode& powerOperatorNode);

		virtual void visit(FunctionCall1ArgAstNode& funcCallNode);

		virtual void visit(ConstantAstNode& constantNode);
	};

}

#endif
