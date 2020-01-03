//GNU General Public License v3.0
//The Gallows - Version 1.0 - December 31st, 2019 - DBT
//More Additions to come in due time.

#include <ctime>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <windows.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <stdexcept>

void Color(uint8_t, uint8_t);
void ClearConsoleToEndOfLine();
void ClearScreen();
void EnhanceHiddenMessage(std::string);

const char BlankFiller = '_';

namespace Gallows
{
    char ConsoleTitle[] = "JesseAI - Ver 1.88";
    COORD CursorCoords = {0, 0};
    COORD NewScreenBufferDimensions = {81, 6000}; // Console Buffer Width, Height
    SMALL_RECT NewsrWindow = {0, 0, 81, 40};      // New Console Window StartX, StartY, EndX, EndY
    COORD MaximumWindowSize = {81, 40};           // Console Window Maximum Width, Height

    CONST UINT8 Palette1 = 0;
    UINT8 CurrentColorPalette = 1;

    UINT8 PrimaryWinColor[] = {0x2E, 0xF0, 0xF0};
    UINT8 CurrentWinColor;

    COLORREF ColorTable[1][16] =
    {
        RGB(   0,  16,   0),
        RGB(   0,  32,   0),
        RGB(   0,  48,   0),
        RGB(   0,  64,   0),
        RGB(   0,  80,   0),
        RGB(   0,  96,   0),
        RGB(   0, 112,   0),
        RGB(   0, 128,   0),
        RGB(   0, 144,   0),
        RGB(   0, 160,   0),
        RGB(   0, 176,   0),
        RGB(   0, 192,   0),
        RGB(   0, 208,   0),
        RGB(   0, 224,   0),
        RGB(   0, 240,   0),
        RGB( 255, 255, 255),

    };

}

std::string randomWord(){                              //Word Bank
std::string words[] = {

        "Alabama",
        "Montgomery",
        "Montana",
        "Helena",
        "Alaska",
        "Juneau",
        "Nebraska",
        "Lincoln",
        "Arizona",
        "Phoenix",
        "Nevada",
        "Carson City",
        "Arkansas",
        "Little Rock",
        "New Hampshire",
        "Concord",
        "California",
        "Sacramento",
        "New Jersey",
        "Trenton",
        "Colorado",
        "Denver",
        "New Mexico",
        "Santa Fe",
        "Connecticut",
        "Hartford",
        "New York",
        "Albany",
        "Delaware",
        "Dover",
        "North Carolina",
        "Raleigh",
        "Florida",
        "Tallahassee",
        "North Dakota",
        "Bismarck",
        "Georgia",
        "Atlanta",
        "Ohio",
        "Columbus",
        "Hawaii",
        "Honolulu",
        "Oklahoma",
        "Oklahoma City",
        "Idaho",
        "Boise",
        "Oregon",
        "Salem",
        "Illinois",
        "Springfield",
        "Pennsylvania",
        "Harrisburg",
        "Indiana",
        "Indianapolis",
        "Rhode Island",
        "Providence",
        "Iowa",
        "Des Moines",
        "South Carolina",
        "Columbia",
        "Kansas",
        "Topeka",
        "South Dakota",
        "Pierre",
        "Kentucky",
        "Frankfort",
        "Tennessee",
        "Nashville",
        "Louisiana",
        "Baton Rouge",
        "Texas",
        "Austin",
        "Maine",
        "Augusta",
        "Utah",
        "Salt Lake City",
        "Maryland",
        "Annapolis",
        "Vermont",
        "Montpelier",
        "Massachusetts",
        "Boston",
        "Virginia",
        "Richmond",
        "Michigan",
        "Lansing",
        "Washington",
        "Olympia",
        "Minnesota",
        "St. Paul",
        "West Virginia",
        "Charleston",
        "Mississippi",
        "Jackson",
        "Wisconsin",
        "Madison",
        "Missouri",
        "Jefferson City",
        "Wyoming",
        "Cheyenne"

	};

	return words[rand() % (*(&words + 1) - words)];
}

int letterFill(char guess, std::string secretword, std::string &guessword){

    int matches=0;

    for (int i = 0; i < (int)secretword.length(); i++)
    {

        if (guess == guessword[i])
        {
			matches--;
		} else if (guess == secretword[i]) {
            guessword[i] = guess;
            matches++;
        }
    }
    return matches;
}

int main(){                                                             //Start

	srand (time(NULL));

	const int Win{1};
	const int StillPlaying{0};
	const int Lose{-1};

	int tries;                                                       //Player tries
	int WinOrLose;
	int LetterMatches;
	bool Replay{true};

	char letter;                                                  //Letter Input
	std::string word;
	std::string HiddenMessage;

	do
	{
		tries = 5;
		WinOrLose = StillPlaying;
		word = randomWord();

		HiddenMessage.clear();
		HiddenMessage.resize (word.length(), BlankFiller);

		transform(word.begin(), word.end(), word.begin(), ::toupper);

        Color(0, 2);

        std::cout << "===============================The Gallows Ver. 1.0===================================\n";
		std::cout << "/   December 31st 1877                                                                \n";
		std::cout << "/                                                                                     \n";
		std::cout << "/   You have been tried for Cattle theft and Murder. It's the day of your execution   \n";               //Intro Line
		std::cout << "/   and you're hoping for a miracle...anything to get you out of being hanged. You    \n";
		std::cout << "/   never thought for a moment about pulling the trigger but here you are. Now your   \n";
		std::cout << "/   here. Suddenly, a cloaked figure offers you one last chance for freedom and save  \n";
		std::cout << "/   yourself from the hangman's noose.                                                \n";
		std::cout << "/                                                                                     \n";
		std::cout << "/   Your word has been chosen. The word is " << word.length() << " letters long!\n";             //Length of the word

		while (WinOrLose == StillPlaying) {

			std::cout << "\n";
			EnhanceHiddenMessage(HiddenMessage);
			std::cout << "\n\n";
			Color(0, 2);

			std::cout << "=Which letter would you like to choose this time? ";
			std::cin >> letter;

			letter = toupper(letter);

			std::cout << "\n";

			LetterMatches = letterFill(letter, word, HiddenMessage);

			if(LetterMatches == 0)
			{
				std::cout << "=Could not find " << letter << " in the word.\n\n";
				tries--;

				std::cout << "=Guess a letter. You have " << tries << " attempt";
				if(tries>1) std::cout << "s";
				std::cout << " left.\n\n";

			} else if(LetterMatches > 0) {

				std::cout << "=Yes!!! There ";

				if(LetterMatches == 1)
				{
					std::cout << "is ";
				} else {
					std::cout << "are ";
				}

				std::cout << LetterMatches << " " << letter;

				if(LetterMatches > 1) std::cout << "'s";

				std::cout << " in your life saving word!\n\n";
			} else if(LetterMatches < 0) {
				std::cout << "=You have already guessed the letter " << letter << ".\n=Please try again!\n\n";
			}

			if (word == HiddenMessage) WinOrLose = Win;
			if (tries < 1) WinOrLose = Lose;
		}

		if(WinOrLose == Win)
		{
			std::cout << "\n\n=Huzzah! You win Walter!\n\n";     //When player wins
			EnhanceHiddenMessage(word);
			Color(0, 2);
			std::cout << " was the word... Was the word...\n=It has groove and it has meaning...\n=It's means you are free to leave the gallows. Don't come back!\n";
		} else {
			std::cout << "\n\n=Sorry, Walter! You lost and were hanged...pity\n";       //When player dies
			std::cout << "=The word to your freedom was: ";
			EnhanceHiddenMessage(word);
			Color(0,2);
			std::cout << "\n";
		}

		do
		{
			std::cout << "\n\n=Would you like to play again (Y / N)?: ";
			std::cin >> letter;

			letter = toupper(letter);

			if(letter == 'N') Replay = false;

		} while ((letter != 'Y') && (letter != 'N'));

		std::cout << "\n\n\n";

	} while(Replay == true);

	std::cout << "=Thank you for playing!\n=Don't come back you hear!!!\n\n";

	return 0;
}

void Color(uint8_t ForegroundColor, uint8_t BackgroundColor)
{
    HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );

    SetConsoleTextAttribute(hstdout, ((ForegroundColor & 0x0F) << 4) | (BackgroundColor & 0x0F));
}

void EnhanceHiddenMessage(std::string HiddenMessage)
{

    for(unsigned int Iterator1 = 0; Iterator1 < HiddenMessage.length(); Iterator1++)
    {
        HiddenMessage[Iterator1] == BlankFiller ? Color(0, 2) : Color(0, 10);
        std::cout << HiddenMessage[Iterator1];
    }

    std::cout << "\n";
}
