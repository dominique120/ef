/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef DYNAMO_H
#define DYNAMO_H
#if defined(DB_DYNAMO)
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentials.h>
#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/GetItemRequest.h>
#include <aws/dynamodb/model/QueryRequest.h>
#include <aws/dynamodb/model/QueryResult.h>
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/PutItemResult.h>
#include <aws/dynamodb/model/UpdateItemRequest.h>
#include <aws/dynamodb/model/UpdateItemResult.h>
#include <aws/dynamodb/model/DeleteItemResult.h>
#include <aws/dynamodb/model/DeleteItemRequest.h>
#include <aws/dynamodb/model/ScanRequest.h>
#include <aws/dynamodb/model/ScanResult.h>

#include "config.h"

class DynamoDB {
public:
	static std::map<std::string, std::string> get_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value);
	static bool update_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value, const std::string& request_body);
	static bool new_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value, const std::string& request_body);
	static bool delete_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value);
	static std::string scan_table_items_dynamo(const Aws::String& table_name);
	static std::string scan_table_items_filer_dynamo(const Aws::String& table_name, const std::map<std::string, std::string>& conditions_and_values);
	static std::string query_index(const Aws::String& table_name, const Aws::String& partition_key, const Aws::String& match);
};
#endif

#endif