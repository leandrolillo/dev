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
			std::list<String> failedTests;

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
					logger->error("Test [%s] FAILED: %s\n", iterator->first.c_str(), e.toString().c_str());
					failedTests.push_back(iterator->first);
				}

			}

			logger->info("\n\nTESTS: %d tests in error%s\n\n", failedTests.size(), getFailedTestsString(failedTests).c_str());
		}

		virtual boolean afterInit()	{
			before();
			//tests
			doTests();

			after();

			return true;
		}

		virtual LoopResult doLoop() {
			return FINISHED;
		}

		void assertTrue(boolean condition)
		{
			assertTrue("", condition);
		}
		void assertTrue(const String &message, boolean condition)
		{
			if(!condition)
			assertFail(message);
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
			char buffer[256];
			sprintf(buffer, "%s. Expected: [%d]. Actual: [%d]", message.c_str(), expected, actual);
			assertTrue(buffer, expected == actual);
		}
		void assertEquals(const String &message, double expected, double actual)
		{
			char buffer[256];
			sprintf(buffer, "%s. Expected: [%.3f]. Actual: [%.3f]", message.c_str(), expected, actual);
			assertTrue(buffer, expected == actual);
		}

		void assertFalse(const String &message, boolean condition)
		{
			if(condition)
			assertFail(message);
		}
		void assertFalse(boolean condition)
		{
			assertFalse("", condition);
		}

		void assertFail(const String &message) {
			throw Exception(message.c_str());
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
