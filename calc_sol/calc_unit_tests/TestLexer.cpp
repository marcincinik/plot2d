#include "stdafx.h"

#include "TestLexer.h"

#include "..\calc_parser\Lexer.h"
#include "CAssert.h"
#include "CUnit.h"
#include <vector>
#include <string>

using namespace std;
using namespace cunit;
using namespace parser;

namespace parser_tests {

	Lexer* lexer = NULL;
	stringstream* lt_s = NULL;

	void lt_setup() {
		lexer = new Lexer();
		lt_s = new stringstream();
	}

	void lt_cleanup() {
		delete lexer;
		delete lt_s;
		lexer = NULL;
		lt_s = NULL;
	}

	void lt_testNextEmpty() {
		*lt_s << " \r\t\n";
		auto_ptr<Lexem> lexem = lexer->next(*lt_s);
		CAssert::assertNull(lexem.get());
        CAssert::assertTrue(lexer->isEof());
	}

	void lt_testNextStateChangeTwice() {
		*lt_s << " 1 2 ";
		auto_ptr<Lexem> lexem1 = lexer->next(*lt_s);
		CAssert::assertTrue(FloatLexem(1.0) == *(lexem1.get()));
        CAssert::assertFalse(lexer->isEof());

		auto_ptr<Lexem> lexem2 = lexer->next(*lt_s);
		CAssert::assertTrue(FloatLexem(2.0) == *(lexem2.get()));
        CAssert::assertFalse(lexer->isEof());

		auto_ptr<Lexem> lexem3 = lexer->next(*lt_s);
		CAssert::assertNull(lexem3.get());
        CAssert::assertTrue(lexer->isEof());
	}

	void lt_testNextFloatInt() {
		*lt_s << "1";
		auto_ptr<Lexem> lexem = lexer->next(*lt_s);
		CAssert::assertTrue(FloatLexem(1.0) == *(lexem.get()));
        CAssert::assertTrue(lexer->isEof());
	}

	void lt_testNextFloatDot() {
		*lt_s << "1.2";
		auto_ptr<Lexem> lexem = lexer->next(*lt_s);
		CAssert::assertTrue(FloatLexem(1.2) == *(lexem.get()));
        CAssert::assertTrue(lexer->isEof());
	}

	void lt_testNextFloatLong() {
		*lt_s << "12.34";
		auto_ptr<Lexem> lexem = lexer->next(*lt_s);
		CAssert::assertTrue(FloatLexem(12.34) == *(lexem.get()));
        CAssert::assertTrue(lexer->isEof());
	}

	void lt_testNextPlus() {
		*lt_s << "+";
		auto_ptr<Lexem> lexem = lexer->next(*lt_s);
		CAssert::assertTrue(PlusLexem() == *(lexem.get()));
        CAssert::assertTrue(lexer->isEof());
	}

	void lt_testNextMinus() {
		*lt_s << "-";
		auto_ptr<Lexem> lexem = lexer->next(*lt_s);
		CAssert::assertTrue(MinusLexem() == *(lexem.get()));
        CAssert::assertTrue(lexer->isEof());
	}

	void lt_testNextMul() {
		*lt_s << "*";
		auto_ptr<Lexem> lexem = lexer->next(*lt_s);
		CAssert::assertTrue(MulLexem() == *(lexem.get()));
        CAssert::assertTrue(lexer->isEof());
	}

	void lt_testNextDiv() {
		*lt_s << "/";
		auto_ptr<Lexem> lexem = lexer->next(*lt_s);
		CAssert::assertTrue(DivLexem() == *(lexem.get()));
        CAssert::assertTrue(lexer->isEof());
	}

	void lt_testNextDash() {
		*lt_s << "^";
		auto_ptr<Lexem> lexem = lexer->next(*lt_s);
		CAssert::assertTrue(DashLexem() == *(lexem.get()));
        CAssert::assertTrue(lexer->isEof());
	}

	void lt_testNextOParen() {
		*lt_s << "(";
		auto_ptr<Lexem> lexem = lexer->next(*lt_s);
		CAssert::assertTrue(OParenLexem() == *(lexem.get()));
        CAssert::assertTrue(lexer->isEof());
	}

	void lt_testNextCParen() {
		*lt_s << ")";
		auto_ptr<Lexem> lexem = lexer->next(*lt_s);
		CAssert::assertTrue(CParenLexem() == *(lexem.get()));
        CAssert::assertTrue(lexer->isEof());
	}

	void lt_testOCParenFloat() {
		*lt_s << "(1)";
		auto_ptr<Lexem> lexem1 = lexer->next(*lt_s);
		CAssert::assertTrue(OParenLexem() == *(lexem1.get()));

		auto_ptr<Lexem> lexem2 = lexer->next(*lt_s);
		CAssert::assertTrue(FloatLexem(1.0f) == *(lexem2.get()));
        
		auto_ptr<Lexem> lexem3 = lexer->next(*lt_s);
		CAssert::assertTrue(CParenLexem() == *(lexem3.get()));
	}

	void lt_testOCParenDoubleFloat() {
		*lt_s << "((1))";
		auto_ptr<Lexem> lexem1 = lexer->next(*lt_s);
		CAssert::assertTrue(OParenLexem() == *(lexem1.get()));

		auto_ptr<Lexem> lexem2 = lexer->next(*lt_s);
		CAssert::assertTrue(OParenLexem() == *(lexem2.get()));

		auto_ptr<Lexem> lexem3 = lexer->next(*lt_s);
		CAssert::assertTrue(FloatLexem(1.0f) == *(lexem3.get()));

		auto_ptr<Lexem> lexem4 = lexer->next(*lt_s);
		CAssert::assertTrue(CParenLexem() == *(lexem4.get()));

		auto_ptr<Lexem> lexem5 = lexer->next(*lt_s);
		CAssert::assertTrue(CParenLexem() == *(lexem5.get()));
	}

	void lt_testNextIdentifier1() {
		*lt_s << "a";
		auto_ptr<Lexem> lexem = lexer->next(*lt_s);
		CAssert::assertTrue(IdentifierLexem("a") == *(lexem.get()));
        CAssert::assertTrue(lexer->isEof());
	}

	void lt_testNextIdentifier2() {
		*lt_s << "a0";
		auto_ptr<Lexem> lexem = lexer->next(*lt_s);
		CAssert::assertTrue(IdentifierLexem("a0") == *(lexem.get()));
        CAssert::assertTrue(lexer->isEof());
	}
	
	void lt_testNextIdentifier3() {
		*lt_s << "_a";
		auto_ptr<Lexem> lexem = lexer->next(*lt_s);
		CAssert::assertTrue(IdentifierLexem("_a") == *(lexem.get()));
        CAssert::assertTrue(lexer->isEof());
	}

	auto_ptr<TestCase> lexerTestCase() {
		auto_ptr<TestCase> tc = auto_ptr<TestCase>(new TestCase(string("LexerTestCase"), 
			lt_setup, lt_cleanup));
		tc.get()->addTest("lt_testNextEmpty", lt_testNextEmpty);
		tc.get()->addTest("lt_testNextStateChangeTwice", lt_testNextStateChangeTwice);
		tc.get()->addTest("lt_testNextFloatInt", lt_testNextFloatInt);
		tc.get()->addTest("lt_testNextFloatDot", lt_testNextFloatDot);
		tc.get()->addTest("lt_testNextFloatLong", lt_testNextFloatLong);
		tc.get()->addTest("lt_testNextPlus", lt_testNextPlus);
		tc.get()->addTest("lt_testNextMinus", lt_testNextMinus);
		tc.get()->addTest("lt_testNextMul", lt_testNextMul);
		tc.get()->addTest("lt_testNextDiv", lt_testNextDiv);
		tc.get()->addTest("lt_testNextDash", lt_testNextDash);
		tc.get()->addTest("lt_testNextOParen", lt_testNextOParen);
		tc.get()->addTest("lt_testOCParenDoubleFloat", lt_testOCParenDoubleFloat);
		tc.get()->addTest("lt_testNextCParen", lt_testNextCParen);
		tc.get()->addTest("lt_testOCParenFloat", lt_testOCParenFloat);
		tc.get()->addTest("lt_testNextIdentifier1", lt_testNextIdentifier1);
		tc.get()->addTest("lt_testIdentifier2", lt_testNextIdentifier2);
		tc.get()->addTest("lt_testIdentifier3", lt_testNextIdentifier3);
		return tc;
	}

}

