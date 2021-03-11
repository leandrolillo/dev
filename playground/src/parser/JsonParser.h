/*
 * JsonParser.h
 *
 *  Created on: 07/02/2013
 *      Author: Lean
 */

#ifndef JSONPARSER_H_
#define JSONPARSER_H_

#include "../resources/FileParser.h"
#include "../javaLike/JavaLike.h"
#include "../math/Math3d.h"
#include "ParsingException.h"
#include <vector>

#define START_OBJECT "{"
#define END_OBJECT "}"
#define VALUE_SEPARATOR ":"
#define START_ARRAY "["
#define END_ARRAY "]"
#define START_VECTOR "<"
#define END_VECTOR ">"

class JsonParser
{
	private:
		Logger *logger;
		FileParser &fileParser;

	public:
		JsonParser(FileParser &parser) : fileParser(parser)
		{
			logger = Logger::getLogger("parser/JsonParser.h");
		}

		String readToken() {
			return fileParser.takeToken();
		}

		String peekToken() {
					return fileParser.peekToken();
		}

		String readElement(String expectedValue, String name)
		{
			String token = fileParser.takeToken();
			if(token != expectedValue)
				throw ParsingException("Expected %s, got [%s] at (%d, %d)", name.c_str(), token.c_str(), fileParser.getLine(), fileParser.getColumn());
			return token;
		}

		String readValueSeparator() {
			return readElement(VALUE_SEPARATOR, "ValueSeparator");
		}

		String readStartVector() {
			return readElement(START_VECTOR, "StartVector");
		}

		String readEndVector() {
			return readElement(END_VECTOR, "EndVector");
		}

		String readStartArray() {
			return readElement(START_ARRAY, "StartArray");
		}

		String readEndArray() {
			return readElement(END_ARRAY, "EndArray");
		}

		String readStartObject()
		{
			return readElement(START_OBJECT, "StartObject");
		}

		String readEndObject()
		{
			return readElement(END_OBJECT, "EndObject");
		}

		String readString()
		{
			String value;
			readElement("\"", "StartString");

			char character;
			while((character = fileParser.takeByte()) != '\"' && character != EOF)
				value += character;

			return value;
		}

		unsigned int readUnsignedInteger()
		{
			unsigned int value = 0;

			String token = fileParser.takeToken();
			char digit;
			bool readValue = false;

			unsigned int currentPosition = 0;
			while(currentPosition < token.length() && '0' <= (digit = token.at(currentPosition++)) && digit <= '9')
			{
				value = value * 10 + (unsigned int)(digit - '0');
				readValue = true;
			}

			if(!readValue)
				throw ParsingException("Unexpected %s at (%d, %d)", fileParser.takeToken().c_str(), fileParser.getLine(), fileParser.getColumn());

			return value;
		}
		real readDecimals()
		{
			real value = 0;
			real modifier = 0.1;

			String token = fileParser.takeToken();
			char digit;
			bool readValue = false;

			unsigned int currentPosition = 0;
			while(currentPosition < token.length() && '0' <= (digit = token.at(currentPosition++)) && digit <= '9')
			{
				value = value  + (real)(digit - '0') * modifier;
				modifier *= 0.1;
				readValue = true;
			}

			if(!readValue)
				throw ParsingException("Unexpected %s at (%d, %d)", fileParser.takeToken().c_str(), fileParser.getLine(), fileParser.getColumn());

			return value;
		}
		char readSign()
		{
			int sign = 1;
			String token = fileParser.peekToken();
			if(token == "-") {
				sign = -1;
				fileParser.takeToken();
			} else if(token == "+")
				fileParser.takeToken();

			return sign;
		}

		int readInteger()
		{
			int sign = readSign();
			int unsignedInteger = readUnsignedInteger();
			return (int)unsignedInteger * sign;
		}

		real readMantissa()
		{
			real integer = 0.0;
			real decimal = 0.0;

			real mantissaSign = readSign();

			if(fileParser.peekToken() != ".")
				integer = readUnsignedInteger();

			if(fileParser.peekToken() == ".") {
				fileParser.takeToken();

				String token = fileParser.peekToken();

				if('0' <= token.at(0) && token.at(0) <= '9')
					decimal = readDecimals();
			}

			return mantissaSign * (integer + decimal);

		}

		int readExponent()
		{
			real value = 0.0;
			String token = fileParser.peekToken();
			if(token.at(0) == 'e' || token.at(0) == 'E') {
				fileParser.takeToken();
				value = readInteger();
			}
			return value;
		}

		real readReal()
		{
			fileParser.setTokenSeparator("	# ()[]{},.:;<>+-*/^�=|&!�?\n\r\"\'\\eE�");
			real mantissa = readMantissa();
			int exponent = readExponent();

			fileParser.setDefaultSpecialCharacters();

			return mantissa * pow(10.0, exponent);
		}


		vector3 readVector3()
		{
			String token;
			real x = 0.0;
			real y = 0.0;
			real z = 0.0;

			readStartVector();
			x = readReal();
			readElement(",", "comma");
			y = readReal();
			readElement(",", "comma");
			z = readReal();
			readEndVector();

			return vector3(x, y ,z);
		}

		vector2 readVector2()
		{
			String token;
			real x = 0.0;
			real y = 0.0;

			readStartVector();
			x = readReal();
			readElement(",", "comma");
			y = readReal();
			readEndVector();

			return vector2(x, y);
		}

		std::vector<vector2> readVector2Array()
		{
			std::vector<vector2>array;

			readStartArray();
			String token;

			while((token = fileParser.peekToken()) != END_ARRAY && token != eof) {
				vector2 vec = readVector2();
				array.push_back(vec);

				if((token = fileParser.peekToken()) == ",")
					fileParser.takeToken();

					//throw ParsingException("Unexpected %s at (%d, %d)", token.c_str(), fileParser.getLine(), fileParser.getColumn());
			}

			readEndArray();

			return array;
		}

		std::vector<vector3> readVector3Array()
		{
			std::vector<vector3>array;

			readStartArray();
			String token;

			while((token = fileParser.peekToken()) != END_ARRAY && token != eof) {
				vector3 vec = readVector3();
				array.push_back(vec);

				if((token = fileParser.peekToken()) == ",")
					fileParser.takeToken();

					//throw ParsingException("Unexpected %s at (%d, %d)", token.c_str(), fileParser.getLine(), fileParser.getColumn());
			}

			readEndArray();

			return array;
		}

		std::vector<String> readStringArray()
		{
			std::vector<String>array;

			readStartArray();
			String token;

			while((token = fileParser.peekToken()) != END_ARRAY && token != eof) {
				String string = readString();
				array.push_back(string);

				if((token = fileParser.peekToken()) == ",")
					fileParser.takeToken();
			}

			readEndArray();

			return array;
		}

};

#endif /* JSONPARSER_H_ */
