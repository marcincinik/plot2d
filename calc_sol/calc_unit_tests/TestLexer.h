#ifndef TEST_LEXER_H
#define TEST_LEXER_H
#include "CUnit.h"
#include <memory>

namespace parser_tests {

	std::auto_ptr<cunit::TestCase> lexerTestCase();

}

#endif