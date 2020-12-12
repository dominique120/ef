/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "db_dynamo.h"

#if defined(DB_DYNAMO)
extern ConfigurationManager g_config;

bool DynamoDB::new_item_dynamo(const Aws::String& table_name, const Aws::String& key_name, const Aws::String& key_value, const std::string& request_body) {
	Aws::Auth::AWSCredentials credentials;
	credentials.SetAWSAccessKeyId(Aws::String(g_config["AWS_ACCESS_KEY"].c_str()));
	credentials.SetAWSSecretKey(Aws::String(g_config["AWS_SECRET_KEY"].c_str()));
	bool use_token;
	std::istringstream(g_config["AWS_USE_SESSION_TOKEN"]) >> std::boolalpha >> use_token;
	if (use_token) {
		credentials.SetSessionToken(Aws::String(g_config["AWS_SESSION_TOKEN"].c_str()));
	}

	Aws::Client::ClientConfiguration client_config;
	client_config.region = g_config["AWS_REGION"].c_str();
	Aws::DynamoDB::DynamoDBClient dynamo_client(credentials, client_config);

	const Aws::String endpoint(g_config["AWS_DYNAMODB_ENDPOINT"].c_str());
	dynamo_client.OverrideEndpoint(endpoint);

	nlohmann::json j = nlohmann::json::parse(request_body);

	Aws::DynamoDB::Model::PutItemRequest pir;
	pir.SetTableName(table_name);

	/* START -- NEW SECTION FOR TESTING*/
	for (const auto& element : j.items()) {
		Aws::DynamoDB::Model::AttributeValue attribute_value;
		if (element.value().is_number_integer()) {
			attribute_value.SetN(element.value().get<int>());
		}
		else if (element.value().is_number_float()) {
			attribute_value.SetN(element.value().get<float>());
		}
		else if (element.value().is_boolean()) {
			attribute_value.SetB(element.value().get<bool>());
		}
		else if (element.value().is_string()) {
			attribute_value.SetS(element.value().get<std::string>().c_str());
		}
		else if (element.value().is_object()) {

			Aws::Map<Aws::String, const std::shared_ptr<Aws::DynamoDB::Model::AttributeValue>> exchangeTransactionObj;
			Aws::Map<Aws::String, const std::shared_ptr<Aws::DynamoDB::Model::AttributeValue>> fromObj;
			Aws::Map<Aws::String, const std::shared_ptr<Aws::DynamoDB::Model::AttributeValue>> toObj;

			Aws::DynamoDB::Model::AttributeValue to_local_attr_val;
			Aws::DynamoDB::Model::AttributeValue from_local_attr_val;
			Aws::DynamoDB::Model::AttributeValue to_exchangeTransactionAttr;
			Aws::DynamoDB::Model::AttributeValue from_exchangeTransactionAttr;

			for (const auto& from_inner_elem : j["exchangeTransaction"]["from"].items()) {
				if (from_inner_elem.value().is_number_float()) {
					from_local_attr_val.SetN(from_inner_elem.value().get<float>());
				}
				else if (from_inner_elem.value().is_string()) {
					from_local_attr_val.SetS(from_inner_elem.value().get<std::string>().c_str());
				}
				fromObj.insert(std::pair<Aws::String, const std::shared_ptr<Aws::DynamoDB::Model::AttributeValue>>(from_inner_elem.key().c_str(), std::make_shared<Aws::DynamoDB::Model::AttributeValue>(from_local_attr_val)));
			}
			from_exchangeTransactionAttr.SetM(fromObj);
			exchangeTransactionObj.insert(std::pair<Aws::String, const std::shared_ptr<Aws::DynamoDB::Model::AttributeValue>>("from", std::make_shared<Aws::DynamoDB::Model::AttributeValue>(from_exchangeTransactionAttr)));
			
			for (const auto& to_inner_elem : j["exchangeTransaction"]["to"].items()) {
				if (to_inner_elem.value().is_number_float()) {
					to_local_attr_val.SetN(to_inner_elem.value().get<float>());
				}
				else if (to_inner_elem.value().is_string()) {
					to_local_attr_val.SetS(to_inner_elem.value().get<std::string>().c_str());
				}
				toObj.insert(std::pair<Aws::String, const std::shared_ptr<Aws::DynamoDB::Model::AttributeValue>>(to_inner_elem.key().c_str(), std::make_shared<Aws::DynamoDB::Model::AttributeValue>(to_local_attr_val)));
			}
			to_exchangeTransactionAttr.SetM(toObj);
			exchangeTransactionObj.insert(std::pair<Aws::String, const std::shared_ptr<Aws::DynamoDB::Model::AttributeValue>>("to", std::make_shared<Aws::DynamoDB::Model::AttributeValue>(to_exchangeTransactionAttr)));

			attribute_value.SetM(exchangeTransactionObj);
		}
		pir.AddItem(element.key().c_str(), attribute_value);
	}
	/* END -- NEW SECTION FOR TESTING*/


	// Add key
	Aws::DynamoDB::Model::AttributeValue attribute_value;
	attribute_value.SetS(key_value);
	pir.AddItem(key_name, attribute_value);

	const Aws::DynamoDB::Model::PutItemOutcome result = dynamo_client.PutItem(pir);
	if (!result.IsSuccess()) {
		std::cout << result.GetError().GetMessage() << std::endl;
		return false;
	}
	return true;
}

#endif