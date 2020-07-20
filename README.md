# XO-game-using-tivac project

you can find a preview of the game in [this video](https://youtu.be/Ig2ccMyu63w)

you also can find the code description down in the bottom

### Hardware needed
*	Tivac with microcontroller tm4c123gh6pm.

*	Nokia5110 LCD.

*	Battery.

* Wires.


### Flowchart (for players)

 ![Flowchart](https://github.com/Ahmed-Hady-168234/XO-game-using-tivac/blob/master/images/flowchart(for%20players).png)


### How to run the game

1-open the project from "XO_Game.uvproj" using Keil ide.

2-build the project.

![](https://github.com/Ahmed-Hady-168234/XO-game-using-tivac/blob/master/images/1.png)

3-debug.

![](https://github.com/Ahmed-Hady-168234/XO-game-using-tivac/blob/master/images/2.png)

4-run.

![](https://github.com/Ahmed-Hady-168234/XO-game-using-tivac/blob/master/images/3.png)

5-use the 2 switches (sw1 to play and sw2 to move) and have fun.

![](https://github.com/Ahmed-Hady-168234/XO-game-using-tivac/blob/master/images/4.png)

### Code desciption

#### Functions description

![](https://github.com/Ahmed-Hady-168234/XO-game-using-tivac/blob/master/images/functions%20description.png)

#### How the Code flows ?

*	First we initialize the portf,micro and Nokia lcd.

*	Then we start the game by showing the borders and cursor and the score.
          Go inside the super loop (while loop).
          
*	get the status if the switches (SW1 for move and SW2 for playing).

*	If condition check if SW1 is pressed.
move to the next column 

*	If condition to check SW2 is pessed.

  . If condition to check whos turn (X or O) and check if this place is empty.
  
     if yes >> print the sign(X or O)according to whose_turn and record the move
     
  .if condition to check if there is a winner.
  
     if there is a winner print that he wins and increase his score and start a new
    game.
    
  .else if all places are complete then print you lose and start a new game.
  
  
*	If condition to check if you should move to the next row or start from 0,0.
