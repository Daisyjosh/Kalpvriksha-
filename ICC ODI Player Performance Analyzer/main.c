#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<limits.h>
#include<errno.h>
#include<math.h>
#include"players_data.h"

#define MAX_TEAMS TEAM_COUNT

typedef struct Player{
    int playerId;
    char name[MAX_NAME_LENGTH];
    int teamId;
    int role;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct Player *next;
}Player;

typedef struct RoleNode{
    Player *player;
    struct RoleNode *next;
}RoleNode;

typedef struct Team{
    int teamId;
    char name[MAX_NAME_LENGTH];
    int totalPlayers;
    float averageBattingStrikeRate;
    float totalStrikeRate;
    int battingPlayerCount;
    Player *head;
    Player *tail;
    RoleNode *roleHead[3];
}Team;

typedef struct HeapNode{
    RoleNode *current;
    int teamIndex;
}HeapNode;

Team teams[MAX_TEAMS];
int teamCount=MAX_TEAMS;
int totalPlayers=0;

int readLine(char *buffer,int size);
void trimWhitespace(char *str);
int readInt(const char *prompt,int *value);
int readFloat(const char *prompt,float *value);
int readNonEmptyString(const char *prompt,char *value,int size);
int binarySearchTeam(int teamId);
float calculatePerformanceIndex(Player *player);
Player *createPlayer(InitialPlayer *initial);
void initializeTeams(void);
void initializePlayers(void);
void addPlayerToTeam(Player *player);
void insertRoleNode(Player *player);
int playerIdExists(int playerId);
void addNewPlayer(void);
void displayPlayer(Player *player);
void displayPlayerGlobal(Player *player,char *teamName);
void displayTeamPlayers(void);
void displayTeamsByStrikeRate(void);
void displayTopKPlayers(void);
void displayPlayersByRole(void);
int heapHigherPriority(HeapNode a,HeapNode b);
void heapifyUp(HeapNode heap[],int index);
void heapifyDown(HeapNode heap[],int heapSize,int index);
void heapPush(HeapNode heap[],int *heapSize,HeapNode node);
HeapNode heapPop(HeapNode heap[],int *heapSize);
void freeMemory(void);

int readLine(char *buffer,int size){
    int ch;
    if(fgets(buffer,size,stdin)==NULL){
        return 0;
    }

    if(strchr(buffer,'\n')==NULL){
        while((ch=getchar())!='\n'&&ch!=EOF);
    }

    buffer[strcspn(buffer,"\n")]='\0';
    return 1;
}

void trimWhitespace(char *str){
    int start=0,end=(int)strlen(str)-1;

    while(str[start]&&isspace((unsigned char)str[start])){
        start++;
    }

    while(end>=start&&isspace((unsigned char)str[end])){
        end--;
    }

    int length=end-start+1;
    memmove(str,str+start,length);
    str[length]='\0';
}

int readInt(const char *prompt,int *value){
    char buffer[100];
    char *end;
    long number;

    while(1){
        printf("%s",prompt);

        if(!readLine(buffer,sizeof(buffer))){
            return 0;
        }

        trimWhitespace(buffer);

        if(buffer[0]=='\0'){
            printf("Input cannot be empty. Try again.\n");
            continue;
        }

        errno=0;
        number=strtol(buffer,&end,10);

        if(end==buffer||*end!='\0'){
            printf("Invalid integer. Try again.\n");
            continue;
        }

        if(errno==ERANGE||number<INT_MIN||number>INT_MAX){
            printf("Integer out of range. Try again.\n");
            continue;
        }

        *value=(int)number;
        return 1;
    }
}

int readFloat(const char *prompt,float *value){
    char buffer[100];
    char *end;
    float number;

    while(1){
        printf("%s",prompt);

        if(!readLine(buffer,sizeof(buffer))){
            return 0;
        }

        trimWhitespace(buffer);

        if(buffer[0]=='\0'){
            printf("Input cannot be empty. Try again.\n");
            continue;
        }

        errno=0;
        number=strtof(buffer,&end);

        if(end==buffer||*end!='\0'||errno==ERANGE||!isfinite(number)){
            printf("Invalid number. Try again.\n");
            continue;
        }

        *value=number;
        return 1;
    }
}

int readNonEmptyString(const char *prompt,char *value,int size){
    while(1){
        printf("%s",prompt);

        if(!readLine(value,size)){
            return 0;
        }

        trimWhitespace(value);

        if(value[0]=='\0'){
            printf("Input cannot be empty. Try again.\n");
            continue;
        }

        return 1;
    }
}

float calculatePerformanceIndex(Player *player){
    if(player->role==BATSMAN){
        return(player->battingAverage*player->strikeRate)/100.0f;
    }
    if(player->role==BOWLER){
        return(player->wickets*2)+(100.0f-player->economyRate);
    }
    if(player->role==ALL_ROUNDER){
        return((player->battingAverage*player->strikeRate)/100.0f)+(player->wickets*2);
    }
    return 0.0f;
}

int binarySearchTeam(int teamId){
    int low=0,high=teamCount-1;

    while(low<=high){
        int mid=low+(high-low)/2;

        if(teams[mid].teamId==teamId){
            return mid;
        }

        if(teams[mid].teamId<teamId){
            low=mid+1;
        }
        else{
            high=mid-1;
        }
    }

    return -1;
}

void initializeTeams(void){
    int teamIndex,roleIndex;

    for(teamIndex=0;teamIndex<TEAM_COUNT;teamIndex++){
        teams[teamIndex].teamId=teamIndex+1;
        strcpy(teams[teamIndex].name,teamNames[teamIndex]);
        teams[teamIndex].totalPlayers=0;
        teams[teamIndex].averageBattingStrikeRate=0.0f;
        teams[teamIndex].totalStrikeRate=0.0f;
        teams[teamIndex].battingPlayerCount=0;
        teams[teamIndex].head=NULL;
        teams[teamIndex].tail=NULL;

        for(roleIndex=0;roleIndex<3;roleIndex++){
            teams[teamIndex].roleHead[roleIndex]=NULL;
        }
    }
}

Player *createPlayer(InitialPlayer *initial){
    Player *player=(Player *)malloc(sizeof(Player));

    if(player==NULL){
        printf("Memory allocation failed.\n");
        exit(1);
    }

    player->playerId=initial->playerId;
    strcpy(player->name,initial->name);
    player->teamId=initial->teamId;
    player->role=initial->role;
    player->totalRuns=initial->totalRuns;
    player->battingAverage=initial->battingAverage;
    player->strikeRate=initial->strikeRate;
    player->wickets=initial->wickets;
    player->economyRate=initial->economyRate;
    player->performanceIndex=calculatePerformanceIndex(player);
    player->next=NULL;

    return player;
}

void insertRoleNode(Player *player){
    int teamIndex=binarySearchTeam(player->teamId);
    int roleIndex=player->role-1;

    RoleNode *newNode=(RoleNode *)malloc(sizeof(RoleNode));

    if(newNode==NULL){
        printf("Memory allocation failed.\n");
        exit(1);
    }

    newNode->player=player;
    newNode->next=NULL;

    RoleNode *current=teams[teamIndex].roleHead[roleIndex];
    RoleNode *previous=NULL;

    while(current!=NULL&&current->player->performanceIndex>=player->performanceIndex){
        previous=current;
        current=current->next;
    }

    if(previous==NULL){
        newNode->next=teams[teamIndex].roleHead[roleIndex];
        teams[teamIndex].roleHead[roleIndex]=newNode;
    }
    else{
        newNode->next=previous->next;
        previous->next=newNode;
    }
}

void addPlayerToTeam(Player *player){
    int teamIndex=binarySearchTeam(player->teamId);
    Team *team=&teams[teamIndex];

    if(team->head==NULL){
        team->head=player;
        team->tail=player;
    }
    else{
        team->tail->next=player;
        team->tail=player;
    }

    team->totalPlayers++;

    if(player->role==BATSMAN||player->role==ALL_ROUNDER){
        team->totalStrikeRate+=player->strikeRate;
        team->battingPlayerCount++;
        team->averageBattingStrikeRate=team->totalStrikeRate/team->battingPlayerCount;
    }

    insertRoleNode(player);
    totalPlayers++;
}

int playerIdExists(int playerId){
    int teamIndex;
    Player *currentPlayer;

    for(teamIndex=0;teamIndex<teamCount;teamIndex++){
        currentPlayer=teams[teamIndex].head;

        while(currentPlayer!=NULL){
            if(currentPlayer->playerId==playerId){
                return 1;
            }
            currentPlayer=currentPlayer->next;
        }
    }

    return 0;
}

void initializePlayers(void){
    int playerIndex;
    Player *player;

    for(playerIndex=0;playerIndex<INITIAL_PLAYERS;playerIndex++){
        player=createPlayer((InitialPlayer *)&initialPlayers[playerIndex]);
        addPlayerToTeam(player);
    }
}

void addNewPlayer(void){
    int teamId,playerId,role,teamIndex;
    InitialPlayer initial;
    Player *player;

    while(1){
        if(!readInt("Enter Team ID to add player: ",&teamId)){
            return;
        }

        teamIndex=binarySearchTeam(teamId);

        if(teamIndex==-1){
            printf("Invalid Team ID. Try again.\n");
            continue;
        }

        if(teams[teamIndex].totalPlayers>=50){
            printf("Team has reached maximum player limit.\n");
            return;
        }

        break;
    }

    printf("Enter Player Details:\n");

    while(1){
        if(!readInt("Player ID: ",&playerId)){
            return;
        }

        if(playerId<1||playerId>1000){
            printf("Player ID must be between 1 and 1000.\n");
            continue;
        }

        if(playerIdExists(playerId)){
            printf("Player ID already exists. Try again.\n");
            continue;
        }

        break;
    }

    initial.playerId=playerId;
    initial.teamId=teamId;

    if(!readNonEmptyString("Name: ",initial.name,MAX_NAME_LENGTH)){
        return;
    }

    while(1){
        if(!readInt("Role (1-Batsman, 2-Bowler, 3-All-rounder): ",&role)){
            return;
        }

        if(role<BATSMAN||role>ALL_ROUNDER){
            printf("Invalid Role. Try again.\n");
            continue;
        }

        break;
    }

    initial.role=role;

    while(1){
        if(!readInt("Total Runs: ",&initial.totalRuns)){
            return;
        }

        if(initial.totalRuns<0){
            printf("Total Runs cannot be negative.\n");
            continue;
        }

        break;
    }

    while(1){
        if(!readFloat("Batting Average: ",&initial.battingAverage)){
            return;
        }

        if(initial.battingAverage<0){
            printf("Batting Average cannot be negative.\n");
            continue;
        }

        break;
    }

    while(1){
        if(!readFloat("Strike Rate: ",&initial.strikeRate)){
            return;
        }

        if(initial.strikeRate<0){
            printf("Strike Rate cannot be negative.\n");
            continue;
        }

        break;
    }

    while(1){
        if(!readInt("Wickets: ",&initial.wickets)){
            return;
        }

        if(initial.wickets<0){
            printf("Wickets cannot be negative.\n");
            continue;
        }

        break;
    }

    while(1){
        if(!readFloat("Economy Rate: ",&initial.economyRate)){
            return;
        }

        if(initial.economyRate<0){
            printf("Economy Rate cannot be negative.\n");
            continue;
        }

        break;
    }

    player=createPlayer(&initial);
    addPlayerToTeam(player);

    printf("Player added successfully to Team %s!\n",teams[teamIndex].name);
}

void displayPlayer(Player *player){
    char roleName[20];

    if(player->role==BATSMAN){
        strcpy(roleName,"Batsman");
    }
    else if(player->role==BOWLER){
        strcpy(roleName,"Bowler");
    }
    else{
        strcpy(roleName,"All-rounder");
    }

    printf("%-5d %-25s %-12s %-8d %-8.1f %-8.1f %-6d %-6.1f %-8.2f\n",
           player->playerId,
           player->name,
           roleName,
           player->totalRuns,
           player->battingAverage,
           player->strikeRate,
           player->wickets,
           player->economyRate,
           player->performanceIndex);
}

void displayPlayerGlobal(Player *player,char *teamName){
    char roleName[20];

    if(player->role==BATSMAN){
        strcpy(roleName,"Batsman");
    }
    else if(player->role==BOWLER){
        strcpy(roleName,"Bowler");
    }
    else{
        strcpy(roleName,"All-rounder");
    }

    printf("%-5d %-25s %-15s %-12s %-8d %-8.1f %-8.1f %-6d %-6.1f %-8.2f\n",
           player->playerId,
           player->name,
           teamName,
           roleName,
           player->totalRuns,
           player->battingAverage,
           player->strikeRate,
           player->wickets,
           player->economyRate,
           player->performanceIndex);
}

void displayTeamPlayers(void){
    int teamId,teamIndex;
    Player *currentPlayer;

    if(!readInt("Enter Team ID: ",&teamId)){
        return;
    }

    teamIndex=binarySearchTeam(teamId);

    if(teamIndex==-1){
        printf("Invalid Team ID.\n");
        return;
    }

    Team *team=&teams[teamIndex];

    printf("Players of Team %s:\n",team->name);
    printf("====================================================================================\n");
    printf("%-5s %-25s %-12s %-8s %-8s %-8s %-6s %-6s %-8s\n",
           "ID","Name","Role","Runs","Avg","SR","Wkts","ER","Perf.Index");
    printf("====================================================================================\n");

    currentPlayer=team->head;

    while(currentPlayer!=NULL){
        displayPlayer(currentPlayer);
        currentPlayer=currentPlayer->next;
    }

    printf("====================================================================================\n");
    printf("Total Players: %d\n",team->totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n",team->averageBattingStrikeRate);
}

void displayTeamsByStrikeRate(void){
    Team *sorted[MAX_TEAMS];
    Team *temporaryTeam;
    int teamIndex,comparisonIndex;

    for(teamIndex=0;teamIndex<teamCount;teamIndex++){
        sorted[teamIndex]=&teams[teamIndex];
    }

    for(teamIndex=0;teamIndex<teamCount-1;teamIndex++){
        for(comparisonIndex=0;comparisonIndex<teamCount-teamIndex-1;comparisonIndex++){
            if(sorted[comparisonIndex]->averageBattingStrikeRate<sorted[comparisonIndex+1]->averageBattingStrikeRate){
                temporaryTeam=sorted[comparisonIndex];
                sorted[comparisonIndex]=sorted[comparisonIndex+1];
                sorted[comparisonIndex+1]=temporaryTeam;
            }
        }
    }

    printf("Teams Sorted by Average Batting Strike Rate\n");
    printf("=========================================================\n");
    printf("%-5s %-20s %-12s %-15s\n","ID","Team Name","Avg Bat SR","Total Players");
    printf("=========================================================\n");

    for(teamIndex=0;teamIndex<teamCount;teamIndex++){
        printf("%-5d %-20s %-12.1f %-15d\n",
               sorted[teamIndex]->teamId,
               sorted[teamIndex]->name,
               sorted[teamIndex]->averageBattingStrikeRate,
               sorted[teamIndex]->totalPlayers);
    }

    printf("=========================================================\n");
}

void displayTopKPlayers(void){
    int teamId,teamIndex,role,k,count=0;
    RoleNode *currentRoleNode;
    char roleName[20];

    if(!readInt("Enter Team ID: ",&teamId)){
        return;
    }

    teamIndex=binarySearchTeam(teamId);

    if(teamIndex==-1){
        printf("Invalid Team ID.\n");
        return;
    }

    while(1){
        if(!readInt("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ",&role)){
            return;
        }

        if(role<BATSMAN||role>ALL_ROUNDER){
            printf("Invalid Role. Try again.\n");
            continue;
        }

        break;
    }

    while(1){
        if(!readInt("Enter number of players: ",&k)){
            return;
        }

        if(k<=0){
            printf("Number of players must be positive.\n");
            continue;
        }

        break;
    }

    if(role==BATSMAN){
        strcpy(roleName,"Batsmen");
    }
    else if(role==BOWLER){
        strcpy(roleName,"Bowlers");
    }
    else{
        strcpy(roleName,"All-rounders");
    }

    printf("Top %d %s of Team %s:\n",k,roleName,teams[teamIndex].name);
    printf("====================================================================================\n");
    printf("%-5s %-25s %-12s %-8s %-8s %-8s %-6s %-6s %-8s\n",
           "ID","Name","Role","Runs","Avg","SR","Wkts","ER","Perf.Index");
    printf("====================================================================================\n");

    currentRoleNode=teams[teamIndex].roleHead[role-1];

    while(currentRoleNode!=NULL&&count<k){
        displayPlayer(currentRoleNode->player);
        currentRoleNode=currentRoleNode->next;
        count++;
    }

    printf("====================================================================================\n");
}

int heapHigherPriority(HeapNode a,HeapNode b){
    float first=a.current->player->performanceIndex;
    float second=b.current->player->performanceIndex;

    if(first>second){
        return 1;
    }

    if(first<second){
        return 0;
    }

    return a.current->player->playerId<b.current->player->playerId;
}

void heapifyUp(HeapNode heap[],int heapIndex){
    int parentIndex;
    HeapNode temporaryNode;

    while(heapIndex>0){
        parentIndex=(heapIndex-1)/2;

        if(heapHigherPriority(heap[parentIndex],heap[heapIndex])){
            break;
        }

        temporaryNode=heap[parentIndex];
        heap[parentIndex]=heap[heapIndex];
        heap[heapIndex]=temporaryNode;
        heapIndex=parentIndex;
    }
}

void heapifyDown(HeapNode heap[],int heapSize,int heapIndex){
    int leftChildIndex,rightChildIndex,highestPriorityIndex;
    HeapNode temporaryNode;

    while(1){
        leftChildIndex=2*heapIndex+1;
        rightChildIndex=2*heapIndex+2;
        highestPriorityIndex=heapIndex;

        if(leftChildIndex<heapSize&&heapHigherPriority(heap[leftChildIndex],heap[highestPriorityIndex])){
            highestPriorityIndex=leftChildIndex;
        }

        if(rightChildIndex<heapSize&&heapHigherPriority(heap[rightChildIndex],heap[highestPriorityIndex])){
            highestPriorityIndex=rightChildIndex;
        }

        if(highestPriorityIndex==heapIndex){
            break;
        }

        temporaryNode=heap[heapIndex];
        heap[heapIndex]=heap[highestPriorityIndex];
        heap[highestPriorityIndex]=temporaryNode;
        heapIndex=highestPriorityIndex;
    }
}

void heapPush(HeapNode heap[],int *heapSize,HeapNode node){
    heap[*heapSize]=node;
    (*heapSize)++;
    heapifyUp(heap,*heapSize-1);
}

HeapNode heapPop(HeapNode heap[],int *heapSize){
    HeapNode result=heap[0];

    heap[0]=heap[*heapSize-1];
    (*heapSize)--;

    if(*heapSize>0){
        heapifyDown(heap,*heapSize,0);
    }

    return result;
}

void displayPlayersByRole(void){
    int role,heapSize=0;
    HeapNode heap[MAX_TEAMS],node;
    RoleNode *currentRoleNode;
    char roleName[20];

    while(1){
        if(!readInt("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ",&role)){
            return;
        }

        if(role<BATSMAN||role>ALL_ROUNDER){
            printf("Invalid Role. Try again.\n");
            continue;
        }

        break;
    }

    if(role==BATSMAN){
        strcpy(roleName,"Batsmen");
    }
    else if(role==BOWLER){
        strcpy(roleName,"Bowlers");
    }
    else{
        strcpy(roleName,"All-rounders");
    }

    for(int teamIndex=0;teamIndex<teamCount;teamIndex++){
        currentRoleNode=teams[teamIndex].roleHead[role-1];

        if(currentRoleNode!=NULL){
            node.current=currentRoleNode;
            node.teamIndex=teamIndex;
            heapPush(heap,&heapSize,node);
        }
    }

    printf("%s of all teams:\n",roleName);
    printf("======================================================================================\n");
    printf("%-5s %-25s %-15s %-12s %-8s %-8s %-8s %-6s %-6s %-8s\n",
           "ID","Name","Team","Role","Runs","Avg","SR","Wkts","ER","Perf.Index");
    printf("======================================================================================\n");

    while(heapSize>0){
        node=heapPop(heap,&heapSize);
        currentRoleNode=node.current;

        displayPlayerGlobal(currentRoleNode->player,teams[node.teamIndex].name);

        if(currentRoleNode->next!=NULL){
            node.current=currentRoleNode->next;
            heapPush(heap,&heapSize,node);
        }
    }

    printf("======================================================================================\n");
}

void freeMemory(void){
    int teamIndex,roleIndex;
    Player *currentPlayer,*nextPlayer;
    RoleNode *currentRoleNode,*nextRoleNode;

    for(teamIndex=0;teamIndex<teamCount;teamIndex++){
        currentPlayer=teams[teamIndex].head;

        while(currentPlayer!=NULL){
            nextPlayer=currentPlayer->next;
            free(currentPlayer);
            currentPlayer=nextPlayer;
        }

        for(roleIndex=0;roleIndex<3;roleIndex++){
            currentRoleNode=teams[teamIndex].roleHead[roleIndex];

            while(currentRoleNode!=NULL){
                nextRoleNode=currentRoleNode->next;
                free(currentRoleNode);
                currentRoleNode=nextRoleNode;
            }
        }
    }
}

int main(void){
    int choice;

    initializeTeams();
    initializePlayers();

    while(1){
        printf("==============================================================================\n");
        printf("ICC ODI Player Performance Analyzer\n");
        printf("==============================================================================\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display all Players of specific role Across All Teams by performance index\n");
        printf("6. Exit\n");
        printf("==============================================================================\n");

        if(!readInt("Enter your choice: ",&choice)){
            freeMemory();
            return 0;
        }

        switch(choice){
            case 1:
                addNewPlayer();
                break;
            case 2:
                displayTeamPlayers();
                break;
            case 3:
                displayTeamsByStrikeRate();
                break;
            case 4:
                displayTopKPlayers();
                break;
            case 5:
                displayPlayersByRole();
                break;
            case 6:
                freeMemory();
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}