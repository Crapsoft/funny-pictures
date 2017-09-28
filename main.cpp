#pragma warning(disable:4996)

#include <iostream>
#include <windows.h>
#include "bitmap_class.h"
#include <string>

using namespace std;

// function to obtain red data from a COLORREF value
int Red(COLORREF color)
{
	return (color & 0x000000ff);
}

// and green
int Green(COLORREF color)
{
	return (color & 0x0000ff00) >> 8;
}

// and also blue
int Blue(COLORREF color)
{
	return (color & 0x00ff0000) >> 16;
}

// and alpha if you're froggy
int Alpha(COLORREF color)
{
	return (color & 0xff000000) >> 24;
}

int main()
{
	string filename;
	cout << "Please enter the path to your bmp: \n";
	cin >> filename;
	char* fileq = strdup(filename.c_str());
	cBitmap test(fileq, 0, 0, 0);
	if (test.GetHeight() == NULL) {
		cout << endl;
		free(fileq);
		system("pause");
		return 0;
	}

	cout << "Please choose your work mode: \n";
	cout << "1 - Cipher the sercet word into the picture \n";
	cout << "2 - Read the word from the picture \n";
	int sw;
	cin >> sw;

	string password;
	COLORREF color(0);
	int redd;
	char* word;

	switch (sw) {
	case 1:
		cout << "Please enter your secret word: \n";
		cin >> password;
		word = strdup(password.c_str());

		int binaryWord[8][8];

		//--------------------------------------------------------------------------------------------------------

		for (int i = 0; i < 8; i++) {
			int temp = (int)word[i];
			int j = 7;
			while (temp != 0)
			{
				binaryWord[i][j--] = temp % 2;
				temp /= 2;
			}
			binaryWord[i][0] = 0;
			//for (int k = 0; k < 4; k++) {
			//	int ss = binaryWord[i][k];
			//	binaryWord[i][k] = binaryWord[i][7 - k];
			//	binaryWord[i][7 - k] = ss;
			//}
		}

		//--------------------------------------------------------------------------------------------------------

		
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				color = test.GetPixel(i, j);
				redd = Red(color);
				if (binaryWord[i][j] != redd % 2) {
					redd++;
					test.PutPixel(i, j, redd,
						Green(color),
						Blue(color),
						Alpha(color));
				}
			}
		}

		//-------------------------------------------------------------------------------------------------------

		filename.erase(filename.end() - 4, filename.end());
		filename.append("_coded.bmp");
		fileq = strdup(filename.c_str());
		test.SaveBitmap(fileq);
		cout << "************************************************\n";
		cout << "*           CONGRATULATIONS, YOU WON           *\n";
		cout << "************************************************\n";

		//-------------------------------------------------------------------------------------------------------

		free(fileq);
		free(word);
		break;

	case 2:
		//cout << "Please enter your secret word: \n";
		//cin >> password;
		//char* word = strdup(password.c_str());

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				color = test.GetPixel(i, j);
				if (Red(color) % 2 == 0) {
					binaryWord[i][j] = 0;
				}
				else {
					binaryWord[i][j] = 1;
				}
			}
		}

		int temp, mnoj;

		char output[8];

		for (int i = 0; i < 8; i++) {
			temp = 0;
			mnoj = 1;
			for (int j = 7; j >= 0; j--) {
				temp += binaryWord[i][j] * mnoj;
				mnoj *= 2;
			}
			output[i] = char(temp);								// temp == char
		}

		cout << "Here's to you: \n";
		for (int i = 0; i < 8; i++)
			cout << output[i];
		break;
	default:
		cout << "Please choose a legit option \n";
		break;
	}


	system("pause");

	return 0;
}