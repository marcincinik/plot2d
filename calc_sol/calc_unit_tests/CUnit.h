#ifndef CUNIT_H
#define CUNIT_H

#include <string>
#include <vector>
#include <exception>
#include <iostream>



namespace cunit {

	typedef void (*TestFunc)();

	class Test {
	private:
		std::string name;
		TestFunc test;
	public:
		Test(std::string name, TestFunc test) 
			: name(name), test(test) {
		}

		std::string getName() {
			return name;
		}

		void runTest() {
			this->test();
		}
	};

	class TestCase {
	private:
		std::string name;
		std::vector<Test> tests;
		TestFunc setup;
		TestFunc cleanup;
	protected:
		

	public:

		TestCase(std::string name, TestFunc setup, TestFunc cleanup)
			: name(name), 
			tests(std::vector<Test>()) {
				this->setup = setup;
				this->cleanup = cleanup;
		}


		void addTest(std::string name, TestFunc testFunction) {
			tests.push_back(Test(name, testFunction));
		}

		void callSetup() {
			setup();
		}

		void callCleanup() {
			cleanup();
		}

		std::vector<Test>& getTests() {
			return tests;
		}

		std::string getName() {
			return name;
		}
	};

	class TestRunner {
	private:
		std::vector<TestCase> testCases;
		TestCase* currentTestCase;
		Test* currentTest;
	protected:

		TestCase* getCurrentTestCase() {
			return currentTestCase;
		}

		Test* getCurrentTest() {
			return currentTest;
		}

		virtual bool runTest() {
			try {
				currentTestCase->callSetup();
				currentTest->runTest();
				return true;
			} catch (std::exception& e) {
				testFailed(e, currentTest);
				return false;
			} catch (...) {
				testFailed(std::current_exception(), currentTest);
				return false;
			}
			currentTestCase->callCleanup();
			
		}

		virtual bool runTestCase() {
			std::vector<Test> tests = currentTestCase->getTests();
			bool result = true;
			for (auto it = tests.begin(); it != tests.end(); ++it) {
				currentTest = &(*it);
				result &= runTest();
			}
			return result;
		}

		virtual void testFailed(std::exception& e, Test* test) = 0;
		virtual void testFailed(std::exception_ptr ePtr, Test* test) = 0;
	public:
		TestRunner(std::vector<TestCase> testCases)
			: testCases(testCases) {
		}

		std::vector<TestCase>& getTestCases() {
			return testCases;
		}

		virtual bool run() {
			bool result = true;
			for (auto it = testCases.begin(); it != testCases.end(); ++it) {
				currentTestCase = &(*it);
				result &= runTestCase();
			}
			return result;
		}
	};

	class StdoutTestRunner : public TestRunner {
	protected:
		virtual void testFailed(std::exception& e, Test* test) {
			std::cout << " FAILED:"
				<< (e.what() != NULL ? e.what() : "")
				<< std::endl;
		}

		virtual void testFailed(std::exception_ptr ePtr, Test* test) {
			std::cout << " FAILED: (unknown)" << std::endl;
		}

		virtual bool runTest() {
			std::cout << "running -> " 
				<< getCurrentTestCase()->getName() 
				<< ":" << getCurrentTest()->getName();
			bool result = TestRunner::runTest();
			if (result) {
				std::cout << " OK" << std::endl;
			} else {
				;
			}
			return result;
		}

	public:
		StdoutTestRunner(std::vector<TestCase> testCases) 
			: TestRunner(testCases) {
		}

		virtual bool run() {
			bool result = TestRunner::run();
			if (result) {
				std::cout << "Success" << std::endl;
			} else {
				std::cout << "Failed" << std::endl;
			}
			return result;
		}
	};
}


#endif
