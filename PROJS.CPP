#include<iostream.h>//cout
#include<conio.h>//clrscr, getch, kbhit
#include<stdlib.h>//itoa, atoi
#include<string.h>//strcpy, strlen, strcmpi, strncmpi
#include<stdio.h>//printf
#include<graphics.h> //Various Graphics Mode functions such as outtextxy,fillellipse,etc.
#include<dos.h> //Delay
#include<process.h>//exit
#include<ctype.h>//toupper, isdigit, isprint
#include<fstream.h>//File Handling - fstream

//-------------
//--Constants--
//-------------
const int FALSE = 0;
const int TRUE = !FALSE;
const int MAX_ROW = 10;
const int MAX_COL = 8;
const int MAX_ENTRY_SIZE = 17 + 1;

//Arrow Keys ASCII
const int UP_KEY = 72;
const int DOWN_KEY = 80;
const int LEFT_KEY = 75;
const int RIGHT_KEY = 77;
const int ENTER_KEY = 13;
const int TILDE_KEY = 96;
const int BACKSPACE_KEY = 8;

//Grid Item Types
const int TYPE_TEXT = 1;
const int TYPE_NUM = 2;
const int TYPE_FAIL = 3;

//--------------
//--Functions---
//--------------

//Displays the intro screen
void intro()
{
	clrscr();
	int gd=DETECT,gm;
	initgraph(&gd,&gm,"A:/BGI");
	setbkcolor(LIGHTGRAY);
	setlinestyle(0,1,1);
	rectangle(120,130,520,330);
	setfillstyle(1,BLUE);
	floodfill(150,170,WHITE);
	settextstyle(0,0,1);
	outtextxy(200,160,"ShreMay-Soft");
	settextstyle(1,0,7);
	outtextxy(250,180,"XSheets");
	setlinestyle(1,1,4);
	line(160,160,185,190);
	line(140,190,160,160);
	line(160,140,160,190);
	rectangle(135,135,190,195);

	for(int i=140;i<500;i+=1)
	{
		line(140,315,i,315);
		delay(7);
		settextstyle(1,0,1);
		outtextxy(150,290,"Loading Resources....");
	 if(kbhit())
	 	break;
	}
	setcolor(BLUE);
	settextstyle(2,0,5);
	outtextxy(420,400,"PRESS ENTER...");

	getch();
	closegraph();
}

//Truncates a string
char* truncstr(char* in)
{
	in[8] = '\0';
	return in;
}

//Starts the program
void main();

//--------------
//---Classes----
//--------------

//Represents an entire table
class grid;

//Represents a single item on a grid
class griditem
{
	char text[MAX_ENTRY_SIZE] //Stores the text of an item
	, numtext[MAX_ENTRY_SIZE]; //Stores the numerical text of an item, if any
	int type //Stores the type of item (Text/Num/Failed Formula)
	, numval; //Stores the numerical value of an item, if any
	private:

		//Sets the numerical value and changes the type of the class
		void setnum(int num)
		{
			type = TYPE_NUM;
			numval = num;
			itoa(numval,numtext,10);
		}

		//Checks whether the entered text is a number (consists of digits)
		int checkdigit()
		{
			if(text[0]=='\0')
				return FALSE;
			int res = TRUE;
			for(int i=0;text[i]!='\0';i++)
			{
				if(!isdigit(text[i]))
				{
					res = FALSE;
					break;
				}
			}
			return res;
		}

	public:

		//Checks and sets for numerical values/formulas
		void checkandsetnum(grid* g)
		{
			if(checkdigit())
			{
				setnum(atoi(text));
			}

			else if(text[0] == '=')
			{
				type = TYPE_FAIL;
				if(strncmpi(text+1,"SUM",3)==0)
				{
					//SUM
					int startcol = toupper(text[5]) - 'A',
					startrow = text[6] - '0' - 1,
					endcol = toupper(text[8]) - 'A',
					endrow = text[9] - '0' - 1;

					int sum = 0;


					if(startcol == endcol)
					{
						for(int i = startrow;i<=endrow;i++)
						{
							sum += g->getnum(i,startcol);
						}
						setnum(sum);
					}
					else if(startrow == endrow)
					{
						for(int i = startcol;i<=endcol;i++)
						{
							sum += g->getnum(startrow,i);
						}
						setnum(sum);
					}
				}
				else if(strncmpi(text+1,"AVG",3)==0)
				{
					//AVG
					int startcol = toupper(text[5]) - 'A',
					startrow = text[6] - '0' - 1,
					endcol = toupper(text[8]) - 'A',
					endrow = text[9] - '0' - 1;

					int sum = 0;


					if(startcol == endcol)
					{
						int count = 0;
						for(int i = startrow;i<=endrow;i++)
						{
							sum += g->getnum(i,startcol);
							count++;
						}
						setnum(sum/count);
					}
					else if(startrow == endrow)
					{
						int count = 0;
						for(int i = startcol;i<=endcol;i++)
						{
							sum += g->getnum(startrow,i);
							count++;
						}
						setnum(sum/count);
					}
				}
				else if(strncmpi(text+1,"PROD",4)==0)
				{
					//PROD
					int startcol = toupper(text[6]) - 'A',
					startrow = text[7] - '0' - 1,
					endcol = toupper(text[9]) - 'A',
					endrow = text[10] - '0' - 1;

					int prod = 1;


					if(startcol == endcol)
					{
						for(int i = startrow;i<=endrow;i++)
						{
							prod *= g->getnum(i,startcol);
						}
						setnum(prod);
					}
					else if(startrow == endrow)
					{
						for(int i = startcol;i<=endcol;i++)
						{
							prod *= g->getnum(startrow,i);
						}
						setnum(prod);
					}
				}
			}
		}

		//Sets the text of the item
		void settext(char* t, grid* g)
		{
			type = TYPE_TEXT;
			strcpy(text,t);
			checkandsetnum(g);
		}

		//Returns the entered text of the item
		char* gettext()
		{
			return text;
		}

		//Returns the display text of the item
		char* getdisplay()
		{
			if(type == TYPE_FAIL)
			{
				return "Invalid Formula";
			}
			else if(type == TYPE_TEXT)
			{
				return gettext();
			}
			else
			{
				return numtext;
			}
		}

		//Returns the numerical value of the item
		int getnum()
		{
			return numval;
		}

		//Constructor — Initializes the member variables
		griditem()
		{
			strcpy(text,"");
			type = TYPE_TEXT;
			numval = 0;
		}
};

//Represents an entire table
class grid
{
	//Stores the addresses of the allocated memory of grid items
	griditem *storage[MAX_ROW][MAX_COL]; 

	//Store the position of the cursor
	int current_row, current_col;
	

	public:
	//Sets the element at indices (i,j) with val. Sets the current elem as default
	void set(char* val,int i,int j, grid* g)
	{
		storage[i][j]->settext(val,g);
	}
	
	//Returns the display text of the element at indices (i,j)
	char* get(int i,int j)
	{
		return storage[i][j]->getdisplay();
	}

	//Returns the entered text of the element at indices (i,j)
	char* gettext(int i,int j)
	{
		return storage[i][j]->gettext();
	}

	//Returns the numerical value of the element at indices (i,j)
	int getnum(int i,int j)
	{
		return storage[i][j]->getnum();
	}

	//Returns the display text of the current element
	char* getcurr()
	{
		return get(current_row,current_col);
	}

	//Returns the entered text of the current element
	char* getcurrtext()
	{
		return gettext(current_row,current_col);
	}

	//Sets the current element at with val
	void setcurr(char* val,grid* g)
	{
		set(val,current_row,current_col,g);
	}

	//Moves the current element cursor up
	void up()
	{
		if(current_row--==0)
		{
			current_row = 0;
		}

	}

	//Moves the current element cursor down
	void down()
	{
		cout<<"hi";
		if(current_row++==MAX_ROW-1)
		{
			current_row = MAX_ROW - 1;
		}
	}

	//Moves the current element cursor left
	void left()
	{
		if(current_col--==0)
		{
			current_col = 0;
		}
	}

	//Moves the current element cursor right
	void right()
	{
		if(current_col++==MAX_COL-1)
		{
			current_col = MAX_COL - 1;
		}
	}

	//Assists in displaying the current cursor of the row 
	void showrow()
	{
		for(int x=0;x<current_col;x++)
		cout<<"         ";
		cout<<char(223)<<"\n";

	}

	//Assists in displaying the current cursor of the column
	void showcol(int j)
	{
	      if(j==current_row)
	      cout<<char(223)<<"|";
	      else
	      cout<<" |";
	}

	//Clears the entire table
	void clear()
	{
		int i;
		for(i = 0; i<MAX_ROW;i++)
		{
			for(int j = 0;j<MAX_COL;j++)
			{
			       delete storage[i][j];
			}
		}
		current_row = current_col = 0;
		for(i = 0; i<MAX_ROW;i++)
		{
			for(int j = 0;j<MAX_COL;j++)
			{
				storage[i][j] = new griditem;
				if(storage[i][j] == NULL)
				{
					clrscr();
					cout<<"Insufficient Memory";
					getch();
					exit(0);
				}
			}
		}
	}

	//Constructor — Reserves memory for the grid
	grid()
	{
		current_row = current_col = 0;
		for(int i = 0; i<MAX_ROW;i++)
		{
			for(int j = 0;j<MAX_COL;j++)
			{
				storage[i][j] = new griditem;
				if(storage[i][j] == NULL)
				{
					clrscr();
					cout<<"Insufficient Memory";
					getch();
					exit(0);
				}
			}
		}
	}

	//Destructor — Frees allocated memory 
	~grid()
	{
		for(int i = 0; i<MAX_ROW;i++)
		{
			for(int j = 0;j<MAX_COL;j++)
			{
			       delete storage[i][j];
			}
		}
	}
};

//Assists in saving data to a file
class fhandler
{
	//Stores the address of the table
	grid* g;
	
	//Stream object
	fstream stream;

	public:
		//Constructor — Intializes the grid address
		fhandler(grid* ga)
		{
			g = ga;
		}

		//Reads from a file
		void read(char* fname)
		{
			stream.open(fname,ios::in);
			if(stream != NULL)
			{
				g->clear();
				while(!stream.eof())
				{
					cout<<"a";
					int i,j;
					char arr[MAX_ENTRY_SIZE];
					stream>>i;
					cout<<i;
					if(stream.eof())
						break;
					stream>>j;
					stream.get(); //To clear \n from the stream
					cout<<j;
					if(stream.eof())
						break;
					stream.getline(arr,MAX_ENTRY_SIZE);
					cout<<arr;
					if(stream.eof())
						break;
					g->set(arr,i,j,g);
				}
			}
			stream.close();
		}

		//Writes to a file
		void write(char* fname)
		{
			stream.open(fname,ios::out);
			for(int i = 0; i<MAX_ROW;i++)
			{
				for(int j = 0;j<MAX_COL;j++)
				{
					if(strlen(g->gettext(i,j))>0)
						stream<<i<<"\n"<<j<<"\n"<<g->gettext(i,j)<<"\n";
				}
			}
			stream.close();
		}
};

//Assists in entering data
class editor
{
	//Stores the text of the field
	char text[MAX_ENTRY_SIZE];

	//Length of the stores text
	int len;

public:
	//Loads in a string to the field
	void load(char* t)
	{
		strcpy(text,t);
		len = strlen(text);
	}

	//Returns the text in the field
	char* get()
	{
		return text;
	}

	//Inserts a single character into the field
	void inschar(char c)
	{
		if(len<MAX_ENTRY_SIZE - 1)
		{
			text[len++] = c;
			text[len] = '\0';
		}
	}

	//Removes a single character from the field
	void del()
	{
		if(len>0)
		{
			text[len-1] = '\0';
			len--;
		}
	}
};

void main()
{
	intro();
	clrscr();
	grid g;
	fhandler handler(&g);
	int iscopied = FALSE;

	char* ins = "-----------------------------             --------------------------\n| Arrow Keys -> move cursor |            |         Formulas         |\n| Keys -> Type              |            | Sum: sum(r(x1):r(x2))    |\n| Enter -> Write            |	         | Prod: prod(r(x1):r(x2))  |\n| `  -> More                |	         | Avg: avg(r(x1):r(x2))    |\n-----------------------------	          --------------------------\n";

	char copytext[MAX_ENTRY_SIZE];
	char ch;
	editor e;
	do
	{
		clrscr();
		cout<<ins;
		cout<<"            -----------------\n";
		printf("INPUT:     |%-17s|\n",e.get());
		cout<<"            -----------------\n";


		char z='A';
		cout<<"       ";
		for(int x=0;x<MAX_COL;x++)
		{
			cout<<"  "<<z;
			if(x!=MAX_COL-1)
			cout<<"      ";
			z++;
		}
		cout<<"\n"<<"         ";
		g.showrow();

		for(int i = 0; i<MAX_ROW;i++)
		{
			for(int j = 0;j<MAX_COL;j++)
			{
			       if(j==0)
			       {cout<<"  ";
			       printf("%2i",i+1);
			       g.showcol(i);}
			       char temp[MAX_ENTRY_SIZE];
			       strcpy(temp,g.get(i,j));
			       printf("%-8s|",truncstr(temp));

			}

			cout<<"\n";
		}
		ch = getch();
		switch(ch)
		{
			case UP_KEY:
				g.up();
				e.load(g.getcurrtext());
			break;
			case DOWN_KEY:
				g.down();
				e.load(g.getcurrtext());
			break;
			case LEFT_KEY:
				g.left();
				e.load(g.getcurrtext());
			break;
			case RIGHT_KEY:
				g.right();
				e.load(g.getcurrtext());
			break;
			case TILDE_KEY:
				char ch2;
				cout<<"c -> Copy ";
				if(iscopied==TRUE)
				{
					cout<<"p -> Paste";
				}
				cout<<"\n";
				cout<<"x -> Clear s -> Save  l -> Load  e -> Exit";
				ch2 = getch();
				switch(ch2)
				{
					case 's':
						cout<<"\nEnter file name and ext to save as: ";
						char name[255];
						cin>>name;
						handler.write(name);
					break;
					case 'l':
						cout<<"\nEnter file name and ext to load: ";
						cin>>name;
						handler.read(name);
					break;
					case 'e':
						exit(0);
					break;
					case 'c':
						strcpy(copytext,g.getcurrtext());
						iscopied = TRUE;
					break;
					case 'p':
						if(iscopied==TRUE)
						{
							g.setcurr(copytext,&g);
						}
					break;
					case 'x':
						g.clear();
					break;
				}
			break;
			case BACKSPACE_KEY:
				e.del();
			break;
			case ENTER_KEY:
				g.setcurr(e.get(),&g);
			break;
			default:
				if(isprint(ch)) //Checks if a printing character
				{
					e.inschar(ch);
				}
		}
	} while(TRUE);
}