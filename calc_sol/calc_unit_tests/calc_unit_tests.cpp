// calc_unit_tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\calc_parser\Lexer.h"
#include "CAssert.h"
#include "TestLexer.h"
#include "TestParser.h"
#include "TestCalculator.h"

using namespace cunit;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	auto_ptr<TestCase> lexerTestCase = parser_tests::lexerTestCase();
	auto_ptr<TestCase> parserTestCase = parser_tests::parserTestCase();
	auto_ptr<TestCase> calculatorTestCase = parser_tests::calculatorTestCase();
	vector<TestCase> testCases = vector<TestCase>();

	testCases.push_back( *(lexerTestCase.get()) );
	testCases.push_back( *(parserTestCase.get()) );
	testCases.push_back( *(calculatorTestCase.get()) );

	StdoutTestRunner testRunner = StdoutTestRunner(testCases);
	testRunner.run();

	//read one character from input
	cin.get();
	return 0;
}

