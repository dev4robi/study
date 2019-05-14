#include "ArrayStack.h"

void AS_CreateStack(ArrayStack** Stack, int Capacity)
{
    (*Stack)            = (ArrayStack*)malloc(sizeof(ArrayStack));
    (*Stack)->Nodes     = (Node*)malloc(sizeof(Node) * Capacity);
    (*Stack)->Capacity  = Capacity;
    (*Stack)->Top       = 0;
}

void AS_DestroyStack(ArrayStack* Stack)
{
    if (Stack != NULL)
    {
        free(Stack->Nodes);
        free(Stack);
    }
}

void AS_Push(ArrayStack* Stack, ElementType Data)
{
    int Position = Stack->Top;

    Stack->Nodes[Position].Data = Data;
    Stack->Top++;
}

ElementType AS_Pop(ArrayStack* Stack)
{
    int Position = --(Stack->Top);
    return Stack->Nodes[Position].Data;
}

ElementType AS_Top(ArrayStack* Stack)
{
    int Position = Stack->Top - 1;
    return Stack->Nodes[Position].Data;
}

int AS_GetSize(ArrayStack* Stack)
{
    return Stack->Top;
}

int AS_IsEmpty(ArrayStack* Stack)
{
    return (Stack->Top == 0);
}

int AS_IsFull(ArrayStack* Stack)
{
    return ((Stack->Top) >= Stack->Capacity);
}

static int AL_Test_main()
{
    int i = 0;
    ArrayStack* Stack = NULL;

    AS_CreateStack(&Stack, 4);

    AS_Push(Stack, 3);
    AS_Push(Stack, 37);
    AS_Push(Stack, 11);
    AS_Push(Stack, 12);

    fprintf(stdout, "Capacity: %d, Size: %d, Top: %d\n", Stack->Capacity, AS_GetSize(Stack), AS_Top(Stack));

    if (AS_IsFull(Stack))
        fprintf(stdout, "Stack Is Full.\n");
    else
        fprintf(stdout, "Stack Is Not Full.\n");

    for (i = 0; i < 4; ++i)
    {
        if (AS_IsEmpty(Stack))
            break;

        fprintf(stdout, "Popped %d, ", AS_Pop(Stack));

        if (!AS_IsEmpty(Stack))
            fprintf(stdout, "CurrentTop: %d\n", AS_Top(Stack));
        else
            fprintf(stdout, "Stack Is Empty.\n");
    }

    AS_DestroyStack(Stack);

    return 0;
}

int main(int argc, char** argv)
{
    return AL_Test_main();
}