/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "util.h"

extern "C" char* strptime(const char* s,
						  const char* f,
						  struct tm* tm) {
	std::istringstream input(s);
	input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
	input >> std::get_time(tm, f);
	if (input.fail()) {
		return nullptr;
	}
	return (char*)(s + input.tellg());
}

time_t String2time_t(const std::string& strDateTime) {
	tm t;
	strptime(strDateTime.c_str(), "%F %T", &t);
	return mktime(&t);
}