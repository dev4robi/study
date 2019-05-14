#include "LCRSTree.h"

LCRSNode* LCRS_CreateNode(ElementType NewData)
{
    LCRSNode* NewNode = (LCRSNode*)malloc(sizeof(LCRSNode));
    NewNode->LeftChild = NULL;
    NewNode->RightSibling = NULL;
    NewNode->Data = NewData;

    return NewNode;
}

void LCRS_DestoryNode(LCRSNode* Node)
{
    free(Node);
}

void LCRS_DestroyTree(LCRSNode* Root)
{
    if (Root->RightSibling != NULL)
    {
        LCRS_DestroyTree(Root->RightSibling);
    }

    if (Root->LeftChild != NULL)
    {
        LCRS_DestroyTree(Root->LeftChild);
    }

    Root->LeftChild = NULL;
    Root->RightSibling = NULL;

    LCRS_DestoryNode(Root);
}

void LCRS_AddChildNode(LCRSNode* ParentNode, LCRSNode* ChildNode)
{
    if (ParentNode->LeftChild == NULL)
    {
        ParentNode->LeftChild = ChildNode;
    }
    else
    {
        LCRSNode* TempNode = ParentNode->LeftChild;
        while (TempNode->RightSibling != NULL)
        {
            TempNode = TempNode->RightSibling;
        }

        TempNode->RightSibling = ChildNode;
    }
}

void LCRS_PrintTree(LCRSNode* Node, int Depth)
{
    int i = 0;

    for (i = 0; i < Depth; ++i)
    {
        fprintf(stdout, " ");
    }

    fprintf(stdout, "%c\n", Node->Data);

    if (Node->LeftChild != NULL)
    {
        LCRS_PrintTree(Node->LeftChild, Depth + 1);
    }

    if (Node->RightSibling != NULL)
    {
        LCRS_PrintTree(Node->RightSibling, Depth);
    }
}

void LCRS_PrintNodeAtLevel(LCRSNode* Root, int Level)
{
    if (Root == NULL)
    {
        return;
    }

    if (Level == 0)
    {
        LCRSNode* TempNode = Root;
        while (TempNode != NULL)
        {
            fprintf(stdout, "%c ", TempNode->Data);
            TempNode = TempNode->RightSibling;
        }

        return;
    }

    if (Root->LeftChild != NULL && Level > 0)
    {
        LCRS_PrintNodeAtLevel(Root->LeftChild, Level - 1);
    }

    if (Root->RightSibling != NULL)
    {
       LCRS_PrintNodeAtLevel(Root->RightSibling, Level);
    }
}

static int LCRS_Test_main()
{
    int i;
    LCRSNode* B = LCRS_CreateNode('B');
    LCRSNode* C = LCRS_CreateNode('C');
    LCRSNode* D = LCRS_CreateNode('D');
    LCRSNode* E = LCRS_CreateNode('E');
    LCRSNode* F = LCRS_CreateNode('F');
    LCRSNode* G = LCRS_CreateNode('G');
    LCRSNode* H = LCRS_CreateNode('H');
    LCRSNode* I = LCRS_CreateNode('I');
    LCRSNode* J = LCRS_CreateNode('J');
    LCRSNode* K = LCRS_CreateNode('K');

    LCRSNode* Root = LCRS_CreateNode('A');
        LCRS_AddChildNode(Root, B);
            LCRS_AddChildNode(B, C);
            LCRS_AddChildNode(B, D);
                LCRS_AddChildNode(D, E);
                LCRS_AddChildNode(D, F);
        LCRS_AddChildNode(Root, G);
            LCRS_AddChildNode(G, H);
        LCRS_AddChildNode(Root, I);
            LCRS_AddChildNode(I, J);
                LCRS_AddChildNode(J, K);

    LCRS_PrintTree(Root, 0);
    
    for (i = 0; i < 6; ++i)
    {
        LCRS_PrintNodeAtLevel(Root, i);
        fprintf(stdout, "\n");
    }

    LCRS_DestroyTree(Root);

    return 0;
}

int main()
{
    return LCRS_Test_main();
}