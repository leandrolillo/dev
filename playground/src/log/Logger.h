/*
 * Logger.h
 *
 *  Created on: 21/01/2013
 *      Author: Lean
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "../javaLike/JavaLike.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include<sys/stat.h>
#include <vector>
#include <string.h>
#include <errno.h>

#define _USE_PRINTF_
//#define DEBUG
//#define VERBOSE


class Logger {
	private:
		String basename;
		//static FILE *fileHandler;

	protected:
		static struct tm *getCurrentTime() {
			time_t t;
			time(&t);
			return localtime(&t);
		}
		static const char *getFileName() {
			return "playground.log";
		}

		static FILE *getFileHandler(){
			static FILE *fileHandler;

			if (fileHandler == null) {
#ifdef DEBUG
				printf("File handler is NULL\n");
#endif
				FILE *existingFile;
				if ((existingFile = fopen(getFileName(), "rt"))) {
					struct stat fileStats;
					fclose(existingFile);

					printf("Log file already exists, checking if new version is required...\n");

					if (!stat(getFileName(), &fileStats)) {
						struct tm *dateTime = localtime(&fileStats.st_ctime);
						char textBuffer[100];
						strftime(textBuffer, 100, "%Y%m%d", dateTime);

						int fileYear = dateTime->tm_year;
						int fileDay = dateTime->tm_yday;

						dateTime = getCurrentTime();

						if (fileYear != dateTime->tm_year || fileDay != dateTime->tm_yday) {
							printf("Log file was NOT created today\n");
							char fileName[256];
							sprintf(fileName, "%s.%s", getFileName(), textBuffer);
							printf("Renaming old log file to [%s]\n", fileName);

							if(rename(getFileName(), fileName)) {
								printf("Error renaming existing log file [%s] to [%s]\n", getFileName(), fileName);
								perror(getFileName());
							}

							if(remove(getFileName())) {
								printf("Error removing log file [%s]\n", getFileName());
								perror(getFileName());
							}

							printf("Opening new log file\n");
							if ((fileHandler = fopen(getFileName(), "wt")) == null) {
								printf("Error opening logger file [%s]\n", getFileName());
								perror(getFileName());
								fileHandler = null;
							}
						} else {
							printf("Appending to existing log file\n");
							if (!(fileHandler = fopen(getFileName(), "at"))) {
								printf("Error opening logger file [%s]\n", getFileName());
								perror(getFileName());
								fileHandler = null;
							}
						}
					}
				} else {
					printf("Appending to existing log file\n");
					if (!(fileHandler = fopen(getFileName(), "wt"))) {
						printf("Error opening logger file [%s]\n", getFileName());
						perror(getFileName());
						fileHandler = null;
					}
				}
			}
			return fileHandler;
		}

		void flush() {
			FILE *fileHandler = getFileHandler();
			fflush(fileHandler);
		}


		Logger()
		{
			this->basename = "";
		}

		void printMessage(const char *type, const char * formato, va_list *args) {
			FILE *fileHandler = getFileHandler();

			if(fileHandler != null) {
				char textBuffer[256];
				strftime(textBuffer, sizeof(textBuffer), "%d/%m/%Y %H:%M:%S", getCurrentTime());

				fprintf(fileHandler, "%s - %s - %s: ", textBuffer, type, basename.c_str());
				printf("%s - %s - %s: ", textBuffer, type, basename.c_str());

				/**
				 * This approach was causing null pointer exceptions because args can only be iterated once
				 * 				//vprintf(formato, *args);
				 * 				//vfprintf(fileHandler, formato, *args);
				 */

				char *tempBuffer = null; //TODO: review if it is better to use a fixed buffer and truncate logs length
				if(vasprintf(&tempBuffer, formato, *args)) {
					printf("%s", tempBuffer);
					fprintf(fileHandler, "%s", tempBuffer);
					free(tempBuffer);
				}


				fprintf(fileHandler, "\n");
				printf("\n");
			}
		}
		void setBasename(const String &basename)
		{
			this->basename = basename;
		}

	public:
		void info(const char *formato, ...)
		{
			va_list args;
			va_start(args, formato);
			printMessage("FINE ", formato, &args);
			va_end(args);
			this->flush();
		}

		void debug(const char *formato, ...)
		{
#ifdef DEBUG
			va_list args;
			va_start(args, formato);
			printMessage("DEBUG ", formato, &args);
			va_end(args);
			this->flush();
#endif
		}
		void error(const char *formato, ...)
		{
			va_list args;
			va_start(args, formato);
			printMessage("SEVERE ", formato, &args);
			va_end(args);

			if(errno != 0 && strlen(strerror(errno)) > 0) {
				printMessage("Error Message", strerror(errno), &args);
				errno = 0;
			}

			this->flush();
		}

		void warn(const char *formato, ...)
		{
			va_list args;
			va_start(args, formato);
			printMessage("WARNING ", formato, &args);
			va_end(args);
			this->flush();
		}

		void verbose(const char *formato, ...)
		{
#if defined(DEBUG) && defined(VERBOSE)
			va_list args;
			va_start(args, formato);
			printMessage("VERBOSE ", formato, &args);
			va_end(args);
			this->flush();
#endif
		}

		const String &getBasename() const {
			return this->basename;
		}

	public:
		static std::vector<Logger *>loggers;
		static Logger *getLogger(String basename)
		{
			for(std::vector<Logger *>::iterator currentLoggerIterator = Logger::loggers.begin();
					currentLoggerIterator != Logger::loggers.end();
				currentLoggerIterator++) {
				if((*currentLoggerIterator)->getBasename() == basename) {
					return *currentLoggerIterator;
				}
			}

			getFileHandler();

			Logger *logger = new Logger();
			logger->setBasename(basename);

			Logger::loggers.push_back(logger);

			return logger;
		}

		static void deleteLoggers()
		{
			for(std::vector<Logger *>::iterator iterator = Logger::loggers.begin(); iterator != Logger::loggers.end(); iterator++)
				delete *iterator;

			loggers.clear();

			if(getFileHandler() != null) {
				fclose(getFileHandler());
			}

		}

	};

#endif /* LOGGER_H_ */
