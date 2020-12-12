#include "vtspch.h"
#include "sqs_handler.h"

extern ConfigurationManager g_config;

std::vector<SQSMessage> SQS::receive_messages_sqs(const std::string& queue_url, const bool& delete_messages) {
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

	client_config.requestTimeoutMs = 30000;

	Aws::SQS::SQSClient sqs(credentials, client_config);

	Aws::SQS::Model::ReceiveMessageRequest rm_req;
	rm_req.SetQueueUrl(queue_url.c_str());
	rm_req.SetMaxNumberOfMessages(10);

	std::vector<SQSMessage> received_messages;

	auto rm_out = sqs.ReceiveMessage(rm_req);
	if (!rm_out.IsSuccess())
	{
		std::cout << "Error receiving message from queue " << queue_url << ": "
			<< rm_out.GetError().GetMessage() << std::endl;
		return received_messages;
	}

	const auto& messages = rm_out.GetResult().GetMessages();
	/*
	if (messages.size() == 0)
	{
		std::cout << "No messages received from queue " << queue_url <<
			std::endl;
		return received_messages;
	}
*/

	for (const auto& message : messages) {
		SQSMessage msg;
		msg.message_body = message.GetBody();
		msg.message_handler = message.GetReceiptHandle();
		msg.message_id = message.GetMessageId();
		received_messages.push_back(msg);
	}

	if (delete_messages) {
		for (const auto& message : received_messages) {
			Aws::SQS::Model::DeleteMessageRequest dm_req;
			dm_req.SetQueueUrl(queue_url.c_str());
			dm_req.SetReceiptHandle(message.message_handler.c_str());

			auto dm_out = sqs.DeleteMessage(dm_req);
			if (!dm_out.IsSuccess()) {
				std::cout << "Error deleting message " << message.message_id <<
					" from queue " << queue_url << ": " <<
					dm_out.GetError().GetMessage() << std::endl;
			}
		}
	}

	return received_messages;
}
