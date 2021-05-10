/*
 * TestRunner.h
 *
 *  Created on: 06/02/2013
 *      Author: Lean
 */

#ifndef TESTRUNNER_H_
#define TESTRUNNER_H_

#include <playground.h>
#include <list>

#define defaultAssertMessage StringFormatter::format("Assertion Failed in [%s], at line [%d]", __FILE__, __LINE__)
#define assertMessage(message) StringFormatter::format("Assertion Failed in [%s], at line [%d]", __FILE__, __LINE__).append(message)

class TestRunner: public PlaygroundRunner {
	private:
		std::map<String, void (TestRunner::*)()> tests;
		Logger *logger;
	public:
		static const unsigned char ID = 100;

	public:
		TestRunner()
		{
			logger = Logger::getLogger("test/TestRunner.h");
		}
		void addTest(String name, void (TestRunner::*testFunction)())
		{
			tests[name] = testFunction;
		}
		virtual unsigned char getId() {
			return TestRunner::ID;
		}

		virtual void before() {
		}
		virtual void after() {}

		virtual void doTests() {
			std::list<String> failedTests;

			logger->info("\n\nRunning [%d] Tests...\n\n", tests.size());

			for(std::map<String, void (TestRunner::*)()>::iterator iterator = tests.begin(); iterator != tests.end(); iterator++)
			{
				try {
					logger->info("TEST [%s]...", iterator->first.c_str());
					printf("TEST [%s]...\n", iterator->first.c_str());
					void (TestRunner::*currentTest)();
					currentTest = iterator->second;

					(this->*currentTest)();

					logger->info("[%s] PASSED.\n", iterator->first.c_str());
					printf("[%s] PASSED.\n", iterator->first.c_str());
				} catch (Exception &e) {
					logger->error("Test [%s] FAILED: %s\n", iterator->first.c_str(), e.toString().c_str());
					printf("Test [%s] FAILED: %s\n", iterator->first.c_str(), e.toString().c_str());
					failedTests.push_back(iterator->first);
				}

			}

			logger->info("\n\nTESTS: %d tests in error%s\n\n", failedTests.size(), getFailedTestsString(failedTests).c_str());
		}

		virtual bool afterInit()	{
			before();
			//tests
			doTests();

			after();

			return true;
		}

		virtual LoopResult doLoop() {
			return FINISHED;
		}

		void assertTrue(bool condition)
		{
			assertTrue("", condition);
		}
		void assertTrue(const String &message, bool condition)
		{
			if(!condition) {
			    assertFail(message);
			}
		}

		void assertEquals(const String &message, const String &expected, const String &actual)
		{
			assertTrue(message + ". Expected: [" + expected + "]. Actual: [" + actual + "]", expected == actual);
		}

		void assertEquals(const String &message, void *expected, void *actual)
		{
			assertTrue(message, expected == actual);
		}

		void assertEquals(const String &message, unsigned int expected, unsigned int actual)
		{
			assertTrue(StringFormatter::format("%s. Expected: [%d]. Actual: [%d]", message.c_str(), expected, actual), expected == actual);
		}

		void assertRealEquals(const String &message, real expected, real actual)
        {
            assertTrue(StringFormatter::format("%s. Expected: [%f]. Actual: [%f]", message.c_str(), expected, actual), equalsZero(expected - actual));
        }

		void assertFalse(const String &message, bool condition)
		{
			if(condition)
			assertFail(message);
		}
		void assertFalse(bool condition)
		{
			assertFalse("", condition);
		}

		void assertFail(const String &message) {
			throw Exception(message.c_str());
		}

		void assertEquals(const String &message, const vector &expected, const vector &actual)
        {
		    String vectorMessage = StringFormatter::format("%s. Expected %s, got %s:",
		            message.c_str(),
		            expected.toString("%.2f").c_str(),
		            actual.toString("%.2f").c_str());
		    assertRealEquals(vectorMessage, expected.x, actual.x);
		    assertRealEquals(vectorMessage, expected.y, actual.y);
		    assertRealEquals(vectorMessage, expected.z, actual.z);
        }

        void assertEquals(const String &message, const matriz_mxn &expected, const matriz_mxn &actual)
        {
            String text = StringFormatter::format("%s. Expected dimensions: [%dx%d]. Actual: [%dx%d]", message.c_str(), expected.getNroFilas(), expected.getNroColumnas(), actual.getNroFilas(), actual.getNroColumnas());
            assertTrue(text, expected.getNroFilas() == actual.getNroFilas() && expected.getNroColumnas() == expected.getNroColumnas());

            for(unsigned int i = 0; i < expected.getNroFilas(); i++) {
                for(unsigned int j = 0; j < expected.getNroColumnas(); j++) {
                    text = StringFormatter::format("%s. Element (%d, %d) - Expected: [%.3f]. Actual: [%.3f]", message.c_str(), i, j, expected(i, j), actual(i, j));
                    assertTrue(text, expected(i, j) == actual(i, j));
                }
            }
        }


	private:
		String getFailedTestsString(std::list<String> &stringList)
		{
			String result(stringList.empty() ? "" :  ": ");

			for(std::list<String>::iterator iterator = stringList.begin(); iterator != stringList.end(); iterator++)
			{
				if(iterator != stringList.begin())
					result.append(", ");

				result.append(*iterator);
			}

			return result;
		}
};


#endif /* TESTRUNNER_H_ */
