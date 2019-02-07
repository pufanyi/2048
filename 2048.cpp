#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

char aa[1000];

char a[] = " ------------------------------- \n";
char b[] = "|       |       |       |       |\n";
string c = "|   ", d = "   |   ", e = "   |\n";

bool fflag;

int neww[4] = { 2,2,4,2 };
int num[5][5];
int used[5][5], b1[5][5];
int score;

#define PERR(bSuccess, api) \
{if(!(bSuccess)) printf("%s:Error %d from %s on line %d\n", __FILE__,GetLastError(), api, __LINE__);}
void MyCls(HANDLE);
inline void clrscr()
{
	HANDLE hStdOut=GetStdHandle(STD_OUTPUT_HANDLE);
	MyCls(hStdOut);
	return;
}

void MyCls(HANDLE hConsole)
{
	COORD coordScreen= {0,0};
	BOOL bSuccess;
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	bSuccess=GetConsoleScreenBufferInfo(hConsole,&csbi);
	PERR(bSuccess,"GetConsoleScreenBufferInfo");
	dwConSize=csbi.dwSize.X * csbi.dwSize.Y;
	bSuccess=FillConsoleOutputCharacter(hConsole,(TCHAR)' ',dwConSize,coordScreen,&cCharsWritten);
	PERR(bSuccess,"FillConsoleOutputCharacter");
	bSuccess=GetConsoleScreenBufferInfo(hConsole,&csbi);
	PERR(bSuccess,"ConsoleScreenBufferInfo");
	bSuccess=FillConsoleOutputAttribute(hConsole,csbi.wAttributes,dwConSize,coordScreen,&cCharsWritten);
	PERR(bSuccess,"FillConsoleOutputAttribute");
	bSuccess=SetConsoleCursorPosition(hConsole,coordScreen);
	PERR(bSuccess,"SetConsoleCursorPosition");
}

void hidecursor()
{
	CONSOLE_CURSOR_INFO cursor_info= {1,0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}

char in()
{
	char ch;
	for (;;)
		if (_kbhit())
			return ch = _getch();
}

void out();

class HELPER
{
	private:
		bool flag;

		void jieshao()
		{
			clrscr();
			puts("��Ϸ����");
			puts("2048��һ������������Ϸ������2048���ĳ�ʼ����������2+2��ɵĻ���4���ڲ�������Ĳ�ͬ�����Ϊһ��һ����ƶ�����ɸ�Ϊˬ���һ�ε��ס���ͬ���ֵķ����ڿ�£����ײʱ����ӡ�ϵͳ��������ַ��鲻��2����4�����Ҫ��취����СС��16��Χ�дճ���2048��������ַ��顣");
			puts("\n��'B'�����أ���'C'������Ϸ��");
			for (;;)
			{
				char ch = in();
				if (ch == 'c' || ch == 'C') flag = true;
				if (ch == 'b' || ch == 'c' || ch == 'B' || ch == 'C')
					return;
			}
		}

		void teach()
		{
			clrscr();
			puts("��Ϸ����");
			puts("1������������ܷ��ڽ��䡣\n2�����ְ�˳��������С�\n3����������������ʹδ����ڵ���һ�� / �������ġ�\n4��ʱ��ע���ϴ�����32���ϣ��Ա�Ҫ�����������\n5���Դ������ڵ�һ��Ϊ��Ҫ�ƶ�����\n6����Ҫ���ڡ��������桱��");
			puts("\n��'B'�����أ���'C'������Ϸ��");
			for (;;)
			{
				char ch = in();
				if (ch == 'c' || ch == 'C') flag = true;
				if (ch == 'b' || ch == 'c' || ch == 'B' || ch == 'C') return;
			}
		}

		void ppress()
		{
			clrscr();
			puts("��������");
			puts("��:'W','��'\n��:'S','��'\n��:'A','��'\n��:'D','��'\n����:'Q'\n�˳�:'O'\n���¿�ʼ:'B'\n����:'H'");
			puts("\n��'B'�����أ���'C'������Ϸ��");
			for (;;)
			{
				char ch = in();
				if (ch == 'c' || ch == 'C') flag = true;
				if (ch == 'b' || ch == 'c' || ch == 'B' || ch == 'C')
					return;
			}
		}

	public:
		void help()
		{
			for (;;)
			{
				clrscr();
				puts("�����ּ���ѡ�񣬰�'B'�����أ���'C'������Ϸ��\n1.��Ϸ����\n2.��������\n3.��Ϸ����");
				char ch = in();
				switch (ch)
				{
					case 'b':
					case 'B':
					case 'c':
					case 'C':
						out();
						return;
					case '1':
						jieshao();
						break;
					case '2':
						ppress();
						break;
					case '3':
						teach();
						break;
				}
				if (flag)
				{
					flag = false;
					out();
					return;
				}
			}
		}
} helper;

int last_move[5][5];
int X, Y;

void color()
{
	string colo;
	for(;;)
	{
		clrscr();
		puts("What color do you want?\npress '1', '2', '3' to choose, and press 's' to save");
		puts("\n1. �׵׺���\n2. �ڵװ���\n") ;
		int co = in();
		switch(co)
		{
			case '1':
				system("color f0");
				colo="color f0";
				break;
			case '2':
				system("color 0f");
				colo="color 0f";
				break;
			case 's':
				out();
				ofstream ffout("color.bak");
				ffout << colo << endl;
				ffout.close();
				return;
		}
	}
}

void beginning()
{
	score = 0;
	memset(num, 0, sizeof(num));
	srand((unsigned)time(NULL));
	X = rand() % 4 + 1;
	Y = rand() % 4 + 1;
	num[X][Y] = neww[rand() % 4];
}

bool pd_ex()
{
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			if (!num[i][j])
				return 1;
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 3; j++)
			if (num[i][j] == num[i][j + 1])
				return 1;
	for (int i = 1; i <= 3; i++)
		for (int j = 1; j <= 4; j++)
			if (num[i][j] == num[i + 1][j])
				return 1;
	return 0;
}

bool compare()
{
	int comp;
	ifstream fin("saveload.data");
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			if ((!(fin >> comp)) || (comp != num[i][j]))
			{
				fin.close();
				return true;
			}
	fin >> comp;
	fin.close();
	return comp != score;
}

void save()
{
	ofstream fout("saveload.data");
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			fout << num[i][j] << ' ';
	fout << score;
	fout.close();
	MessageBox(NULL, "����ɹ���", "WARNING", MB_OK);
}

void load()
{
	ifstream fin("saveload.data");
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
		{
			if ((!(fin >> num[i][j])) || (num[i][j] & 1))
			{
				MessageBox(NULL, "�Ҳ����Ϸ��ļ���׼�����¿�ʼ", "WARNING", MB_OK);
				beginning();
				ofstream fout("saveload.data");
				for (int i = 1; i <= 4; i++)
					for (int j = 1; j <= 4; j++)
						fout << num[i][j] << " ";
				fout << score;
				fout.close();
				return;
			}
		}
	fin >> score;
	fin.close();
}

#define ssaa (aa+strlen(aa))

void print(int x, int y)
{
	if (num[x][y] != 0)
		if (num[x][y]<10)
			sprintf(ssaa,"   %d   ",num[x][y]);
		else
		{
			if (num[x][y]<100)
				sprintf(ssaa,"   %d  ",num[x][y]);
			else if (num[x][y]<1000)
				sprintf(ssaa,"  %d  ",num[x][y]);
			else if (num[x][y]<10000)
				sprintf(ssaa,"  %d ",num[x][y]);
			else if (num[x][y]<100000)
				sprintf(ssaa," %d  ",num[x][y]);
			else if (num[x][y]<1000000)
				sprintf(ssaa,"%d ",num[x][y]);
			else
				sprintf(ssaa,"%d",num[x][y]);
		}
	else sprintf(ssaa,"       ");
}


void out()
{
	memset(aa,0,sizeof(aa));
	sprintf(ssaa,"%s%s|",a,b);
	print(1, 1);
	sprintf(ssaa,"|");
	print(1, 2);
	sprintf(ssaa,"|");
	print(1, 3);
	sprintf(ssaa,"|");
	print(1, 4);
	sprintf(ssaa,"|\n");//1
	sprintf(ssaa,"%s%s%s|",b,a,b);
	print(2, 1);
	sprintf(ssaa,"|");
	print(2, 2);
	sprintf(ssaa,"|");
	print(2, 3);
	sprintf(ssaa,"|");
	print(2, 4);
	sprintf(ssaa,"|\n");//2
	sprintf(ssaa,"%s%s%s|",b,a,b);
	print(3, 1);
	sprintf(ssaa,"|");
	print(3, 2);
	sprintf(ssaa,"|");
	print(3, 3);
	sprintf(ssaa,"|");
	print(3, 4);
	sprintf(ssaa,"|\n");
	sprintf(ssaa,"%s%s%s|",b,a,b);
	print(4, 1);
	sprintf(ssaa,"|");
	print(4, 2);
	sprintf(ssaa,"|");
	print(4, 3);
	sprintf(ssaa,"|");
	print(4, 4);
	sprintf(ssaa,"|\n");
	sprintf(ssaa,"%s%s",b,a);
	sprintf(ssaa,"Score:%d\nPress 'H' to find help.\n",score);
	clrscr();
	puts(aa);
	fflag=false;
}

#undef ssaa

inline void Left()
{
	memset(b1, 0, sizeof b1);
	memset(used, 0, sizeof used);
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			if (num[i][j])
			{
				int nx = j;
				while (nx != 1 && (!b1[i][nx - 1])) --nx;
				if (nx == 1)
					b1[i][nx] = num[i][j];
				else
				{
					bool flag = 0;
					if (b1[i][nx - 1] == num[i][j])
						if (!used[i][nx - 1])
						{
							b1[i][nx - 1] <<= 1;
							score += b1[i][nx - 1];
							used[i][nx - 1] = 1;
							flag = 1;
						}
					if (!flag)
						b1[i][nx] = num[i][j];
				}
			}
	swap(num, b1);
}

inline void Right()
{
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 2; j++)
			swap(num[i][j], num[i][4 - j + 1]);
	Left();
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 2; j++)
			swap(num[i][j], num[i][4 - j + 1]);
}

void Down()
{
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			b1[j][4 - i + 1] = num[i][j];
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			num[i][j] = b1[i][j];
	Left();
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			b1[j][4 - i + 1] = num[i][j];
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			num[i][j] = b1[i][j];
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			b1[j][4 - i + 1] = num[i][j];
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			num[i][j] = b1[i][j];
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			b1[j][4 - i + 1] = num[i][j];
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			num[i][j] = b1[i][j];
}

void Up()
{
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			b1[j][4 - i + 1] = num[i][j];
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			num[i][j] = b1[i][j];

	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			b1[j][4 - i + 1] = num[i][j];
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			num[i][j] = b1[i][j];
	Down();
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			b1[j][4 - i + 1] = num[i][j];
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			num[i][j] = b1[i][j];
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			b1[j][4 - i + 1] = num[i][j];
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			num[i][j] = b1[i][j];
}

bool pd()
{
	int ls[5][5];
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			if (num[i][j] == 0)
				return 1;
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			ls[i][j] = num[i][j];
	Left();
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			if (ls[i][j] != num[i][j])
				return 1;
	Right();
	Right();
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			if (ls[i][j] != num[i][j])
				return 1;
	Left();
	Up();
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			if (ls[i][j] != num[i][j])
				return 1;
	Down();
	Down();
	for (int i = 1; i <= 4; i++)
		for (int j = 1; j <= 4; j++)
			if (ls[i][j] != num[i][j])
				return 1;
	Up();
	return 0;
}

void play();

int main();

void choice()
{
	clrscr();
	if (MessageBox(NULL, "Restart?", "question", MB_YESNO) == IDYES)
	{
		beginning();
		play();
	}
	else
		exit(0);
}


void play()
{
	out();
	for (;;)
	{
		char way = in();
		for (int i = 1; i <= 4; i++)
			for (int j = 1; j <= 4; j++)
				last_move[i][j] = num[i][j];
		switch (way)
		{
			case 'a':
				//case 'A':
			case 75:
				Left();
				break;
			case 'd':
				//case 'D':
			case 77:
				Right();
				break;
			case 's':
				//case 'S':
			case 80:
				Down();
				break;
			case 'w':
				//case 'W':
			case 72:
				Up();
				break;
			case 'q':
				//case 'Q':
				save();
				continue;
			case 'o':
				//case 'O':
				if (MessageBox(NULL, "Are you sure?", "WARNING", MB_YESNO) == IDYES)
				{
					if (compare())
						if (MessageBox(NULL, "Do you want to save?", "WARNING", MB_YESNO) == IDYES)
							save();
					exit(0);
				}
				else continue;
			case 'b':
				clrscr();
				if (MessageBox(NULL, "Are you sure?", "WARNING", MB_YESNO) == IDYES)
				{
					main();
					exit(0);
				}
				else
				{
					out();
					continue;
				}
			case 'h':
				helper.help();
				continue;
			case 'c':
				color();
				continue;
			default:
				if(fflag==false)
				{
					fflag=true;
					cerr << "Sorry, I don't seem to understand. You can press 'H' to find help." << '\n';
				}
				continue;
		}
		bool full = 1, usable = 0;
		for (int i = 1; i <= 4; i++)
			for (int j = 1; j <= 4; j++)
				if (last_move[i][j] != num[i][j])
					usable = 1;
		if (!usable)
		{
			out();
			cerr << "\nInvalid operation\n";
		}
		else
		{
			for (int i = 1; i <= 4; i++)
				for (int j = 1; j <= 4; j++)
					if (num[i][j])
					{
						full = 0;
						break;
					}
			if (!full)
			{
				for(;;)
				{
					X = (rand() % 4) + 1;
					Y = (rand() % 4) + 1;
					if (!num[X][Y])
					{
						num[X][Y] = neww[rand() % 4];
						break;
					}
				}
			}
			out();
		}
		if (!pd_ex())
		{
			int maxx = 0;
			for (int i = 1; i <= 4; i++)
				for (int j = 1; j <= 4; j++)
				{
					maxx = max(maxx, num[i][j]);
				}
			if (maxx >= 2048)
			{
				cout << "Game Over\nYour Biggest Number:" << maxx << "\n";
			}
			else
				MessageBox(NULL, "You lose!", "2048", MB_OK);
			choice();
		}
	}
}

int main()
{
	hidecursor();
	MessageBox(NULL, "Welcome to 2048 games.", "WELCOME", MB_OK);
	MessageBox(NULL, "�������Ϸʱ���벻Ҫ����д������", "helper", MB_OK);
	if (MessageBox(NULL, "Load Game?", "question", MB_YESNO) == IDYES) load();
	else beginning();
	char aaa[20];
	freopen("color.bak","r",stdin);
	gets(aaa);
	fclose(stdin);
	system(aaa);
	clrscr();
	play();
	return 0;
}
