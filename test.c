// Github: https://github.com/lynn-baydoun/Project_270_phase_1.git
#include <stdio.h> //in/out
#include <stdlib.h> //memory alloc
#include <string.h> //string manipulation
#include <time.h> // time and randomness

#define MAX_SPELLS 100  //max number of spells
#define SPELL_LENGTH 30  // max length of spell 

// Define a struct to represent a spell
struct Spell {
    char name[SPELL_LENGTH];
    int isUsed; // Flag to track if the spell has been chosen
};

// Function to check if a spell has already been cast
int isSpellRepeated(struct Spell *chosenSpells, int numChosenSpells, char *spell) {
    for (int i = 0; i < numChosenSpells; i++) {
        if (strcmp(chosenSpells[i].name, spell) == 0) {
            return 1; // Spell is repeated
        }
    }
    return 0; // Spell is not repeated
}

// Function to check if two spells match (last letter of previous spell with the first letter of the new spell)
int doSpellsMatch(char *spell1, char *spell2) {
    return (spell1[strlen(spell1) - 1] == spell2[0]);
}

// Function to check if there are available spells that satisfy the last letter and first letter condition (this will help with terminating the game and annoucing the winner)
int areAvailableSpells(struct Spell *spells, int numSpells, struct Spell *chosenSpells, int numChosenSpells) {
    for (int i = 0; i < numSpells; i++) {
        if (!spells[i].isUsed && spells[i].name[0] == chosenSpells[numChosenSpells - 1].name[strlen(chosenSpells[numChosenSpells - 1].name) - 1]) {
            return 1; // There is an available spell that satisfies the condition
        }
    }
    return 0; // No available spells satisfy the condition
}

int main() {
    FILE *file;
    struct Spell spells[MAX_SPELLS];
    struct Spell chosenSpells[MAX_SPELLS];
    int numSpells, numChosenSpells = 0;
    char player1[30], player2[30];
    int currentPlayer;
    int availableSpells;

    // Read spells from 'spells.txt' file
    file = fopen("spells.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    fscanf(file, "%d", &numSpells);
    for (int i = 0; i < numSpells; i++) {
        fscanf(file, "%s", spells[i].name);
        spells[i].isUsed = 0; // Initialize all spells as unused
    }
    fclose(file);

    // Initialize random seed for coin toss
    srand(time(NULL));

    // Get player names
    printf("Enter Player 1's name: ");
    scanf("%s", player1);
    printf("Enter Player 2's name: ");
    scanf("%s", player2);

    // Initialize available spells count
    availableSpells = numSpells;

    // Display the list of spells
    printf("\nAvailable Spells:\n");
    for (int i = 0; i < numSpells; i++) {
        printf("%-35s", spells[i].name);
        if ((i + 1) % 5 == 0) {
            printf("\n");
        }
    }
    printf("\n");

    // Toss a coin to determine the starting player
    int p = rand() % 2 + 1;
    printf("\n%s will start.\n", (currentPlayer == 1) ? player1 : player2);

    while (availableSpells > 0) { // Check if there are available spells left 
        int validMove = 0;
        char chosenSpell[SPELL_LENGTH];

        // Prompt the current player for their move
        printf("Enter a spell, %s: ", (currentPlayer == 1) ? player1 : player2);
        scanf("%s", chosenSpell);

        if (numChosenSpells > 0) {
            // Check if the spell has already been cast
            if (isSpellRepeated(chosenSpells, numChosenSpells, chosenSpell)) {
                printf("Invalid spell! %s wins because the other player repeated a spell.\n", (currentPlayer == 1) ? player2 : player1);
                break;
            }

            // Check if the spell matches the last character of the previous spell
            if (!doSpellsMatch(chosenSpells[numChosenSpells - 1].name, chosenSpell)) {
                printf("Invalid spell! %s wins because the other player chose a spell that does not match the last character.\n", (currentPlayer == 1) ? player2 : player1);
                break;
            }
        }

        // Check if the spell is in the list of available spells
        int spellIndex = -1;
        for (int i = 0; i < numSpells; i++) {
            if (strcmp(spells[i].name, chosenSpell) == 0 && !spells[i].isUsed) {
                spellIndex = i;
                break;
            }
        }

        if (spellIndex == -1) {
            printf("Invalid spell! %s wins because the other player entered a spell not in the list.\n", (currentPlayer == 1) ? player2 : player1);
            break;
        }

        // Mark the chosen spell as used
        spells[spellIndex].isUsed = 1;

        // Add the chosen spell to the list of chosen spells
        strcpy(chosenSpells[numChosenSpells].name, chosenSpell);
        numChosenSpells++;

        // Mark the chosen spell as used
        availableSpells--;

        // Switch to the other player before checking for available spells
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
        
        // Check if there are no more available spells left
        if (!areAvailableSpells(spells, numSpells, chosenSpells, numChosenSpells)) {
            printf("No more available spells that satisfy the condition. %s wins!\n", (currentPlayer == 1) ? player2 : player1);
            break;
        }
    }

    return 0;
}