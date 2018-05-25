#include <stdio.h>
#include <stdbool.h>
#define HEIGHT 10+2
#define WIDTH 10+2
#define SHIP2 1
#define SHIP3 0
#define SHIP4 0
#define SHIP5 0

int rand_range(int low, int high);
void srand_range(unsigned int seed);
void build_table(char arr[HEIGHT][WIDTH]);
void debug_build_table(char arr[HEIGHT][WIDTH]);//DELETE
void print_table(char arr[HEIGHT][WIDTH]);
void erase_list(int list[][6]);
void fill_ships_list(int length,int x,int y,int dirX,int dirY,int list[SHIP2+SHIP3+SHIP4+SHIP5][6]);
void greetings();
void comp_place_ship(char arr[HEIGHT][WIDTH], int ship_length,int list[SHIP2+SHIP3+SHIP4+SHIP5][6]);
void player_place_ship(char arr[HEIGHT][WIDTH], int ship_length,int list[SHIP2+SHIP3+SHIP4+SHIP5][6]);
void comp_place_all_ships(int ship2,int ship3,int ship4,int ship5,char arr[HEIGHT][WIDTH],int list[SHIP2+SHIP3+SHIP4+SHIP5][6]);
void player_place_all_ships(int ship2,int ship3,int ship4,int ship5,char arr[HEIGHT][WIDTH],int list[SHIP2+SHIP3+SHIP4+SHIP5][6]);
bool check_location_error(int x, int y,int dirX,int dirY, int length);
bool check_ships_intersection(int x, int y,int dirX,int dirY, int length,char arr[HEIGHT][WIDTH]);
bool check_direction_value(int dirX, int dirY);
bool player_turn(char player_map[HEIGHT][WIDTH], char player_attacks_map[HEIGHT][WIDTH],int comp_ships_list[SHIP2+SHIP3+SHIP4+SHIP5][6]);
void print_maps_status(char player_map[HEIGHT][WIDTH], char player_attacks_map[HEIGHT][WIDTH]);
bool check_attack_point(int x, int y, char map[HEIGHT][WIDTH]);
bool check_double_shot(int x, int y, char map[HEIGHT][WIDTH]);
bool check_hit(int x,int y,int comp_ships_list[SHIP2+SHIP3+SHIP4+SHIP5][6],char player_attacks_map[HEIGHT][WIDTH]);
void update_map(int x,int y,char val,char map[HEIGHT][WIDTH]);
void print_list(int list[SHIP2+SHIP3+SHIP4+SHIP5][6]);
bool check_if_drown(int row,int ships_list[SHIP2+SHIP3+SHIP4+SHIP5][6]);
bool check_winner(char who, int ships_list[SHIP2+SHIP3+SHIP4+SHIP5][6]);

int main()
{
    int level;
    //int ships_num = SHIP2+SHIP3+SHIP4+SHIP5;
    greetings();
    scanf("%d", &level);

    char player_map[HEIGHT][WIDTH];
    char player_attacks_map[HEIGHT][WIDTH];
    char comp_map[HEIGHT][WIDTH];
    int player_ships_list[SHIP2+SHIP3+SHIP4+SHIP5][6];
    int comp_ships_list[SHIP2+SHIP3+SHIP4+SHIP5][6];

    build_table(player_map);
    build_table(player_attacks_map);
    build_table(comp_map);

    erase_list(player_ships_list);
    erase_list(comp_ships_list);

   // player_place_all_ships(SHIP2,SHIP3,SHIP4,SHIP5,player_map,player_ships_list);
    comp_place_all_ships(SHIP2,SHIP3,SHIP4,SHIP5,comp_map,comp_ships_list);
    print_list(comp_ships_list); //TODO: delete.

    print_table(comp_map);

    while(!player_turn(player_map,player_attacks_map,comp_ships_list))
    {
        print_table(player_attacks_map);
    }


  return 0;
}

bool player_turn(char player_map[HEIGHT][WIDTH], char player_attacks_map[HEIGHT][WIDTH],int comp_ships_list[SHIP2+SHIP3+SHIP4+SHIP5][6])
{
    int x;
    int y;
    char comma;
    print_maps_status(player_map,player_attacks_map);
    do
    {
       scanf("%d %c %d",&x,&comma,&y);
    }
    while(!check_attack_point(x,y,player_attacks_map));

    return check_hit(x,y,comp_ships_list,player_attacks_map);
}

bool check_hit(int x,int y,int comp_ships_list[SHIP2+SHIP3+SHIP4+SHIP5][6],char player_attacks_map[HEIGHT][WIDTH])
{
    //adjust x y values.
    for(int row=0;row<SHIP2+SHIP3+SHIP4+SHIP5;row++)
    {
        int length = comp_ships_list[row][0];
        int shipX=comp_ships_list[row][1];
        int shipY=comp_ships_list[row][2];
        int shipdirX=comp_ships_list[row][3];
        int shipdirY=comp_ships_list[row][4];

        for(int i=0;i<length;i++)
        {
            printf("Funk check_hit - ShipX: %d, ShipY: %d\n",shipX,shipY);

            if(x==shipX&&y==shipY)
            {
                //there is actual hit.
                printf("HIT!\n");
                update_map(x,y,'V',player_attacks_map);
                comp_ships_list[row][5]--; //single ship life -1.

                check_if_drown(row,comp_ships_list);
                if(check_winner('p',comp_ships_list))
                {
                    return true; //game ends.
                }
                return false; //game proceeds.
            }
            else
            {
                shipX=shipX+shipdirX;
                shipY=shipY+shipdirY;
            }
        }
    }
    update_map(x,y,'X',player_attacks_map);
    return false; //game proceeds.
}

bool check_winner(char who, int ships_list[SHIP2+SHIP3+SHIP4+SHIP5][6])
{
    int total_lifes=0;
    //check list.
    for(int row=0;row<SHIP2+SHIP3+SHIP4+SHIP5;row++)
    {
        //summarize lifes.
        total_lifes=total_lifes+ships_list[row][5];
    }

    if(total_lifes==0)
    {
        if(who=='p')
        {
            printf("player wins!");
        }
        if(who=='c')
        {
            printf("comp wins!");
        }
        return true;
    }
    else
    return false;
    }

bool check_if_drown(int row,int ships_list[SHIP2+SHIP3+SHIP4+SHIP5][6])
{
    if(ships_list[row][5]==0)
    {
        printf("Battleship of size %d has been drown",ships_list[row][0]);
        return true;
    }
    else
    return false;
}

void update_map(int x,int y,char val,char map[HEIGHT][WIDTH])
{
    //adjust
    int row = HEIGHT-y-3;
    int col = x+2;
    map[row][col]=val;
    //printf("------------Value of the cell in updated map: %c val printed\n",map[row][col]);
    //print_table(map);
}

bool check_attack_point(int x, int y, char map[HEIGHT][WIDTH])
{
    //adjust
    y = HEIGHT-y-3;
    x = x+2;
    //check for syntax error and double shot.
    if(check_location_error(x,y,0,0,0)&&check_double_shot(x,y, map))
    return true;
    else
    printf("Error: Incorrect parameters! Please enter coordinates for attack again:\n");
    return false;
}

bool check_double_shot(int x, int y, char map[HEIGHT][WIDTH])
{
    if(map[y][x]!=' '){
        return false; //Doubleshot exist. Bad.
    }
    else
    return true; //No doubleshot. Good.
}

void print_maps_status(char player_map[HEIGHT][WIDTH], char player_attacks_map[HEIGHT][WIDTH])
{
     printf("Your following table:\n");
    print_table(player_attacks_map);
    printf("The computer's following table: (Player ships)\n");
    print_table(player_map);
    printf("It's your turn!\n");
    printf("Enter coordinates for attack:\n");
}

void player_place_all_ships(int ship2,int ship3,int ship4,int ship5,char arr[HEIGHT][WIDTH],int list[SHIP2+SHIP3+SHIP4+SHIP5][6])
{
    for(int i=0;i<ship2;i++)
    {
       print_table(arr);
       player_place_ship(arr,2,list);
    }
    for(int i=0;i<ship3;i++)
    {
        print_table(arr);
       player_place_ship(arr,3,list);
    }
    for(int i=0;i<ship4;i++)
    {
        print_table(arr);
        player_place_ship(arr,4,list);
    }
    for(int i=0;i<ship5;i++)
    {
        print_table(arr);
       player_place_ship(arr,5,list);
    }
    print_table(arr);
}

void player_place_ship(char arr[HEIGHT][WIDTH], int ship_length, int list[SHIP2+SHIP3+SHIP4+SHIP5][6])
{
    //choose starting point and direction
    static
    int x, y, dirX, dirY;
    char comma1,comma2;
    printf("Enter location for Battleship of size %d:\n",ship_length);
    if(6==scanf("%d %c %d %d %c %d",&x,&comma1,&y,&dirX,&comma2,&dirY))
    printf("ADD INPUT CHECKER!!!");
    else
    printf("ADD INPUT CHECKER!!!");
    //TODO: check input errors.
    y = HEIGHT-y-3;
    x = x+2;
    while(!(check_direction_value(dirX,dirY)&&check_location_error(x,y,dirX,dirY,ship_length)&&check_ships_intersection(x,y,dirX,dirY,ship_length,arr)))
        {
            printf("Error: Incorrect parameters! Please enter location for the Battleship size %d again:\n",ship_length);
            scanf("%d %c %d %d %c %d",&x,&comma1,&y,&dirX,&comma2,&dirY);
            //TODO: check input errors.
            y = HEIGHT-y-3;
            x = x+2;
        }

        fill_ships_list(ship_length,x,y,dirX,dirY,list);

    for(int length=0;length<ship_length;length++)
        {
            arr[y][x]='*';
            y=y-dirY;
            x=x+dirX;
        }
}

void comp_place_all_ships(int ship2,int ship3,int ship4,int ship5,char arr[HEIGHT][WIDTH],int list[SHIP2+SHIP3+SHIP4+SHIP5][6])
{
    for(int i=0;i<ship2;i++)
    {
       comp_place_ship(arr,2,list);
    }
    for(int i=0;i<ship3;i++)
    {
       comp_place_ship(arr,3,list);
    }
    for(int i=0;i<ship4;i++)
    {
        comp_place_ship(arr,4,list);
    }
    for(int i=0;i<ship5;i++)
    {
       comp_place_ship(arr,5,list);
    }
}

void comp_place_ship(char arr[HEIGHT][WIDTH], int ship_length, int list[SHIP2+SHIP3+SHIP4+SHIP5][6])
{
    //TODO: choose starting point and direction
    int x,y,dirX, dirY;
    do {
    x = rand_range(0,WIDTH-3);
    y = rand_range(0,HEIGHT-3);
    dirX = rand_range(-1,1);
    dirY = rand_range(-1,1);
    y = HEIGHT-y-3;
    x = x+2;
    } while(!(check_direction_value(dirX,dirY)&&check_location_error(x,y,dirX,dirY,ship_length)&&check_ships_intersection(x,y,dirX,dirY,ship_length,arr)));

    fill_ships_list(ship_length,x,y,dirX,dirY,list);

            for(int length=0;length<ship_length;length++)
            {
                    arr[y][x]='*';
                    y=y-dirY;
                    x=x+dirX;
            }
}

void fill_ships_list(int length,int x,int y,int dirX,int dirY,int list[SHIP2+SHIP3+SHIP4+SHIP5][6])
{
    int row = 0;
    while(list[row][0]!=0)
    {
        row++;
    }

    //adjust coordinates.
    y = HEIGHT-y-3;
    x = x-2;

    list[row][0] = length;
    list[row][1] = x;
    list[row][2] = y;
    list[row][3] = dirX;
    list[row][4] = dirY;
    list[row][5] = length;
}

bool check_direction_value(int dir1, int dir2)
{
    if(dir1==0&&dir2==0)
    {
        return false;
    }
    else if(dir1>1||dir1<-1)
    {
        return false;
    }
    else if(dir2>1||dir2<-1)
    {
        return false;
    }
    else
    return true;
}

bool check_ships_intersection(int x, int y,int dirX,int dirY, int length,char arr[HEIGHT][WIDTH])
{
    for(int i=0;i<length-1;i++)
    {
            if(arr[y][x]!=' ')
            return false; //Intersection exist. Bad.
            y=y-dirY;
            x=x+dirX;
    }
    return true; //No intersection. Good.
}

bool check_location_error(int x, int y,int dirX,int dirY, int length)
{
    for(int i=0;i<length-1;i++)
    {
            y=y-dirY;
            x=x+dirX;
    }
    //printf("Max X: %d    Max Y: %d\n",x,y);

    if((y<=(HEIGHT-3)&&y>=0)&&(x>=2&&x<=(WIDTH-1)))
    return true; //Valid location. Good.
    else
    return false; //Bad location.
}

//Erase ships location list.
void erase_list(int list[SHIP2+SHIP3+SHIP4+SHIP5][6])
{
    for(int row=0;row<SHIP2+SHIP3+SHIP4+SHIP5;row++)
    {
        for(int col=0;col<6;col++)
        {
            list[row][col]=0;
        }
    }
}

//Builds a table with coordinates.
void build_table(char arr[HEIGHT][WIDTH])
{
    for(int row=0;row<HEIGHT;row++)
    {
        for(int col=0;col<WIDTH;col++)
        {
                if(row==HEIGHT-1)
                     arr[row][col] = col+46;
                if(col==0)
                    arr[row][col] = 45+HEIGHT-row;
                if(col==1)
                    arr[row][col] = '|';
                if(row==HEIGHT-2)
                     arr[row][col] = '_';
                 if((row<=HEIGHT-3&&col>=2)||(row>HEIGHT-3&&col<2))
                    arr[row][col] = ' ';
        }
    }
}

//Builds a table with coordinates.
void debug_build_table(char arr[HEIGHT][WIDTH])
{
    for(int row=0;row<HEIGHT;row++)
    {
        for(int col=0;col<WIDTH;col++)
        {
                if(row==HEIGHT-1)
                     arr[row][col] = col+46;
                if(col==0)
                    arr[row][col] = 45+HEIGHT-row;
                if(col==1)
                    arr[row][col] = '|';
                if(row==HEIGHT-2)
                     arr[row][col] = '_';
                 if((row<=HEIGHT-3&&col>=2)||(row>HEIGHT-3&&col<2))
                    arr[row][col] = '.';
        }
    }
}


//Prints the  table.
void print_table(char arr[HEIGHT][WIDTH])
{
    for(int row=0;row<HEIGHT;row++)
    {
        for(int col=0;col<WIDTH;col++)
        {
        printf("%c", arr[row][col]);
        }
        printf("\n");
    }
}

void print_list(int list[SHIP2+SHIP3+SHIP4+SHIP5][6])
{
    for(int row=0;row<SHIP2+SHIP3+SHIP4+SHIP5;row++)
    {
        for(int col=0;col<6;col++)
        {
        printf("%d", list[row][col]);
        }
        printf("\n");
    }
}

//Greetings Message ans SEED INPUT.
void greetings()
{
    unsigned int seed;
    printf("Welcome to Battleship!\n");
    printf("Please enter seed:\n");
    scanf("%u", &seed);
    srand_range(seed);
    printf("Please enter level:\n");
}

int rand_range (int low, int high) // RAND_MAX assumed to be 32767
{
    static unsigned int next = 1;
    next = next * 1103515245 + 12345;
    return ((unsigned int)(next/65536) % 32768) % (high - low + 1) + low;
}

void srand_range (unsigned int seed)
{
    for (int i = 0; (unsigned int)i < seed; i++) {
        rand_range(i,i+1);
    }
}
