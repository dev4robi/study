#include <stdio.h>
#include <string.h>
#include "msgType.h"

int main(int argc, char *argv[])
{
    MessageType message;

    if (argc < 2)
    {
        printf("메시지 타입. request:req, response:res\n");
        return 0;
    }

    if (!strncmp(argv[1], "req", 3))
    {
        message.messageType = REQUEST;
        setMsg(&message);
    }
    else if (!strncmp(argv[1], "res", 3))
    {
        message.messageType = RESPONSE;
        setMsg(&message);
    }
    else
    {
        printf("Somthing is wrong!\n");
    }

    showMsg(&message);
    return 0;
}