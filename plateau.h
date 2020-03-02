#ifndef PLATEAU_H
#define PLATEAU_H
#define MIN_SIZE 5
#define LINE_SIZE 5
#define MAX_LINES_ON_POINT 20

/******** Board ********/

/**Structure for the board

   holds a two dimensional array of bool for wether of not there is point on the case
   holds an array of lines that represent the 5-points alignements that have already been done
   holds ints for the size of the aformentionned 
*/
typedef struct str_board* board;


/**Structure for line

   holds an enum corresponding to the direction of the line
   holds the starting point of the line (direction, x, y)
*/
typedef struct str_line* line;

/**Enum to designated a direction*/
typedef enum {RIGHT,BOTTOM_RIGHT,BOTTOM,BOTTOM_LEFT} DIRECTION;

/**Contructor for a board

   This allocates the memory for a board of length 5
   It is initialised without a point or line
*/
board createBoard();

/**Destructor for a board*/
void destroyBoard(board b);

/**Size accessor, boards are always squares*/
int getSize(board b);

/**Lines accessor*/
line* getLines(board b);

/**lines_counter accessor*/
int getLinesCount(board b);

/**Resize the board

   Reallocates memory of the given board's points and sets its original values
   The new height and width have to be greater than the original height and width of the board
   A delay can be setted to shift board's origin, e.g.: if axe_delay is setted at 1 the origin of the board will move from (0,0) to (1,1)
   Return 1 given sizes and delay are valid
   Return 0 otherwise
 */
int resize(board b, int size_x, int size_y, int axe_delay);

/**Check if any move is possible

   Return 1 if no further move is possible
   Return 0 overwise
*/
int isOver(board b);

/**Initialise the board b to a n*n board randomly

   There is no guarantee the board can be played properly
   Return 0 on success
*/
int initRandom(board b,int n);

/**Initialise the board b with the content of the file at filename

   Return 1 on success
   Return 0 on failure, and output on stderr
*/
int initFile(board b,char* filename);

/**Saves the board b as the file filename
   
   If the file doesn't exists, creates one
   If the file already exits, overwrites it

    Instance of saved file :
    ____________________________
   | size_x=x                   |
   | size_y=y                   |
   | points=0,0,0,1,1,0,...,1   |
   | lines_counter=counter      |
   | lines_capacity=capacity    |
   | lines=d,x,y;d,x,y          |
   |____________________________|
   
   Return 1 on success
   Return 0 on failure,and output on stderr
*/
int saveBoard(board b,char* filename);

/******** Line ********/


/**Match line and values

   Return 1 if values matches the line
   Return 0 overwise
*/
int compareLine(line l, int x, int y, DIRECTION d);

/**X accessor*/
int getX(line l);

/**Y accessor*/
int getY(line l);

/**Direction accessor*/
DIRECTION getD(line l);

/**Return wether there a line at case (x,y) on board b in the direction d

   Return 1 if there is
   Return 0 if there isn't or if the case is unreachable
*/
int isLine(board b,int x,int y, DIRECTION d);

/**Adds a connection to the case (x,y)

   Return 1 on success
   Return 0 if there is a missing point or if there is already a connection
*/
int addLine(board b,int x,int y, DIRECTION d);

/**Delete a line in the board

   Sets the pointer value of the board to NULL
   Return 1 on success
   Return 0 if there is no line matches
*/
int deleteLine(board b, int x, int y, DIRECTION d);

/**Delete all lines on a corresponding point

   Return 1 on success
   Return 0 if there is no point
*/
int deleteLinesOnPoint(board b, int x, int y);

/**Test wether a line can be added to the board

   Test if a line can be added at the position (x,y) in the direction d to the board
   The test assumes there is or will be a point at the position ignored_x and ignored_y (x,y)
   The test prevents the addition a point if any rule is not respected
   
   Return 1 you can add the line with a point on (x,y) position in the direction d
   Return 0 if a point is missing on the line or if there is an overlap
*/
int alignable(board b,int x,int y, DIRECTION d, int ignored_x, int ignored_y);

/**Provide all possible lines for a play at the position (x,y)

   Gather all playable lines overlapping on (x,y)
   The test assumes there is or will be a point at the position (x,y)
   Return a pointer of lines
*/
line* getPossibleLines(board b,int x,int y);

/**Display lines of the given board*/
void displayPossibleLines(board b, int x,int y);

/******** Point ********/

/**Return wether there is a point at case (x,y) on board b

   Return 1 if there is one
   Return 0 if there isn't or if the case is unreachable
*/
int isPoint(board b,int x,int y);

/**Add a point 
   
   Resize the board if necessary, delaying every position if x or y are lower than 0
   Return 1 on success
   Return 0 either if there is already a point there or if the case is unreachable
*/
int addPoint(board b, int x, int y);

/**Delete a point from the board and all the given line associated

   Return 1 on success
   Return 0 if there is no point or if the case is unreachable
*/
int deletePoint(board b,int x,int y);

/**Check if move (x,y) is valid

   Return 0 if move is valid
   Return 1 if there isn't any possible line overlapping a point in (x,y)
   Return 2 if there is already a point
   Return 3 if the move is invalid or if the case is unreachable
*/
int isValidMove(board b, int x,int y);

/**Return the list of possible moves for board b

   Return a pointer to an array of minimal size 1
   The first element is the number of possible moves
   Then there is the list of moves x first
   ex :            [3,1,2,2,1,3,4]
   number of moves<-|        |---|->third move (3,4) 
*/
int* getPossibleMoves(board b);

/**Display points of the given board*/
void displayPoints(board b);

#endif
