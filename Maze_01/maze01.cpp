#include <iostream>

#define ENTER 10
#define FALSE 0
#define TRUE 1
#define MAX_STACK 100

using namespace std;

typedef int **MAZE;

typedef struct {
	short int iX;
	short int iY;
}DIRECTION;
DIRECTION m_Move[8];

typedef struct {
	short int iX;
	short int iY;	
	short int dir;
}ELEMENT;

void Initialize();
void Draw(MAZE maze, int Count_iY, int temp_iX);
void Path(MAZE &maze, MAZE &mark, int *EXIT_iX, int *EXIT_iY);
ELEMENT Stack_pop(int *ptop, ELEMENT *pStack);
void Stack_push(int *ptop, ELEMENT Pos, ELEMENT *pStack);
void Delete(MAZE &maze, MAZE &mark, FILE **fp, int *pCount_iY);

void main()
{
	char text;
	int Count_iX = 0, Count_iY = 1, temp_iX;
	int EXIT_iX, EXIT_iY;

	FILE *fp_map;
	fopen_s(&fp_map, "MapTile.txt", "rt");

	if (fp_map == NULL)
		cout << "ERROR" << endl;
	else
	{
		Initialize();

		while (!feof(fp_map))
		{
			text = fgetc(fp_map);

			if (text == '0' || text == '1')
				Count_iX++, temp_iX = Count_iX;
			else if (text == ENTER)
				Count_iY++, Count_iX = 0;
		}

		EXIT_iX = temp_iX - 2, EXIT_iY = Count_iY - 2;

		MAZE maze = new int *[Count_iY];
		MAZE mark = new int *[Count_iY];
		
		for (int i = 0; i < Count_iY; i++)
		{
			maze[i] = new int[temp_iX];
			mark[i] = new int[temp_iX];
		}

		fseek(fp_map, 0, SEEK_SET);

		for (int i = 0; i < Count_iY; i++)
		{
			for (int j = 0; j < temp_iX; j++)
			{
				text = fgetc(fp_map);

				if (text == '0')
					maze[i][j] = 0;
				else if (text == '1')
					maze[i][j] = 1;
				else if (text == ENTER)
					j--;

				mark[i][j] = 0;
			}
		}

		Draw(maze, Count_iY, temp_iX);
		Path(maze, mark, &EXIT_iX, &EXIT_iY);

		Delete(maze, mark, &fp_map, &Count_iY);
	}
	system("pause");
}

void Initialize()
{
	short int iY[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };

	for (int i = 0; i < 8; ++i)
	{
		m_Move[i].iX = iY[(i + 2) % 8];
		m_Move[i].iY = iY[i];
	}
}

void Draw(MAZE maze, int Count_iY, int temp_iX)
{
	for (int i = 0; i < Count_iY; ++i)
	{
		for (int j = 0; j < temp_iX; ++j)
			cout << maze[i][j] << " ";

		cout << endl;
	}
}

void Delete(MAZE &maze, MAZE &mark, FILE **fp, int *pCount_iY)
{
	delete[] *maze;
	delete[] *mark;

	delete[] maze;
	delete[] mark;

	fclose(*fp);
}

void Path(MAZE &maze, MAZE &mark, int *EXIT_iX, int *EXIT_iY)
{
	int itop, iY, iX, next_iY, next_iX, dir, found = FALSE;
	ELEMENT m_Pos, stack[MAX_STACK];

	itop = 0, mark[1][1] = 1;
	stack[0].iY = 1, stack[0].iX = 1, stack[0].dir = 1;

	while (itop >= 0 && !found)
	{
		m_Pos = Stack_pop(&itop, stack);
		
		iY = m_Pos.iY, iX = m_Pos.iX;
		dir = m_Pos.dir;

		while (dir < 8 && !found)
		{
			next_iY = iY + m_Move[dir - 1].iX;
			next_iX = iX + m_Move[dir - 1].iY;

			if (next_iY == *EXIT_iY && next_iX == *EXIT_iX)
				found = TRUE;
			else if (!maze[next_iY][next_iX] && !mark[next_iY][next_iX])
			{
				mark[next_iY][next_iX] = 1;

				m_Pos.iY = iY, m_Pos.iX = iX;
				m_Pos.dir = ++dir;

				Stack_push(&itop, m_Pos, stack);
				
				iY = next_iY, iX = next_iX;
				dir = 0;
			}
			else
				++dir;
		}

	}

	if (found)
	{
		cout << endl;

		cout << "The path is:" << endl;
		cout << "row col" << endl << endl;
		
		for (int i = 0; i <= itop; i++)
			cout << "Pos (" << stack[i].iX << ", " << stack[i].iY << ")" << endl;
		cout << "Pos (" << iX << ", " << iY << ")" << endl;
		cout << "Pos (" << *EXIT_iX << ", " << *EXIT_iY << ")" << endl << endl;
	}
	else
		printf("The maze does not have a path\n");
}

ELEMENT Stack_pop(int *ptop, ELEMENT *pStack)
{
	if (*ptop == -1)
	{
		cout << "Stack empty" << endl;
		return pStack[MAX_STACK];
	}
	return pStack[(*ptop)--];
}

void Stack_push(int *ptop, ELEMENT Pos, ELEMENT *pStack)
{
	if (*ptop >= (MAX_STACK - 1))
		cout << "Stack full" << endl;
	else
		pStack[++(*ptop)] = Pos;
}