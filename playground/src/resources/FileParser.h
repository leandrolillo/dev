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

	public:
		FileParser(const String filename)
		{
			this->filename = filename;
			this->fileStream = null;

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

	};

#endif /* FILEPARSER_H_ */
