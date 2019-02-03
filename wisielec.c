#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>
#include<windows.h>
#include<stdbool.h>

void MoveToXY(int x, int y){
    COORD position;
    position.X=x;
    position.Y=y;
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), position );
}
typedef struct{
    char nick[20];
    int wins;
    int losses;
}statistic;
typedef struct{
    char password[50];
    int length;
    int category;
}guessHaslo;
//sprawdzenie czy wprowadzony znak jest liczba
int checkString(){
char choice[20];
int number;
    do{
        printf("\tWyb\242r ");
        scanf(" %s",choice);
        if(choice>='0' && choice<='9'){
        number=atoi(choice);
        return number;
     }
    }while(number==atoi(choice));
}
//sprawdzenie czy liczba zostala uzyta (sluzy do zakrywania liter w alfabecie)
int checkLetter(char letter,char letters[],int numbersLetters){
int i;
int j=0;
    for(i=0; i<numbersLetters; i++){
        if(letter==letters[i]){
            MoveToXY(25,21);
            printf("Litera %c zostala juz uzyta\n",letter);
            return 0;
        }
        else{
            j++;
            if(j==numbersLetters){
                letters[numbersLetters-1]=letter;
                return 1;
            }
        }
    }
j=0;
}
//okreslenie ilosci slow w kazdym dziale
int numberOfWordsAndCursorsInSection(int *words, int lenWord, int *cursors, int lenCur){
    FILE *passwords;
    char cursor;
    int position=0,i=0,word=0;
    if ((passwords=fopen("hasla.txt","r"))==NULL) {
        printf ("\nNie mogę otworzyć pliku do odczytu!\n");
        exit(1);
    }
    while((cursor = getc(passwords)) != EOF){
        position++;
        if(cursor=='\n')word++;
        //Na koncu danego dzialu w pliku wstawione zostaly nr dzialu. Dzieki temu mozna okreslic ilosc hasel w danym dziale
        if(cursor>='0' && cursor<='3'){
            *cursors=position;
            cursors++;
            //Zmniejszone o 1 z powodu uwzglednienia liczb(od 2 dzialu), ktore sluza tylko do wyznaczania "granicy" miedzy kategoriami
            *words=word-1;
            word=0;
            words++;
       }
    }
    fclose(passwords);
}
//odczyt z pliku wylosowanego hasla
char getPassword(char *ps, int len, int section, int randomIndex, int setCursor){
    FILE *passwords;
    char cursor;
    int word=0;
    if ((passwords=fopen("hasla.txt","r"))==NULL) {
                printf ("\nNie mogę otworzyć pliku do odczytu!\n");
                exit(1);
            }
    //Ustawienie kursora z tablicy zawieracjace kursory
    fseek(passwords,setCursor,SEEK_SET);
    while((cursor = getc(passwords)) != section){
        if(cursor=='\n') word++;
        if(word==randomIndex){
            //Pobranie hasla z pliku. Hasla musza byc bez polskich znakow
            fgets(ps,50,passwords);
             break;
        }
    }
}
guessHaslo randomPassword(int category, int numberOfWords[3],int cursorsPositions[3]){
    guessHaslo Haslo;
    int i,len=0;
    srand(time(NULL));
    int randomIndex;
    char *password;
    password=(char*)malloc(50*sizeof(*password));
    //losowanie hasla i pobieranie go z pliku
    switch(category){
        case 1:
            randomIndex=rand() % numberOfWords[0];
            getPassword(password,50,category,randomIndex,0);
            break;
        case 2:
            randomIndex=1+rand() % numberOfWords[1];
            getPassword(password,50,category,randomIndex,cursorsPositions[0]);
        break;
        case 3:
            randomIndex=1+rand() % numberOfWords[2];
            getPassword(password,50,category,randomIndex,cursorsPositions[1]);
            break;
        }
    //Przypisanie elementow do struktury
    len=strlen(password);
    strcpy(Haslo.password,password);
    Haslo.length=len;
    Haslo.category=category;
    free(password);
    return Haslo;
}

int guessPassword(char password[], int length, char *prompt){
    char letters[50];
    char letter;    //pojedynczy znak ktory bedzie brany pod uwage podczas spr hasla

    int numberUsedLetters=1;
    //przechowywanie uzytych liter
    char usedLetters[numberUsedLetters];

    char hiddenPassword[length]; //definicja ukrytego hasla

    int errors=6;   //ilosc bledow do wykonania

    char alphabet[]="a\245bc\206de\251fghijkl\210mn\344o\242pqrs\230tuvwxyz\253\276";
    int numberOfLetters=strlen(alphabet);

    int bufor=0;
    int i=0 , j=0;

    int state;
    int pointers=0; //zliczanie odgadnietych liter
    int fullLength=0; //pomaga okreslic czy dana litera jest hasle

    //wspolrzedne:
    //Komunikat o mozliwych bledow do wykonania
    int infoAboutErrorsX=10;
    int infoAboutErrorsY=15;

    int txtX=10;
    int txtY=23;

    //wspolzedne zakrytego hasla
    int hiddenPasswordX=11;
    int hiddenPasswordY=19;

    //odswiezanie lini. Rozmiar odpowiada szerokosci okna konsoli
    char spaces[100];
        for(i=0; i<100; i++){
        spaces[i]=' ';
        spaces[100]='\0';
        }

    MoveToXY(10,1);
    printf("%s",prompt);

    MoveToXY(infoAboutErrorsX,infoAboutErrorsY);
    printf("Ilosc prob: %d",errors);

    MoveToXY(hiddenPasswordX,hiddenPasswordY);
    for(i=0; i<length; i++){
        hiddenPassword[i]=password[i];
        if((hiddenPassword[i]>='a' && hiddenPassword[i]<='z')||\
            hiddenPassword[i]=='\245'|| hiddenPassword[i]=='\206'||\
            hiddenPassword[i]=='\251'|| hiddenPassword[i]=='\210'||\
            hiddenPassword[i]=='\344'|| hiddenPassword[i]=='\242'||\
            hiddenPassword[i]=='\230'|| hiddenPassword[i]=='\253'|| hiddenPassword[i]=='\276' ){       //tworzenie zakretego hasla z uzyciem polskich znakow

                hiddenPassword[i]='_';
        }
        printf("%c ", hiddenPassword[i]);
    }

    while(errors!=0) {

        int xAlphabet=45;
        int yAlphabet=7;

//tworzenie tabeli z alfabetem
        for(i=0; i<numberOfLetters; i++){
            if(i%7==0){
            MoveToXY(xAlphabet=45,++yAlphabet);
            }
            MoveToXY(xAlphabet+=2,yAlphabet);
            printf("%c",alphabet[i]);
        }
        do{
            MoveToXY(txtX,txtY);
            printf("Wprowadz litere ");
            scanf("%s",letters);
    //odswiezanie lini
            MoveToXY(txtX,txtY);
            printf("%s",spaces);
    //przypisanie pierwszego znaku jesli wprowadzono wiecej niz powinno
            letter=letters[0];
            if((letter>='a' && letter<='z')||\
                letter=='\245'||letter=='\206'||letter=='\251'||\
                letter=='\210'||letter=='\344'||letter=='\242'||\
                letter=='\230'||letter=='\253'|| letter=='\276' ){ //dopuszczanie tylko małych polskich liter
            //sprawdzenie czy litera wystapila w hasle
                    state=checkLetter(letter,usedLetters,numberUsedLetters);
                //gdy dana litera juz wystapila nastepuje zamiana uzytej litery na kreske
                    for(i=0; i<numberOfLetters; i++){
                        if(letter==alphabet[i]){
                        bufor=i;
            //      usedLetters[numberUsedLetters-1]=letter;
                        }
                    }
                    alphabet[bufor]='_';
            }

            else printf("Dopuszczalne sa tylko male litery");
        }while(state!=1);
    /*W momencie gdy litera jest mala i nie byla jeszcze wykorzystana do odgadniecia w hasle
    nastepuje sprawdzenie czy wystepuje w hasle*/
        if(state==1){
    //powiekszenie rozmiaru tablicy z wykorzystanymi literami
            numberUsedLetters++;
                     //wyszukiwanie liter
            for(i=0; i<length; i++){
                if(letter==password[i]){ //wyszukiwanie liter
                    MoveToXY(hiddenPasswordX,hiddenPasswordY);
                    hiddenPassword[i]=password[i];
                }
                else{
                    fullLength++;
                    if(fullLength==length){  //jezeli w ciagu nie znajduje sie dana litera nastepuje wyswietlenie wisielca
                        hangman(--errors);
                        MoveToXY(infoAboutErrorsX,infoAboutErrorsY);
                        printf("\a Pozosta\210a jeszcze %d pr\242ba",errors);
                    }
                }
                if(password[i]==hiddenPassword[i]) ++pointers; // zliczanie ilosci liter w hasle

                //czyszczenie info. o wystepujacej literze
                MoveToXY(0,21);
                printf("%s",spaces);
            }
        }
        MoveToXY(hiddenPasswordX,hiddenPasswordY);
        for(i=0; i<length; i++){
            printf("%c ", hiddenPassword[i]);
            if(pointers==length){       //sprawdzanie czy ilosc zliczonych liter jest rowna dlugosci liter w hasle
                MoveToXY(hiddenPasswordX,hiddenPasswordY);
                for(i=0; i<length; i++){
                    printf("%c ",password[i]);
                }
                MoveToXY(10,23);
                printf("Wygrales!");
                MoveToXY(10,24);
                system("pause");
                return 1;
            }
            if(errors==0){
                MoveToXY(10,23);
                printf("Przegra\210e\230. \a\a\a");
            MoveToXY(10,24);
                printf("Poprawne has\210o to: ");
                for(i=0; i<length; i++){
                    printf("%c ",password[i]);
                }
            MoveToXY(10,25);
                system("pause");
                return 0;
            }
        }
    fullLength=0;     //zredukowanie ilosci nieodnalezionych liter
    pointers=0;  //zredukowanie zliczonych liter
    }
        length=0;
    }

//pobranie informacji o kategori hasla
char *infoAboutSection(char *prompt, int category){
    if(category==1){
        prompt="Kategoria: informatyka";
    }
    else if(category==2){
        prompt="Kategoria: jedzenie";
    }
    else{
        prompt="Kategoria: pa\344stwa";
    }
    return prompt;
}
void singleGame(){
    statistic player;

    guessHaslo Haslo;
    Haslo.category=0;
    int nameLength=0;

    int choice; //podjecie decyzji o kontynuacji gry
    int result; //wynik gry

    int i, sG;

    int category;
    int *sectionPositions;
    sectionPositions=(int*)malloc(3*sizeof(*sectionPositions));
    int *numberOfWords; //informacja i ilosci slow w dziale
    numberOfWords=(int*)malloc(3*sizeof(*numberOfWords));
    char *section;
    section=(char*)malloc(50*sizeof(*section));

    printf("\n\tWitamy w grze jednoosobowej.\n");
    printf("\tProsze wprowadzic nazwe gracza.\n\t");
    scanf("%s",player.nick);
    player.wins=0;
    player.losses=0;

    numberOfWordsAndCursorsInSection(numberOfWords,3,sectionPositions,3);
if(sectionPositions!=NULL && numberOfWords!=NULL){
    //Dodanie o 1. W przeciwnym wypadku zostanie odjety ostatnie haslo
    numberOfWords[0]+=1;

    //z powodu okreslania ilosci slow w dziale za pomoca getc() "ucina" pierwsze znaki i dodaje ta strate poprzez dodawanie ilosci wydzielonych slow
    sectionPositions[0]+=numberOfWords[0];
    sectionPositions[1]=sectionPositions[1]+numberOfWords[0]+numberOfWords[1];
    sectionPositions[2]=sectionPositions[2]+numberOfWords[0]+numberOfWords[1]+numberOfWords[2];
}
    do{
        printf("\tJaka kategorie hasel wybierasz?\n\t1. informatyka(%d)\n\t2. jedzenie(%d)\n\t3. panstwa(%d)\n\t4.Powrot do menu gry\n",numberOfWords[0],numberOfWords[1],numberOfWords[2]);
//sprawdzenie czy wprowadzona zostala liczba
        category=checkString();
//przypisanie kategorii
        section=infoAboutSection(section,category);
        system("cls");
        }while(!(category>=1 && category<=4 ));
    if(category==4) return;
    else Haslo=randomPassword(category,numberOfWords,sectionPositions);
    system("cls");
    result=guessPassword(Haslo.password,Haslo.length,section);

    if(result==1){
        player.wins++;
    }
    else{
        player.losses++;
    }

    do{
        system("cls");
        do{
        printf("\t\t\tGrasz dalej %s ?\n \t\t\t 1. Tak\n \t\t\t 2. Zmien kategorie\n \t\t\t 3.Nie\n",player.nick);
        choice=checkString();
        }while(!(choice>=1 && choice<=3));
        if(choice==1){
            Haslo=randomPassword(category,numberOfWords,sectionPositions);
            system("cls");
            result=guessPassword(Haslo.password,Haslo.length,section);
            if(result==1){
                player.wins++;
            }
            else{
                player.losses++;
            }
        }
        else if(choice==2){
            do{
                printf("\tJaka kategorie hasel wybierasz?\n\t1. informatyka(%d)\n\t2. jedzenie(%d)\n\t3. panstwa(%d)\n",numberOfWords[0],numberOfWords[1],numberOfWords[2]);
        //sprawdzenie czy wprowadzona zostala liczba
                category=checkString();
        //przypisanie kategorii
            section=(char*)malloc(50*sizeof(*section));
                section=infoAboutSection(section,category);
                system("cls");
            }while(!(category>=1 && category<=3 ));
            Haslo=randomPassword(category,numberOfWords,sectionPositions);
            section=infoAboutSection(section,category);
            system("cls");
            result=guessPassword(Haslo.password,Haslo.length,section);
        }
        else{
free(sectionPositions);
free(numberOfWords);
sectionPositions=NULL;
numberOfWords=NULL;
            FILE *staty;
            if ((staty=fopen("Ios.txt","a"))==NULL) {
                printf ("Nie mogę otworzyć pliku test.txt do zapisu!\n");
                exit(1);
            }
            fprintf(staty,"%s %d %d\n",player.nick,player.wins,player.losses);
                       // printf("Zapisano do pliku\n");
            fclose(staty);
        }
    }while(choice!=3);
}
void two_player_game(){

    statistic Player1;
    int nameLength1=0;
    Player1.wins=0;

    statistic Player2;
    int nameLength2=0;
    Player1.wins=0;

    int passwordLength=0;   //dlugosc hasla
    char password[51];
    char prompt[51];

    int result;
    int check; // sprawdzenie czy nazwy graczy nie sa takie same
    int sG; //menu gry
    int chP; //wybór kto ma wprowadzaæ haslo
    int gamecontiniue;

    char player1[20];
     // dlugosc nazwy gracza 1
    int win1=0;     //wygrane gracza 1
    char player2[20];
    //int nameLength2=0;  //dlugosc nazwy gracza 2
    int win2=0; //wygrane gracza 2

FILE *IIos;

    do{
        printf("\t1.Przejscie do gry\n \t2. Stan gry miedzy graczami \n \t3. Wyjscie z gry \n");
        sG=checkString();
        switch(sG){
            case 1:
                //Sprawdzenie czy nazwy graczy zostaly wprowadzone (umozliwnienie sprawdzenia wynikow bez ponownego wprowadzania nazw)
                if(nameLength1==0 && nameLength2==0){
                    printf("\t\tWitamy w trybie gry dwuosobowej.\n \t\tProsze wprowadzic nazwy graczy.\n");
                    printf("\t\tGracz 1: ");
                    scanf(" %s",Player1.nick);
                    nameLength1=strlen(Player1.nick);
                    do{
                    printf("\t\tGracz 2: ");
                    scanf(" %s",Player2.nick);
                    check=strcmp(Player1.nick,Player2.nick);
                    if(check==0) printf("\t\tNazwe gracza 2 nalezy zmienic\n");
                    nameLength2=strlen(Player2.nick);
                    }while(check==0);
                }

                    do{
                    printf("\t\tKto wprowadza pierwszy haslo?\n \t\t\t 1. %s \n \t\t\t 2. %s \n \t\t 3. Powrot do menu gry\n\t", Player1.nick,Player2.nick);
                    chP=checkString();
                    }while(!(chP>=1 && chP<=3));
                    if(chP==1 || chP==2){
                        printf("\t\t\tWprowadz haslo (maksymalnie do 50 znakow)\n \t\t\t");
                        scanf(" %[^\n]s",password);
                        printf("\t\t\tJaka podpowiedz do hasla?\n \t\t\t");
                        scanf(" %[^\n]s",prompt);
                        passwordLength=strlen(password); //okreslenie dl hasla
                        system("cls"); //czyszczenie konsoli
                        result = guessPassword(password,passwordLength,prompt);
                        if(chP==1){
                            Player2.wins+=result;
                        }
                        else{
                            Player1.wins+=result;
                        }
                    }
                    else if(chP==3){
                        break;
                    }
                    else{
                        printf("\t\twybrana opcja jest niedostepna\n");
                    }
                    do{
                        do{
                            system("cls");
                            printf("\n\tPrzeprowadzic kolejna runde gry?\n\t1. Tak\n\t2. Nie\n");
                            gamecontiniue=checkString();
                        }while(!(gamecontiniue>=1 && gamecontiniue<=2));
                        if(gamecontiniue==1){
                        switch(chP){
                            case 1:
                                    printf("Haslo wymysla %s\n",Player2.nick);
                                    printf("\t\t\tWprowadz haslo (maksymalnie do 50 znakow)\n \t\t\t");
                                    scanf(" %[^\n]s",password);
                                    printf("\t\t\tJaka podpowiedz do hasla?\n \t\t\t");
                                    scanf(" %[^\n]s",prompt);
                                    chP=2;
                                    passwordLength=strlen(password); //okreslenie dl hasla
                                    system("cls"); //czyszczenie konsoli
                                    result = guessPassword(password,passwordLength,prompt);
                                    if(result==1){
                                        Player1.wins+=result;
                                    }
                                   break;
                                case 2:
                                    printf("Haslo wymysla %s\n",Player1.nick);
                                    printf("\t\t\tWprowadz haslo (maksymalnie do 50 znakow)\n \t\t\t");
                                    scanf(" %[^\n]s",password);
                                    printf("\t\t\tJaka podpowiedz do hasla?\n \t\t\t");
                                    scanf(" %[^\n]s",prompt);
                                    chP=1;
                                    passwordLength=strlen(password); //okreslenie dl hasla
                                    system("cls"); //czyszczenie konsoli
                                    result = guessPassword(password,passwordLength,prompt);
                                    if(result==1){
                                        Player2.wins+=result;
                                    }
                                    break;
                                }
                            }


                    }while(gamecontiniue!=2);
                    result=0;
                break;

            case 2:
                printf("%s : %d \n %s : %d",Player1.nick,Player1.wins,Player2.nick,Player2.wins);
                if(Player1.wins>Player2.wins){
                    printf("\n\tWygrawa %s\n",Player1.nick);
                }
                else if(Player2.wins>Player2.wins){
                    printf("\n\tWygrywa %s\n",Player2.nick);
                }
                else{
                    printf("\n\tRemis\n");
                }
                break;
            case 3:
                if ((IIos=fopen("IIos.txt", "a"))==NULL) {
                    printf ("Nie mogę otworzyć pliku test.txt do zapisu!\n");
                    exit(1);
                }
                printf("\tKoniec gry\n Wyniki:\n %s : %d \n %s : %d",Player1.nick,Player1.wins,Player2.nick,Player2.wins);
                if(Player1.wins>Player2.wins){
                    printf("\n\tWygrywa %s\n",Player1.nick);
                    fprintf(IIos,"%s %d %s %d %s\n",Player1.nick,Player1.wins,Player2.nick,Player2.wins,Player1.nick);
                }
                else if(Player2.wins>Player1.wins){
                    printf("\n\tWygrywa %s\n",Player2.nick);
                    fprintf(IIos,"%s %d %s %d %s\n",Player1.nick,Player1.wins,Player2.nick,Player2.wins,Player2.nick);
                }
                else{
                    printf("\n\tRemis\n");
                    fprintf(IIos,"%s %d %s %d Remis\n",Player1.nick,Player1.wins,Player2.nick,Player2.wins);
                }
                fclose(IIos);
                break;
            }
        }while(sG!=3);
}
void hangman(int error){
    int xHangman=10;
    int yHangman=7;
    switch(error){
            case 0:
            MoveToXY(xHangman,yHangman);
            printf("||===== ");
            MoveToXY(xHangman,++yHangman);
            printf("||    | ");
            MoveToXY(xHangman,++yHangman);
            printf("||   %cO/",'\\');
            MoveToXY(xHangman,++yHangman);
            printf("||    | ");
            MoveToXY(xHangman,++yHangman);
            printf("||   / %c",'\\');
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            break;
            case 1:
            MoveToXY(xHangman,yHangman);
            printf("||===== ");
            MoveToXY(xHangman,++yHangman);
            printf("||    | ");
            MoveToXY(xHangman,++yHangman);
            printf("||   %cO/",'\\');
            MoveToXY(xHangman,++yHangman);
            printf("||    | ");
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            break;
            case 2:
            MoveToXY(xHangman,yHangman);
            printf("||===== ");
            MoveToXY(xHangman,++yHangman);
            printf("||    | ");
            MoveToXY(xHangman,++yHangman);
            printf("||   %cO/",'\\');
            MoveToXY(xHangman,++yHangman);
            printf("||     ");
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            break;
            case 3:
            MoveToXY(xHangman, yHangman);
            printf("||===== ");
            MoveToXY(xHangman,++yHangman);
            printf("||    | ");
            MoveToXY(xHangman,++yHangman);
            printf("||    O ");
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            break;
            case 4:
            MoveToXY(xHangman, yHangman);
            printf("||===== ");
            MoveToXY(xHangman,++yHangman);
            printf("||    | ");
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            break;
            case 5:
            MoveToXY(xHangman, yHangman);
            printf("||===== ");
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            MoveToXY(xHangman,++yHangman);
            printf("||      ");
            break;
            default:
            break;
            xHangman=4;
            yHangman=7;
        }
 }
 //DO tabeli
void addSpaceCell(char cell[], int len){
    int i;
    int space=0;
    //wymiar komorki dla nazwy uzytkownika
    int cellNick=20;
     //wymiar komorki dla rubryki ze statystykami(wygrane/przegrane)
    int cellNumber=14;
    if(cell[0]>='0' && cell[0]<='9'){
        cellNumber-=len;
        space=cellNumber;
    }
    else{
        cellNick-=len;
        space=cellNick;
    }
    char addSpace[space];
        for(i=0; i<space; i++){
            addSpace[i]=' ';
        }
        addSpace[space]='\0';
        strcat(cell,addSpace);
}
void lineTable(int len){
    int i;
    for(i=0;i<len; i++){
        printf("-");
    }
    printf("\n");
}
int main(void){

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); // uchwyt standardowego wyjœcia
    COORD c2; // struktura potrzebna do ustawienia rozmiarów bufora pamiêci
    c2.X = 100; // szerokosc na 80 szerokoœci znaków
    c2.Y = 39; // wysokosc na 25 wysokoœci znaków
    SetConsoleScreenBufferSize(handle, c2); // ustawia rozmiar bufora (wyœwietlanego tekstu
    SMALL_RECT sr; //wspolrzedne krancowe konsoli
    sr.Left = 0;
    sr.Top = 0;
    sr.Right = c2.X-1;
    sr.Bottom = c2.Y-1;
    SetConsoleWindowInfo(handle,true,&sr); //ustawienie bierzacego rozmiaru konsoli
    int wb;
    int staty,i=0,dl=0;
    FILE *Ios, *IIos;
    char zPliku[100];
    do{
        printf("\n\t\t\t     #####################\n");
        printf("\t\t\t     #  W I S I E L E C  #\n");
        printf("\t\t\t     #####################\n\n");

        printf("1.Tryb jednoosobowy\n2.Tryb dwuosobowy\n3. Statystyki\n4.Wyj\230cie\n");
        wb=checkString();
        switch(wb){
            case 1:
                system("cls");
                singleGame();
                break;
            case 2:
                two_player_game();
            break;
            case 3:
                do{
                printf("1. tryb jednoosobowy\n2. Tryb dwuoosobowy\n3. Powr\242t do menu\n");

                staty=checkString();
                    switch(staty){
                    case 1:
                        Ios=fopen("Ios.txt","r");
                        if(Ios==NULL){
                            printf("Problem z odczytem pliku\n");
                        }
                            lineTable(55);
                            printf("| Nazwa u\276ytkownika   | Wygrane rundy |Przegrane rundy|\n");
                            lineTable(55);
                            while(fscanf(Ios,"%s",zPliku)!=EOF){
                            i++;
                            dl=strlen(zPliku);
                            addSpaceCell(zPliku,dl);
                            printf("| %s",zPliku);
                            if(i%3==0) printf("|\n");
                            }
                            lineTable(55);
                            fclose(Ios);
                            i=0;
                        break;
                    case 2:
                             IIos=fopen("IIos.txt","r");
                        if(IIos==NULL){
                            printf("Problem z odczytem pliku\n");
                        }
                            lineTable(99);
                            printf("| Nazwa u\276ytkownika 1 | Wygrane rundy | Nazwa u\276ytkownika 2 | Wygrane rundy |     Kto wygral?     |\n");
                            lineTable(99);
                            while(fscanf(IIos," %s",zPliku)!=EOF){
                            dl=strlen(zPliku);
                            addSpaceCell(zPliku,dl);
                            printf("| %s",zPliku);
                            i++;
                            if(i%5==0) printf("|\n");
                            }
                            lineTable(99);
                            printf("\n");
                        fclose(IIos);
                        i=0;
                    break;
                    }
                }while(staty!=3);
            break;
            }
        }while(wb!=4);
return 0;
}
