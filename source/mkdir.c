#include "../header/main.h"

int Mode2Permission(DirectoryNode* dirNode)
{
    char buffer[4];
    int tmp;

    for(int i=0;i<9;i++)
        dirNode->permission.permission[i] = 0;

    sprintf(buffer, "%d", dirNode->permission.mode);

    for(int i=0;i<3;i++){
        tmp = buffer[i] - '0';
        for (int j = 2 ; j >= 0 ; j--)
        {
            dirNode->permission.permission[3*i+j] = tmp%2;
            tmp /= 2;
        }
    }

int HasPermission(DirectoryNode* dirNode, char o)
{
    if(usrList->current->id.UID== 0)
        return 0;

    if(usrList->current->id.UID == dirNode->id.UID){
        if(o == 'r'){
            if(dirNode->permission[0] == 0)
                return -1;
            else
                return 0;
        }
        if(o == 'w'){
            if(dirNode->permission[1] == 0)
                return -1;
            else
                return 0;
        }
        if(o == 'x'){
            if(dirNode->permission[2] == 0)
                return -1;
            else
                return 0;
        }
    }
    else if(usrList->current->id.GID == dirNode->id.GID){
        if(o == 'r'){
            if(dirNode->permission[3] == 0)
                return -1;
            else
                return 0;
        }
        if(o == 'w'){
            if(dirNode->permission[4] == 0)
                return -1;
            else
                return 0;
        }
        if(o == 'x'){
            if(dirNode->permission[5] == 0)
                return -1;
            else
                return 0;
        }
    }
    else{
        if(o == 'r'){
            if(dirNode->permission[6] == 0)
                return -1;
            else
                return 0;
        }
        if(o == 'w'){
            if(dirNode->permission[7] == 0)
                return -1;
            else
                return 0;
        }
        if(o == 'x'){
            if(dirNode->permission[8] == 0)
                return -1;
            else
                return 0;
        }
    }
    return -1;
}

int MakeDir(DirectoryTree* dirTree, char* dirName, char type)
{
    DirectoryNode* NewNode = (DirectoryNode*)malloc(sizeof(DirectoryNode));
    DirectoryNode* tmpNode = NULL;

    if(HasPermission(currentDirectoryTree->current, 'w') != 0){
        printf("mkdir: '%s' Can not create directory: Permission denied.\n", dirName);
        free(NewNode);
        return -1;
    }
    if(strcmp(dirName, ".") == 0 || strcmp(dirName, "..") == 0){
        printf("mkdir: '%s' Can not create directory.\n", dirName);
        free(NewNode);
        return -1;
    }
    tmpNode = IsExistDir(currentDirectoryTree, dirName, type);
    if(tmpNode != NULL && tmpNode->type == 'd'){
        printf("mkdir: '%s' Can not create directory: File exists \n", dirName);
        free(NewNode);
        return -1;
    }
    time(&ltime);
    today = localtime(&ltime);

    NewNode->firstChild = NULL;
    NewNode->nextSibling = NULL;

    strncpy(NewNode->name, dirName, MAX_NAME);
    if(dirName[0] == '.'){
        NewNode->type = 'd';
        //rwx------
        NewNode->permission.mode = 700;
        NewNode->SIZE = 4096;
    }
    else if(type == 'd'){
        NewNode->type = 'd';
        //rwxr-xr-x
        NewNode->permission.mode = 755;
        NewNode->SIZE = 4096;
    }
    else{
        NewNode->type = 'f';
        //rw-r--r--
        NewNode->permission.mode = 644;
        NewNode->SIZE = 0;
    }
    Mode2Permission(NewNode);
    NewNode->id.UID = userList->current->id.UID;
    NewNode->id.GID = userList->current->id.GID;
    NewNode->date.month = today->tm_mon + 1;
    NewNode->date.day = today->tm_mday;
    NewNode->date.hour = today->tm_hour;
    NewNode->date.minute = today->tm_min;
    NewNode->parent = currentDirectoryTree->current;

    if(currentDirectoryTree->current->firstChild == NULL){
        currentDirectoryTree->current->firstChild = NewNode;
    }
    else{
        tmpNode = currentDirectoryTree->current->firstChild;

        while(tmpNode->nextSibling!= NULL){
            tmpNode = tmpNode->nextSibling;
        }
        tmpNode->nextSibling = NewNode;
    }

    return 0;
}

char* getDir(char* dirPath)
{
    char* tmpPath = (char*)malloc(MAX_DIR);
    char* str = NULL;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];

    strncpy(tmp, dirPath, MAX_DIR);
    str = strtok(dirPath, "/");
    while(str != NULL){
        strncpy(tmp2, str, MAX_DIR);
        str  = strtok(NULL, "/");
    }
    strncpy(tmpPath, tmp, strlen(tmp)-strlen(tmp2)-1);
    tmpPath[strlen(tmp)-strlen(tmp2)-1] = '\0';

    return tmpPath;
}

int moveCurrent(DirectoryTree* currentDirectoryTree, char* dirPath)
{
    DirectoryNode* tempNode = NULL;
    if(strcmp(dirPath,".") == 0){
    }
    else if(strcmp(dirPath,"..") == 0){
        if(currentDirectoryTree->current != currentDirectoryTree->root){
            currentDirectoryTree->current = currentDirectoryTree->current->parent;
        }
    }
    else{
        tempNode = IsExistDir(currentDirectoryTree, dirPath, 'd');
        if(tempNode != NULL){
            currentDirectoryTree->current = tempNode;
        }
        else
            return -1;
    }
    return 0;
}

int movePath(DirectoryTree* currentDirectoryTree, char* dirPath)
{
    DirectoryNode* tempNode = NULL;
    char tempPath[MAX_DIR];
    char* str = NULL;
    int val = 0;
    strncpy(tempPath, dirPath, MAX_DIR);
    tempPath[MAX_DIR - 1] = '\0'; // 보안을 위해 널 종료 문자를 추가
    tempNode = currentDirectoryTree->current;

    strncpy(tmpPath, dirPath, MAX_DIR);
    tmpNode = dirTree->current;
    //입력 값이 루트로 가는 값일 때ㅐ

    if(strcmp(dirPath, "/") == 0){
        currentDirectoryTree->current = currentDirectoryTree->root;
    }
    else{
        //if input is absolute path
        if(dirPath[0] == '/'){
            currentDirectoryTree->current = currentDirectoryTree->root; // Set current to root for absolute path
            // 이 부분을 수정하여 절대 경로를 올바르게 처리
            str = strtok(tempPath + 1, "/"); // +1 to skip the first '/'
        } else {
            // Relative path, start tokenizing from the beginning
            str = strtok(tempPath, "/");
        }
        while(str != NULL){
            val = MoveCurrent(dirTree, str);
            //경로가 존재하지 않을 때

            val = moveCurrent(currentDirectoryTree, str);
            //if input path doesn't exist
            if(val != 0){
                currentDirectoryTree->current = tempNode; // Restore original current node
                return -1;
            }
            str = strtok(NULL, "/");
        }
    }
    return 0;
}

void* mkdirThread(void* arg) {
    ThreadTree* t1 = (ThreadTree*)arg;
  
    DirectoryTree* currentDirectoryTree = t1->threadTree;
    char* cmd = t1->command;
    int option = t1->option;

    DirectoryNode* tmpNode = currentDirectoryTree->current;

    char tmp[MAX_DIR];
    char pStr[MAX_DIR];
    char tmpStr[MAX_DIR];
    char directoryName[MAX_DIR];
    int directoryLength = 0;
    int directoryExist;

    strncpy(tmp, cmd, MAX_DIR);

    if (strstr(cmd, "/") == NULL) {

        MakeDir(currentDirectoryTree, cmd, 'd');

    } 
    else if (option == 1) {
        int length = strlen(tmp);
        int flag = 0;
        if (tmp[0] == '/') {

            currentDirectoryTree->current = currentDirectoryTree->root;

            flag = 1;
        }
        if (tmp[length - 1] == '/') {
            length -= 1;
        }
        for (; flag < length; flag++) {
            pStr[flag] = tmp[flag];
            pStr[flag + 1] = '\0';
            directoryName[directoryLength++] = tmp[flag];
            if (tmp[flag] == '/') {
                directoryName[--directoryLength] = '\0';
                strncpy(tmpStr, pStr, flag - 1);

                directoryExist = moveCurrent(currentDirectoryTree, directoryName);
                if (directoryExist == -1) {
                    MakeDir(currentDirectoryTree, directoryName, 'd');
                    directoryExist = moveCurrent(currentDirectoryTree, directoryName);

                }
                directoryLength = 0;
            }
        }
        directoryName[directoryLength] = '\0';

        MakeDir(currentDirectoryTree, directoryName, 'd');
        currentDirectoryTree->current = tmpNode;
    } 
    else {
        char* p_directory = getDir(cmd);
        directoryExist = movePath(currentDirectoryTree, p_directory);

        if (directoryExist != 0) {
            printf("mkdir: '%s': No such file or directory.\n", p_directory);
        } 
        else {
            char* str = strtok(tmp, "/");
            char* p_directory_name;
            while (str != NULL) {
                p_directory_name = str;
                str = strtok(NULL, "/");
            }

            MakeDir(currentDirectoryTree, p_directory_name, 'd');
            currentDirectoryTree->current = tmpNode;

        }
    }
    pthread_exit(NULL);
}

int Mkdir(DirectoryTree* currentDirectoryTree, char* cmd)
{
    DirectoryNode *tmpNode = NULL;
    char *str;
    int isDirectoryExist;
    int tmpMode;
    int count = 0;

    if (!cmd) {
        printf("mkdir: Invalid option\n");
        printf("Try 'mkdir --help' for more information.\n");
        return -1;
    }
    
    pthread_t threadPool[MAX_THREAD];
    ThreadTree threadTree[MAX_THREAD];

    tmpNode = currentDirectoryTree->current;

    if (cmd[0] == '-') {
        if (!strcmp(cmd, "-p")) {
            str = strtok(NULL, " ");
            if (!str) {
                printf("mkdir: Invalid option\n");
                printf("Try 'mkdir --help' for more information.\n");
                return -1;
            }
            while (str) {
                threadTree[count].threadTree = currentDirectoryTree;
                threadTree[count].option = 1;
                threadTree[count++].command = str;
                str = strtok(NULL, " ");
            }
        } else if (!strcmp(cmd, "--help")) {
            printf("Usage: mkdir [option]... [directory]...\n");
            printf("  Create the DIRECTORY(ies), if they do not already exists.\n\n");
            printf("  Options:\n");
            printf("    -p, --parents  \t no error if existing, make parent directories as needed\n");
            printf("      --help\t Display this help and exit\n");
            return -1;
        } else {
            str = strtok(cmd, "-");
            if (!str) {
                printf("mkdir: Invalid option\n");
                printf("Try 'mkdir --help' for more information.\n");
                return -1;
            } else {
                printf("mkdir: Unrecognized option -- '%s'\n", str);
                printf("Try 'mkdir --help' for more information.\n");
                return -1;
            }
        }
    } else {
        str = strtok(NULL, " ");
        threadTree[count].threadTree = currentDirectoryTree;
        threadTree[count].option = 0;
        threadTree[count++].command = cmd;
        while (str) {
            threadTree[count].threadTree = currentDirectoryTree;
            threadTree[count].option = 0;
            threadTree[count++].command = str;
            str = strtok(NULL, " ");
        }
    }
    for (int i = 0; i < count; i++) {
        pthread_create(&threadPool[i], NULL, mkdirThread, (void *)&threadTree[i]);
        pthread_join(threadPool[i], NULL);
    }
    return 0;

}

