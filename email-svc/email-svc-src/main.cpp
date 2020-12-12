/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "main.h"

ConfigurationManager g_config;

int main(int argc, char* argv[]) {
	std::cout << "email sender service - Version 0.0.1a" << std::endl;

	std::cout << "A microservice written by Dominique Verellen." << std::endl;
	std::cout << "Contact: dominique120@live.com." << std::endl;
	std::cout << std::endl;

	std::cout << "Initializing - Loading Configuration." << std::endl;
	g_config.load(argc, argv);

	std::cout << "Initializing - Setting up AWS SDK." << std::endl;
	const Aws::SDKOptions options;
	Aws::InitAPI(options);

	std::cout << "Initializing - Starting email service." << std::endl;
	auto listen_future = std::async(std::launch::async, sqs_listener);

	std::cout << "Initializing - Registering handlers." << std::endl;
	httplib::Server server;
	register_handlers(server);

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
}

void sqs_listener() {
	while (true) {
		std::vector<SQSMessage> messages = SQS::receive_messages_sqs(g_config["AWS_SQS_QUEUE_URL"], true);
		
		for (const auto& element : messages) {
			try {
				const auto msg_body = nlohmann::json::parse(element.message_body);
				std::cout << "Se envio un email para la transaccion: " << msg_body["transactionID"] << "\n";
			}
			catch (const nlohmann::json::exception& e) {
				std::cout << "Se recibio un mensaje con un formato incorrecto." << "\n";
				std::cout << "Error: " << e.what() << "\n";
			}			
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}
