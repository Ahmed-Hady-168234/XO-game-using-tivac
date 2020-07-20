#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "TExaS.h"
#include "graphs.h" // contains arrays of X and O and the border


/*
By Ahmed Hady 
ahmed168234@feng.bu.edu.eg

*/
void app_init();
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
void PortF_Init(void);
void startNewGame(unsigned char arr[]);
char theWinnerIs(unsigned char arr[]);
void printScore(void);
char allComplete(unsigned char arr[]);
unsigned long SW1,SW2;  //input from PF4,PF0


#define whose_turn(turn) ((turn == 1)? 1:2)

#define record_the_move(player) (moves[cursor_row*3 + cursor_col] = player)

#define isPlaceEmpty(moves) (moves[cursor_row*3 + cursor_col] == 0)

#define showCursor() if((flag1 == 1 || flag2 == 1)&& cursorFlag == 0){\
			cursorFlag = 1;\
			Nokia5110_SetCursor(col_CursorIndex[cursor_col],row_CursorIndex[cursor_row]);\
			Nokia5110_OutChar(' ');\
			printScore();}

#define EMPTY 0

enum{
	o_id = 1,
	x_id = 2
};	

const unsigned char columnPos[] = {3,24,44};
const unsigned char rowPos[] = {14,30,46};
const unsigned char row_CursorIndex[] = {1,3,5};
const unsigned char col_CursorIndex[] = {1,4,7};
unsigned char turn = o_id , cursor_row = 0, cursor_col = 0,cursorIndex = 0,flag1=0,flag2=0, x_score = 0, o_score = 0,cursorFlag=0;

int main(void){
	unsigned char moves[9]={0};//array to record the moves
	
	app_init();//initialization of the game
	startNewGame(moves);//start
	
	/*   ---------How code inside While works---------
1-//get the status of the switches
2-check if move switch is pressed
3-check if play switch is pressed
4-check if there is a winner
5-check if the cursor moved from column 2 (go to the next row or start from 0,0)
	*/
	
	while(1){
//1-get the status of the switches
		SW1 = (GPIO_PORTF_DATA_R&0x10);     // read PF4 into SW1
    SW2 = GPIO_PORTF_DATA_R&0x01;       // read PF0 into SW2
		
		//Switch 1 to move
		//Switch 2 to put x or o

//2-is move switch pressed ?
		if(!SW1 && flag1 !=1){                     // Switch 1 (move) is pressed 
			cursor_col++;														 // move to the next column
			flag1 =1;
			cursorIndex = cursor_row*3 + cursor_col; //get the index 0 to 8		
		  Nokia5110_DisplayBuffer();               // draw buffer  
			cursorFlag = 0;
    }else if(SW1 && flag1 == 1) 							 
			flag1 =0;
		
//3-is play switch pressed ?
		if(!SW2 && flag2 !=1){                     //Switch 2 to put x or o is pressed
			
				if(whose_turn(turn) == o_id && moves[cursorIndex] == EMPTY){//is it O turn ?
					Nokia5110_PrintBMP(columnPos[cursor_col],rowPos[cursor_row], o, 10);
					record_the_move(o_id);
					turn = x_id;
					
				}else if(whose_turn(turn) == x_id && moves[cursorIndex] == EMPTY){//is it X turn ?
					Nokia5110_PrintBMP(columnPos[cursor_col],rowPos[cursor_row], x, 10);
					record_the_move(x_id);
					turn = o_id;
				}
				Nokia5110_DisplayBuffer();     // draw buffer   
				printScore();
				
				flag2 =1;
				
//4-check if there is a winner
				if(theWinnerIs(moves) == o_id){//O is the winner ?
					Nokia5110_SetCursor(2, 4);
					Nokia5110_OutString("O won!");
					Delay100ms(5);
					o_score++;
					startNewGame(&moves[0]);
	
				}else if(theWinnerIs(moves) == x_id){//X is the winner ?
					Nokia5110_SetCursor(2, 4);
					Nokia5110_OutString("X won!");
					Delay100ms(5);
					x_score++;
					startNewGame(&moves[0]);
				}else if (allComplete(moves)){ 				//if all the places are full>>>no one wins
					Nokia5110_SetCursor(2, 4);
					Nokia5110_OutString("you lose");
					Delay100ms(5);
					startNewGame(&moves[0]);
				}
    }else if(SW2 && flag2 == 1) 
				flag2 =0;		
  	
//5-should i move to the next row or go back to 0,0 ?		
		if(cursor_col>2 && cursor_row<2){					 //move to the next row
			cursor_col= 0;
			cursor_row++;
		}else if (cursor_col>2 && cursor_row >= 2){//go back again to 0,0
			cursor_col = 0;
			cursor_row = 0;
		}
		
		
	}
}
void app_init(){
  TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
	PortF_Init();        // Call initialization of port PF4, PF3, PF2, PF1, PF0    
  Nokia5110_Init();	
}
//portf initialization for the switches
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}

void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}

char theWinnerIs(unsigned char arr[]){//check if there is a winner
	char winner = 0;//initially no one won
	/*0 1 2
		3 4 5
		6 7 8 */
	
	/*
		to win you should have x or o in 
	0,1,2 or 3,4,5 or 6,7,8 or
	0,3,6 or 1,4,7 or 2,5,8 or
	0,4,8 or 2,4,6
	*/
	
	//check if X win`
	if((arr[0] == x_id && arr[1] == x_id && arr[2] == x_id) ||
		 (arr[3] == x_id && arr[4] == x_id && arr[5] == x_id) ||
		 (arr[6] == x_id && arr[7] == x_id && arr[8] == x_id) ||
		 
		 (arr[0] == x_id && arr[3] == x_id && arr[6] == x_id) ||
		 (arr[1] == x_id && arr[4] == x_id && arr[7] == x_id) ||
		 (arr[2] == x_id && arr[5] == x_id && arr[8] == x_id) ||
		 
	   (arr[0] == x_id && arr[4] == x_id && arr[8] == x_id) ||
		 (arr[2] == x_id && arr[4] == x_id && arr[6] == x_id) 		)
		 winner = x_id; // the winner is X
	
	//check if O win`
	else if((arr[0] == o_id && arr[1] == o_id && arr[2] == o_id) ||
		 (arr[3] == o_id && arr[4] == o_id && arr[5] == o_id) ||
		 (arr[6] == o_id && arr[7] == o_id && arr[8] == o_id) ||
		 
		 (arr[0] == o_id && arr[3] == o_id && arr[6] == o_id) ||
		 (arr[1] == o_id && arr[4] == o_id && arr[7] == o_id) ||
		 (arr[2] == o_id && arr[5] == o_id && arr[8] == o_id) ||
		
	   (arr[0] == o_id && arr[4] == o_id && arr[8] == o_id) ||
		 (arr[2] == o_id && arr[4] == o_id && arr[6] == o_id) 		)
			
		 winner =  o_id; // the winner is O
		
	return winner;
	//0 menas no one won 
	//1 means X won
	//2 means O won
}
void startNewGame(unsigned char moves[]){//start new game
	char i;
	Nokia5110_ClearBuffer();
	Nokia5110_PrintBMP(0,48, XO_Borders, 10);//save borders in the buffer
	showCursor();
	Nokia5110_DisplayBuffer(); 							 // draw buffer   
	printScore();
	
	//start from 0,0
	cursor_row = 0;
	cursor_col = 0;
	cursorIndex  = 0;

	//clear the moves array
	for(i = 0 ;i<9; ++i)
		moves[i] = 0;	
	
	
}

void printScore(void){//print score of O and X
	Nokia5110_SetCursor(9, 1);
	Nokia5110_OutString("X=");
	Nokia5110_OutChar('0'+x_score);
	
	Nokia5110_SetCursor(9, 4);
	Nokia5110_OutString("O=");
	Nokia5110_OutChar('0'+o_score);
	
}
char allComplete(unsigned char arr[]){//check if all places aren't empty
	char i =0;
	for(i = 0 ;i<9; ++i)
		if(arr[i] == 0)
			return 0;//there is a empty place
	return 1;	   //no empty places
}