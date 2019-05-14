#include "Chaning.h"

HashTable* CHT_CreateHashTable(int TableSize)
{
    HashTable* HT = (HashTable*)malloc(sizeof(HashTable));
    HT->Table = (List*)malloc(sizeof(List) * TableSize);

    memset(HT->Table, 0, sizeof(List) * TableSize);

    HT->TableSize = TableSize;

    return HT;
}

void CHT_DestroyHashTable(HashTable* HT)
{
    int i = 0;

    for (i = 0; i < HT->TableSize; ++i)
    {
        List L = HT->Table[i];
        CHT_DestroyList(L);
    }

    free(HT->Table);
    free(HT);
}

Node* CHT_CreateNode(KeyType Key, ValueType Value)
{
    Node* NewNode = (Node*)malloc(sizeof(Node));

    NewNode->Key = (char*)malloc(sizeof(char) * (strlen(Key) + 1));
    strcpy(NewNode->Key, Key);

    NewNode->Value = (char*)malloc(sizeof(char) * (strlen(Value) + 1));
    strcpy(NewNode->Value, Value);
    NewNode->Next = NULL;

    return NewNode;
}

void CHT_DestroyNode(Node* TheNode)
{
    free(TheNode->Key);
    free(TheNode->Value);
    free(TheNode);
}

void CHT_DestroyList(List L)
{
    if (L == NULL)
        return;

    if (L->Next != NULL)
        CHT_DestroyList(L->Next);

    CHT_DestroyNode(L);
}

void CHT_Set(HashTable* HT, KeyType Key, ValueType Value)
{
    int Address = CHT_Hash(Key, strlen(Key), HT->TableSize);
    Node* NewNode = CHT_CreateNode(Key, Value);

    if (HT->Table[Address] == NULL)
    {
        HT->Table[Address] = NewNode;
    }
    else
    {
        List L = HT->Table[Address];
        NewNode->Next = L;
        HT->Table[Address] = NewNode;

        fprintf(stdout, "Collision occured : Key(%s), Address(%d)\n", Key, Address);
    }
}

ValueType CHT_Get(HashTable* HT, KeyType Key)
{
    int Address = CHT_Hash(Key, strlen(Key), HT->TableSize);
    List TheList = HT->Table[Address];
    List Target = NULL;

    if (TheList == NULL)
        return NULL;

    while (1)
    {
        if (strcmp(TheList->Key, Key) == 0)
        {
            Target = TheList;
            break;
        }

        if (TheList->Next == NULL)
            return NULL;
        else
            TheList = TheList->Next;
    }

    return Target->Value;
}

int CHT_Hash(KeyType Key, int KeyLength, int TableSize)
{
    int i = 0;
    int HashValue = 0;

    for (i = 0; i < KeyLength; ++i)
    {
        HashValue = (HashValue << 3) + Key[i];
    }

    HashValue = HashValue % TableSize;

    return HashValue;
}

static int CHT_Test_main()
{
    HashTable* HT = CHT_CreateHashTable(12289);

    CHT_Set(HT, "MSFT",     "Microsoft Corporation");
    CHT_Set(HT, "JAVA",     "Sun Microsystems");
    CHT_Set(HT, "REDH",     "Red hat Linux");
    CHT_Set(HT, "APAC",     "Apache Org");
    CHT_Set(HT, "ZYMZZ",    "Unisy Ops Check"); // APAC 과 충돌
    CHT_Set(HT, "IBM",      "IBM Ltd.");
    CHT_Set(HT, "ORCL",     "Oracle Corporation");
    CHT_Set(HT, "CSCO",     "Cisco Systems, Inc.");
    CHT_Set(HT, "GOOG",     "Google Inc.");
    CHT_Set(HT, "YHOO",     "Yahoo! Inc.");
    CHT_Set(HT, "NOVL",     "Novell, Inc.");

    fprintf(stdout, "\n");
    fprintf(stdout, "Key:%s, Value:%s\n", "MSFT", CHT_Get(HT, "MSFT"));
    fprintf(stdout, "Key:%s, Value:%s\n", "JAVA", CHT_Get(HT, "JAVA"));
    fprintf(stdout, "Key:%s, Value:%s\n", "REDH", CHT_Get(HT, "REDH"));
    fprintf(stdout, "Key:%s, Value:%s\n", "ZYMZZ", CHT_Get(HT, "ZYMZZ"));
    fprintf(stdout, "Key:%s, Value:%s\n", "IBM", CHT_Get(HT, "IBM"));
    fprintf(stdout, "Key:%s, Value:%s\n", "ORCL", CHT_Get(HT, "ORCL"));
    fprintf(stdout, "Key:%s, Value:%s\n", "CSCO", CHT_Get(HT, "CSCO"));
    fprintf(stdout, "Key:%s, Value:%s\n", "GOOG", CHT_Get(HT, "GOOG"));
    fprintf(stdout, "Key:%s, Value:%s\n", "YHOO", CHT_Get(HT, "YHOO"));
    fprintf(stdout, "Key:%s, Value:%s\n", "NOVL", CHT_Get(HT, "NOVL"));

    CHT_DestroyHashTable(HT);

    return 0;
}

int main()
{
    return CHT_Test_main();
}