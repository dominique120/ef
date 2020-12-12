#include "vtspch.h"
#include "sqs_handler.h"

extern ConfigurationManager g_config;

void SQS::send_message_sqs(const std::string& queue_url, const std::string& msg_body) {
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

    Aws::SQS::Model::SendMessageRequest sm_req;
    sm_req.SetQueueUrl(queue_url.c_str());
    sm_req.SetMessageBody(msg_body.c_str());

    auto sm_out = sqs.SendMessage(sm_req);
    if (sm_out.IsSuccess()) {
        std::cout << "Successfully sent message to " << queue_url <<
            std::endl;
    }
    else {
        std::cout << "Error sending message to " << queue_url << ": " <<
            sm_out.GetError().GetMessage() << std::endl;
    }
}
