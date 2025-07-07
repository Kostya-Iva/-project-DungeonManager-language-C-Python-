#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct Unit 
{	
	char name[30];

	float initiative; 
	float initiativeRate; 
	
	char team[10];
	
	int healthPoints;
	int armorClass;
	
	int hitRate;
	int damage;
	
	char status[30];
	
	//NEW FUNCTIONS : SHOW LOGS, STOP, LOOP FOR 
	//new battle logics
};


float rollCheckIfCritical()
{
	float result = 1 + rand() % 20;
	if (result == 20)
	{
		printf("\nCRITICAL SUCCES! ");
	}
	else if (result == 1)
	{
		printf("\nCRITICAL FAILURE! ");
	}
	return result;
}

void initiativeRoll(struct Unit units[], int count)
{
	for (int i = 0; i < count; i++)
	{
		 units[i].initiative = units[i].initiativeRate + rollCheckIfCritical(); 
		 if (units[i].initiative - units[i].initiativeRate == 20 || units[i].initiative - units[i].initiativeRate == 1)
		 {
			 printf("for: %s", units[i].name);
		 }
	}
}

//sorting algorithm
int partition (struct Unit units[], int low, int high) 
{
    float pivot = units[high].initiative;
    int i = (low - 1);
    for (int j = low; j < high; j++) 
	{
        if (units[j].initiative >= pivot) 
		{
            i++;
            struct Unit temp = units[i];
            units[i] = units[j];
            units[j] = temp;
        }
    }
	
    struct Unit temp = units[i + 1];
    units[i + 1] = units[high];
    units[high] = temp;
    return (i + 1);
}

void sortInitiative (struct Unit units[], int low, int high)
{
    if (low < high) 
	{
        int pi = partition(units, low, high);
        sortInitiative(units, low, pi - 1);
        sortInitiative(units, pi + 1, high);
    }
}
//sorting algorithm

void initiativeConflict(struct Unit units[], int count)
{
	for (int i = 0; i < count; i++)
	{
		float modifier = 0.5;
		for (int j = i + 1; j < count; j++)
		{
			if (units[i].initiative == units[j].initiative)
			{
				//printf("\nOccured");
				units[j].initiative += modifier;
				modifier /= 2.0;
			}
		}
	}
}

void attackDamage(struct Unit *units, int i, int j, int isCritical)
{
    int finalDamage = units[j].damage;
    if (isCritical)
    {
        finalDamage *= 2;
    }
    units[i].healthPoints -= finalDamage;
}

//ATTACK LOGIC?
void attackCheck(struct Unit *units, int count, int j)
{
	if (strcmp(units[j].team, "PLAYER") == 0)
	{
		//printf("\n*I will attack the enemy*");
		for (int i = 0; i < count; i++)
		{
			if (strcmp(units[i].team, "MONSTER") == 0 && units[i].healthPoints > 0)
			{
				float rollResult = rollCheckIfCritical() + units[j].hitRate;
				//printf("\nMy target is: %s", units[i].name);
				//printf("\n*My attack roll result is %d*", rollResult);
				//printf("\nENEMY AC: %d", units[i].armorClass);
				if (rollResult > (float)units[i].armorClass || rollResult - (float)units[j].hitRate == 20)
				{
					printf("\n*My attack landed!*\n");
					//printf("%g\n", rollResult);
					
					int oldHP = units[i].healthPoints;
					if(rollResult - (float)units[j].hitRate == 20)
						attackDamage(units, i, j, 1);
					else
						attackDamage(units, i, j, 0);
					printf("\n%s's HP:%d -> %d \n", units[i].name, oldHP, units[i].healthPoints);
				}
				else
				{
					printf("\n*I missed!*\n");
					//printf("%g", rollResult);
				}
				break;
			}
		}
	}
	else if (strcmp(units[j].team, "MONSTER") == 0)
	{
		//printf("\n*I will attack the player*");
		for (int i = 0; i < count; i++)
		{
			if (strcmp(units[i].team, "PLAYER") == 0 && units[i].healthPoints > 0)
			{
				float rollResult = rollCheckIfCritical() + (float)units[j].hitRate;
				//printf("\nMy target is: %s", units[i].name);
				//printf("\n*My attack roll result is %d*", rollResult);
				//printf("\nPLAYER AC: %d", units[i].armorClass);
				if (rollResult > (float)units[i].armorClass || rollResult - (float)units[j].hitRate == 20)
				{
					printf("\n*My attack landed!*\n");
					//printf("%g\n", rollResult);
					
					int oldHP = units[i].healthPoints;
					if(rollResult - (float)units[j].hitRate == 20)
						attackDamage(units, i, j, 1);
					else
						attackDamage(units, i, j, 0);
					printf("\n%s's HP:%d -> %d \n", units[i].name, oldHP, units[i].healthPoints);
				}
				else
				{
					printf("\n*I missed!*\n");
					//printf("%g", rollResult);
				}
				break;
			}
		}
	}
	else
	{
		printf("\n*I don't know what to do*");
	}
}

int isEveryoneAliveInTeam(struct Unit *units, int count, int *PlayerIndex, int *EnemyIndex)
{
	for(int i = 0; i < count; i++)
	{
	//	printf("%s is %s", units[i].name, units[i].status);
		if (strcmp(units[i].team, "PLAYER") == 0 && strcmp(units[i].status, "ALIVE") == 0)
		{
			if (units[i].healthPoints <= 0)
			{
				(*PlayerIndex)--;
				strcpy(units[i].status, "DEAD");
				printf("\nPlayer is now DEAD\n");
			}
		}
		else if (strcmp(units[i].team, "MONSTER") == 0 && strcmp(units[i].status, "ALIVE") == 0)
		{
			if(units[i].healthPoints <= 0)
			{
				(*EnemyIndex)--;
				strcpy(units[i].status, "DEAD");
				printf("\nMonster is now DEAD\n");
			}
		}
		else
		{
			//printf("\ncouldn't find anyone\n");
			continue;
		}
	}
}

int main()
{
	srand(time(NULL));
	int PlayerCount = 0; 
	int EnemyCount = 0;
	int battleSize = 0;
	
	FILE *filePlayer = fopen("Units.txt", "r");
	if (filePlayer == NULL)
	{
		printf("Error with file occured");
		return 1;
	}	
	
	//SIZE 
	char buffer[100];
    while (fgets(buffer, sizeof(buffer), filePlayer) != NULL) 
	{
		battleSize++;
    }

	struct Unit *units = malloc(battleSize * sizeof(struct Unit));
	if (units == NULL)
	{
		printf("Memory allocation failed\n");
		return 1;
	}
	
	int i = 0;
	rewind(filePlayer);
	
	//GET DATA FROM FILE
	while (fgets(buffer, sizeof(buffer), filePlayer) != NULL)
	{
		char * token;
		//GET NAME
		token = strtok(buffer, ",");
		strncpy(units[i].name, token, sizeof(units[i].name) - 1);
		units[i].name[sizeof(units[i].name) - 1] = '\0'; 
		
		//GET INITIATIVE RATE
		token = strtok(NULL, ",");
        if (token != NULL) 
		{
            units[i].initiativeRate = atof(token); 
        }
		else 
		{
            units[i].initiativeRate = 0.0;
        }
		
		//GET TEAM 
		token = strtok(NULL, ",");
		token[strcspn(token, "\n")] = 0;
		strncpy(units[i].team, token, sizeof(units[i].team) - 1);
		units[i].team[sizeof(units[i].team) - 1] = '\0'; 
		if (strcmp(units[i].team, "MONSTER") == 0)
		{
			EnemyCount++;
		}
		else if (strcmp(units[i].team, "PLAYER") == 0)
		{
			PlayerCount++;
		}
		
		//GET HP
		token = strtok(NULL, ",");
        if (token != NULL) 
		{
            units[i].healthPoints = atoi(token); 
        }
		else 
		{
            units[i].healthPoints = 0;
        }
		
		
		//GET AC
		token = strtok(NULL, ",");
        if (token != NULL) 
		{
            units[i].armorClass = atoi(token); 
        }
		else 
		{
            units[i].armorClass = 0;
        }
		
		//GET HITRATE
		token = strtok(NULL, ",");
        if (token != NULL) 
		{
            units[i].hitRate = atoi(token); 
        }
		else 
		{
            units[i].hitRate = 0;
        }
		
		//GET DAMAGE
		token = strtok(NULL, ",");
        if (token != NULL) 
		{
            units[i].damage = atoi(token); 
        }
		else 
		{
            units[i].damage = 0;
        }
		
		//GET STATUS
		token = strtok(NULL, ",");
		token[strcspn(token, "\n")] = 0;
		strncpy(units[i].status, token, sizeof(units[i].status) - 1);
		units[i].status[sizeof(units[i].status) - 1] = '\0'; 
		i++;
	}
	

	//TEST
	printf("There are %d Units!\n", battleSize);
	printf("There are %d Players and %d Enemies!\n", PlayerCount, EnemyCount);
	printf("Battle begins!\nRoll for the initiative!\n");
	
	initiativeRoll(units, battleSize);
	printf("\n");
	initiativeConflict(units, battleSize);
	sortInitiative(units, 0, battleSize - 1);

	printf("\nInitiatives with rates are:\n");
	for (int i = 0; i < battleSize; i++)
	{
		printf("%s", units[i].name);
		printf(": %g ", units[i].initiative);
		printf("(%s) ", units[i].team);
		printf("HP:%d, ", units[i].healthPoints);
		printf("AC:%d ", units[i].armorClass);
		printf("HITRATE:+%d\n", units[i].hitRate);
	}
	
	int roundIndex = 1;
	while(EnemyCount > 0 && PlayerCount > 0)
	{
		printf("\nRound %d\n", roundIndex);

		for (int j = 0; j < battleSize; j++)
		{
			printf("%s's Turn", units[j].name);
			if(strcmp(units[j].status, "DEAD") == 0)
			{
				printf("\n");
				printf("\n...I'm already dead...\n");
				printf("\n");
			}
			else
			{
				attackCheck(units, battleSize, j);
				isEveryoneAliveInTeam(units, battleSize, &PlayerCount, &EnemyCount);
				//getchar();
			}
		}
		roundIndex++;
	}
	printf("\nBattle is over!\n");
	if(EnemyCount <= 0 && PlayerCount > 0 )
		printf("Players remained victorious!\n");
	
	else if(PlayerCount <= 0 && EnemyCount > 0)
		printf("Enemies remained victorious!\n");
	
	else
		printf("An error occured!");

	int PlayerBattlResult = 0;
	int EnemyBattlResult = 0;
	for (int i = 0; i < battleSize; i++)
	{
		if(strcmp(units[i].team, "PLAYER") == 0)
			PlayerBattlResult =+ units[i].healthPoints;

		else if(strcmp(units[i].team, "MONSTER") == 0)
			EnemyBattlResult =+ units[i].healthPoints;

		else
			printf("I see no one\n");

	}
	
	//printf("Player Health result: %d\n", PlayerBattlResult);
	//printf("Enemy Health result: %d\n", EnemyBattlResult);
	int BattleResult = EnemyBattlResult - PlayerBattlResult;
	printf ("Battle Result is: %d", BattleResult);
	if (BattleResult <= -10)
		printf("\nBattle is Easy");
	else if (BattleResult > -10 && BattleResult <= 10)
		printf("\nBattle is Fair");
	else if (BattleResult > 10 && BattleResult <= 30)
		printf("\nBattle is Hard");
	else
		printf("\nBattle is Impossible!");
	
	free(units);
	fclose(filePlayer);
	return 0;
}