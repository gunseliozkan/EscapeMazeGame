#include <stdio.h>

#define MAX_WEIGHT 99999
#define SHORTEST_DAMAGE 4
#define COMFORTABLE_DAMAGE 2
#define SAFEST_DAMAGE 6
#define TOTAL_LIFE 30


typedef enum _tileType {BORDER, WALL, EMPTY, USED, START} TILE_TYPE;
typedef enum _pathType {DISTANCE, COMFORT, DANGER} PATH_TYPE;
typedef enum _printType {P_DISTANCE, P_COMFORT, P_DANGER, P_TRAP} PRINT_TYPE;



typedef struct _tiles{
	TILE_TYPE type;
	int distance;
	int dangerLevel;
	int comfortLevel;
	int trapDamage;
}TILES;


typedef struct _coord{
	int x;
	int y;
}Coord;


typedef struct _path{
	Coord coords[200];
	int size;
	int totalDistance;
	int totalDanger;
	int totalComfort;
	int totalDamage;
}Path;


/*starter functions*/
int testMaze(char * fileName, Coord startTile, PATH_TYPE pType);
void readMaze(TILES maze[100][100], char * fileName, int *w, int *h);
void printMaze(TILES maze[100][100], int a, int b);
void printMazeWithValues(TILES maze[100][100], int a, int b, PRINT_TYPE pType);
void printAll(TILES maze[100][100], int a, int b);
void markPath(TILES maze[100][100], Path path);
void clearBoard(TILES maze[100][100]);

/*Required*/
int isStuck(TILES maze[100][100], Coord currentTile);
int isExit(TILES maze[100][100], Coord currentTile);
Path shortestExit(TILES maze[100][100], Coord currentTile, Path path, int *minWeight);
Path comfortableExit(TILES maze[100][100], Coord currentTile, Path path, int *minWeight);
Path safestExit(TILES maze[100][100], Coord currentTile, Path path, int *minWeight);

/*Optional*/
void addCoord(TILES maze[100][100], Path *path, Coord coord, PATH_TYPE pType);
void removeLastCoord(TILES maze[100][100], Path *path, Coord coord, PATH_TYPE pType);


int testMaze(char * fileName, Coord startTile, PATH_TYPE pType){
	int w = 0, h = 0;
	Path path, resultPath;
	int result = 0;

	TILES maze[100][100];
	int minWeight = MAX_WEIGHT;

	printf("Board name: %s\n", fileName);

	readMaze(maze, fileName, &w, &h);

	addCoord(maze, &path, startTile, pType);
	switch(pType){
		case DISTANCE:
			resultPath = shortestExit(maze, startTile, path, &minWeight);
			break;
		case COMFORT:
			resultPath = comfortableExit(maze, startTile, path, &minWeight);
			break;
		case DANGER:
			resultPath = safestExit(maze, startTile, path, &minWeight);
			break;
	}
	removeLastCoord(maze, &path, startTile, pType);

	markPath(maze, resultPath);
	maze[startTile.x][startTile.y].type = START;

	switch(pType){
		case DISTANCE:
			printf("Shortest Path\n");
			result = resultPath.totalDistance;
			printMazeWithValues(maze, w, h, P_DISTANCE);
			break;
		case COMFORT:
			printf("Comfortable Path\n");
			result = resultPath.totalComfort;
			printMazeWithValues(maze, w, h, P_COMFORT);
			break;
		case DANGER:
			printf("Safest Path\n");
			result = resultPath.totalDanger;
			printMazeWithValues(maze, w, h, P_DANGER);
			break;
	}

	printMaze(maze, w, h);

	printf("Total weight: %d\n", result);
	printf("-------------------------------------\n");
	return result;
}

/*marks path tiles as USED for printing*/
void markPath(TILES maze[100][100], Path path){
	int i = 0;

	for (i = 0; i < path.size; ++i){
		maze[path.coords[i].x][path.coords[i].y].type = USED;
	}
}

void readMaze(TILES maze[100][100], char * fileName, int *w, int *h){
	int i = 0, j = 0, tile = 0, distance, comfort, danger, trap;
	FILE *fp = fopen (fileName, "r");

	clearBoard(maze);

	fscanf(fp, "%d", w);
	fscanf(fp, "%d", h);

	/* Tile type*/
	for (i = 0; i < *w; ++i){
		for (j = 0; j < *h; ++j){
			fscanf(fp, "%d", &tile);
			switch(tile){
				case 0:
					maze[i][j].type = EMPTY;
					break;
				case 1:
					maze[i][j].type = WALL;
					break;
				case 2:
					maze[i][j].type = USED;
					break;
				case 3:
					maze[i][j].type = BORDER;
					break;
			}
		}
	}

	fscanf(fp, "%d", &distance);/*useless seperator*/

	/* distance values */
	for (i = 0; i < *w; ++i){
		for (j = 0; j < *h; ++j){
			fscanf(fp, "%d", &distance);
			maze[i][j].distance = distance;
		}
	}

	fscanf(fp, "%d", &comfort);/*useless seperator*/

	/* comfort values */
	for (i = 0; i < *w; ++i){
		for (j = 0; j < *h; ++j){
			fscanf(fp, "%d", &comfort);
			maze[i][j].comfortLevel = comfort;
		}
	}
	fscanf(fp, "%d", &danger);/*useless seperator*/

	/* danger values */
	for (i = 0; i < *w; ++i){
		for (j = 0; j < *h; ++j){
			fscanf(fp, "%d", &danger);
			maze[i][j].dangerLevel = danger;
		}
	}

	/* trap values */
	fscanf(fp, "%d", &trap);/*useless seperator*/

	for (i = 0; i < *w; ++i){
		for (j = 0; j < *h; ++j){
			fscanf(fp, "%d", &trap);
			maze[i][j].trapDamage = trap;
		}
	}

	fclose(fp);
}

void printMaze(TILES maze[100][100], int a, int b){
	int i = 0, j = 0;

	printf("  ");
	for (j = 0; j < b; ++j){
		printf("%d ", j%10);
	}
	printf("\n");

	for (i = 0; i < a; ++i){
		printf("%d ", i%10);
		for (j = 0; j < b; ++j){
			switch(maze[i][j].type){
				case EMPTY:
					printf("  ");
					break;
				case WALL:
					printf("W ");
					break;
				case USED:
					printf(". ");
					break;
				case BORDER:
					printf("B ");
					break;
				case START:
					printf("* ");
					break;
			}
		}
		printf("\n");
	}
	printf("\n");
}

void printMazeWithValues(TILES maze[100][100], int a, int b, PRINT_TYPE pType){
	int i = 0, j = 0;

	printf("  ");
	for (j = 0; j < b; ++j){
		printf("%d ", j%10);
	}
	printf("\n");

	for (i = 0; i < a; ++i){
		printf("%d ", i%10);
		for (j = 0; j < b; ++j){
			if(maze[i][j].type == EMPTY || maze[i][j].type == USED || maze[i][j].type == START){
				if(pType == P_DISTANCE)
					printf("%d ", maze[i][j].distance);
				else if(pType == P_COMFORT)
					printf("%d ", maze[i][j].comfortLevel);
				else if(pType == P_DANGER)
					printf("%d ", maze[i][j].dangerLevel);
				else
					printf("%d ", maze[i][j].trapDamage);
			}else if(maze[i][j].type == BORDER){
				printf("B ");
			}else{
				printf("  ");
			}
		}
		printf("\n");
	}
	printf("\n");
}

void printAll(TILES maze[100][100], int a, int b){
	printf("Distance Map:\n");
	printMazeWithValues(maze, a, b, P_DISTANCE);
	printf("Comfort Map:\n");
	printMazeWithValues(maze, a, b, P_COMFORT);
	printf("Danger Map:\n");
	printMazeWithValues(maze, a, b, P_DANGER);
	printf("Trap Map:\n");
	printMazeWithValues(maze, a, b, P_TRAP);
	printf("Path:\n");
	printMaze(maze, a, b);
}

void clearBoard(TILES maze[100][100]){
	int i = 0, j = 0;

	for (i = 0; i < 100; ++i){
		for (j = 0; j < 100; ++j){
			maze[i][j].type = BORDER;
		}
	}
}

/***********************************************************/

int isStuck(TILES maze[100][100], Coord currentTile){
	/*student code*/
	int stuck=0;
	int isEx= isExit(maze, currentTile);

	if(isEx == 0)
	{
		if(maze[currentTile.x+1][currentTile.y].type!=EMPTY && maze[currentTile.x-1][currentTile.y].type!=EMPTY && maze[currentTile.x][currentTile.y+1].type!=EMPTY && maze[currentTile.x][currentTile.y-1].type!=EMPTY)
			stuck=1;
	}
	return stuck;
}

int isExit(TILES maze[100][100], Coord currentTile){
	/*student code*/
	int exit=0;

	if(maze[currentTile.x+1][currentTile.y].type==BORDER || maze[currentTile.x-1][currentTile.y].type==BORDER || maze[currentTile.x][currentTile.y+1].type==BORDER || maze[currentTile.x][currentTile.y-1].type==BORDER)
		exit=1;

	return exit;
}

Path shortestExit(TILES maze[100][100], Coord currentTile, Path path, int *minWeight){
	/* student code */
	Path p_up, p_down, p_left, p_right;
	int minUp=-1, minDown=-1, minLeft=-1, minRight=-1;
	int i;
	Coord up,down,left,right;

	up.x = currentTile.x-1;
	up.y = currentTile.y;
	down.x = currentTile.x+1;
	down.y = currentTile.y;
	left.x = currentTile.x;
	left.y = currentTile.y-1;
	right.x = currentTile.x;
	right.y = currentTile.y+1;


	for(i = 0; i < path.size - 1; ++i)
	{
		if(path.coords[i].x == currentTile.x && path.coords[i].y == currentTile.y)
		{
			*minWeight = -1;
			return path;
		}
	}

	if(path.coords[path.size - 1].x != currentTile.x || path.coords[path.size - 1].y != currentTile.y)
		addCoord(maze, &path, currentTile, DISTANCE);


	if(maze[currentTile.x][currentTile.y].type == EMPTY)
	{
		if(path.totalDamage >= TOTAL_LIFE)
		{
			*minWeight = -1;
			return path;
		}

		*minWeight = maze[currentTile.x][currentTile.y].distance;

		if(isExit(maze, currentTile)==1)
			return path;

		p_up = shortestExit(maze, up, path, &minUp);
		p_down = shortestExit(maze, down, path, &minDown);
		p_right = shortestExit(maze, right, path, &minRight);
		p_left = shortestExit(maze, left, path, &minLeft);

		if( minUp != -1 && ( (minDown == -1 && minLeft == -1 && minRight == -1) || (minDown != -1 && minUp < minDown) || (minLeft != -1 && minUp < minLeft) || (minRight != -1 && minUp < minRight) ) )
		{
			*minWeight += minUp;
			return p_up;
		}
		else if( minDown != -1 && ( (minUp == -1 && minLeft == -1 && minRight == -1) || (minUp != -1 && minDown < minUp) || (minLeft != -1 && minDown < minLeft) || (minRight != -1 && minDown < minRight))  )
		{
			*minWeight += minDown;
			return p_down;
		}
		else if( minLeft != -1 && ( (minDown == -1 && minUp == -1 && minRight == -1) ||  (minDown != -1 && minLeft < minDown) || (minUp != -1 && minLeft < minUp) || (minRight != -1 && minLeft < minRight))  )
		{
			*minWeight += minLeft;
			return p_left;
		}
		else if( minRight != -1 && ( (minDown == -1 && minLeft == -1 && minUp == -1) || (minUp != -1 && minRight < minDown) || (minLeft != -1 && minRight < minLeft) || (minUp != -1 && minRight < minDown))  )
		{
			*minWeight += minRight;
			return p_right;
		}
		else
			*minWeight = -1;
	}
	else
		*minWeight = -1;

	return path;
}

Path comfortableExit(TILES maze[100][100], Coord currentTile, Path path, int *minWeight){
	/* student code, RECURSION */
	Path p_up, p_down, p_left, p_right;
	int minUp=-1, minDown=-1, minLeft=-1, minRight=-1;
	int i;
	Coord up,down,left,right;

	up.x = currentTile.x-1;
	up.y = currentTile.y;
	down.x = currentTile.x+1;
	down.y = currentTile.y;
	left.x = currentTile.x;
	left.y = currentTile.y-1;
	right.x = currentTile.x;
	right.y = currentTile.y+1;

	for(i = 0; i < path.size - 1; ++i)
	{
		if(path.coords[i].x == currentTile.x && path.coords[i].y == currentTile.y)
		{
			*minWeight = -1;
			return path;
		}
	}

	if(path.coords[path.size - 1].x != currentTile.x || path.coords[path.size - 1].y != currentTile.y)
		addCoord(maze, &path, currentTile, COMFORT);

	if(maze[currentTile.x][currentTile.y].type == EMPTY)
	{
		if(path.totalDamage >= TOTAL_LIFE)
		{
			*minWeight = -1;
			return path;
		}

		*minWeight = maze[currentTile.x][currentTile.y].comfortLevel;

		if(isExit(maze, currentTile)==1)
			return path;


		p_up = comfortableExit(maze, up, path, &minUp);
		p_down = comfortableExit(maze, down, path, &minDown);
		p_right = comfortableExit(maze, right, path, &minRight);
		p_left = comfortableExit(maze, left, path, &minLeft);

		if( minUp != -1 && ( (minDown == -1 && minLeft == -1 && minRight == -1) || (minDown != -1 && minUp < minDown) || (minLeft != -1 && minUp < minLeft) || (minRight != -1 && minUp < minRight) ) )
		{
			*minWeight += minUp;
			return p_up;
		}
		else if( minDown != -1 && ( (minUp == -1 && minLeft == -1 && minRight == -1) || (minUp != -1 && minDown < minUp) || (minLeft != -1 && minDown < minLeft) || (minRight != -1 && minDown < minRight))  )
		{
			*minWeight += minDown;
			return p_down;
		}
		else if( minLeft != -1 && ( (minDown == -1 && minUp == -1 && minRight == -1) ||  (minDown != -1 && minLeft < minDown) || (minUp != -1 && minLeft < minUp) || (minRight != -1 && minLeft < minRight))  )
		{
			*minWeight += minLeft;
			return p_left;
		}
		else if( minRight != -1 && ( (minDown == -1 && minLeft == -1 && minUp == -1) || (minUp != -1 && minRight < minUp) || (minLeft != -1 && minRight < minLeft) || (minDown != -1 && minRight < minDown))  )
		{
			*minWeight += minRight;
			return p_right;
		}
		else
			*minWeight = -1;

	}
	else
		*minWeight = -1;

	return path;
}

Path safestExit(TILES maze[100][100], Coord currentTile, Path path, int *minWeight){
	/* student code, RECURSION */
	Path p_up, p_down, p_left, p_right;
	int minUp=-1, minDown=-1, minLeft=-1, minRight=-1;
	int i;
	Coord up,down,left,right;

	up.x = currentTile.x-1;
	up.y = currentTile.y;
	down.x = currentTile.x+1;
	down.y = currentTile.y;
	left.x = currentTile.x;
	left.y = currentTile.y-1;
	right.x = currentTile.x;
	right.y = currentTile.y+1;

	for(i = 0; i < path.size - 1; ++i)
	{
		if(path.coords[i].x == currentTile.x && path.coords[i].y == currentTile.y)
		{
			*minWeight = -1;
			return path;
		}
	}

	if(path.coords[path.size - 1].x != currentTile.x || path.coords[path.size - 1].y != currentTile.y)
		addCoord(maze, &path, currentTile, DANGER);


	if(maze[currentTile.x][currentTile.y].type == EMPTY)
	{
		if(path.totalDamage >= TOTAL_LIFE)
		{
			*minWeight = -1;
			return path;
		}

		*minWeight = maze[currentTile.x][currentTile.y].dangerLevel;

		if(isExit(maze, currentTile)==1)
			return path;


		p_up = safestExit(maze, up, path, &minUp);
		p_down = safestExit(maze, down, path, &minDown);
		p_right = safestExit(maze, right, path, &minRight);
		p_left = safestExit(maze, left, path, &minLeft);

		if( minUp != -1 && ( (minDown == -1 && minLeft == -1 && minRight == -1) || (minDown != -1 && minUp < minDown) || (minLeft != -1 && minUp < minLeft) || (minRight != -1 && minUp < minRight) ) )
		{
			*minWeight += minUp;
			return p_up;
		}
		else if( minDown != -1 && ( (minUp == -1 && minLeft == -1 && minRight == -1) || (minUp != -1 && minDown < minUp) || (minLeft != -1 && minDown < minLeft) || (minRight != -1 && minDown < minRight))  )
		{
			*minWeight += minDown;
			return p_down;
		}
		else if( minLeft != -1 && ( (minDown == -1 && minUp == -1 && minRight == -1) ||  (minDown != -1 && minLeft < minDown) || (minUp != -1 && minLeft < minUp) || (minRight != -1 && minLeft < minRight))  )
		{
			*minWeight += minLeft;
			return p_left;
		}
		else if( minRight != -1 && ( (minDown == -1 && minLeft == -1 && minUp == -1) || (minUp != -1 && minRight < minUp) || (minLeft != -1 && minRight < minLeft) || (minDown != -1 && minRight < minDown))  )
		{
			*minWeight += minRight;
			return p_right;
		}
		else
			*minWeight = -1;

	}
	else
		*minWeight = -1;


	return path;
}


/***********************************************************/


void addCoord(TILES maze[100][100], Path *path, Coord coord, PATH_TYPE pType){
	/* student code (optional) */
	int i = (path->size)++;

	path->coords[i] = coord;

	path->totalDistance += maze[coord.x][coord.y].distance;
	path->totalComfort += maze[coord.x][coord.y].comfortLevel;
	path->totalDanger += maze[coord.x][coord.y].dangerLevel;

	path->totalDamage += maze[coord.x][coord.y].trapDamage + (pType == COMFORT ? COMFORTABLE_DAMAGE : pType == DISTANCE ? SHORTEST_DAMAGE : SAFEST_DAMAGE);

}

void removeLastCoord(TILES maze[100][100], Path *path, Coord coord, PATH_TYPE pType){
	/* student code (optional) */
	(path->size)--;

	path->totalDistance -= maze[coord.x][coord.y].distance;
	path->totalComfort -= maze[coord.x][coord.y].comfortLevel;
	path->totalDanger -= maze[coord.x][coord.y].dangerLevel;

	path->totalDamage -= maze[coord.x][coord.y].trapDamage + (pType == COMFORT ? COMFORTABLE_DAMAGE : pType == DISTANCE ? SHORTEST_DAMAGE : SAFEST_DAMAGE);
}



int main(){

	Coord startTile1;
	startTile1.x = 5;
	startTile1.y = 2;


	testMaze("maze1.txt", startTile1, DISTANCE);
	testMaze("maze1.txt", startTile1, COMFORT);
	testMaze("maze1.txt", startTile1, DANGER);

	return 0;
}
