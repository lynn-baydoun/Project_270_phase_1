#include <stdio.h>  //in/out
#include <stdlib.h> //memory alloc
#include <string.h> //string manipulation
#include <time.h>   //time and randomness

#define MAX_SPELLS 100
#define SPELL_LENGTH 30

// check if a spell is valid, if valid => 1, 0 otherwise
int isValidSpell(char *spell, char *prevSpell)
{
    // spell has not been cast before
    if (strcmp(spell, prevSpell) == 0)
    {
        printf("Game over. Spell has already been cast.\n");
        return 0;
    }

    // 1st character of current spell = the last character of the previous one
    if (strlen(prevSpell) > 0 && spell[0] != prevSpell[strlen(prevSpell) - 1])
    {
        printf("Game over. Spell does not match the previous spell.\n");
        return 0;
    }

    return 1;
}

int main()
{
    // random number generation
    srand(time(NULL));

    // read spells from file
    FILE *file = fopen("spells.txt", "r");
    if (!file)
    {
        printf("Error opening file\n");
        return 1;
    }

    char spells[MAX_SPELLS][SPELL_LENGTH];
    int numSpells;

    fscanf(file, "%d", &numSpells);

    for (int i = 0; i < numSpells; ++i)
    {
        fscanf(file, "%s", spells[i]);
    }

    fclose(file);

    // prompt players for their names
    char player1[30], player2[30];
    printf("Enter name for Player 1: ");
    scanf("%s", player1);
    printf("Enter name for Player 2: ");
    scanf("%s", player2);

    // print spells
    printf("Available Spells:\n");
    for (int i = 0; i < numSpells; ++i)
    {
        printf("%-15s", spells[i]);
        if ((i + 1) % 5 == 0)
        {
            printf("\n");
        }
    }

    // coin toss
    int currentPlayer = rand() % 2 + 1;
    printf("\n%s will start.\n", (currentPlayer == 1) ? player1 : player2);

    // loop
    char prevSpell[SPELL_LENGTH] = "";
    // white the conditions of the spells are met
    while (1)
    {
        char currentSpell[SPELL_LENGTH];
        printf("%s, enter your spell: ", (currentPlayer == 1) ? player1 : player2);
        scanf("%s", currentSpell);

        // Check if the spell is valid
        if (!isValidSpell(currentSpell, prevSpell))
        {
            break;
        }

        // update the prev spell
        strcpy(prevSpell, currentSpell);

        // switch to other player
        currentPlayer = (currentPlayer == 1) ? 2 : 1;

        // check for winner if there are no more spells that meet the conditions
        if (numSpells == 0)
        {
            printf("%s wins! %s ran out of spells.\n", (currentPlayer == 1) ? player2 : player1,
                   (currentPlayer == 1) ? player1 : player2);
            break;
        }
    }

    return 0;
}
