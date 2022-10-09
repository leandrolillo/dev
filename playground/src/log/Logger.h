/*
 * Logger.h
 *
 *  Created on: 21/01/2013
 *      Author: Lean
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <JavaLike.h>
#include <StringFormatter.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include<sys/stat.h>
#include <vector>
#include <string.h>
#include <errno.h>

enum class LogLevel {
    ERROR=0,
    WARN,
    INFO,
    DEBUG,
    VERBOSE
};

constexpr LogLevel DEFAULT_LOG_LEVEL = LogLevel::INFO;
//constexpr LogLevel DEFAULT_LOG_LEVEL = LogLevel::DEBUG;

class Appender {
protected:
    String target;
public:
    static struct tm* getCurrentTime() {
        time_t t;
        time(&t);
        return localtime(&t);
    }
public:
    Appender(const String &target) {
        this->target = target;
    }

    virtual ~Appender() {
    }

    const String& getTarget() const {
        return target;
    }
    virtual void append(const char *message) = 0;
    virtual void flush() = 0;
};

class FileAppender: public Appender {
private:
    FILE *fileHandler = null;

public:
    FileAppender(String target) : Appender(target) {
    }

    ~FileAppender() {
        if (getFileHandler() != null) {
            fclose(getFileHandler());
        }
    }

    void append(const char *message) override {
        fprintf(getFileHandler(), "%s", message); // use ("%s", message) instead of directly message to get rid of warnings.
    }

    void flush() override {
        FILE *fileHandler = getFileHandler();
        fflush(fileHandler);
    }

protected:
    const char* getFileName() const {
        return this->getTarget().c_str();
    }

    FILE* getFileHandler() {
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

                    dateTime = Appender::getCurrentTime();

                    if (fileYear != dateTime->tm_year || fileDay != dateTime->tm_yday) {
                        printf("Log file was NOT created today\n");
                        char newfileName[256];
                        sprintf(newfileName, "%s.%s", getFileName(), textBuffer);
                        printf("Renaming old log file to [%s]\n", newfileName);

                        if (rename(getFileName(), newfileName)) {
                            printf("Error renaming existing log file [%s] to [%s]\n", getFileName(), newfileName);
                            perror(getFileName());
                        }

                        if (remove(getFileName())) {
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
};

class StdoutAppender: public Appender {
public:
    StdoutAppender() : Appender("stdout") {

    }

    void append(const char *message) override {
        printf("%s", message); // use ("%s", message) instead of directly message to get rid of warnings.
    }

    void flush() override {
    }
};

class Logger {
protected:
    String basename;
    std::vector<Appender*> appenders;
    LogLevel logLevel = LogLevel::INFO;
    //static FILE *fileHandler;

public:
    Logger(const String &basename) {
        this->basename = basename;
    }
//    ~Logger() {
//        printf("Deleting logger %s\n",  this->basename.c_str());
//
//    }

    const String& getBasename() const {
        return this->basename;
    }

    void setLogLevel(LogLevel logLevel) {
        this->logLevel = logLevel;
    }

protected:
    void messageAppenders(const String &message) {
        for (auto appender : appenders) {
            appender->append(message.c_str());
            appender->flush();
        }
    }

    void printMessage(const char *type, const char *formato, va_list *args) {
        if (!this->appenders.empty()) {
            char textBuffer[256];
            strftime(textBuffer, sizeof(textBuffer), "%d/%m/%Y %H:%M:%S", Appender::getCurrentTime());

            messageAppenders(StringFormatter::format("%s - %s - %s: ", textBuffer, type, basename.c_str()));
            messageAppenders(StringFormatter::formatVarArgs(formato, args));
            messageAppenders("\n");
        }
    }
public:
    void addAppender(Appender *appender) {
        this->appenders.push_back(appender);
    }

    void info(const char *formato, ...) {
        if(logLevel >= LogLevel::INFO) {
            va_list args;
            va_start(args, formato);
            printMessage("FINE ", formato, &args);
            va_end(args);
        }
    }

    void debug(const char *formato, ...) {
        if(logLevel >= LogLevel::DEBUG) {
			va_list args;
			va_start(args, formato);
			printMessage("DEBUG ", formato, &args);
			va_end(args);
        }
    }
    void error(const char *formato, ...) {
        va_list args;
        va_start(args, formato);
        printMessage("SEVERE ", formato, &args);
        va_end(args);

        //This does not seem to be very reliable
        if (errno != 0 && strlen(strerror(errno)) > 0) {
            printMessage(StringFormatter::format("SEVERE [%d]: ", errno).c_str(), strerror(errno), &args);
            errno = 0;
        }
    }

    void warn(const char *formato, ...) {
        if(logLevel >= LogLevel::WARN) {
            va_list args;
            va_start(args, formato);
            printMessage("WARNING ", formato, &args);
            va_end(args);
        }
    }

    void verbose(const char *formato, ...) {
        if(logLevel >= LogLevel::VERBOSE) {
			va_list args;
			va_start(args, formato);
			printMessage("VERBOSE ", formato, &args);
			va_end(args);
        }
    }
};

class LoggerFactory {
private:
    static std::vector<std::unique_ptr<Appender>> appenders;
    static std::vector<std::unique_ptr<Logger>> loggers;

public:
    static Appender* getAppender(String output) {
        for (auto &appender : appenders) {
            if (appender->getTarget() == output) {
                return appender.get();
            }
        }

        Appender *appender = null;

        if (output == "stdout") {
            appender = new StdoutAppender();
        } else {
            appender = new FileAppender(output);
        }

        LoggerFactory::appenders.push_back(std::unique_ptr<Appender>(appender));
        return appender;
    }

    static Logger* getLogger(String basename) {
        for (auto &logger : loggers) {
            if (logger->getBasename() == basename) {
                return logger.get();
            }
        }

        //getFileHandler();

        Logger *logger = new Logger(basename);
        logger->setLogLevel(DEFAULT_LOG_LEVEL);
        logger->addAppender(getAppender("playground.log"));

        LoggerFactory::loggers.push_back(std::unique_ptr<Logger>(logger));

        return logger;
    }
};
#endif /* LOGGER_H_ */
