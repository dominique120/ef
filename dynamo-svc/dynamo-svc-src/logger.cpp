/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "config.h"

#include "logger.h"

void Logger::log_event(const httplib::Request& req, const httplib::Response& res) {
	auto time = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(time);

	std::string t = std::ctime(&end_time);
	
	std::cout << req.method << " to: " << req.path << " - At: " << t.substr(0, t.length() - 1) <<
		" - Response: " << res.status << " - Source: " << req.remote_addr << ":" << req.remote_port << "\n";	
}
