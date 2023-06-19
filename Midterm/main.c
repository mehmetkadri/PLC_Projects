#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct{
char *word; //word and corresponding hint
char *clue;
int x; //Starting x and y positions
int y;
char direction; //H for horizontal, V for vertical
int f; //solved or not
} Word_t;

FILE* getFile();
Word_t* loadTextFile( FILE* file, int wordCount );
char** createArray(int rowCount, int colCount);
int isBoardFilled(int rowCount, int colCount, char** board);
void displayBoard(int rowCount, int colCount, char** board);
int findIndex(int arr[], int size, int val);
char** updateBoard(char** board, Word_t* array, int solve);
void playGame(char** board, int wordCount, Word_t *array, int rowCount, int colCount, int knownWordCount);

int main(){

    //Dear Professor, I've added an extra part to my code to show the word if needed. I hope it's not a problem for you. Take care :)
    //Hocam koduma ek olarak kelimeyi göstermek için bir kýsým ekledim. Umarým sýkýntý oluþturmaz. Kendinize iyi bakýn :)

    printf("Dear Professor, I've added an extra part to my code to show the word if needed.\nI hope it's not a problem for you. Take care :)\n\n");

    char** board;

    char firstLine[10];

    FILE* file = getFile();

    int rowCount,colCount,wordCount;

    fgets(firstLine, sizeof(firstLine), file);
    int index=strlen(firstLine)-2;
    rowCount = firstLine[0] - '0' ;
    colCount = firstLine[2] - '0' ;
    wordCount = firstLine[4] - '0' ;
    if(index==5){
        wordCount=(wordCount*10) + (firstLine[5] - '0');
    }
    Word_t* array = (Word_t*)malloc(wordCount * sizeof(Word_t));

    array = loadTextFile(file, wordCount);

    board = createArray(rowCount,colCount);

    playGame(board, wordCount, array, rowCount, colCount, 0 /*knownWordCount*/);

    fclose(file);
    return 0;
}

FILE* getFile(){

    char fileName[256];
    printf("Please enter the name of the file: ");
    scanf("%s",&fileName);

    FILE* txtFile = fopen(fileName, "r");

    if (txtFile == NULL){
        exit(EXIT_FAILURE);
    }

    return txtFile;

}

Word_t* loadTextFile( FILE* file, int wordCount ){

    Word_t* tempArray = (Word_t*)malloc(wordCount * sizeof(Word_t));

    for (int i = 0; i < wordCount; i++) {
        (*(tempArray + i)).word = (char *) malloc(256 * sizeof (char));
        (*(tempArray + i)).clue = (char *) malloc(256 * sizeof (char));

        fscanf(file, "%c %d %d %s %[^\n]\n", &((tempArray + i))->direction, &((tempArray + i))->x, &((tempArray + i))->y, ((tempArray + i))->word, ((tempArray + i))->clue);

        (*(tempArray + i)).f = 0;

    }

    return tempArray;
}

char** createArray(int rowCount, int colCount){

    char** temporaryBoard = (char**)malloc(rowCount * sizeof(char*));

    for(int i = 0 ; i < rowCount ; i++){
        temporaryBoard[i] = (char*)malloc(colCount * sizeof(char));
    }

    for(int i = 0 ; i < rowCount ; i++){
        for(int j = 0 ; j < colCount ; j++){
            temporaryBoard[i][j] = '#';
        }
    }

    return temporaryBoard;
}

int isBoardFilled(int rowCount, int colCount, char** board){

    for(int i = 0 ; i < rowCount ; i++){
        for(int j = 0 ; j < colCount ; j++){
            if(board[i][j]== '_'){
                return 0;
            }
        }
    }
    return 1;
}

void displayBoard(int rowCount, int colCount, char** board){

    printf("\t  ");
    for(int i = 0 ; i < colCount ; i++){
        printf("%d  ",i+1);
    }
    printf("\n\t  ");
    for(int i = 0 ; i < colCount ; i++){
        printf("-- ");
    }

    for(int i = 0 ; i < rowCount ; i++){
        printf("\n%d\t| ",i+1);
        for(int j = 0 ; j < colCount ; j++){
            printf("%c  ",board[i][j]);
        }
    }
    printf("\n\t");
    for(int i = 0 ; i < colCount ; i++){
        printf("***");
    }
    printf("\n");

}

int findIndex(int arr[], int size, int val){

    if(val>size){
        return 0;
    }else if(val<=0){
        return 0;
    }else if(arr[val-1]==0){
        return 2;
    }else{
        return 1;
    }

}

char** updateBoard(char** board, Word_t *array, int solve){

    int col = ((array+solve)->x)-1;
    int row = ((array+solve)->y)-1;

    for(int i = 0 ; i < strlen((array+solve)->word) ; i++){

        if((array+solve)->f==1){
            board[col][row]=(array+solve)->word[i];
        }else if((array+solve)->f==0){
            board[col][row]='_';
        }

        if((array+solve)->direction=='H'){
            row++;
        }else if((array+solve)->direction=='V'){
            col++;
        }

    }

}

void playGame(char** board, int wordCount, Word_t *array, int rowCount, int colCount, int knownWordCount){

    int inputQuestion;
    char *inputQuestionChar[256];
    char inputAnswer[256];
    int arr[wordCount];

    for(int i = 0 ; i < wordCount ; i++){
        arr[i]=5;
    }

    for(int i = 0 ; i < wordCount ; i++){
        updateBoard(board, array, i);
    }

    printf("\nGame is %d rows x %d cols with %d words\n", rowCount, colCount, wordCount);
    printf("Words and clues are read!\n");


    while(knownWordCount<wordCount){
        for(int i = 0 ; i < wordCount ; i++){
            if(arr[i]!=0){
                if(isBoardFilled(rowCount, colCount, board)){
                    goto outOfWhile;
                }

                printf("Current puzzle:\n\n");
                displayBoard(rowCount,colCount,board);
                printf("\nAsk for hint:\n");

                printf("#  Direction\trow\tcol\n");
                printf("----------------------------\n");
                for(int i = 0 ; i < wordCount ; i++){
                    if(arr[i]==0){
                        continue;
                    }
                    printf("%d: ",i+1);
                    if((array+i)->direction=='H'){
                        printf("Horizontal\t");
                    }else if((array+i)->direction=='V'){
                        printf("Vertical\t");
                    }
                    printf("%d\t%d\n",(array+i)->x,(array+i)->y);
                }
                printf("\n");

                solvedBefore:

                printf("Enter -1 to exit\n");

                printf("Which word to solve next?: ");

                scanf("%s",&inputQuestionChar);

                inputQuestion = atoi(inputQuestionChar);

                if(inputQuestion == 0){
                    printf("Not a number!\n");
                    goto solvedBefore;
                }

                if(inputQuestion==-1){
                    knownWordCount=wordCount+1;
                    break;
                }else if((findIndex(arr, wordCount, inputQuestion))==0){
                    printf("Invalid question number\n");
                    goto solvedBefore;
                }else if((findIndex(arr, wordCount, inputQuestion))==2){
                    printf("That word has been solved before\n");
                    goto solvedBefore;
                }

                printf("Current hint: %s\n",(array+inputQuestion-1)->clue);
                printf("If you want to see the solution, enter 'solution*'.\n");
                printf("Enter your solution: ");
                scanf("%s",inputAnswer);

                for(int upperIndex=0;upperIndex<strlen(inputAnswer);upperIndex++){
                    inputAnswer[upperIndex]=toupper(inputAnswer[upperIndex]);
                }

                if((strcmp(inputAnswer,"SOLUTION*"))==0){
                    printf("Word was '%s'\n\n",(array+inputQuestion-1)->word);
                }else if((strcmp(inputAnswer,((array+inputQuestion-1)->word)))==0){
                    printf("\nCorrect!\n\n");
                    (array+inputQuestion-1)->f = 1;
                    updateBoard(board,array,inputQuestion-1);
                    arr[inputQuestion-1]=0;
                    knownWordCount++;
                    continue;
                }else{
                    printf("WRONG ANSWER\n");
                    continue;
                }
            }
        }
    }

    outOfWhile:

    if(knownWordCount==wordCount||isBoardFilled(rowCount, colCount, board)){
        printf("Current puzzle:\n\n");
        displayBoard(rowCount,colCount,board);
        printf("Congratulations! You beat the puzzle!\n");
    }

}
