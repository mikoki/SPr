#include <stdio.h>
#include <string.h>

int main()
{
    struct Participant {
    char first_name[20];
    char last_name[20];
    int life;
    };

    struct Participant Participant1;
    struct Participant Participant2;
    struct Participant Participant3;
    struct Participant Participant4;
    struct Participant Participant5;
    struct Participant Participant6;
    struct Participant Participant7;
    struct Participant Participant8;
    struct Participant Participant9;
    struct Participant Participant10;
    struct Participant Participant11;
    struct Participant Participant12;
    struct Participant Participant13;
    struct Participant Participant14;
    struct Participant Participant15;
    struct Participant Participant16;
    struct Participant read_from_file;

// Create participants

    strcpy(Participant1.first_name, "Emil"); 
    strcpy(Participant1.last_name, "Ivanov");
    Participant1.life = 100;

    strcpy(Participant2.first_name, "Konstantin"); 
    strcpy(Participant2.last_name, "Vladimirov");
    Participant2.life = 100;

    strcpy(Participant3.first_name, "Elena"); 
    strcpy(Participant3.last_name, "Krasimirova");
    Participant3.life = 100;

    strcpy(Participant4.first_name, "Anton"); 
    strcpy(Participant4.last_name, "Kamenov");
    Participant4.life = 100;

    strcpy(Participant5.first_name, "Samuil"); 
    strcpy(Participant5.last_name, "Petrov");
    Participant5.life = 100;

    strcpy(Participant6.first_name, "Daqna"); 
    strcpy(Participant6.last_name, "Marinova");
    Participant6.life = 100;

    strcpy(Participant7.first_name, "Viktor"); 
    strcpy(Participant7.last_name, "Atanasov");
    Participant7.life = 100;

    strcpy(Participant8.first_name, "Mariq"); 
    strcpy(Participant8.last_name, "Ognqnova");
    Participant8.life = 100;

    strcpy(Participant9.first_name, "Radostina"); 
    strcpy(Participant9.last_name, "Mirkova");
    Participant9.life = 100;

    strcpy(Participant10.first_name, "Atanas"); 
    strcpy(Participant10.last_name, "Blagoev");
    Participant10.life = 100;

    strcpy(Participant11.first_name, "Borqna"); 
    strcpy(Participant11.last_name, "Stefanova");
    Participant11.life = 100;

    strcpy(Participant12.first_name, "Lidiq"); 
    strcpy(Participant12.last_name, "Popgeorgieva");
    Participant12.life = 100;

    strcpy(Participant13.first_name, "Kaloqn"); 
    strcpy(Participant13.last_name, "Dimitrov");
    Participant13.life = 100;

    strcpy(Participant14.first_name, "Teodora"); 
    strcpy(Participant14.last_name, "Slavcheva");
    Participant14.life = 100;    


    strcpy(Participant15.first_name, "Anatoli"); 
    strcpy(Participant15.last_name, "Ivanov");
    Participant15.life = 100;    


    strcpy(Participant16.first_name, "Mila"); 
    strcpy(Participant16.last_name, "Plamenova");
    Participant16.life = 100;    

// Container - array of structures

    struct Participant participant[16] = {Participant1, Participant2, Participant3, 
        Participant4, Participant5, Participant6, 
        Participant7, Participant8, Participant9, 
        Participant10, Participant11, Participant12, 
        Participant13, Participant14, Participant15, 
        Participant16};

// Write in binary file

    FILE *fh = fopen ("participants.bin", "wb");
    if (fh != NULL) {
        for(int i = 0; i < (sizeof participant / sizeof participant[0]); i++){
            fwrite (&participant[i], sizeof (struct Participant), 1, fh);
        }
    }
    fclose (fh);

// Read from binary file
    fopen ("participants.bin", "rb");
    fseek(fh, 0, SEEK_SET); //return to the beginning of the file
    if (fh != NULL) {
        while(fread(&read_from_file, sizeof(struct Participant), 1, fh)){ 
            printf ("%s %s %d\n", read_from_file.first_name, read_from_file.last_name, read_from_file.life); 
        }
    }
    fclose (fh);

    return 0;
}