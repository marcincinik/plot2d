
#include "stdafx.h"
#include "Lexer.h"
#include "Parser.h"
#include <memory>

namespace parser {

	using namespace std;

	/*** Parser ************************************/

	Parser& Parser::begin() {
		readNextSymbol();
		return *this;
	}

	void Parser::readNextSymbol() {
		currentLexem = lexer.next(inputStream);
	}


	/*
	Technological note: 
	I could have similar methods per each
	lexem type, but they would look exactly the same.
	To resolve that problem more polymorphic - not really
	usefull, because Lexems would all have a method to check their
	type and this is exactly the role of dynamic_cast*/
	template <class LexemType>
	auto_ptr<LexemType> Parser::accept() {
		if (currentLexem.get() != NULL 
			&& dynamic_cast<LexemType*>(currentLexem.get()) != NULL) {
				//downcast is save here, because condition
				//verified the right type
				//auto_ptr is required to handle ownership transfer properly
				LexemType* lexemPtr = dynamic_cast<LexemType*>(currentLexem.release());
				auto_ptr<LexemType> result = auto_ptr<LexemType>(lexemPtr);
				//advance automata state
				readNextSymbol();
				return result;

		} else {
			return auto_ptr<LexemType>();
		}
	}

	template <class LexemType>
	auto_ptr<LexemType> Parser::expect() {
		auto_ptr<LexemType> l = accept<LexemType>();
		if (l.get() != NULL) {
			return l;
		} else {
			throw SyntaxException(lexer.getLineNo(), lexer.getCharNo());
		}
	}

	template <class LexemType>
	bool Parser::symbol() {
		return currentLexem.get() != NULL 
			&& dynamic_cast<LexemType*>(currentLexem.get()) != NULL;
	}


	/* parse expression */
	AstNode* Parser::expr() {
		return addExpr();
	}

	/* parse additive expression '+' or '-' */
	AstNode* Parser::addExpr() {
		bool add = false;
		bool sub = false;
		//non-terminal
		AstNode* addExpressionSymbol = NULL;
		//left argument of the operator
		AstNode* left = mulExpr();
		while ((add = symbol<PlusLexem>()) || (sub = symbol<MinusLexem>())) {
			//advance automata
			readNextSymbol();
			//right argument of the operator
			AstNode* right = mulExpr();
			if (add) {
				addExpressionSymbol = new AddOperatorAstNode(left, right);
			} else if (sub) {
				addExpressionSymbol = new SubOperatorAstNode(left, right);
			} else {
				//should not occur
				throw "illegal state";
			}
			left = addExpressionSymbol;
		}
		if (addExpressionSymbol != NULL) {
			//additive symbol
			return addExpressionSymbol;
		} else {
			//not an additive symbol - recursive expansion
			return left;
		}
	}

	/* parse multiplicative expression '*' or '/'*/
	AstNode* Parser::mulExpr() {
		bool mul = false;
		bool div = false;
		//left argument of the operator
		AstNode* left = powExpr();
		//non-terminal
		AstNode* mulExpressionSymbol = NULL;
		while ((mul=symbol<MulLexem>()) || (div=symbol<DivLexem>())) {
			//advance automata
			readNextSymbol();
			//right argument of the operator
			AstNode* right = powExpr();
			if (mul) {
				mulExpressionSymbol = new MulOperatorAstNode(left, right);
			} else if (div) {
				mulExpressionSymbol = new DivOperatorAstNode(left, right);
			} else {
				//should not occur
				throw "illegal state";
			}
			left = mulExpressionSymbol;
		}
		if (mulExpressionSymbol != NULL) {
			//multiplicative expression
			return mulExpressionSymbol;
		} else {
			//not a multiplicative symbol - recursive expansion
			return left;
		}
	}

	/* parse power expression */
	AstNode* Parser::powExpr() {
		//left argument of 'power'
		AstNode* left = factor();
		AstNode* powerExprSymbol = NULL; //non-terminal
		//read next dash '^'
		while (symbol<DashLexem>()) {
			//advance automata
			readNextSymbol();
			//read right argument of 'power'
			AstNode* right = factor();
			//the power expressions becomes the left
			//side of the next power (if any)
			powerExprSymbol = new PowerOperatorAstNode(left, right);
			left = powerExprSymbol;
		}
		if (powerExprSymbol != NULL) {
			//power expression
			//one or more '^'
			return powerExprSymbol;
		} else {
			//not a power-expr - recursive expansion
			return left;
		}
	}

	/* parse basic factor */
	AstNode* Parser::factor() {
		AstNode* result = NULL;

		//optional: consume '-'
		auto_ptr<MinusLexem> minusLexem = accept<MinusLexem>();
		auto_ptr<FloatLexem> floatLexem;
		auto_ptr<OParenLexem> oParenLexem;
		if ((floatLexem = accept<FloatLexem>()).get() != NULL) {
			//consume Float
			double d = floatLexem.get()->getValue();
			result = new FloatLiteralAstNode(d);
		} else if (symbol<IdentifierLexem>()) {
			result = funcCall();
		} else if ((oParenLexem = accept<OParenLexem>()).get()) {
			//opening parenthesis - this is expression enclosed in parenthesis
			result = expr();
			//closing parenthesis is mandatory now
			auto_ptr<CParenLexem> cParenLexem = expect<CParenLexem>();
		} else {
			//No viable alternative form of the factor rule
			throw SyntaxException(lexer.getLineNo(), lexer.getCharNo());
		}

		if (minusLexem.get() != NULL) {
			//unary negation
			result = new UnaryNegationAstNode(result);
		}

		return result;
	}

	AstNode* Parser::funcCall() {
		//required function identifier
		VariableAstNode* var = variable();
		//non-terminal
		FunctionCall1ArgAstNode* funcCallSymbol = NULL;
		auto_ptr<OParenLexem> oParenLexem;
		if ( (oParenLexem = accept<OParenLexem>()).get() != NULL) {
			//only 1-arg functions allowed - argument is compulsory
			AstNode* arg1 = expr();
			//closing parenthesis is mandatory
			expect<CParenLexem>();

			std:string id = var->getVarIdentifier();
			//lookup function in the lookup table
			if (functionLookupTable != NULL 
				&& functionLookupTable->exists(id)) {

					Function1Arg* function = functionLookupTable->lookup(id);
					funcCallSymbol = new FunctionCall1ArgAstNode(
						id,
						function,
						arg1);
			} else {
				throw SyntaxException(lexer.getLineNo(), lexer.getCharNo(), 
					string("unknown function " + id));
			}

		}
		if (funcCallSymbol != NULL) {
			//function expression
			return funcCallSymbol;
		} else {
			//not a function-expr - recursive expansion
			return var;
		}
	}

	VariableAstNode* Parser::variable() {
		//required identifier
		auto_ptr<IdentifierLexem> identifierLexem = expect<IdentifierLexem>();
		string id = identifierLexem->getIdentifier();
		if (constantLookupTable != NULL 
			&& constantLookupTable->exists(id)) {

			//constant
			return new ConstantAstNode(
				id,	constantLookupTable->lookup(id));
		} else {
			return new VariableAstNode(identifierLexem->getIdentifier());
		}
	}

	/*** End of parser *****************************/


	/*** AST Node implementations ******************/
	void FloatLiteralAstNode::visitPostOrder(AstVisitor& visitor) {
		visitor.visit(*this);
	}

	void VariableAstNode::visitPostOrder(AstVisitor& visitor) {
		visitor.visit(*this);
	}

	void UnaryNegationAstNode::visitPostOrder(AstVisitor& visitor) {
		getArgument()->visitPostOrder(visitor);
		visitor.visit(*this);
	}

	void AddOperatorAstNode::visitPostOrder(AstVisitor& visitor) {
		getLeft()->visitPostOrder(visitor);
		getRight()->visitPostOrder(visitor);
		visitor.visit(*this);
	}

	void SubOperatorAstNode::visitPostOrder(AstVisitor& visitor) {
		getLeft()->visitPostOrder(visitor);
		getRight()->visitPostOrder(visitor);
		visitor.visit(*this);
	}

	void MulOperatorAstNode::visitPostOrder(AstVisitor& visitor) {
		getLeft()->visitPostOrder(visitor);
		getRight()->visitPostOrder(visitor);
		visitor.visit(*this);

	}

	void DivOperatorAstNode::visitPostOrder(AstVisitor& visitor) {
		getLeft()->visitPostOrder(visitor);
		getRight()->visitPostOrder(visitor);
		visitor.visit(*this);
	}

	void PowerOperatorAstNode::visitPostOrder(AstVisitor& visitor) {
		getLeft()->visitPostOrder(visitor);
		getRight()->visitPostOrder(visitor);
		visitor.visit(*this);
	}

	void FunctionCall1ArgAstNode::visitPostOrder(AstVisitor& visitor) {
		arg1->visitPostOrder(visitor);
		visitor.visit(*this);
	}

	void ConstantAstNode::visitPostOrder(AstVisitor& visitor) {
		visitor.visit(*this);
	}

	/*** End of AST Node implementations *************/

	/*** Exceptions ********************************/
	SyntaxException::SyntaxException(int lineNo, int charNo) 
		: lineNo(lineNo), charNo(charNo) {
			stringstream sb;
			sb << "Syntax error at " << lineNo << ":" << charNo;
			this->s = sb.str();
	}

	SyntaxException::SyntaxException(int lineNo, int charNo, string text) 
	: lineNo(lineNo), charNo(charNo){
			stringstream sb;
			sb << "Syntax error at " << lineNo << ":" << charNo
				<< " " << text;
			this->s = sb.str();
	}

	const char *SyntaxException::what() const {
		return s.c_str();
	}

	/*** End of Exceptions *************************/

	/*** RPNTextVisitor ****************************/
	string RPNTextVisitor::getRPNText() {
		return b.str();
	}

	void RPNTextVisitor::space() {
		if (tokens > 0) {
			b << " ";
		}
		tokens++;
	}

	void RPNTextVisitor::visit(FloatLiteralAstNode& floatLiteralNode) {
		space();
		b << floatLiteralNode.getLiteralValue();
	}

	void RPNTextVisitor::visit(VariableAstNode& variableNode){
		space();
		b << variableNode.getVarIdentifier();
	}

	void RPNTextVisitor::visit(UnaryNegationAstNode& unaryNegationNode){
		space();
		b << unaryNegationNode.getSymbol();
	}

	void RPNTextVisitor::visit(AddOperatorAstNode& addOperatorNode){
		space();
		b << addOperatorNode.getSymbol();
	}

	void RPNTextVisitor::visit(SubOperatorAstNode& subOperatorNode){
		space();
		b << subOperatorNode.getSymbol();
	}

	void RPNTextVisitor::visit(MulOperatorAstNode& mulOperatorNode){
		space();
		b << mulOperatorNode.getSymbol();
	}

	void RPNTextVisitor::visit(DivOperatorAstNode& divOperatorNode){
		space();
		b << divOperatorNode.getSymbol();
	}

	void RPNTextVisitor::visit(PowerOperatorAstNode& powerOperatorNode){
		space();
		b << powerOperatorNode.getSymbol();
	}

	void RPNTextVisitor::visit(FunctionCall1ArgAstNode& funcCallNode) {
		space();
		b << funcCallNode.getFunctionName();
	}

	void RPNTextVisitor::visit(ConstantAstNode& constantNode){ 
		space();
		b << constantNode.getVarIdentifier();
	}
	/*** End of RPNTextVisitor *********************/
}