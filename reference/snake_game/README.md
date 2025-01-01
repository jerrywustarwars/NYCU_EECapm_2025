To play the game, follow these steps:
(compile: 
g++ main.cpp lib/*.cpp -o main_terminal,
WARNING if using a text editor built-in terminal, use the following instead:
g++ main.cpp lib/*.cpp -DBUILT_IN -o main_built_in)

1.Starting the Game:
When you launch the game, you will see a prompt to press the SPACE bar to start or the 'S' key to view settings->
Press the SPACE bar to begin the game.

2.Game Controls:
Use the arrow keys (or wasd keys) to control the movement of the snake. The snake will move in the direction of the key pressed.
The objective is to eat food that appears on the map, which will increase your score and the length of the snake.

3.Game Settings:
Before starting, you can press 'S' to view and modify game settings-> This includes options like game mode (easy, medium, hard), boundary wrap around, speed increase, randomized initial positions, and more.
Adjust the settings according to your preference and save them before starting the game.
I've modify the initial hight to 40 to fit the screen better.

4.Gameplay Mechanics:
As you play, the snake will move continuously. You need to navigate it to eat food while avoiding collisions with walls or the snake's own body.
If you have enabled speed increase in the settings, the snake's speed will gradually increase as you progress.

5.Food Lifespan:
Food has a lifespan, which means it will disappear after a certain time if not eaten. If the lifespan is set to a non-zero value, keep an eye on the food as it will start blinking when it's about to disappear.

6.Game Over:
The game ends when the snake collides with a wall. At this point, your final score will be displayed.

7.Restarting the Game:
After the game is over, you will be prompted to restart the game by entering 'y' for yes or 'n' for no. If you choose to restart, the game will begin again with the same settings->
Enjoy playing the game and try to achieve the highest score possible