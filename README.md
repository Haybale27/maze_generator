This program generates a maze using the "hunt and kill" algorithm and solves the path from bottom left to top right.
If you set the display time variables to higher variables than 0, you can see the generation in real time and the solving of the maze real time.
Also, you can set the width and height of the maze. Just make sure that both values are the same, odd numbers, and less than 80, because otherwise, it gets a little quirky.
Until I add a functionality to automatically open the terminal in another window and set width and height and everything, be wary about setting your console window size big enough for the maze to fit.
Otherwise, you can veiw your maze in the maze_output.txt file after you run the program. The solve path doesn't show in the file however because I want it to just be the maze.

The output file not including the solve path is for later implementation of this program into a wolfenstein/DooM-like maze-based game.
I will do that project by building my own raytracing engine that uses ascii in the terminal rather than pixels. 
This maze project is simply to build the levels for that game. That will be a separate project, separate repo, however.
The solving of the maze was just a fun bonus challenge. However, I guess I can probably use that aspect of the program for enemies later on.
