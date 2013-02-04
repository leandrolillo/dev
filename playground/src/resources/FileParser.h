/*
 * FileParser.h
 *
 *  Created on: 03/02/2013
 *      Author: Lean
 */

#ifndef FILEPARSER_H_
#define FILEPARSER_H_

#include "core/javaLike.h"
#include "log/Logger.h"
#include "exceptions/InvalidArgumentException.h"
#include <string>
#include<stdio.h>

class FileParser {
	private:
		String filename;
		FILE * fileStream;
		Logger *logger;

		const char *tokenSeparator;
		const char *blanks;

		boolean isInSet(char character, const char *set) {
			const char *currentTokenSeparator = set;
			while(*currentTokenSeparator != '\0') {
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
			while((caracter = fgetc(getStream())) != '\n');
		}

		void takeBlanks() {
			while(isBlank(fgetc(getStream())));

			if(feof(getStream()))
				fseek(getStream(), 0, SEEK_END);
			else
				fseek(getStream(), -1, SEEK_CUR);
		}


	public:
		FileParser(const String filename)
		{
			this->filename = filename;
			this->fileStream = null;

			tokenSeparator = "	# ()[]{},.:;<>+-*/^�=|&!�?\n\r\"\'\\�";
			blanks = " \n\r\t,";

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
					throw Exception("Error al abrir el archivo [%s]", filename.c_str());

				}

				logger->debug("File opened for reading binary [%s]", filename.c_str());
			}
			return fileStream;
		}

		String peekToken() {
			return "";
		}

		String takeToken() {
			char tokenBuffer[256];

			char *token = tokenBuffer;

			unsigned short longitud = 0;

			takeBlanks();
			while(!isTokenSeparator(*token = fgetc(getStream()))) {
				token++;
				longitud++;
			}
			if(longitud == 0) *(token+1) = '\0';
			else {
				*token = '\0';
				fseek(getStream(), -1, SEEK_CUR);
			}
			return(token);
		}


	};

#endif /* FILEPARSER_H_ */
