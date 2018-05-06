#ifndef TESTPARSER_H
#define TESTPARSER_H
#include "CUnit.h"
#include <memory>

namespace parser_tests {
	std::auto_ptr<cunit::TestCase> parserTestCase();
}

#endif
