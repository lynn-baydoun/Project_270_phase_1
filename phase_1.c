//https://github.com/lynn-baydoun/Project_270_phase_1.git
#include <stdio.h>  //in/out
#include <stdlib.h> //memory alloc
#include <string.h> //string manipulation
#include <time.h>   // time and randomness

#define MAX_SPELLS 100  // max number of spells
#define SPELL_LENGTH 30 // max length of spell

// Define a struct to represent a spell
struct Spell
{
    char name[SPELL_LENGTH];
    int isUsed; // Flag to track if the spell has been chosen
};

// Function to check if a spell has already been cast
int isSpellRepeated(struct Spell *chosenSpells, char *spell,int *numChosenSpells)
{
    if(*numChosenSpells>0){
        for (int i = 0; i < *numChosenSpells; i++)
        {
            if (strcmp(chosenSpells[i].name, spell) == 0)
            {
                return 1; // Spell is repeated
            }
        }
    }
    return 0; //spell is not repeated
}

// Function to check if two spells match (last letter of previous spell with the first letter of the new spell)
int doSpellsMatch(char spell1[], char *spell2, int *numChosenSpells)
{
    if(*numChosenSpells == 0){
        return 1;
    }
    return (spell1[strlen(spell1) - 1] == spell2[0]);
}

// Function to check if there are available spells that satisfy the last letter and first letter condition (this will help with terminating the game and annoucing the winner)
int areAvailableSpells(struct Spell *spells, int numSpells, struct Spell *chosenSpells, int *numChosenSpells)
{
    if(*numChosenSpells==0){
        return 1;
    }
    char lastChosenSpellLastLetter = chosenSpells[*numChosenSpells - 1].name[strlen(chosenSpells[*numChosenSpells - 1].name) - 1];
    for (int i = 0; i < numSpells; i++)
    {
        if (!spells[i].isUsed && spells[i].name[0] == lastChosenSpellLastLetter)
        {
            return 1; // There is an available spell that satisfies the condition
        }
    }
    return 0; // No available spells satisfy the condition
}

int spellList(int numSpells, char *chosenSpell, struct Spell *spells ){
    // Check if the spell is in the list of available spells
        int spellIndex = -1;
        for (int i = 0; i < numSpells; i++)
        {
            if (strcmp(spells[i].name,chosenSpell) == 0 && !spells[i].isUsed)
            {
                spellIndex = i;
                break;
            }
        }

        return spellIndex;           
        
}

int isValid(int numSpells, char *chosenSpell, struct Spell *spells,int *numChosenSpells,struct Spell *chosenSpells,int loser,char player1[]){
    int index = spellList(numSpells,chosenSpell,spells);
        if(isSpellRepeated(chosenSpells, chosenSpell,numChosenSpells)){
            printf("Invalid spell! %s win(s) because the other player repeated a spell.\n",loser==1 ? "The computer":player1);
            return 0;
        }else if(index == -1){
            printf("Invalid spell! %s wins because the other player entered a spell not in the list.\n",loser==1 ? "The computer":player1);
            return 0;
        }else if(!doSpellsMatch(chosenSpells[*numChosenSpells - 1].name,chosenSpell,numChosenSpells)){
            printf("Invalid spell! %s win(s) because the other player chose a spell that does not match the last character.\n",loser==1 ? "The computer":player1);
            return 0;
        }

    // Mark the chosen spell as used
    spells[index].isUsed = 1;
    // Add the chosen spell to the list of chosen spells
    strcpy(chosenSpells[*numChosenSpells].name, chosenSpell);
    (*numChosenSpells)++;
    return 1;
}
int HumanTurn(char *chosenSpell, char player1[],int numSpells, struct Spell *spells,int *numChosenSpells,struct Spell *chosenSpells,int loser){
    // Prompt the current player for their move
    printf("Enter a spell, %s: ", player1);
    scanf(" %s", chosenSpell);
    return isValid(numSpells,chosenSpell,spells,numChosenSpells,chosenSpells,1,player1);
}

//Function that is used in games for decision making
int minimax(struct Spell *spells, int numSpells, char prevSpell[SPELL_LENGTH], int difficulty){
    if (numSpells == 0){
        return (difficulty == 2) ? -1 : 1;
    }//In hard mode, bot tries to maximize its score.
    int bestScore = (difficulty == 2) ? - 1 : 1;

    for(int i = 0; i < numSpells; i++){
        if(!spells[i].isUsed && doSpellsMatch(prevSpell, spells[i].name, NULL)){
            char newPrevSpell[SPELL_LENGTH];
            strcpy(newPrevSpell, spells[i].name);
            int newScore = minimax(spells, numSpells - 1, newPrevSpell, difficulty);
            if(difficulty == 2){
                if (newScore > bestScore){
                    bestScore = newScore;
                }
            }
            else{
                if (newScore < bestScore){
                    bestScore = newScore;
                }
            }
        }
    }
    return bestScore;
}

int BotTurn(int difficulty, struct Spell *spells,int numSpells,int *numChosenSpells,struct Spell *chosenSpells,int loser,char player1[]){
    int result = 0;
    if(difficulty == 0 || *numChosenSpells == 0){
        int index = (int) rand() % numSpells; 
        char *chosenSpellBot = spells[index].name;
        printf("Computer's choice: %s\n",spells[index].name);
        result = isValid(numSpells, chosenSpellBot, spells, numChosenSpells, chosenSpells, 0, player1);        
    }else if(difficulty == 1){
        for(int i = 0;i < numSpells;i++){
            char *currentSpell = spells[i].name;
            if(doSpellsMatch(chosenSpells[*numChosenSpells - 1].name, currentSpell, numChosenSpells) && !spells[i].isUsed){
                result = 1;
                spells[i].isUsed = 1;
                (*numChosenSpells)++; 
                strcpy(chosenSpells[*numChosenSpells-1].name, spells[i].name);
                printf("Computer's choice: %s\n",currentSpell);
                break;
            }
        }
    }
    //The bot uses the minimax algorithm to select the best move.
    else if(difficulty == 2){
        int bestScore = -1;
        int bestMoveIndex = - 1;
        for (int i = 0; i < numSpells; i++){
            if(!spells[i].isUsed){
                char *currentSpell = spells[i].name;
                int currentScore = minimax(spells, numSpells, currentSpell, difficulty);
                if(currentScore > bestScore){
                    bestScore = currentScore;
                    bestMoveIndex = i;
                }
            }
        }  
        if(bestMoveIndex != -1){
            result = 1;
            spells[bestMoveIndex].isUsed = 1;
            strcpy(chosenSpells[*numChosenSpells].name, spells[bestMoveIndex].name);
            (*numChosenSpells)++;
            printf("Computer's choice:%s\n", spells[bestMoveIndex].name);
        }
    }

    return result;
}

int main()
{
    FILE *file;
    struct Spell spells[MAX_SPELLS];
    struct Spell chosenSpells[MAX_SPELLS];
    int numSpells, numChosenSpells = 0;
    char player1[30];
    int availableSpells;
    char *chosenSpell = (char*) malloc(sizeof(char)*SPELL_LENGTH) ;
   
  
    // Read spells from 'spells.txt' file
    file = fopen("spells.txt", "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    fscanf(file, "%d", &numSpells);
    for (int i = 0; i < numSpells; i++)
    {
        fscanf(file, "%s", spells[i].name);
        spells[i].isUsed = 0; // Initialize all spells as unused
    }
    fclose(file);

    // Initialize random seed for coin toss
    srand(time(NULL));

    // Get player names
    printf("Enter Player 1's name: ");
    scanf("%s", player1);

    printf("Enter the level of difficulty(0 for easy, 1 for medium, 2 for hard): ");
    int difficulty;
    scanf("%d",&difficulty);
    // Initialize available spells count
    availableSpells = numSpells;

    // Display the list of spells
    printf("\nAvailable Spells:\n");
    for (int i = 0; i < numSpells; i++)
    {
        printf("%-35s", spells[i].name);
        if ((i + 1) % 5 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");

    // Toss a coin to determine the starting player
    int currentPlayer = rand() % 2;
    printf("\n%s will start.\n", (currentPlayer == 1) ? player1 : "The computer");

    while (availableSpells > 0)
    { // Check if there are available spells left
        if(currentPlayer == 1){
            if(!HumanTurn(chosenSpell,player1,numSpells,spells,&numChosenSpells,chosenSpells,1)){
                printf("Game over");
                break;
            }
        }else{
            if(!BotTurn(difficulty,spells,numSpells,&numChosenSpells,chosenSpells,0,player1)){
                printf("Game Over");
                break;
            }
        }

        // Mark the chosen spell as used
        availableSpells--;

        // Switch it to the other player before checking for available spells
        currentPlayer = (currentPlayer == 1) ? 0 : 1;

        // Check if there are no more available spells left
        if (!areAvailableSpells(spells, numSpells,chosenSpells, &numChosenSpells))
        {
            printf("No more available spells that satisfy the condition. %s wins!\n", (currentPlayer == 1) ? "The computer" : player1);
            break;
        }
    }

    return 0;
}