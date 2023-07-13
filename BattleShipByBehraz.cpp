#include <iostream>
#include <chrono>
#include <time.h>
#include <Windows.h>
#include <fstream>
#include <conio.h>
#include <vector>
#include <string>
#include <future> 

using namespace std;

class Clock {//clock !!!
public:
	int seconds, minutes, hours;
	chrono::system_clock::time_point start = chrono::system_clock::now();
	void setTheBaseOfTime() {
		start = chrono::system_clock::now();
	}
	void setCurrentTime() {
		chrono::system_clock::time_point end = chrono::system_clock::now();
		chrono::duration<double> pastSeconds = (end - start);
		int time = (int)pastSeconds.count();
		seconds = time % 60;
		minutes = (time % 3600) / 60;
		hours = time / 3600;
	}
	void show() {
		cout << ((hours < 10) ? "0" : "") << hours << ":" << ((minutes < 10) ? "0" : "") << minutes << ":" << ((seconds < 10) ? "0" : "") << seconds;
	}
	void wait(int seconds) {//just waiting for some time
		time_t start = time(NULL), finish;
		do {
			finish = time(NULL);
		} while (difftime(finish, start) < seconds);
	}
};

//--------------- globals
bool printTime = 0, threadIsOver = false;
COORD xsys = { 0,0 }, xfyf = { 0,0 };
Clock cl;
//---------------set start and end time points for clock(live)
void setStartingPoint() {
	HANDLE hh = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO mycbsi;
	GetConsoleScreenBufferInfo(hh, &mycbsi);
	xsys = mycbsi.dwCursorPosition;
}

void setEndingPoint() {
	HANDLE hh = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO mycbsi;
	GetConsoleScreenBufferInfo(hh, &mycbsi);
	xfyf = mycbsi.dwCursorPosition;
}
//---------------
class Score {//score and its calculation
public:
	class SecondPlayer {
	public:
		double successful, failed, remained, size;
		void set(int n) {
			successful = 0;
			failed = 0;
			remained = n;
			size = n;
		}
		double calculateScore() {
			return (successful + remained + ((double)size / (failed + 1)));
		}
	};
	class FirstPlayer {
	public:
		double successful, failed, remained, size;
		string username;
		void set(int n, string uname) {
			successful = 0;
			failed = 0;
			remained = n;
			size = n;
			username = uname;
		}
		double calculateScore() {
			return (successful + remained + ((double)size / (failed + 1)));
		}
	};

};

class Board {//board and its print , reload , state
public:
	int size = 0;
	vector<vector<char>> board;
	vector<vector<int>> StateOfboard;
	void set(int n) {
		size = n;
		for (int i = 0; i < size; i++) {
			vector<char> temp;
			vector<int> temp2;
			board.push_back(temp);
			StateOfboard.push_back(temp2);
			for (int j = 0; j < size - 1; j++) {
				board[i].push_back((char)(254));
				StateOfboard[i].push_back(0);
			}
		}
	}
	void print() {
		//top top
		cout << "   " << (char)(201);
		for (int i = 0; i <= 3 * size + 1; i++) {
			if (i == ((3 * size) + 1) / 2) {
				cout << (char)(209);
			}
			else {
				cout << (char)(205);
			}
		}
		cout << (char)(187);
		cout << endl;
		//top text
		cout << "   " << (char)(186) << "  ";
		for (int i = 0; i < size - 1; i++) {
			if (i == size / 2) {
				cout << (char)(179) << "  ";
			}
			if (i == 0) {
				cout << "You";
			}
			else if (i == size / 2) {
				cout << "Bot";
			}
			else {
				cout << "   ";
			}
		}
		cout << (char)(186);
		cout << endl;
		//top index
		cout << "   " << (char)(186) << "  ";
		for (int i = 0; i < size - 1; i++) {
			if (i == size / 2) {
				cout << (char)(179) << "  ";
			}
			cout << i << ((i < 10) ? "  " : " ");
		}
		cout << (char)(186);
		cout << endl;
		//top bottem
		cout << (char)(201) << (char)(205) << (char)(205) << (char)(206);
		for (int i = 0; i <= 3 * size + 1; i++) {
			if (i == ((3 * size) + 1) / 2) {
				cout << (char)(216);
			}
			else {
				cout << (char)(205);
			}
		}
		cout << (char)(185);
		cout << endl;
		//chart
		for (int i = 0; i < size; i++) {
			cout << (char)(186) << ((i < 10) ? "0" : "") << i << (char)(186) << "  ";
			for (int j = 0; j < size - 1; j++) {
				if (j == size / 2) {
					cout << (char)(179) << "  ";
				}
				cout << board[i][j] << "  ";
			}
			cout << (char)(186);
			cout << endl;
		}
		//bottom
		cout << (char)(200) << (char)(205) << (char)(205) << (char)(202);
		for (int i = 0; i <= 3 * size + 1; i++) {
			if (i == ((3 * size) + 1) / 2) {
				cout << (char)(207);
			}
			else {
				cout << (char)(205);
			}
		}
		cout << (char)(188);
		cout << endl;
	}
	void reload() {
		COORD x0y0 = { 0,0 }, xy = { 0,0 };
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO cbsi;
		GetConsoleScreenBufferInfo(h, &cbsi);
		xy = cbsi.dwCursorPosition;
		SetConsoleCursorPosition(h, x0y0);
		print();
		SetConsoleCursorPosition(h, xy);
	}
};

class SaveAndLoad {//seve and load of game
public:
	string fileName = "save.txt";
	int size = 0;

	SaveAndLoad(string fname) {
		fileName = fname;
		ifstream ifs;
		ifs.open(fileName);
		int data;
		ifs >> data;
		ifs.close();
		if (data != 1) {
			ofstream ofs;
			ofs.open(fileName);
			ofs << 0 << endl;
			ofs.close();
		}
	}
	bool isSaved() {
		ifstream ifs;
		ifs.open(fileName);
		int data;
		ifs >> data;
		ifs.close();
		return data;
	}
	void loadSize() {
		ifstream ifs;
		ifs.open(fileName);
		int data;
		ifs >> data;
		ifs >> data;
		size = data;
		ifs.close();
	}
	void saveNow(Board b, Score::FirstPlayer fp, Score::SecondPlayer sp) {
		ofstream ofs;
		ofs.open(fileName);
		ofs << 1 << " ";
		ofs << b.size << endl;
		for (int i = 0; i < b.size; i++) {
			for (int j = 0; j < b.size - 1; j++) {
				ofs << b.board[i][j] << " ";
			}
			ofs << endl;
		}
		for (int i = 0; i < b.size; i++) {
			for (int j = 0; j < b.size - 1; j++) {
				ofs << b.StateOfboard[i][j] << " ";
			}
			ofs << endl;
		}
		ofs << fp.successful << " " << fp.failed << " " << fp.remained << " ";
		ofs << sp.successful << " " << sp.failed << " " << sp.remained << " ";
		ofs << fp.username;
		ofs.close();
	}
	void loadNow(Board& b, Score::FirstPlayer& fp, Score::SecondPlayer& sp) {
		ifstream ifs;
		ifs.open(fileName);
		int data;
		ifs >> data;
		ifs >> data;
		char data2;
		for (int i = 0; i < b.size; i++) {
			for (int j = 0; j < b.size - 1; j++) {
				ifs >> data2;
				b.board[i][j] = data2;
			}
		}
		for (int i = 0; i < b.size; i++) {
			for (int j = 0; j < b.size - 1; j++) {
				ifs >> data;
				b.StateOfboard[i][j] = data;
			}
		}
		ifs >> fp.successful >> fp.failed >> fp.remained;
		ifs >> sp.successful >> sp.failed >> sp.remained;
		ifs >> fp.username;
		sp.size = size;
		fp.size = size;
		ifs.close();
	}
};

class ClearScrean {//clearing the screen
public:
	COORD x1y1 = { 0,0 }, x2y2 = { 0,0 }, x0y0 = { 0,0 };
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	void setClearStartingPoint() {
		GetConsoleScreenBufferInfo(h, &cbsi);
		x1y1 = cbsi.dwCursorPosition;
	}
	void setClearEndingPoint() {
		GetConsoleScreenBufferInfo(h, &cbsi);
		x2y2 = cbsi.dwCursorPosition;
	}
	void clear() {
		SetConsoleCursorPosition(h, x1y1);
		for (int i = 0; i < (x2y2.Y - x1y1.Y); i++) {
			cout << "                                                                                                                           "
				<< "                                                                                                                             ";
		}
		SetConsoleCursorPosition(h, x1y1);
	}
	void resetScrean() {
		setClearEndingPoint();
		SetConsoleCursorPosition(h, x0y0);
		setClearStartingPoint();
		clear();
	}
};

class Menu {//menu and gamemessages(win or lose or tie)
public:
	string spaces = "\t\t\t\t\t   ";
	char show() {
		char choice;
		ClearScrean myclear;
		do {
			myclear.setClearStartingPoint();
			COORD x0y0 = { 0,0 }, xy = { 0,0 };
			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_SCREEN_BUFFER_INFO cbsi;
			GetConsoleScreenBufferInfo(h, &cbsi);
			xy = cbsi.dwCursorPosition;
			cout << "\n\n\n\n\n";
			cout << spaces << "    " << (char)(174) << " Battle Ship " << (char)(175);
			cout << "\n\n";
			cout << spaces <<(char)(254) << "  [p] : Play BattleShip" << endl;
			cout << "\n";
			cout << spaces << (char)(254) << "  [r] : Resume last seved game" << endl;
			cout << "\n";
			cout << spaces << (char)(254) << "  [s] : Show scoreboard" << endl;
			cout << "\n";
			cout << spaces << (char)(254) << "  [e] : Exit" << endl;
			cout << "\n";
			cout << spaces;
			choice = _getch();
			myclear.setClearEndingPoint();
			myclear.clear();
			SetConsoleCursorPosition(h, x0y0);

		} while (choice != 'e' && choice != 'p' && choice != 'r' && choice != 's');
		return choice;
	}
	void gameMessage(string message) {
		cout << "\n\n\n\n\n";
		cout << spaces << (char)(174) <<" " << message <<" " << (char)(175)<< endl;
		cout << endl;
		cout << spaces<<"Press any key to return to menu";
		_getch();
	}
};

class Bot {//bot moves
public:
	int x, y, size = 0;
	void set(int n) {
		size = n;
	}
	void botSetsX() {
		srand(time(NULL));
		x = (size / 2) + rand() % (size / 2);
	}
	void botSetsY() {
		srand(time(NULL));
		y = rand() % size;
	}
	void botAttacks() {
		srand(time(NULL));
		x = rand() % (size / 2);
		srand(time(NULL));
		y = rand() % size;
	}
};

void resetGame(Board& b, Score::FirstPlayer& fp, Score::SecondPlayer& sp) {//reseting the board and scores
	b.set(3);
	fp.set(0,"");
	sp.set(0);
}

class ScoreBoard {//score board save and load and print
public:
	string fileName = "savescore.txt";
	int listSize = 0;
	vector<pair<string, double>> list;

	ScoreBoard(string fname) {
		fileName = fname;
		ifstream ifs;
		ifs.open(fileName);
		int data;
		ifs >> data;
		ifs.close();
		if (data != 1) {
			ofstream ofs;
			ofs.open(fileName);
			ofs << 0 << endl;
			ofs.close();
		}
	}
	bool isSaved() {
		ifstream ifs;
		ifs.open(fileName);
		int data;
		ifs >> data;
		ifs.close();
		return data;
	}
	void loadSize() {
		ifstream ifs;
		ifs.open(fileName);
		int data;
		ifs >> data;
		ifs >> data;
		listSize = data;
		ifs.close();
	}
	void loadNow() {
		ifstream ifs;
		ifs.open(fileName);
		double data;
		string data2;
		ifs >> data;
		ifs >> data;
		for (int i = 0; i < listSize; i++) {
			ifs >> data2 >> data;
			list.push_back(make_pair(data2, (double)data));
		}
		ifs.close();
	}
	void saveNow() {
		ofstream ofs;
		ofs.open(fileName);
		ofs << 1 << " ";
		ofs << list.size() << endl;
		for (int i = 0; i < list.size(); i++) {
			ofs << list[i].first << " " << (double)list[i].second << endl;
		}
		ofs.close();
	}
	void show() {	
		cout << "\n   " << (char)(201) << (char)(205) << " " << "Score Board : " << endl;
		cout << "   " << (char)(186) << endl;
		for (int i = 0; i < list.size(); i++) {
			cout << "   "<<(char)(199) <<" " << list[i].first << " : " << (double)list[i].second << endl;
			cout << "   " << (char)(186) << endl;
		}
		cout << "   " << (char)(200)<<(char)(205)<<" ";
	}
	void update(string uname, double score) {
		bool isInTheList = false;
		for (int i = 0; i < list.size(); i++) {
			if (list[i].first == uname) {
				if ((double)score > (double)list[i].second) {
					list[i].second = (double)score;
				}
				else {
					list[i].second = (double)list[i].second;
				}
				isInTheList = true;
				break;
			}
		}
		if (!isInTheList) {
			list.push_back(make_pair(uname, (double)score));
		}
	}
	void clearList() {
		list.clear();
		listSize = 0;
	}
};

int myGame() {//game
	bool exited = false;
	ClearScrean myClear[8];//to clear diffrent parts of screen
	Menu myMenu;
	Score::FirstPlayer myP1Score;
	Score::SecondPlayer myP2Score;
	ScoreBoard myScoreBoard("savescore.txt");
	while (!exited) {//menu loop
		printTime = 0;
		if (myScoreBoard.isSaved()) {
			myScoreBoard.clearList();
			myScoreBoard.loadSize();
			myScoreBoard.loadNow();
		}
		char choice = myMenu.show();
		cl.setTheBaseOfTime();
		if (choice == 'p' || choice == 'r') {
			SaveAndLoad mySAL("save.txt");
			Board myBoard;
			Clock myClock;
			myClock.setTheBaseOfTime();
			Bot myBot;

			const int timeToWait = 1;
			bool isSaved = mySAL.isSaved();
			if (choice == 'r' && !isSaved) {
				choice = 'p';
			}
			myClear[0].setClearStartingPoint();
			int n;
			string username;
			if (choice == 'p') {
				myClear[7].setClearStartingPoint();
				cout << "Enter your username : ";
				cin >> username;
				myClear[7].setClearEndingPoint();
				myClear[7].clear();
				do {
					myClear[7].setClearStartingPoint();
					cout << "Enter the game board size : ";
					cin >> n;
					myClear[7].setClearEndingPoint();
					myClear[7].clear();
				} while (n < 3);
			}
			else {
				mySAL.loadSize();
				n = mySAL.size;
			}
			myClear[0].setClearEndingPoint();
			myClear[0].clear();
			myBoard.set(n);
			myBoard.print();
			myBot.set(n);
			setStartingPoint();
			if (choice == 'p') {
				for (int i = 0; i < n;) {//set loop
					myClear[1].setClearStartingPoint();
					printTime = 1;
					cout << endl;
					cout << "Enter your ship's position(y,x) : ";
					int x, y;
					cin >> y >> x;
					bool InRightPlace = (y >= 0 && y <= n - 1) && (x >= 0 && x < n / 2);
					if (!InRightPlace || myBoard.StateOfboard[y][x]) {
						cout << "Invalid input";
						myClock.wait(timeToWait);
					}
					else {
						myBoard.StateOfboard[y][x] = 1;
						myBoard.board[y][x] = (char)(23);
						i++;
						do {
							myBot.botSetsX();
							myBot.botSetsY();
						} while (myBoard.StateOfboard[myBot.y][myBot.x]);
						myBoard.StateOfboard[myBot.y][myBot.x] = -1;
						//---------------------------------------------for debug
						//myBoard.board[myBot.y][myBot.x] = (char)(23);
						//---------------------------------------------for debug
					}
					printTime = 0;
					this_thread::sleep_for(chrono::milliseconds(1200));
					myClear[1].setClearEndingPoint();
					myClear[1].clear();
					myBoard.reload();
				}
				myP1Score.set(n, username);
				myP2Score.set(n);
			}
			else {
				mySAL.loadNow(myBoard, myP1Score, myP2Score);
				myBoard.reload();
			}
			bool gameOver = false;
			bool autoSave = false;
			bool dontSave = false;
			while (!gameOver) {// game loop
				myClear[2].setClearStartingPoint();
				printTime = 1;
				cout << "\t     Your score : " << myP1Score.calculateScore() << endl;
				bool alreadyAttacked = false;
				cout << "Enter your attack position(y,x) : ";
				int x, y;
				cin >> y >> x;
				bool InRightPlace = (y >= 0 && y <= n - 1) && (x >= n / 2 && x <= n - 2);
				if (InRightPlace) {
					alreadyAttacked = (myBoard.board[y][x] == '0' || myBoard.board[y][x] == '1');
				}
				if (!InRightPlace || alreadyAttacked) {
					cout << "Invalid input";
					myClock.wait(timeToWait);
				}
				else {
					if (myBoard.StateOfboard[y][x] == 0) {
						myP1Score.failed++;
						myBoard.board[y][x] = '0';
					}
					else {
						myBoard.board[y][x] = '1';
						myP1Score.successful++;
						myP2Score.remained--;
					}
					do {
						myBot.botAttacks();
						alreadyAttacked = (myBoard.board[myBot.y][myBot.x] == '0' || myBoard.board[myBot.y][myBot.x] == '1');
					} while (alreadyAttacked);

					if (myBoard.StateOfboard[myBot.y][myBot.x] == 0) {
						myP2Score.failed++;
						myBoard.board[myBot.y][myBot.x] = '0';
					}
					else {
						myBoard.board[myBot.y][myBot.x] = '1';
						myP2Score.successful++;
						myP1Score.remained--;
					}
				}
				printTime = 0;
				this_thread::sleep_for(chrono::milliseconds(1200));
				myClear[2].setClearEndingPoint();
				myClear[2].clear();
				myBoard.reload();
				printTime = 1;
				if (myP1Score.remained == 0 || myP2Score.remained == 0) {
					gameOver = true;
					break;
				}
				if (!dontSave) {
					printTime = 0;
					this_thread::sleep_for(chrono::milliseconds(1200));
					myClear[3].setClearStartingPoint();
					cout << " [y]:yes \n [n]:no \n [a]:auto save \n [d]:don't save at all(always no)\nDo you want to save and quit? : ";
					char quitOrnotOptions;
					cin >> quitOrnotOptions;
					myBoard.reload();
					if (quitOrnotOptions == 'y') {
						mySAL.saveNow(myBoard, myP1Score, myP2Score);
						break;
					}
					else if (quitOrnotOptions == 'a') {
						autoSave = true;
						dontSave = true;
					}
					else if (quitOrnotOptions == 'd') {
						dontSave = true;
					}
					myClear[3].setClearEndingPoint();
					myClear[3].clear();
				}
				if (autoSave) {
					mySAL.saveNow(myBoard, myP1Score, myP2Score);
				}
			}
			printTime = 0;
			this_thread::sleep_for(chrono::milliseconds(1200));
			myClear[5].resetScrean();
			myScoreBoard.update(myP1Score.username, (double)myP1Score.calculateScore());
			myScoreBoard.saveNow();
			string myMessage;//game message creation
			if (myP1Score.remained == 0 && myP2Score.remained == 0) {
				myMessage = ((string)("The game was tied ") + (char)(175) + (string)("\n\n\t\t\t\t\t   ") + ((char)(174)) + (string)(" Your score : ") + to_string(myP1Score.calculateScore()));
			}
			else if (myP2Score.remained == 0) {
				myMessage = (myP1Score.username + (string)(" was the winner ") + (char)(175) + (string)("\n\n\t\t\t\t\t   ") + (char)(174) + (string)(" Your score : ")  + to_string(myP1Score.calculateScore()));
			}
			else if (myP1Score.remained == 0) {
				myMessage = ((string)("Player two(bot) was the winner ") + (char)(175) + (string)("\n\n\t\t\t\t\t   ") + (char)(174) + (string)(" Your score : ") + to_string(myP1Score.calculateScore()));
			}
			else {
				myMessage = "Game saved successfully";
			}
			myMenu.gameMessage(myMessage);
			resetGame(myBoard, myP1Score, myP2Score);
		}
		else {//show score board 
			if (choice == 's') {
				myClear[6].setClearStartingPoint();
				if (myScoreBoard.isSaved()) {
					myScoreBoard.show();
				}
				else {
					cout << "Scoreboard is empty \n";
				}
				cout << "Press any key to return to menu";
				_getch();
				myClear[6].setClearEndingPoint();
				myClear[6].clear();
			}
			else {
				exited = true;
			}
		}
		myClear[4].resetScrean();
	}
	threadIsOver = true;
	return 0;
}

void showTime() {// live clock
	HANDLE hh = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO mycbsi;
	while (!threadIsOver) {
		if (printTime == 1) {
			this_thread::sleep_for(chrono::milliseconds(500));
			setEndingPoint();
			SetConsoleCursorPosition(hh, xsys);
			cl.setCurrentTime();
			cl.show();
			SetConsoleCursorPosition(hh, xfyf);
			this_thread::sleep_for(chrono::milliseconds(500));
		}
	}
}

int main() {// running showTime and myGame at the same time (two threads)
	auto fa = async(launch::async, showTime);
	auto fb = async(launch::async, myGame);
	return 0;
}
