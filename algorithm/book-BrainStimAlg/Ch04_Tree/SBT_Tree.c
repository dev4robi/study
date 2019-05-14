#include "SBT_Tree.h"

SBTNode* SBT_CreateNode(ElementType NewData)
{
    SBTNode* NewNode = (SBTNode*)malloc(sizeof(SBTNode));
    NewNode->Left = NULL;
    NewNode->Right = NULL;
    NewNode->Data = NewData;

    return NewNode;
}

void SBT_DestroyNode(SBTNode* Node)
{
    if (Node != NULL) free(Node);
}

void SBT_DestroyTree(SBTNode* Root)
{
    if (Root == NULL) return;

    SBT_DestroyTree(Root->Left);
    SBT_DestroyTree(Root->Right);
    SBT_DestroyNode(Root);
}

void SBT_PreorderPrintTree(SBTNode* Node) // 전위
{
    if (Node == NULL) return;

    fprintf(stdout, " %c", Node->Data);
    SBT_PreorderPrintTree(Node->Left);
    SBT_PreorderPrintTree(Node->Right);
}

void SBT_InorderPrintTree(SBTNode* Node) // 중위
{
    if (Node == NULL) return;

    SBT_PreorderPrintTree(Node->Left);
    fprintf(stdout, " %c", Node->Data);
    SBT_PreorderPrintTree(Node->Right);
}

void SBT_PostorderPrintTree(SBTNode* Node) // 후위
{
    if (Node == NULL) return;

    SBT_PreorderPrintTree(Node->Left);
    SBT_PreorderPrintTree(Node->Right);
    fprintf(stdout, " %c", Node->Data);
}

static int SBT_Test_main()
{
    SBTNode* A = SBT_CreateNode('A');
    SBTNode* B = SBT_CreateNode('B');
    SBTNode* C = SBT_CreateNode('C');
    SBTNode* D = SBT_CreateNode('D');
    SBTNode* E = SBT_CreateNode('E');
    SBTNode* F = SBT_CreateNode('F');
    SBTNode* G = SBT_CreateNode('G');

    A->Left = B;        //       A 
    B->Left = C;        //   B       E
    B->Right = D;       // C   D   F   G

    A->Right = E;
    E->Left = F;
    E->Right = G;

    fprintf(stdout, "Preorder...\n");
    SBT_PreorderPrintTree(A);
    fprintf(stdout, "\n\n");

    fprintf(stdout, "Inorder...\n");
    SBT_InorderPrintTree(A);
    fprintf(stdout, "\n\n");

    fprintf(stdout, "Postorder...\n");
    SBT_PostorderPrintTree(A);
    fprintf(stdout, "\n\n");

    return 0;
}

int main()
{
    return SBT_Test_main();
}