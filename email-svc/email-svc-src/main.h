/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef MAIN_H
#define MAIN_H

#ifdef _WIN32
#define _WIN32_WINNT 0x0a00
#endif

#ifdef _MSC_VER
#pragma warning(disable:4267) // 'var' : conversion from 'size_t' to 'type', possible loss of data
#endif

#include <httplib.h>

#include "config.h"

#include "sqs_handler.h"

int main(int argc, char* argv[]);
void register_handlers(httplib::Server& svr); 
void sqs_listener();

#endif