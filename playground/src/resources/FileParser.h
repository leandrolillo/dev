/*
 * FileParser.h
 *
 *  Created on: 03/02/2013
 *      Author: Lean
 */

#ifndef FILEPARSER_H_
#define FILEPARSER_H_

#include "../javaLike/JavaLike.h"
#include "../log/Logger.h"
#include "../exceptions/InvalidArgumentException.h"
#include <string>
#include<stdio.h>

#define eof std::to_string(EOF)
#define eol "\n"

#define char_eof (char)EOF
#define char_eol '\n'


class FileParser {
	private:
		String filename;
		FILE * fileStream;
		Logger *logger;

		const char *tokenSeparator;
		const char *blanks;
		char commentChar;

		unsigned int line;
		unsigned int column;

		//TODO: Move this to a stack and make its methods public
		unsigned int lineBackup;
		unsigned int columnBackup;
		fpos_t position;


		boolean isInSet(char character, const char *set) {
			const char *currentTokenSeparator = set;
			while(*currentTokenSeparator != char_eof) {
				if(*currentTokenSeparator == character)
					return true;

				currentTokenSeparator++;
			}

			return false;
		}

		boolean isTokenSeparator(char character)
		{
			return isInSet(character, tokenSeparator);
		}

		boolean isBlank(char character)
		{
			return isInSet(character, blanks);
		}

		void takeUntilEOL() {
			char caracter;
			while((caracter = takeByte()) != char_eol && caracter != char_eof);
		}

		void takeBlanks() {
			while(isBlank(takeByte()));

			if(feof(getStream()))
				fseek(getStream(), 0, SEEK_END);
			else
				fseek(getStream(), -1, SEEK_CUR);
		}
		
		void takeBlanksAndComments() {
			char character;
			char exit;
			
			do {
				exit = true;
				while(isBlank(character = takeByte()));
				if(character == commentChar) {
					takeUntilEOL();
					exit = false;
				}
			} while(!exit);

			if(feof(getStream()))
				fseek(getStream(), 0, SEEK_END);
			else
				fseek(getStream(), -1, SEEK_CUR);
		}

		void popPosition() {
			line = lineBackup;
			column = columnBackup;

			if(fsetpos(getStream(), &position)!= 0)
				throw new Exception("Could not restore stream position");
		}
		void pushPosition() {
			lineBackup = line;
			columnBackup = column;

			if(fgetpos(getStream(), &position) != 0)
				throw new Exception("Could not save stream position");

		}

	public:
		FileParser(const String filename)
		{
			this->filename = filename;
			this->fileStream = null;

			this->setDefaultSpecialCharacters();

			position = -1;
			line = 1;
			column = 1;
			lineBackup = line;
			columnBackup = column;


			logger = Logger::getLogger("resources/FileParser.h");
		}

		~FileParser()
		{
			this->close();
		}

		size_t read(void* data, size_t size, size_t count)
		{
			return fread(data, size, count, getStream());
		}

		void close()
		{
			if(fileStream != null) {
				logger->debug("Closing file [%s]", filename.c_str());
				fclose(fileStream);
				fileStream = null;
			}
		}

		const String& getFilename() const
		{
			return filename;
		}

		FILE *getStream()
		{
			if(fileStream == null) {
				if(filename == "")
					throw InvalidArgumentException("FileName can't be null");

				if((fileStream = fopen(filename.c_str(), "rb")) == null) {
					logger->error("Error al abrir el archivo [%s]", filename.c_str());
					throw InvalidArgumentException("Error al abrir el archivo [%s]", filename.c_str());

				}

				logger->debug("File opened for reading binary [%s]", filename.c_str());
			}
			return fileStream;
		}

		int peekByte()
		{
			pushPosition();
			int byte = takeByte();
			popPosition();

			return byte;
		}

		int takeByte()
		{
			int byte = fgetc(getStream());
			if((char)byte == char_eol) {
				line++;
				column = 0;
			} else if((char) byte != '\r' && (char)byte != char_eof && (char) byte != char_eof) {
				column++;
			}

			return byte;
		}

		String peekLine() {
			pushPosition();
			String line = takeLine();
			popPosition();

			return line;
		}
		String takeLine()
		{
			String line;
			char caracter[2];
			caracter[1] = char_eof;

			while((caracter[0] = takeByte()) != char_eol && caracter[0] != char_eof)
				line.append(caracter);

			return line;
		}


		String peekToken() {
			pushPosition();
			String token = takeToken();
			popPosition();
			return token;
		}

		String takeToken() {
			char tokenBuffer[256];

			char *token = tokenBuffer;

			unsigned short longitud = 0;

			takeBlanksAndComments();
			while(!isTokenSeparator(*token = takeByte())) {
				token++;
				longitud++;
			}
			if(longitud == 0) *(token+1) = char_eof;
			else {
				*token = char_eof;
				fseek(getStream(), -1, SEEK_CUR);
			}
			return(tokenBuffer);
		}

		unsigned int getColumn() const
		{
			return column;
		}

		unsigned int getLine() const
		{
			return line;
		}

		const char* getTokenSeparator() const
		{
			return tokenSeparator;
		}

		void setTokenSeparator(const char* tokenSeparator)
		{
			this->tokenSeparator = tokenSeparator;
		}

		void setDefaultSpecialCharacters()
		{
			tokenSeparator = "	# ()[]{},.:;<>+-*/^�=|&!�?\n\r\"\'\\�";
			blanks = " \n\r\t";
			commentChar = '#';
		}
		unsigned int size()
		{
			pushPosition();
			fseek(getStream(), 0, SEEK_END);
			unsigned int size = ftell(getStream());
			popPosition();
			return size;
		}
	};

#endif /* FILEPARSER_H_ */
