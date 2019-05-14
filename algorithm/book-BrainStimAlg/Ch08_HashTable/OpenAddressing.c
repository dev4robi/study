#include "OpenAddressing.h"

HashTable* OAHT_CreateHashTable(int TableSize)
{
    HashTable* HT = (HashTable*)malloc(sizeof(HashTable));
    HT->Table = (ElementType*)malloc(sizeof(ElementType) * TableSize);

    memset(HT->Table, 0, sizeof(ElementType) * TableSize);

    HT->TableSize = TableSize;
    HT->OccupiedCount = 0;

    return HT;
}

void OAHT_DestroyHashTable(HashTable* HT)
{
    int i = 0;

    for (i = 0; i < HT->TableSize; ++i)
    {
        OAHT_ClearElement(&(HT->Table[i]));
    }

    free(HT->Table);
    free(HT);
}

void OAHT_ClearElement(ElementType* Element)
{
    if (Element->Status == EMPTY)
        return;

    free(Element->Key);
    free(Element->Value);
}

void OAHT_Set(HashTable** HT, KeyType Key, ValueType Value)
{
    int     KeyLen, Address, StepSize;
    double  Usage;

    Usage = (double)(*HT)->OccupiedCount / (*HT)->TableSize;

    if (Usage > 0.5)
    {
        OAHT_Rehash(HT);
    }

    KeyLen   = strlen(Key);
    Address  = OAHT_Hash(Key, KeyLen, (*HT)->TableSize);
    StepSize = OAHT_Hash2(Key, KeyLen, (*HT)->TableSize);

    while ((*HT)->Table[Address].Status != EMPTY && strcmp((*HT)->Table[Address].Key, Key) != 0)
    {
        fprintf(stdout, "Collision occured! : Key(%s), Address(%d), StepSize(%d)\n", Key, Address, StepSize);
        Address = (Address + StepSize) % (*HT)->TableSize;
    }

    (*HT)->Table[Address].Key = (char*)malloc(sizeof(char) * (KeyLen + 1));
    strcpy((*HT)->Table[Address].Key, Key);

    (*HT)->Table[Address].Value = (char*)malloc(sizeof(char) * (strlen(Value) + 1));
    strcpy((*HT)->Table[Address].Value, Value);

    (*HT)->Table[Address].Status = OCCUPIED;
    ++((*HT)->OccupiedCount);

    fprintf(stdout, "Key(%s) entered at address(%d)\n", Key, Address);
}

ValueType OAHT_Get(HashTable* HT, KeyType Key)
{
    int KeyLen = strlen(Key);
    int Address = OAHT_Hash(Key, KeyLen, HT->TableSize);
    int StepSize = OAHT_Hash2(Key, KeyLen, HT->TableSize);

    while (HT->Table[Address].Status != EMPTY && strcmp(HT->Table[Address].Key, Key) != 0)
    {
        Address = (Address + StepSize) % HT->TableSize;
    }

    return HT->Table[Address].Value;
}

int OAHT_Hash(KeyType Key, int KeyLength, int TableSize)
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

int OAHT_Hash2(KeyType Key, int KeyLength, int TableSize)
{
    int i = 0;
    int HashValue = 0;

    for (i = 0; i < KeyLength; ++i)
    {
        HashValue = (HashValue << 2) + Key[i];
    }

    // 나머지 연산으로 같은 값을 가지지 않도록 테이블 크기에서 3을 뺀 수로 나눈다
    HashValue = HashValue % (TableSize - 3);

    return HashValue + 1;
}

void OAHT_Rehash(HashTable** HT)
{
    int i = 0;
    ElementType* OldTable = (*HT)->Table;

    HashTable* NewHT = OAHT_CreateHashTable((*HT)->TableSize * 2);

    fprintf(stdout, "Rehashed. New table size is : %d\n", NewHT->TableSize);

    for (i = 0; i < (*HT)->TableSize; ++i)
    {
        if (OldTable[i].Status == OCCUPIED)
        {
            OAHT_Set(&NewHT, OldTable[i].Key, OldTable[i].Value);
        }
    }

    OAHT_DestroyHashTable((*HT));
    (*HT) = NewHT;
}

static int OAHT_Test_main()
{
    HashTable* HT = OAHT_CreateHashTable(12289);

    OAHT_Set(&HT, "MSFT",     "Microsoft Corporation");
    OAHT_Set(&HT, "JAVA",     "Sun Microsystems");
    OAHT_Set(&HT, "REDH",     "Red hat Linux");
    OAHT_Set(&HT, "APAC",     "Apache Org");
    OAHT_Set(&HT, "ZYMZZ",    "Unisy Ops Check"); // APAC 과 충돌
    OAHT_Set(&HT, "IBM",      "IBM Ltd.");
    OAHT_Set(&HT, "ORCL",     "Oracle Corporation");
    OAHT_Set(&HT, "CSCO",     "Cisco Systems, Inc.");
    OAHT_Set(&HT, "GOOG",     "Google Inc.");
    OAHT_Set(&HT, "YHOO",     "Yahoo! Inc.");
    OAHT_Set(&HT, "NOVL",     "Novell, Inc.");

    fprintf(stdout, "\n");
    fprintf(stdout, "Key:%s, Value:%s\n", "MSFT", OAHT_Get(HT, "MSFT"));
    fprintf(stdout, "Key:%s, Value:%s\n", "JAVA", OAHT_Get(HT, "JAVA"));
    fprintf(stdout, "Key:%s, Value:%s\n", "REDH", OAHT_Get(HT, "REDH"));
    fprintf(stdout, "Key:%s, Value:%s\n", "ZYMZZ", OAHT_Get(HT, "ZYMZZ"));
    fprintf(stdout, "Key:%s, Value:%s\n", "IBM", OAHT_Get(HT, "IBM"));
    fprintf(stdout, "Key:%s, Value:%s\n", "ORCL", OAHT_Get(HT, "ORCL"));
    fprintf(stdout, "Key:%s, Value:%s\n", "CSCO", OAHT_Get(HT, "CSCO"));
    fprintf(stdout, "Key:%s, Value:%s\n", "GOOG", OAHT_Get(HT, "GOOG"));
    fprintf(stdout, "Key:%s, Value:%s\n", "YHOO", OAHT_Get(HT, "YHOO"));
    fprintf(stdout, "Key:%s, Value:%s\n", "NOVL", OAHT_Get(HT, "NOVL"));

    OAHT_DestroyHashTable(HT);

    return 0;
}

int main()
{
    return OAHT_Test_main();
}