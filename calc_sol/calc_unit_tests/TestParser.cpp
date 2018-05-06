#include "stdafx.h"
#include "TestParser.h"
#include "CAssert.h"
#include "CUnit.h"
#include "..\calc_parser\Parser.h"
#include <sstream>

using namespace std;
using namespace cunit;
using namespace parser;

namespace parser_tests{
	using namespace std;

	Parser* parser = NULL;
	stringstream* pt_s = NULL;
	RPNTextVisitor* pt_visitor = NULL;
	AstNode* pt_ast = NULL;
	FunctionLookupTable* pt_flookup = NULL;
	ConstantLookupTable* pt_clookup = NULL;

	void pt_setup() {
		pt_s = new stringstream();
		pt_flookup = new FunctionLookupTable();
		pt_clookup = new ConstantLookupTable();
		parser = new Parser(*pt_s, pt_clookup, pt_flookup);
		pt_visitor = new RPNTextVisitor();
		pt_ast = NULL;
	}

	void pt_cleanup() {
		delete parser;
		delete pt_s;
		delete pt_visitor;
		delete pt_flookup;
		delete pt_clookup;
		if (pt_ast != NULL) {
			delete pt_ast;
			pt_ast = NULL;
		}
		parser = NULL;
		pt_s = NULL;
		pt_visitor = NULL;
	}

		/*
	void testMulExpr1() {
		*pt_s << "1*2";
		parser->begin();
		AstNode* pt_ast = parser->factor();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("1.0 2.0 *"), pt_visitor->getRPNText());
	}
	*/

	void pt_testFactorFloat() {
		*pt_s << "1";
		parser->begin();
		pt_ast = parser->factor();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("1"), pt_visitor->getRPNText());
	}

	void pt_testFactorParenthesis1() {
		*pt_s << "(1)";
		parser->begin();
		pt_ast = parser->factor();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("1"), pt_visitor->getRPNText());
	}

//	void pt_testFactorParenthesis2() {
//	}

	//void testFactorUnaryNegation1() {
	//}

	void pt_testAddExpr1() {
		*pt_s << "1+2";
		parser->begin();
		AstNode* pt_ast = parser->addExpr();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("1 2 +"), pt_visitor->getRPNText());
	}

	void pt_testAddExpr2() {
		*pt_s << "1-2";
		parser->begin();
		AstNode* pt_ast = parser->addExpr();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("1 2 -"), pt_visitor->getRPNText());
	}

	void pt_testAddExpr3() {
		*pt_s << "1+2-3";
		parser->begin();
		AstNode* pt_ast = parser->addExpr();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("1 2 + 3 -"), pt_visitor->getRPNText());
	}

	void pt_testAddExpr4Brackets() {
		*pt_s << "1+(2-3)";
		parser->begin();
		AstNode* pt_ast = parser->addExpr();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("1 2 3 - +"), pt_visitor->getRPNText());
	}

	void pt_testAddExpr4AndMul() {
		*pt_s << "1+2*3";
		parser->begin();
		AstNode* pt_ast = parser->addExpr();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("1 2 3 * +"), pt_visitor->getRPNText());
	}

	void pt_testMulExpr1() {
		*pt_s << "1*2";
		parser->begin();
		AstNode* pt_ast = parser->mulExpr();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("1 2 *"), pt_visitor->getRPNText());
	}

	void pt_testMulExpr2() {
		*pt_s << "1/2";
		parser->begin();
		AstNode* pt_ast = parser->mulExpr();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("1 2 /"), pt_visitor->getRPNText());
	}

	void pt_testMulExpr3() {
		*pt_s << "1/2*3";
		parser->begin();
		AstNode* pt_ast = parser->mulExpr();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("1 2 / 3 *"), pt_visitor->getRPNText());
	}

	void pt_testPower() {
		*pt_s << "1^2";
		parser->begin();
		AstNode* pt_ast = parser->powExpr();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("1 2 ^"), pt_visitor->getRPNText());
	}

	void pt_testIdentifier1() {
		*pt_s << "a";
		parser->begin();
		AstNode* pt_ast = parser->addExpr();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("a"), pt_visitor->getRPNText());
	}

	void pt_testIdentifier2() {
		*pt_s << "1+abc+2";
		parser->begin();
		AstNode* pt_ast = parser->addExpr();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("1 abc + 2 +"), pt_visitor->getRPNText());
	}

	class pt_DummyFunction : public Function1Arg {
	public:
		virtual double eval(double in) {
			return in;
		}
	};

	void pt_testFunction1Arg_1() {
		pt_flookup->add(string("a"), new pt_DummyFunction());
		*pt_s << "a(b)";
		parser->begin();
		AstNode* pt_ast = parser->funcCall();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("b a"), pt_visitor->getRPNText());
	}

	void pt_testFunction1Arg_2() {
		pt_flookup->add(string("a"), new pt_DummyFunction());
		*pt_s << "a(b+1)";
		parser->begin();
		AstNode* pt_ast = parser->funcCall();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("b 1 + a"), pt_visitor->getRPNText());
	}

	void pt_testConstant1() {
		pt_clookup->add(string("c"), 1.0f);
		*pt_s << "c";
		parser->begin();
		AstNode* pt_ast = parser->variable();
		pt_ast->visitPostOrder(*pt_visitor);
		CAssert::assertEquals(string("c"), pt_visitor->getRPNText());
		CAssert::assertNotNull( dynamic_cast<ConstantAstNode*>(pt_ast) );
	}

	/*void pt_test() {
		*pt_s << "x^";
		parser->begin();
		AstNode* pt_ast = parser->expr();
		pt_ast->visitPostOrder(*pt_visitor);
		
	}*/

	auto_ptr<cunit::TestCase> parserTestCase() {
		auto_ptr<TestCase> tc = auto_ptr<TestCase>(new TestCase(string("ParserTestCase"), pt_setup, pt_cleanup));
		tc->addTest("pt_testFactorFloat", pt_testFactorFloat);
		tc->addTest("pt_testFactorParenthesis1", pt_testFactorParenthesis1);
		//tc->addTest("pt_testFactorParenthesis2", pt_testFactorParenthesis2);
		//tc->addTest("pt_testFactorUnaryNegation1", pt_testFactorUnaryNegation1);
		tc->addTest("pt_testAddExpr1", pt_testAddExpr1);
		tc->addTest("pt_testAddExpr2", pt_testAddExpr2);
		tc->addTest("pt_testAddExpr3", pt_testAddExpr3);
		tc->addTest("pt_testAddExpr4Brackets", pt_testAddExpr4Brackets);
		tc->addTest("pt_testAddExpr4AndMul", pt_testAddExpr4AndMul);
		tc->addTest("pt_testMulExpr1", pt_testMulExpr1);
		tc->addTest("pt_testMulExpr2", pt_testMulExpr2);
		tc->addTest("pt_testMulExpr3", pt_testMulExpr3);
		tc->addTest("pt_testPower", pt_testPower);
		tc->addTest("pt_testIdentifier1", pt_testIdentifier1);
		tc->addTest("pt_testIdentifier2", pt_testIdentifier2);
		tc->addTest("pt_testFunction1Arg_1", pt_testFunction1Arg_1);
		tc->addTest("pt_testFunction1Arg_2", pt_testFunction1Arg_2);
		tc->addTest("pt_testConstant1", pt_testConstant1);
	//	tc->addTest("pt_test", pt_test);
		return tc;
	}

}