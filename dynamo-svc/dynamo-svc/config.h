/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef CONFIG_H
#define CONFIG_H

class ConfigurationManager {
public:

	bool load(int argc, char* argv[]);
	bool reload(const std::string& new_configuration);

	std::string operator [] (const std::string& config_name) {
		return this->configuration_table.at(config_name);
	}

private:
	std::map<std::string, std::string> configuration_table;
};

#endif