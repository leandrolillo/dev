/*
 * FileParser.h
 *
 *  Created on: 03/02/2013
 *      Author: Lean
 */

#ifndef FILEPARSER_H_
#define FILEPARSER_H_

#include <JavaLike.h>
#include <Logger.h>
#include <InvalidArgumentException.h>
#include <string>
#include<stdio.h>

class FileParser {
public:
    static const String eof;
    static const String eol;

    static const char charEof = (char)-1;
    static const char charNewl = '\n';
    static const char charEol = '\0';

private:
    String filename;
    FILE *fileStream;
    Logger *logger = LoggerFactory::getLogger("resources/FileParser");

    const char *tokenSeparator;
    const char *blanks;
    char commentChar;

    unsigned int line;
    unsigned int column;

    //TODO: Move this to a stack and make its methods public
    unsigned int lineBackup;
    unsigned int columnBackup;
    fpos_t position;

    bool isInSet(char character, const char *set) {

        const char *currentTokenSeparator = set;
        while (*currentTokenSeparator != '\0') {
            if (*currentTokenSeparator == character) {
                return true;
            }

            currentTokenSeparator++;
        }

        return false;
    }

    bool isTokenSeparator(char character) {
        return isInSet(character, tokenSeparator) || character == FileParser::charEof ;
    }

    bool isBlank(char character) {
        return isInSet(character, blanks);
    }

    void takeUntilEOL() {
        char caracter;
        while ((caracter = takeByte()) != FileParser::charNewl && caracter != FileParser::charEof )
            ;
    }

    void takeBlanks() {
        while (isBlank(takeByte()))
            ;

        if (feof(getStream()))
            fseek(getStream(), 0, SEEK_END);
        else
            fseek(getStream(), -1, SEEK_CUR);
    }

    void takeBlanksAndComments() {
        char character;
        char exit;

        do {
            exit = true;
            while (isBlank(character = takeByte()))
                ;
            if (character == commentChar) {
                takeUntilEOL();
                exit = false;
            }
        } while (!exit);

        if (feof(getStream()))
            fseek(getStream(), 0, SEEK_END);
        else
            fseek(getStream(), -1, SEEK_CUR);
    }

    void popPosition() {
        line = lineBackup;
        column = columnBackup;

        if (fsetpos(getStream(), &position) != 0)
            throw new Exception("Could not restore stream position");
    }
    void pushPosition() {
        lineBackup = line;
        columnBackup = column;

        if (fgetpos(getStream(), &position) != 0)
            throw new Exception("Could not save stream position");

    }

public:
    FileParser(const String filename) {
        this->filename = filename;
        this->fileStream = null;

        this->setDefaultSpecialCharacters();

        position = -1;
        line = 1;
        column = 1;
        lineBackup = line;
        columnBackup = column;
    }

    ~FileParser() {
        this->close();
    }

    /**
     * returns the number of elements (of size_t) actually read. If that number < count then there was an error reading or reached eof.
     */
    size_t read(void *data, size_t size, size_t count) {
        return fread(data, size, count, getStream());
    }

    bool skip(size_t size, size_t count) {

        if (fseek(getStream(), size * count, SEEK_CUR) != 0) {
            if (feof(getStream()))
                fseek(getStream(), 0, SEEK_END);

            return false;
        }

        return true;
    }

    void close() {
        if (fileStream != null) {
            logger->verbose("Closing file [%s]", filename.c_str());
            fclose(fileStream);
            fileStream = null;
        }
    }

    const String& getFilename() const {
        return filename;
    }

    FILE* getStream() {
        if (fileStream == null) {
            if (filename == "")
                throw InvalidArgumentException("FileName can't be null");

            if ((fileStream = fopen(filename.c_str(), "rb")) == null) {
                logger->error("Error al abrir el archivo [%s]", filename.c_str());
                throw InvalidArgumentException("Error al abrir el archivo [%s]", filename.c_str());

            }

            logger->verbose("File opened for reading binary [%s]", filename.c_str());
        }
        return fileStream;
    }

    int peekByte() {
        pushPosition();
        int byte = takeByte();
        popPosition();

        return byte;
    }

    int takeByte() {
        int byte = fgetc(getStream());
        //logger->info("Took byte [%d] ['%c'] from %s", byte, byte, getFilename().c_str());
        if ((char) byte == FileParser::charNewl) {
            line++;
            column = 0;
        } else if ((char) byte != '\r' && (char) byte != FileParser::charEof) {
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
    String takeLine() {
        String line;
        char caracter[2];
        caracter[1] = FileParser::charEof;

        while ((caracter[0] = takeByte()) != FileParser::charNewl && caracter[0] != FileParser::charEof )
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
        char tokenBuffer[256] = "";

        char *token = tokenBuffer;

        unsigned short longitud = 0;

        takeBlanksAndComments();

        while (!isTokenSeparator(*token = takeByte())) {
            token++;
            longitud++;
        }

        if (longitud == 0)
            *(token + 1) = FileParser::charEol;
        else {
            *token = FileParser::charEol;
            fseek(getStream(), -1, SEEK_CUR);
        }

        return (tokenBuffer);
    }

    unsigned int getColumn() const {
        return column;
    }

    unsigned int getLine() const {
        return line;
    }

    const char* getTokenSeparator() const {
        return tokenSeparator;
    }

    void setTokenSeparator(const char *tokenSeparator) {
        this->tokenSeparator = tokenSeparator;
    }

    void setDefaultSpecialCharacters() {
        tokenSeparator = "	# ()[]{},.:;<>+-*/^®=|&!ø?\n\r\"\'\\"; // + std::to_string((char)-1)).c_str();
        blanks = " \n\r\t";
        commentChar = '#';
    }
    unsigned int size() {
        pushPosition();
        fseek(getStream(), 0, SEEK_END);
        unsigned int size = ftell(getStream());
        popPosition();
        return size;
    }
};

#endif /* FILEPARSER_H_ */
