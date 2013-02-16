/*
 * TestRunner.h
 *
 *  Created on: 06/02/2013
 *      Author: Lean
 */

#ifndef TESTRUNNER_H_
#define TESTRUNNER_H_

#include "core/playground.h"
#include <list>

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
			unsigned int testsInError = 0;
			logger->info("\n\nRunning [%d] Tests...\n\n", tests.size());

			for(std::map<String, void (TestRunner::*)()>::iterator iterator = tests.begin(); iterator != tests.end(); iterator++)
			{
				try {
					logger->info("TEST [%s]...", iterator->first.c_str());
					void (TestRunner::*currentTest)();
					currentTest = iterator->second;

					(this->*currentTest)();

					logger->info("[%s] PASSED.\n", iterator->first.c_str());
				} catch (Exception &e) {
					logger->error("Test [%s] FAILED: %s", iterator->first.c_str(), e.toString().c_str());
					testsInError++;
				}

			}

			logger->info("\n\nTESTS: %d tests in error\n\n", testsInError);
		}

		virtual boolean afterInit()	{
			before();
			//tests
			doTests();

			after();

			return true;
		}

		void assertTrue(boolean condition)
		{
			assertTrue("", condition);
		}
		void assertTrue(String message, boolean condition)
		{
			if(!condition)
			assertFail(message);
		}

		void assertEquals(String message, const String &expected, const String &actual)
		{
			assertTrue(message + ". Expected: [" + expected + "]. Actual: [" + actual + "]", expected == actual);
		}

		void assertEquals(String message, void *expected, void *actual)
		{
			assertTrue(message, expected == actual);
		}

		void assertEquals(String message, unsigned int expected, unsigned int actual)
		{
			char buffer[256];
			sprintf(buffer, "%s. Expected: [%d]. Actual: [%d]", message.c_str(), expected, actual);
			assertTrue(buffer, expected == actual);
		}

		void assertFalse(String message, boolean condition)
		{
			if(condition)
			assertFail(message);
		}
		void assertFalse(boolean condition)
		{
			assertFalse("", condition);
		}

		void assertFail(String message) {
			throw Exception(message.c_str());
		}
};


#endif /* TESTRUNNER_H_ */
