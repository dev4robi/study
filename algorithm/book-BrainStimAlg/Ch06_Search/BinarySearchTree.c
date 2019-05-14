#include "BinarySearchTree.h"

BSTNode* BST_CreateNode(ElementType NewData)
{
    BSTNode* NewNode = (BSTNode*)malloc(sizeof(BSTNode));
    NewNode->Left = NULL;
    NewNode->Right = NULL;
    NewNode->Data = NewData;

    return NewNode;
}

void BST_DestroyNode(BSTNode* Node)
{
    if (Node != NULL)
    {
        Node->Left = NULL;
        Node->Right = NULL;
        free(Node);
    }
}

void BST_DestroyTree(BSTNode* Tree)
{
    if (Tree->Right != NULL) BST_DestroyTree(Tree->Right);
    if (Tree->Left != NULL)  BST_DestroyTree(Tree->Left);
    
    BST_DestroyNode(Tree);
}

BSTNode* BST_SearchTree(BSTNode* Tree, ElementType Target)
{
    if (Tree == NULL) return NULL;

    if      (Tree->Data == Target) { return Tree; }
    else if (Tree->Data > Target)  { BST_SearchTree(Tree->Left, Target);  }
    else                           { BST_SearchTree(Tree->Right, Target); }
}

BSTNode* BST_SearchMinNode(BSTNode* Tree)
{
    if (Tree == NULL) return NULL;
    if (Tree->Left == NULL) return Tree;
    else return BST_SearchMinNode(Tree->Left);
}

void BST_InsertNode(BSTNode* Tree, BSTNode* Child)
{
    if (Tree->Data < Child->Data)
    {
        if (Tree->Right == NULL)
            Tree->Right = Child;
        else
            BST_InsertNode(Tree->Right, Child);
    }
    else if (Tree->Data >= Child->Data)
    {
        if (Tree->Left == NULL)
            Tree->Left = Child;
        else
            BST_InsertNode(Tree->Left, Child);
    }
}

BSTNode* BST_RemoveNode(BSTNode* Tree, BSTNode* Parent, ElementType Target)
{
    BSTNode* Removed = NULL;

    if (Tree == NULL) return NULL;

    if (Tree->Data > Target)
        Removed = BST_RemoveNode(Tree->Left, Tree, Target);
    else if (Tree->Data < Target)
        Removed = BST_RemoveNode(Tree->Right, Tree, Target);
    else // Tree->Data == Target : 값을 찾음
    {
        Removed = Tree;

        if (Tree->Left != NULL && Tree->Right != NULL) // Target노드의 자식이 둘
        {
            // 최소값을 찾아 삭제할 노드의 위치에 넣음
            BSTNode* MinNode = BST_SearchMinNode(Tree->Right);
            MinNode = BST_RemoveNode(Tree, NULL, MinNode->Data);
            Tree->Data = MinNode->Data;
        }
        else if (Tree->Left == NULL || Tree->Right == NULL) // Target노드의 자식이 하나
        {
            BSTNode* Temp = NULL;

            if (Tree->Left != NULL)
                Temp = Tree->Left;
            else
                Temp = Tree->Right;

            if (Parent->Left == Tree)
                Parent->Left = Temp;
            else
                Parent->Right = Temp;
        }
        else // Target노드의 자식이 없음 (잎 노드)
        {
            // 부모의 자식을 NULL로 설정 후 삭제
            if (Parent->Left == Tree) Parent->Left = NULL;
            else Parent->Right = NULL;
        }
    }

    return Removed;
}

void BST_InorderPrintTree(BSTNode* Node)
{
    if (Node == NULL) return;

    BST_InorderPrintTree(Node->Left);
    fprintf(stdout, "[%d]", Node->Data);
    BST_InorderPrintTree(Node->Right);
}

static int BST_Test_main()
{
    BSTNode* Tree = BST_CreateNode(123);
    BSTNode* Node = NULL;

    BST_InsertNode(Tree, BST_CreateNode(22));
    BST_InsertNode(Tree, BST_CreateNode(9918));
    BST_InsertNode(Tree, BST_CreateNode(424));
    BST_InsertNode(Tree, BST_CreateNode(17));
    BST_InsertNode(Tree, BST_CreateNode(3));

    BST_InsertNode(Tree, BST_CreateNode(98));
    BST_InsertNode(Tree, BST_CreateNode(34));

    BST_InsertNode(Tree, BST_CreateNode(760));
    BST_InsertNode(Tree, BST_CreateNode(317));
    BST_InsertNode(Tree, BST_CreateNode(1));

    BST_InorderPrintTree(Tree);
    fprintf(stdout, "\n");

    fprintf(stdout, "Removeing 98...\n");
    Node = BST_RemoveNode(Tree, NULL, 98);
    BST_DestroyNode(Node);
    BST_InorderPrintTree(Tree);
    fprintf(stdout, "\n");

    fprintf(stdout, "Inserting 111...\n");
    BST_InsertNode(Tree, BST_CreateNode(111));
    BST_InorderPrintTree(Tree);
    fprintf(stdout, "\n");

    BST_DestroyTree(Tree);

    return 0;
}

int main()
{
    return BST_Test_main();
}