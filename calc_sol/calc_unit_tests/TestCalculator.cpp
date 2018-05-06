#include "stdafx.h"

#include "TestCalculator.h"
#include "..\calc_parser\Calculator.h"

#include "CAssert.h"
#include "CUnit.h"
#include <vector>
#include <string>

using namespace std;
using namespace cunit;
using namespace parser;
using namespace calc;

namespace parser_tests {

	Calculator* calc;
	stringstream* ct_s = NULL;
	FunctionLookupTable* ct_ftl;
	ConstantLookupTable* ct_clt;
	Parser* ct_parser;
	AstNode* ct_ast = NULL;

	void ct_setup() {
		ct_ftl = new StdFunctionLookupTable();
		ct_clt = new StdConstantLookupTable();
		ct_s = new stringstream();
		ct_parser = new Parser(*ct_s, ct_clt, ct_ftl);
		ct_ast = NULL;
	}

	void ct_cleanup() {
		delete calc;
		delete ct_s;
		delete ct_ftl;
		delete ct_clt;
		delete ct_parser;
		if (ct_ast != NULL) {
			delete ct_ast;
		}
		calc = NULL;
		ct_s = NULL;
		ct_ftl = NULL;
		ct_clt = NULL;
		ct_parser = NULL;
		ct_ast = NULL;
	}

	void ct_testRPNPlus() {
		*ct_s << "1 2 +";
		calc = new Calculator(string("x"), ct_ftl, ct_clt, *ct_s);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(3.0f, result);
	}

	void ct_testASTPlus() {
		*ct_s << "1+2";
		ct_parser->begin();
		ct_ast = ct_parser->expr();
		calc = new Calculator(string("x"), ct_ftl, ct_clt, ct_ast);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(3.0f, result);
	}

	void ct_testRPNMinus() {
		*ct_s << "1 2 -";
		calc = new Calculator(string("x"), ct_ftl, ct_clt, *ct_s);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(-1.0f, result);
	}

	void ct_testASTMinus() {
		*ct_s << "1-2";
		ct_parser->begin();
		ct_ast = ct_parser->expr();
		calc = new Calculator(string("x"), ct_ftl, ct_clt, ct_ast);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(-1.0f, result);
	}

	void ct_testRPNUnaryNeg() {
		*ct_s << "1 ~";
		calc = new Calculator(string("x"), ct_ftl, ct_clt, *ct_s);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(-1.0f, result);
	}

	void ct_testASTUnaryNeg() {
		*ct_s << "-1";
		ct_parser->begin();
		ct_ast = ct_parser->expr();
		calc = new Calculator(string("x"), ct_ftl, ct_clt, ct_ast);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(-1.0f, result);
	}

	void ct_testRPNMul() {
		*ct_s << "2 3 *";
		calc = new Calculator(string("x"), ct_ftl, ct_clt, *ct_s);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(6.0f, result);
	}

	void ct_testASTMul() {
		*ct_s << "2*3";
		ct_parser->begin();
		ct_ast = ct_parser->expr();
		calc = new Calculator(string("x"), ct_ftl, ct_clt, ct_ast);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(6.0f, result);
	}

	void ct_testRPNDiv() {
		*ct_s << "6 3 /";
		calc = new Calculator(string("x"), ct_ftl, ct_clt, *ct_s);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(2.0f, result);
	}

	void ct_testASTDiv() {
		*ct_s << "6/3";
		ct_parser->begin();
		ct_ast = ct_parser->expr();
		calc = new Calculator(string("x"), ct_ftl, ct_clt, ct_ast);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(2.0f, result);
	}

	void ct_testRPNPow() {
		*ct_s << "2 3 ^";
		calc = new Calculator(string("x"), ct_ftl, ct_clt, *ct_s);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(8.0f, result);
	}

	void ct_testASTPow() {
		*ct_s << "2^3";
		ct_parser->begin();
		ct_ast = ct_parser->expr();
		calc = new Calculator(string("x"), ct_ftl, ct_clt, ct_ast);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(8.0f, result);
	}

	void ct_testRPNVariable() {
		*ct_s << "x";
		calc = new Calculator(string("x"), ct_ftl, ct_clt, *ct_s);
		double result = calc->calculate(1.0f);
		CAssert::assertEquals(1.0f, result);
	}

	void ct_testASTVariable() {
		*ct_s << "x";
		ct_parser->begin();
		ct_ast = ct_parser->expr();
		calc = new Calculator(string("x"), ct_ftl, ct_clt, ct_ast);
		double result = calc->calculate(1.0f);
		CAssert::assertEquals(1.0f, result);
	}

	void ct_testRPNConst() {
		ct_clt->add(string("c"), 1.0);
		*ct_s << "c";
		calc = new Calculator(string("x"), ct_ftl, ct_clt, *ct_s);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(1.0f, result);
	}

	void ct_testASTConst() {
		ct_clt->add(string("c"), 1.0);
		*ct_s << "c";
		ct_parser->begin();
		ct_ast = ct_parser->expr();
		calc = new Calculator(string("x"), ct_ftl, ct_clt, ct_ast);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(1.0f, result);
	}

	void ct_testRPNFunc() {
		ct_ftl->add(string("f"), new FunctionIdentity());
		*ct_s << "2.0 f";

		calc = new Calculator(string("x"), ct_ftl, ct_clt, *ct_s);
		double result = calc->calculate(1.0f);
		CAssert::assertEquals(2.0f, result);
	}

	void ct_testASTFunc() {

		ct_ftl->add(string("f"), new FunctionIdentity());
		*ct_s << "f(2.0)";
		ct_parser->begin();
		ct_ast = ct_parser->expr();
		calc = new Calculator(string("x"), ct_ftl, ct_clt, ct_ast);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(2.0f, result);
	}

	void ct_testRPNWiki() {
		*ct_s << "12 2 3 4 * 10 5 / + * +";
		calc = new Calculator(string("x"), ct_ftl, ct_clt, *ct_s);
		double result = calc->calculate(1.0f);
		CAssert::assertEquals(40.0f, result);
	}

	void ct_testASTWiki() {
		*ct_s << "((2+7)/3+(14-3)*4)/2";
		ct_parser->begin();
		ct_ast = ct_parser->expr();
		calc = new Calculator(string("x"), ct_ftl, ct_clt, ct_ast);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(23.5f, result);
	}

	void ct_testSaveLoad1() {
		stringstream s2;
		*ct_s << "1~ 2 +";
		calc = new Calculator(string("x"), ct_ftl, ct_clt, *ct_s);
		calc->save(s2);
		s2.flush();
		Calculator calc2(string("x"), ct_ftl, ct_clt, s2);
		double result = calc2.calculate(0.0f);
		CAssert::assertEquals(1.0f, result);
	}

	void ct_testSaveLoad2() {
		stringstream s2;
		*ct_s << "12 2 3 4 * 10 5 / + * +";
		calc = new Calculator(string("x"), ct_ftl, ct_clt, *ct_s);
		calc->save(s2);
		s2.flush();
		Calculator calc2(string("x"), ct_ftl, ct_clt, s2);
		double result = calc2.calculate(0.0f);
		CAssert::assertEquals(40.0f, result);
	}

	void ct_testASTSin() {
		*ct_s << "sin(x)";
		ct_parser->begin();
		ct_ast = ct_parser->expr();
		calc = new Calculator(string("x"), ct_ftl, ct_clt, ct_ast);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(0.0f, result);
	}

	/*void ct_test() {
		*ct_s << "y";
		ct_parser->begin();
		ct_ast = ct_parser->expr();
		calc = new Calculator(string("x"), ct_ftl, ct_clt, ct_ast);
		double result = calc->calculate(0.0f);
		CAssert::assertEquals(0.0f, result);
	}
	*/

	std::auto_ptr<cunit::TestCase> calculatorTestCase() {
		auto_ptr<TestCase> tc = auto_ptr<TestCase>(
			new TestCase(string("CalculatorTestCase"), 
			ct_setup, ct_cleanup));

		tc->addTest(string("ct_testRPNPlus"), ct_testRPNPlus);
		tc->addTest(string("ct_testASTPlus"), ct_testASTPlus);
		tc->addTest(string("ct_testRPNMinus"), ct_testRPNMinus);
		tc->addTest(string("ct_testASTMinus"), ct_testASTMinus);
		tc->addTest(string("ct_testRPNUnaryNeg"), ct_testRPNUnaryNeg);
		tc->addTest(string("ct_testASTUnaryNeg"), ct_testASTUnaryNeg);
		tc->addTest(string("ct_testRPNMul"), ct_testRPNMul);
		tc->addTest(string("ct_testASTMul"), ct_testASTMul);
		tc->addTest(string("ct_testRPNDiv"), ct_testRPNDiv);
		tc->addTest(string("ct_testASTDiv"), ct_testASTDiv);
		tc->addTest(string("ct_testRPNPow"), ct_testRPNPow);
		tc->addTest(string("ct_testASTPow"), ct_testASTPow);
		tc->addTest(string("ct_testRPNVariable"), ct_testRPNVariable);
		tc->addTest(string("ct_testASTVariable"), ct_testASTVariable);
		tc->addTest(string("ct_testRPNConst"), ct_testRPNConst);
		tc->addTest(string("ct_testASTConst"), ct_testASTConst);
		tc->addTest(string("ct_testRPNFunc"), ct_testRPNFunc);
		tc->addTest(string("ct_testASTFunc"), ct_testASTFunc);
		tc->addTest(string("ct_testRPNWiki"), ct_testRPNWiki);
		tc->addTest(string("ct_testASTWiki"), ct_testASTWiki);
		tc->addTest(string("ct_testSaveLoad1"), ct_testSaveLoad1);
		tc->addTest(string("ct_testSaveLoad2"), ct_testSaveLoad2);
		tc->addTest(string("ct_testASTSin"), ct_testASTSin);
		//tc->addTest(string("ct_test"), ct_test);
		return tc;
	}
}



