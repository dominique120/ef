/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "main.h"

ConfigurationManager g_config;

int main(int argc, char* argv[]) {
	std::cout << "Vitanza Service - Version 0.5.2a" << std::endl;
	std::cout << "Compiled with " << BOOST_COMPILER << std::endl;
	std::cout << "Compiled on " << __DATE__ << ' ' << __TIME__ << " for platform ";
	std::cout << BOOST_PLATFORM << "." << std::endl;


	std::cout << "Compiled for: "
#if defined(DB_DYNAMO)
		<< "DynamoDB." << std::endl;
	std::cout << "Aws sdk version: " << AWS_SDK_VERSION_STRING << std::endl;
#elif defined(DB_MYSQL)
		<< "MySQL." << std::endl;
#endif

	std::cout << "A microservice written by Dominique Verellen." << std::endl;
	std::cout << "Contact: dominique120@live.com." << std::endl;
	std::cout << std::endl;

	std::cout << "Initializing - Loading Configuration." << std::endl;
	g_config.load(argc, argv);

	std::cout << "Initializing - Setting up AWS SDK." << std::endl;
	const Aws::SDKOptions options;
	Aws::InitAPI(options);

	std::cout << "Initializing - Registering handlers." << std::endl;
	httplib::Server server;
	register_handlers(server);

	server.set_logger([](const auto& req, const auto& res) {
		Logger::log_event(req, res);
		});

	std::cout << "Init done - Local address: " << g_config["SERVER_IP"] << " bound using port " << g_config["SERVER_PORT"] << std::endl;
	server.listen(g_config["SERVER_IP"].c_str(), std::stoi(g_config["SERVER_PORT"]));

	Aws::ShutdownAPI(options);
	std::cout << "Exiting..." << std::endl;
	return (EXIT_SUCCESS);
}

void register_handlers(httplib::Server& svr) {


	svr.Post("/config", [](const httplib::Request& req, httplib::Response& res) {
		if (g_config.reload(req.body)) {
			res.status = 201;
		}
		else {
			res.status = 400;
		}
		});

	svr.Get("/health", [](const httplib::Request& req, httplib::Response& res) {
		res.status = 200;
		});




	svr.Post("/exchange-money", [](const httplib::Request& req, httplib::Response& res) {
		const auto uuid = boost::uuids::random_generator()();
		const std::string id = boost::uuids::to_string(uuid);

		auto j = nlohmann::json::parse(req.body);
		j["transactionID"] = id;

		DynamoDB::new_item_dynamo("exchange-money", "transactionID", id.c_str(), j.dump());
		SQS::send_message_sqs(g_config["AWS_SQS_QUEUE_URL"], j.dump());

		res.status = 201;
		});

}
