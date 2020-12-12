/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <httplib.h>

class Logger {
public:

	static void log_event(const httplib::Request& req, const httplib::Response& res);

};

#endif