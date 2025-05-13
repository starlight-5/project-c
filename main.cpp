#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 32 // 이름 최대 길이

// 노드 클래스 정의 (C 스타일 구조체처럼 사용)
class Node
{
public:
    char name[MAX_NAME_LEN]; // 왕 이름
    Node *children[10];      // 자식 노드 포인터 배열 (최대 10명)
    int childCount;          // 자식 수
    int depth;
};

// 노드 생성 함수
Node *createNode(const char *name)
{
    Node *node = (Node *)malloc(sizeof(Node)); // 동적 메모리 할당
    strcpy(node->name, name);                  // 이름 복사
    node->childCount = 0;                      // 자식 수 초기화

    for (int i = 0; i < 10; ++i)
    {
        node->children[i] = NULL; // 자식 포인터 초기화
    }
    return node;
}

// 이름으로 노드 찾기 (이미 존재하는 노드인지 확인)
Node *findNode(Node *list[], int count, const char *name)
{
    for (int i = 0; i < count; ++i)
    {
        if (strcmp(list[i]->name, name) == 0) // 이름이 같은 노드 발견
            return list[i];
    }
    return NULL; // 못 찾았으면 NULL 반환
}

// 트리 출력 함수 (사진처럼: 이름: 자식1 자식2 ...)
void printTree(Node *node)
{
    printf("%s: ", node->name); // 현재 노드 이름 출력
    for (int i = 0; i < node->childCount; i++)
    {
        printf("%s ", node->children[i]->name); // 자식들 이름 출력
    }
    printf("\n");

    for (int i = 0; i < node->childCount; i++)
    {
        printTree(node->children[i]); // 자식 노드 재귀적으로 출력
    }
}
void print(Node *node)
{
    printf("%s ", node->name); // 현재 노드 이름 출력

    for (int i = 0; i < node->childCount; i++)
    {
        print(node->children[i]); // 자식 노드 재귀적으로 출력
    }
}
void print(Node *node, char *cp)
{
    if (strcmp(node->name, cp) != 0)
        printf("%s ", node->name); // 현재 노드 이름 출력

    for (int i = 0; i < node->childCount; i++)
    {
        print(node->children[i]); // 자식 노드 재귀적으로 출력
    }
}
void rprint(Node *node[], int c)
{

    for (int i = c - 1; strcmp(node[i + 1]->name, "태조") != 0; i--)
    {
        // 자식 노드 재귀적으로 출력
        printf("%s ", node[i]->name); // 현재 노드 이름 출력
    }
}
void targetPrint(Node *node, char *cp)
{
    if (strcmp(node->name, cp) == 0)
    {
        print(node, cp);
        return;
    }
    for (int i = 0; i < node->childCount; i++)
    {
        targetPrint(node->children[i], cp); // 자식 노드 재귀적으로 출력
    }
}
void loserPrint(Node *node)
{
    if (node->childCount == 0)
        printf("%s ", node->name); // 현재 노드 이름 출력

    for (int i = 0; i < node->childCount; i++)
    {
        loserPrint(node->children[i]); // 자식 노드 재귀적으로 출력
    }
}
void bestWinnerPrint(Node *node[], int nodecount)
{
    Node *temp = node[0];
    int n = 0;
    for (int i = 0; i < nodecount - 1; i++)
    {
        if (temp->childCount > node[i + 1]->childCount)
        {
            n = 0;
        }
        else if (temp->childCount < node[i + 1]->childCount)
        {
            temp = node[i + 1];
            n = 0;
        }
        else
        {
            n += 1;
        }
    }
    if (n != 0)
    {
        printf("No king has the most descendants.");
    }
    else
    {
        printf("%s", temp->name);
    }
}
void targetWinnerPrint(Node *node, char *cp, int depth)
{
    if (node->depth == depth)
    {
        printf("%s ", node->name);
    }
    for (int i = 0; i < node->childCount; i++)
    {
        targetWinnerPrint(node->children[i], cp, depth); // 자식 노드 재귀적으로 출력
    }
}
int findDepth(Node *node[], int nodecount, char *cp)
{

    for (int i = 0; i < nodecount; i++)
    {
        if (strcmp(node[i]->name, cp) == 0)
        {
            return node[i]->depth;
        }
    }
}
int main()
{
    FILE *fp = fopen("zosun.txt", "r"); // 텍스트 파일 열기
    if (!fp)
    {
        printf("Unable to open file.\n");
        return 1;
    }

    Node *nodeList[100]; // 모든 노드를 저장할 배열
    int nodeCount = 0;   // 노드 개수

    char rootName[MAX_NAME_LEN] = {
        0,
    }; // 왕조의 시조 이름
    fgets(rootName, sizeof(rootName), fp); // 첫 줄 읽기
    rootName[strcspn(rootName, "\n")] = 0; // 개행 문자 제거

    Node *root = createNode(rootName); // 시조 노드 생성
    nodeList[nodeCount++] = root;      // 리스트에 추가
    root->depth = 0;                   // 깊이

    char son[MAX_NAME_LEN] = {
        0,
    };
    char father[MAX_NAME_LEN] = {
        0,
    }; // 관계 정보 저장용
    int i = 1;                                    // 깊이 초기화
    while (fscanf(fp, "%s %s", son, father) == 2) // 관계 한 줄씩 읽기
    {
        Node *fatherNode = findNode(nodeList, nodeCount, father);
        Node *sonNode = findNode(nodeList, nodeCount, son);
        // 아들 노드가 없으면 생성 후 리스트에 추가
        if (!sonNode)
        {
            sonNode = createNode(son);
            nodeList[nodeCount++] = sonNode; // 동적메모리 해제를 위해
            sonNode->depth = fatherNode->depth + 1;
        }

        // 아버지 노드에 아들 추가
        fatherNode->children[fatherNode->childCount++] = sonNode;
        fatherNode = nullptr;
        sonNode = nullptr;
    }

    fclose(fp); // 파일 닫기

    printf("조선 왕조 트리 (이름: 자식들):\n");
    printTree(root); // 트리 출력 시작

    print(root);
    printf("\n");

    rprint(nodeList, nodeCount);
    printf("\n");

    printf("%d", nodeCount);
    printf("\n");

    char temp[15] = {
        0,
    };
    strcpy(temp, "인조");
    targetPrint(root, temp);
    printf("\n");

    loserPrint(root);
    printf("\n");

    bestWinnerPrint(nodeList, nodeCount);
    printf("\n");

    char temp1[15] = {
        0,
    };
    strcpy(temp1, "정종");
    targetWinnerPrint(root, temp1, findDepth(nodeList, nodeCount, temp1));
    printf("\n");

    for (int i = 0; i < nodeCount; i++)
        printf("%s ", nodeList[i]);
    // 동적 메모리 해제
    for (int i = 0; i < nodeCount; ++i)
    {
        free(nodeList[i]);
    }

    return 0;
}
