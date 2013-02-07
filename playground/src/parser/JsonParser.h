/*
 * JsonParser.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef JSONPARSER_H_
#define JSONPARSER_H_

#include "resources/FileParser.h"
#include "ParsingException.h"

#define START_OBJECT "{"
#define END_OBJECT "}"
#define VALUE_SEPARATOR ":"
#define START_ARRAY "["
#define END_ARRAY "]"

class JsonParser
{
	private:
		Logger *logger;
		FileParser &fileParser;

	public:
		JsonParser(FileParser &parser) : fileParser(parser)
		{
			this->fileParser = fileParser;

			logger = Logger::getLogger("parser/JsonParser.h");
		}

		String readToken() {
			return fileParser.takeToken();
		}

		String peekToken() {
					return fileParser.peekToken();
		}

		void readValueSeparator()
		{
			String token = fileParser.takeToken();
			if(token != VALUE_SEPARATOR)
				throw ParsingException("Expected ValueSeparator, got [%s] at (%d, %d)", token.c_str(), fileParser.getLine(), fileParser.getColumn());

		}

		std::vector<vector3> readVectorArray()
		{
			readStartArray();
			readEndArray();

			return std::vector<vector3>();
		}

		void readStartArray()
		{
			String token = fileParser.takeToken();
			if(token != START_ARRAY)
				throw ParsingException("Expected StartArray, got [%s] at (%d, %d)", token.c_str(), fileParser.getLine(), fileParser.getColumn());
		}

		void readEndArray()
		{
			String token = fileParser.takeToken();
			if(token != END_ARRAY)
				throw ParsingException("Expected EndArray, got [%s] at (%d, %d)", token.c_str(), fileParser.getLine(), fileParser.getColumn());
		}

		void readStartObject()
		{
			String token = fileParser.takeToken();
			if(token != START_OBJECT)
				throw ParsingException("Expected StartObject, got [%s] at (%d, %d)", token.c_str(), fileParser.getLine(), fileParser.getColumn());
		}

		void readEndObject()
		{
			String token = fileParser.takeToken();
			if(token != END_OBJECT)
				throw ParsingException("Expected EndObject, got [%s] at (%d, %d)", token.c_str(), fileParser.getLine(), fileParser.getColumn());
		}
};

#endif /* JSONPARSER_H_ */
