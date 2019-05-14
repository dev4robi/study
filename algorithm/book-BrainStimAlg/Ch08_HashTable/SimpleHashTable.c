#include "SimpleHashTable.h"

HashTable* SHT_CreateHashTable(int TableSize)
{
    HashTable* HT   = (HashTable*)malloc(sizeof(HashTable));
    HT->Table       = (Node*)malloc(sizeof(Node) * TableSize);
    HT->TableSize   = TableSize;

    return HT;
}

void SHT_Set(HashTable* HT, KeyType Key, ValueType Value)
{
    int Address = SHT_Hash(Key, HT->TableSize);

    HT->Table[Address].Key = Key;
    HT->Table[Address].Value = Value;
}

ValueType SHT_Get(HashTable* HT, KeyType Key)
{
    int Address = SHT_Hash(Key, HT->TableSize);

    return HT->Table[Address].Value;
}

void SHT_DestroyHashTable(HashTable* HT)
{
    free(HT->Table);
    free(HT);
}

int SHT_Hash(KeyType Key, int TableSize)
{
    return Key % TableSize;
}

static int SHT_Test_main()
{
    HashTable* HT = SHT_CreateHashTable(193);

    SHT_Set(HT, 418, 32114);
    SHT_Set(HT, 9, 514);
    SHT_Set(HT, 27, 8917);
    SHT_Set(HT, 1031, 268);

    fprintf(stdout, "Key:%d, Value:%d\n", 418, SHT_Get(HT, 418));
    fprintf(stdout, "Key:%d, Value:%d\n", 9, SHT_Get(HT, 9));
    fprintf(stdout, "Key:%d, Value:%d\n", 27, SHT_Get(HT, 27));
    fprintf(stdout, "Key:%d, Value:%d\n", 1031, SHT_Get(HT, 1031));

    SHT_DestroyHashTable(HT);

    return 0;
}

int main()
{
    return SHT_Test_main();
}