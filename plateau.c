#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "plateau.h"

/******** Structures def ********/

/**Structure for line

   holds an enum corresponding to the direction of the line
   holds the starting point of the line (direction, x, y)
*/
struct str_line {
	DIRECTION direction;
	int pos_x;
	int pos_y;
};

/**Structure for the board

   holds a two dimensional array of bool for wether of not there is point on the case
   holds an array of lines that represent the 5-points alignements that have already been done
   holds ints for the size of the aformentionned 
*/
struct str_board {
	//board
	int size_x;
	int size_y;
	int* points;
	//lines
	int lines_counter;
	int lines_capacity;
	line* lines;
};

/******** Board ********/

/**Contructor for a board

   This allocates the memory for a board of length 5
   It is initialised without a point or line
*/
board createBoard() {
	board instance = (board)malloc(sizeof(struct str_board));
	instance->size_x = MIN_SIZE;
	instance->size_y = MIN_SIZE;
	instance->points = (int*)calloc(instance->size_x * instance->size_y, sizeof(int));
	instance->lines_counter = 0;
	instance->lines_capacity = MIN_SIZE;
	instance->lines = (line*)malloc(sizeof(struct str_line) * instance->lines_capacity);
	return instance;
}

/**Destructor for a board*/
void destroyBoard(board b) {
	free(b->points);
	for(int i = 0; i < b->lines_capacity; i++)
			free(*(b->lines + i));
	free(b->lines);
	free(b);
}

int getSize(board b)
{
  return b->size_x;
}

line* getLines(board b)
{
  return b->lines;
}

int getLinesCount(board b)
{
  return b->lines_counter;
}

/**Resize the board

   Reallocates memory of the given board's points and sets its original values
   The new height and width have to be greater than the original height and width of the board
   A delay can be setted to shift board's origin, e.g.: if axe_delay is setted at 1 the origin of the board will move from (0,0) to (1,1)
   Return 1 given sizes and delay are valid
   Return 0 otherwise
 */
int resize(board b, int size_x, int size_y, int axe_delay) {
	int result = 1;
	if(size_x < b->size_x || size_y < b->size_y || axe_delay > size_x - b->size_x || axe_delay > size_y - b->size_y)
		result = 0;
	else {
		if(axe_delay != 0) {
			line l;
			for(int i = 0; i < b->lines_counter; i++) {
				l = *(b->lines + i);
				l->pos_x += axe_delay;
				l->pos_y += axe_delay;
			}

		}
		int* newPoints = (int*)calloc(size_x * size_y, sizeof(int));
		for(int x = 0; x < b->size_x; x++)
			for(int y = 0; y < b->size_y; y++)
				*(newPoints + x + axe_delay + (y + axe_delay) * size_x) = *(b->points + x + y * b->size_x);
		free(b->points);
		b->size_x = size_x;
		b->size_y = size_y;
		b->points = newPoints;
	}
	return result;
}

/**Check if any move is possible

   Return 1 if no further move is possible
   Return 0 otherwise
*/
int isOver(board b) {
	int* moves = getPossibleMoves(b);
	int over = *(moves) == 0;
	free(moves);
	return over;
}

/**Initialise the board b to a n*n board randomly

   There is no guarantee the board can be played properly
   Return 1 on success
   Return 0 otherwise
*/
int initRandom(board b,int n) {
	int result = 0;	
	if(n > MIN_SIZE && (result = resize(b, n, n, 0))) {
		for(int x = 0; x < b->size_x; x++)
			for(int y = 0; y < b->size_y; y++)
				*(b->points + x + y * b->size_x) = rand() % 2;
	}
	return result;
}

/**Initialise the board b with the content of the file at filename

   Return 1 on success
   Return 0 on failure, and output on stderr
*/
int initFile(board b,char* filename) {
	int result = 0;
	char buffer[1000];
	char* current_line;
	FILE* fp = fopen(filename, "r");
	result = (fp != NULL);
	if(result) {
		int is_points = 1, points_counter = 0, lines_counter = 0;
		char* save_ptr; char* inner_values; char* inner_value;
		while(fgets(buffer, 1000, fp) != NULL) {
			current_line = strtok(buffer, "=");
			if(strcmp(current_line, "size_x") == 0) {
				b->size_x = atoi(inner_value = strtok(NULL, "\n"));
				b->size_x = atoi(inner_value);
			}
			else if(strcmp(current_line, "size_y") == 0) {
				inner_value = strtok(NULL, "\n");
				b->size_y = atoi(inner_value);
			}
			else if(strcmp(current_line, "lines_counter") != 0 && is_points) {
				if(strcmp(current_line, "points") == 0) {
					free(b->points);
					b->points = (int*)calloc(b->size_x * b->size_y, sizeof(int));
					inner_values = strtok(NULL, "\n");
				}
				else {
					inner_values = 	strstr(current_line, "\n") ? strtok(NULL, "\n") : current_line;
				}
				inner_value = strtok(inner_values, ",");
				while(inner_value != NULL) {
					*(b->points + points_counter) = atoi(inner_value);
					points_counter ++;
					inner_value = strtok(NULL, ",");
				}
			}
			else if(strcmp(current_line, "lines_counter") == 0) {
				is_points = 0;
				inner_value = strtok(NULL, "\n");
				b->lines_counter = atoi(inner_value);
			}
			else if(strcmp(current_line, "lines_capacity") == 0) {
				for(int i = 0; i < b->lines_capacity; i++)
					free(*(b->lines + i));
				inner_value = strtok(NULL, "\n");
				b->lines_capacity = atoi(inner_value);
			}
			else {
				if(strcmp(current_line, "lines") == 0) {
					free(b->lines);
					b->lines = (line*)calloc(b->lines_capacity, sizeof(struct str_line));
					current_line = strtok(NULL, "\n");
				}
				else {
					current_line = 	strstr(current_line, "\n") ? strtok(current_line, "\n") : current_line;
				}
				inner_values = strtok_r(current_line, ";", &save_ptr);
				while(inner_values != NULL) {
					inner_value = strtok(inner_values, ",");
					if(inner_value != NULL && !strstr(inner_value, "\n")) {
						line newLine = (line)malloc(sizeof(struct str_line));
						newLine->direction = atoi(inner_value);
						inner_value = strtok(NULL, ",");
						if(inner_value != NULL && !strstr(inner_value, "\n")) {
							newLine->pos_x = atoi(inner_value);
							inner_value = strtok(NULL, ",");
							if(inner_value != NULL && !strstr(inner_value, "\n")) {
								newLine->pos_y = atoi(inner_value);
								*(b->lines + lines_counter) = newLine;
								lines_counter ++;
							}
							else free(newLine);
						}
						else free(newLine);
					}					
					inner_values = strtok_r(NULL, ";", &save_ptr);
				}
			}
		}
	}
	fclose(fp);
	return result;
}

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
int saveBoard(board b,char* filename) {
	int result;
	FILE* fp = fopen(filename, "w");
	result = (fp != NULL);
	if(result) {
		fprintf(fp, "size_x=%d\n", b->size_x);
		fprintf(fp, "size_y=%d\n", b->size_y);
		fprintf(fp, "points=");
		for(int x = 0; x < b->size_x; x++)
				for(int y = 0; y < b->size_y; y++)
					fprintf(fp, "%d%c", *(b->points + x + y * b->size_x), x == b->size_x - 1 && y == b->size_y - 1 ? ' ' : ',');
		fprintf(fp, "\n");
		fprintf(fp, "lines_counter=%d\n", b->lines_counter);
		fprintf(fp, "lines_capacity=%d\n", b->lines_capacity);
		fprintf(fp, "lines=");
		line l;
		for(int i = 0; i < b->lines_counter; i++) {
			l = *(b->lines + i);
			fprintf(fp, "%d,%d,%d%c", l->direction, l->pos_x, l->pos_y, (i == b->lines_counter - 1 ? ' ' : ';'));
		}
	}
	fclose(fp);
	return result;
}

/******** Line ********/

/**Match line and values

   Return 1 if values matches the line
   Return 0 overwise
*/
int compareLine(line l, int x, int y, DIRECTION d) {
	return l->direction == d
		&& l->pos_x == x
		&& l->pos_y == y;
}

/**X accessor*/
int getX(line l)
{
  return l->pos_x;
}

/**Y accessor*/
int getY(line l)
{
  return l->pos_y;
}

/**Direction accessor*/
DIRECTION getD(line l)
{
  return l->direction;
}


/**Return wether there a line at case (x,y) on board b in the direction d

   Return 1 if there is
   Return 0 if there isn't or if the case is unreachable
*/
int isLine(board b,int x,int y, DIRECTION d) {
	int result = 0;
	for(int i = 0; i < b->lines_counter; i++)
		if(compareLine(*(b->lines + i), x, y, d))
			result = 1;
	return result;
}

/**Adds a connection to the case (x,y)

   Return 1 on success
   Return 0 if there is a missing point or if there is already a connection
*/
int addLine(board b,int x,int y, DIRECTION d) {
	int result = 1;
	switch(d) {
		case RIGHT:
			for(int i = 0; i < LINE_SIZE; i++)
				result &= isPoint(b, x + i, y); break;
		case BOTTOM_RIGHT:
			for(int i = 0; i < LINE_SIZE; i++)
				result &= isPoint(b, x + i, y + i); break;
		case BOTTOM:
			for(int i = 0; i < LINE_SIZE; i++)
				result &= isPoint(b, x, y + i); break;
		case BOTTOM_LEFT:
			for(int i = 0; i < LINE_SIZE; i++)
				result &= isPoint(b, x - i, y + i); break;
		default: result = 0;
	}
	if(result) {
		b->lines_counter++;
		if(b->lines_capacity <= b->lines_counter) {
			//avoid to reallocate memories every time 2 * capacity + |lines - capacity|
			b->lines_capacity =  b->lines_counter - b->lines_capacity + b->lines_capacity * 2;
			line* newLines = (line*)malloc(sizeof(struct str_line) * b->lines_capacity);
			for(int i = 0; i < b->lines_counter - 1; i++)
				*(newLines + i) = *(b->lines + i);
			free(b->lines);
			b->lines = newLines;
		}
		line newLine = (line)malloc(sizeof(struct str_line));
		newLine->pos_x = x;
		newLine->pos_y = y;
		newLine->direction = d;
		*(b->lines + b->lines_counter - 1) = newLine;
	}
	return result;
}

/**Delete a line in the board

   Sets the pointer value of the board to NULL
   Return 1 on success
   Return 0 if there is no line matches
*/
int deleteLine(board b, int x, int y, DIRECTION d) {
	int result = 0;
	for(int i = 0; i < b->lines_counter; i++) {
		if(compareLine(*(b->lines + i), x, y, d)) {
			result = 1;
			b->lines_counter --;
			free(*(b->lines + i));
			*(b->lines + i) = NULL;
		}
	}
	return result;
}

/**Delete all lines on a corresponding point

   Return 1 on success
   Return 0 if there is no point
*/
int deleteLinesOnPoint(board b, int x, int y) {
	int result = isPoint(b, x, y);
	if(result) {
		for(int i = 1 - LINE_SIZE; i < 1; i++) {
			deleteLine(b, x + i, y, RIGHT);
			deleteLine(b, x + i, y + i, BOTTOM_RIGHT);
			deleteLine(b, x, y + i, BOTTOM);
			deleteLine(b, x - i, y + i, BOTTOM_LEFT);
		}
	}	
	return result;
}

/**Test wether a line can be added to the board

   Test if a line can be added at the position (x,y) in the direction d to the board
   The test assumes there is or will be a point at the position ignored_x and ignored_y (x,y)
   The test prevents the addition a point if any rule is not respected
   
   Return 1 you can add the line with a point on (x,y) position in the direction d
   Return 0 if a point is missing on the line or if there is an overlap
*/
int alignable(board b,int x,int y, DIRECTION d, int ignored_x, int ignored_y) {
	int result = 1;
	switch(d) {
		case RIGHT:
			for(int i = 0; i < LINE_SIZE; i++)
				result &= (x + i == ignored_x && y == ignored_y) || isPoint(b, x + i, y);
			for(int i = 2 - LINE_SIZE; i < LINE_SIZE - 1; i++)
				result &= !isLine(b, x + i, y, RIGHT);
			break;
		case BOTTOM_RIGHT:
			for(int i = 0; i < LINE_SIZE; i++)
				result &= (x + i == ignored_x && y + i == ignored_y) || isPoint(b, x + i, y + i);
			for(int i = 2 - LINE_SIZE; i < LINE_SIZE - 1; i++)
				result &= !isLine(b, x + i, y + i, BOTTOM_RIGHT);
			break;
		case BOTTOM: 
			for(int i = 0; i < LINE_SIZE; i++)
				result &= (x == ignored_x && y + i == ignored_y) || isPoint(b, x, y + i);
			for(int i = 2 - LINE_SIZE; i < LINE_SIZE - 1; i++)
				result &= !isLine(b, x, y + i, BOTTOM);
			break;
		case BOTTOM_LEFT:
			for(int i = 0; i < LINE_SIZE; i++)
				result &= (x - i == ignored_x && y + i == ignored_y) || isPoint(b, x - i, y + i);
			for(int i = 2 - LINE_SIZE; i < LINE_SIZE - 1; i++)
				result &= !isLine(b, x + i, y + i, BOTTOM_LEFT);
			break;
		default: result = 0;
	}
	return result;
}

/**Provide all possible lines for a play at the position (x,y)

   Gather all playable lines overlapping on (x,y)
   The test assumes there is or will be a point at the position (x,y)
   Return a pointer of lines
*/
line* getPossibleLines(board b,int x,int y) {
	line* newLines = (line*)calloc(MAX_LINES_ON_POINT, sizeof(struct str_line));
	for(int i = 0; i < MAX_LINES_ON_POINT; i++)
		*(newLines + i) = NULL;
	int lines_counter = 0;
	for(int i = 1 - LINE_SIZE; i < 1; i++) {
		if((i == 0 || isPoint(b, x + i, y)) && alignable(b, x + i, y, RIGHT, x , y)) {
			line l = (line)malloc(sizeof(struct str_line));
			l->direction = RIGHT;
			l->pos_x = x + i;
			l->pos_y = y;
			*(newLines + lines_counter) = l; 
			lines_counter ++;
		}
		if((i == 0 || isPoint(b, x + i, y + i)) && alignable(b, x + i, y + i, BOTTOM_RIGHT, x , y)) {
			line l = (line)malloc(sizeof(struct str_line));
			l->direction = BOTTOM_RIGHT;
			l->pos_x = x + i;
			l->pos_y = y + i;
			*(newLines + lines_counter) = l;
			lines_counter ++;
		}
		if((i == 0 || isPoint(b, x, y + i)) && alignable(b, x, y + i, BOTTOM, x , y)) {
			line l = (line)malloc(sizeof(struct str_line));
			l->direction = BOTTOM;
			l->pos_x = x;
			l->pos_y = y + i;
			*(newLines + lines_counter) = l; 
			lines_counter ++;
		}
		if((i == 0 || isPoint(b, x - i, y + i)) && alignable(b, x - i, y + i, BOTTOM_LEFT, x , y)) {
			line l = (line)malloc(sizeof(struct str_line));
			l->direction = BOTTOM_LEFT;
			l->pos_x = x - i;
			l->pos_y = y + i;
			*(newLines + lines_counter) = l; 
			lines_counter ++;
		}
	}
	return newLines;
}

/**Display lines of the given board*/
void displayPossibleLines(board b, int x,int y) {
	line* possibleLines = getPossibleLines(b, x, y);
	line currentLine;
	for(int i = 0; i < MAX_LINES_ON_POINT; i++) {
		currentLine = *(possibleLines + i);
		if(currentLine != NULL)
			printf(" direction : %d, x : %d, y : %d; ", currentLine->direction, currentLine->pos_x, currentLine->pos_y);
	}
	printf("\n");
	free(possibleLines);
}

/******** Point ********/

/**Return wether there is a point at case (x,y) on board b

   Return 1 if there is one
   Return 0 if there isn't or if the case is unreachable
*/
int isPoint(board b,int x,int y) {
	int result = 0;
	if(x < b->size_x && y < b->size_y && x > -1 && y >= -1) 
		result = *(b->points + x + y * b->size_x);
	return result;
}

/**Add a point 
   
   Resize the board if necessary, delaying every position if x or y are lower than 0
   Return 1 on success
   Return 0 either if there is already a point there or if the case is unreachable
*/
int addPoint(board b, int x, int y) {
	int result = 0;
	if(x < 0 || y < 0) {
		int delay = x < y ? x : y;
		if(resize(b, b->size_x - delay, b->size_y - delay, - delay))
			*(b->points + x - delay + (y - delay) * b->size_x) = 1;
		else
			result = 1;
	}
	else if(x >= b->size_x || y >= b->size_y) {
		int delay = x > y ? x : y;
		if(resize(b, b->size_x + delay, b->size_y + delay, 0))
			*(b->points + x + y * b->size_x) = 1;
		else
			result = 1;
	}
	else if(x > -1 && y > -1 && x < b->size_x && y < b->size_y) {
		result = isPoint(b, x, y);
		*(b->points + x + y * b->size_x) = 1;
	}
	return result;
}

/**Delete a point from the board and all the given line associated

   Return 1 on success
   Return 0 if there is no point or if the case is unreachable
*/
int deletePoint(board b, int x, int y) {
	int result = 0;
	if(x > -1 && y > -1 && x < b->size_x && y < b->size_y) {
		result = isPoint(b, x, y);
		*(b->points + x + y * b->size_x) = 0;
		deleteLinesOnPoint(b, x, y);
	}
	return result;
}

/**Check if move (x,y) is valid

   Return 0 if move is valid
   Return 1 if there isn't any possible line overlapping a point in (x,y)
   Return 2 if there is already a point
   Return 3 if the move is invalid or if the case is unreachable
*/
int isValidMove(board b, int x,int y) {
	int result = 1;
	if(b->size_x + 1 < x || b->size_y + 1 < y || x < -1 || y < -1)
		result = 3;
	else if(isPoint(b, x, y))
		result = 2;
	else {
		line* lines = getPossibleLines(b, x, y);
		line l;
		if(*lines != NULL) {
			result = 0;
			for(int i = 0; i < MAX_LINES_ON_POINT; i++) {
				l = *(lines + i);
				free(l);
			}
			free(lines);
		}
	}
	return result;
}

/**Return the list of possible moves for board b

   Return a pointer to an array of minimal size 1
   The first element is the number of possible moves
   Then there is the list of moves x first
   ex :            [3,1,2,2,1,3,4]
   number of moves<-|        |---|->third move (3,4) 
*/
int* getPossibleMoves(board b) {
	int* moves = (int*)calloc((b->size_x + 2) * (b->size_y + 2) * 2, sizeof(int));
	int counter = 1;
	for(int x = -1; x <= b->size_x; x++)
		for(int y = -1; y <= b->size_y; y++)
			if(isValidMove(b, x, y) == 0) {
				*(moves + counter * 2 - 1) = x;
				*(moves + counter * 2) = y;
				counter++;
			}
	*(moves) = counter;
	return moves;
}

/**Display points of the given board*/
void displayPoints(board b) {
	for(int y = 0; y < b->size_y; y++) {
		for(int x = 0; x < b->size_x; x++)
			printf("%d", *(b->points + x + y * b->size_x ));
		printf("\n");
	}
}
