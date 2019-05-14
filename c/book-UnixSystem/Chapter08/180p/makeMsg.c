#include <stdio.h>
#include <string.h>
#include "msgType.h"

MessageType* setMsg(MessageType *message)
{
    if (message->messageType == REQUEST)
    {
        message->uType.request.reqType = 1;
        strcpy(message->uType.request.reqData, "Request.\0");
        return message;
    }
    else if (message->messageType == RESPONSE)
    {
        message->uType.response.isDone = 1;
        strcpy(message->uType.response.resData, "Result 1.Data:3, 2.Number:10...\0");
        return message;
    }

    return 0;
}

void showMsg(MessageType *message)
{
    if (message->messageType == REQUEST)
    {
        printf("Request Type: %d\n", message->uType.request.reqType);
        printf("Request Data: %s\n", message->uType.request.reqData);
    }
    else if (message->messageType == RESPONSE)
    {
        printf("Response is done: %d\n", message->uType.response.isDone);
        printf("Response Data: %s\n", message->uType.response.resData);
    }
}