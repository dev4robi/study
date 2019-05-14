#define REQUEST 1
#define RESPONSE 2

/* request를 담은 스트럭처 */
typedef struct
{
    int reqType;
    char reqData[10];
} RequestType;

/* response를 담은 스트럭처 */
typedef struct
{
    int isDone;
    char resData[30];
} ResponseType;

/* request 또는 response를 담게될 message 스트럭처 */
typedef struct
{
    /* messageType에 따라 union 속의 타입이 결정 */
    unsigned int messageType;
    union
    {
        /* messageType = REUQEST 인 경우 */
        RequestType request;
        /*messageType = RESPONSE 인 경우 */
        ResponseType response;
    } uType;
} MessageType;

MessageType* setMsg(MessageType *message);
void showMsg(MessageType *message);