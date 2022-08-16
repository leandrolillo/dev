/*
 * Logger.cpp
 *
 *  Created on: Aug 16, 2022
 *      Author: leandro
 */


#include "Logger.h"

std::vector<Logger *> LoggerFactory::loggers;
std::vector<Appender *> LoggerFactory::appenders;
