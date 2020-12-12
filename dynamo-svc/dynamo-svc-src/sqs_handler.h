/*
 * Copyright Dominique Verellen. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#ifndef SQS_HANDLER_H
#define SQS_HANDLER_H

#include "config.h"

#include <aws/core/Aws.h>
#include <aws/sqs/SQSClient.h>
#include <aws/sqs/model/SendMessageRequest.h>
#include <aws/sqs/model/SendMessageResult.h>
#include <aws/sqs/model/ReceiveMessageRequest.h>
#include <aws/sqs/model/ReceiveMessageResult.h>
#include <aws/sqs/model/DeleteMessageRequest.h>
#include <aws/core/auth/AWSCredentials.h>


class SQS {
public:
	static void send_message_sqs(const std::string& queue_url, const std::string& body);
};
#endif
