#include "../header/main.h"

void createAndAttachNode(DirectoryTree *currentDirectoryTree, char *nodePath, DirectoryNode *newNode, DirectoryNode *tempNode) {
    //디렉토리 구조 안에 적절하게 노드를 위치시키는 함수 
    if (nodePath) {
        nodePath[strlen(nodePath)] = '\0';
        movePath(currentDirectoryTree, nodePath);
        newNode->parent = currentDirectoryTree->current;
        //현재 경로의 노드 아래에 새로운 노드를 추가하기 위해 parent를 현재 노드로 설정함
        //현재 경로 노드 아래에 노드가 없다면 
        if (!currentDirectoryTree->current->firstChild) {
            //바로 아래에 노드 생성 
            currentDirectoryTree->current->firstChild = newNode;
        } else {
            //이미 노드가 존재한다면
            tempNode = currentDirectoryTree->current->firstChild;
            //바로 아래 노드 리스트들 가장 뒷쪽에 배치시킴
            while (tempNode->nextSibling) {
                tempNode = tempNode->nextSibling;
            }
            tempNode->nextSibling = newNode;
        }
    } else {
        currentDirectoryTree->root = newNode;
        currentDirectoryTree->current = currentDirectoryTree->root;
    }
}

int readNode(DirectoryTree *currentDirectoryTree, char *temp) {
    //Directory.txt 파일에서 현재 저장된 파일 및 디렉토리 정보를 가져와 구조체로 만들어 Tree에 넣는 메소드 
    DirectoryNode *newNode = (DirectoryNode *)malloc(sizeof(DirectoryNode)), *tempNode = NULL;
    char *nodePath;

    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    newNode->parent = NULL;
    //ex) etc d 755 4096 0 0 6 1 1 23 /     이런 정보가 있다고 하면 앞에서부터 하나씩 Node에 속성을 넣는다.
    //폴더 및 파일명 저장 
    nodePath = strtok(temp, " ");
    strncpy(newNode->name, nodePath, MAX_NAME);
    //폴더 및 파일 유형 저장 (-, d, l 등)
    nodePath = strtok(NULL, " ");
    newNode->type = nodePath[0];
    //Node의 Permission 정보 저장 
    nodePath = strtok(NULL, " ");
    newNode->permission.mode = atoi(nodePath);
    // modeToPermission(newNode);  //permission 정보를 배열에 저장하는 코드 (755의 권한을 크기 9의 배열에 binary하게 표현)
    //Node 크기 저장
    nodePath = strtok(NULL, " ");
    newNode->SIZE = atoi(nodePath);
    //Node의 user id 저장 
    nodePath = strtok(NULL, " ");
    newNode->id.UID = atoi(nodePath);
    //Node의 group id 저장
    nodePath = strtok(NULL, " ");
    newNode->id.GID = atoi(nodePath);
    //Node의 월 정보 저장 
    nodePath = strtok(NULL, " ");
    newNode->date.month = atoi(nodePath);
    //Node의 일 정보 저장
    nodePath = strtok(NULL, " ");
    newNode->date.day = atoi(nodePath);
    //Node의 시간 정보 저장
    nodePath = strtok(NULL, " ");
    newNode->date.hour = atoi(nodePath);
    //Node의 분 정보 저장
    nodePath = strtok(NULL, " ");
    newNode->date.minute = atoi(nodePath);
    //str은 이제 해당 노드의 위치를 나타냄.
    nodePath = strtok(NULL, " ");
    //DirectoryTree에 Node를 적절한 위치에 추가하기 
    createAndAttachNode(currentDirectoryTree, nodePath, newNode, tempNode);
    return 0;
}

DirectoryNode *IsExistDir(DirectoryTree *currentDirectoryTree, char *dirName, char type) {
    for (DirectoryNode *current = currentDirectoryTree->current->firstChild; current; current = current->nextSibling) {
        // printf("%s\n",current->name);
        if (!strcmp(current->name, dirName) && current->type == type) {
            return current;
        }
    }
    return NULL;
}