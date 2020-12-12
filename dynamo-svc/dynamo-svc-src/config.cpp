/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include "vtspch.h"
#include "config.h"

bool ConfigurationManager::load(int argc, char* argv[]) {

	// Load data from config file
	const std::string name("config.json");
	std::ifstream f(name.c_str());
	if (!f.good()) {
		std::cout << "[Error - ConfigManager::load] - Configuration file not found. Exiting..." << std::endl;
		exit(EXIT_FAILURE);
	}
	try {
		const auto j = nlohmann::json::parse(f, nullptr, true, true);
		const std::map<std::string, std::string> mp = j;
		this->configuration_table = mp;
	} catch (nlohmann::json::exception& e) {
		std::cout << "[Error - ConfigManager::load] - Failed to parse Configuration file: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	f.close();

	// If the program is started with two commandline args being "ip" and "port"
	// then parse them and override those that were loaded in the config file
	if (argc == 1 ) {
		return true;
	}

	if (argc == 3) {
		std::vector<std::string> all_args;

		try {
			all_args.assign(argv + 1, argv + argc);

			this->configuration_table [ "SERVER_IP" ] = all_args.at(0);
			this->configuration_table [ "SERVER_PORT" ] = all_args.at(1);
		} catch (std::exception& e) {
			std::cout << e.what();
		}
	} else {
		std::cout << "[Error - ConfigManager::load] - proper usage is -> ./vts \"ip\" \"port\" or no args. Ignoring input..." << std::endl;
	}

	return true;
}

bool ConfigurationManager::reload(const std::string& new_configuration) {	
	try {
		const auto j = nlohmann::json::parse(new_configuration, nullptr, true, true);
		const std::map<std::string, std::string> mp = j;
		this->configuration_table = mp;
		return true;
	} catch (nlohmann::json::exception& e) {
		std::cout << "[Error - ConfigManager::load] - Malformed configuration request: " << e.what() << std::endl;
		return false;
	}
}
