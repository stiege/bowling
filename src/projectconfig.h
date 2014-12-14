#ifndef projectconfig_H
#define projectconfig_H

/*
_________                _____.__
\_   ___ \  ____   _____/ ____\__| ____
/    \  \/ /  _ \ /    \   __\|  |/ ___\
\     \___(  <_> )   |  \  |  |  / /_/  >
 \______  /\____/|___|  /__|  |__\___  /
        \/            \/        /_____/
*/

#define SCORECARD_FRAME_SLOTS 10
/*
"|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |"
"|   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   |   10  |"
*/

#define SCORE_ROLL_SLOTS 20
/*
"| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |   |   |   |   |   |   |   |   |   |   |20 |"
"|       |       |       |       |       |       |       |       |       |       |";
*/

#define SIZE_OF_ROLL_SLOTS ( sizeof("|   |") - sizeof("|") )
/*
The last '|' in " |   | " belongs to the next row.
We're also implicitly subtracting the size of the termination character
(but it was implicitly added to begin with).
*/

#define SIZE_OF_FRAME ( sizeof("|       |") - sizeof("|") )
/*
The last '|' in  " |       | " belongs to the next frame
*/

/* 
We have numbered everything above from 1 
*/
#define FIRST_ELEMENT 1 
#define ROLL_FIRST_ELEMENT FIRST_ELEMENT
#define FRAME_FIRST_ELEMENT FIRST_ELEMENT


#define ROLLS_PER_FRAME ROLL_FIRST_ELEMENT + 1
/*
| 1 | 2 |
|   1   |
*/


#endif