/*
 * Logger.cpp
 *
 *  Created on: Aug 16, 2022
 *      Author: leandro
 */


#include "Logger.h"

std::vector<std::unique_ptr<Logger>> LoggerFactory::loggers;
std::vector<std::unique_ptr<Appender>> LoggerFactory::appenders;
