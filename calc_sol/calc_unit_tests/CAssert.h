#ifndef CASSERT_H
#define CASSERT_H

#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <exception>

using namespace std;

namespace cunit {

	class CAssertError : public exception {
	public:
		CAssertError(const char* what) : exception(what) {
			;
		}
	};

	class CAssert {

	private:
		static void reportError(
			const string& expected, 
			const string& value) {

				stringstream s;
				s << "Assertion error:" << " expected " << expected << " but found " << value << endl;
				throw CAssertError(s.str().c_str());
		}
	public:

		static void assertEquals(int a, int b) {
			if (a != b) {
				stringstream sa;
				stringstream sb;
				sa << a;
				sb << b;
				reportError(sa.str(), sb.str());
			}
		}

		static void assertEquals(double a, double b) {
			if (a != b) {
				stringstream sa;
				stringstream sb;
				sa << a;
				sb << b;
				reportError(sa.str(), sb.str());
			}
		}

		static void assertEquals(std::string a, std::string b) {
			if (a != b) {
				reportError(a, b);
			}
		}

		static void assertNull(void* ptr) {
			if (ptr != NULL) {
				stringstream s;
				s << "Assertion error:" << " expected NULL but found non-NULL" << endl;
				throw CAssertError(s.str().c_str());
			}
		}

		static void assertNotNull(void* ptr) {
			if (ptr == NULL) {
				stringstream s;
				s << "Assertion error:" << " expected non-NULL but found NULL" << endl;
				throw CAssertError(s.str().c_str());
			}
		}

		static void assertTrue(bool val) {
			if (!val) {
				stringstream s;
				s << "Assertion error:" << " expected true but found false" << endl;
				throw CAssertError(s.str().c_str());
			}
		}

		static void assertFalse(bool val) {
			if (val) {
				stringstream s;
				s << "Assertion error:" << " expected false but found true" << endl;
				throw CAssertError(s.str().c_str());
			}
		}
	};

}

#endif

