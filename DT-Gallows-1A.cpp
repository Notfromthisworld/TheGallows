//GNU General Public License v3.0
//The Gallows - Version 1.0 - December 31st, 2019 - DBT
//More Additions to come in due time.

#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <windows.h>


void Color(uint8_t);
void Color(uint8_t, uint8_t);
void ClearConsoleToEndOfLine();
void ClearScreen();
void EnhanceHiddenMessage(std::string);
int letterFill(char, std::string, std::string &);
void PrintExecutionMessage();
std::string Get_File_Contents(const std::string);
std::vector<std::string_view> SplitStringToVector(std::string_view, std::string_view);

void ClearScreen();
void SetPalette(UINT8 PaletteNum);

HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );
CONSOLE_SCREEN_BUFFER_INFOEX csbiEX;

namespace Gallows
{
    const char BlankFiller = '_';

    char ConsoleTitle[] = "The Gallows - Ver 1.0";


    COORD CursorCoords = {0, 0};
    COORD NewScreenBufferDimensions = {87, 6000}; // Console Buffer Width, Height
    SMALL_RECT NewsrWindow = {0, 0, 81, 40};      // New Console Window StartX, StartY, EndX, EndY
    COORD MaximumWindowSize = {87, 40};           // Console Window Maximum Width, Height

    CONST UINT8 Palette1 = 0;

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
        RGB( 128, 255, 128)
    };
}



int main(){                                                             //Start

    const int Win{1};
    const int StillPlaying{0};
    const int Lose{-1};

	int tries;                                                       //Player tries
	int WinOrLose;
	int LetterMatches;
	bool Replay{true};
    //bool UserAlreadyGuessedThisCharacter;
    //long long unsigned int iteration;

	char letter;                                                  //Letter Input
	std::string word;
	std::string HiddenMessage;
	std::string WordsList;
	std::string WordsFile = "WordList2.txt";
	std::vector <std::string_view> Words;
    //std::vector <std::string> UserCharacterGuesses;

    SetConsoleTitle(Gallows::ConsoleTitle);

	srand (time(NULL));

	WordsList = Get_File_Contents(WordsFile);
	Words = SplitStringToVector(WordsList, "\r\n");

    SetPalette(Gallows::Palette1);

	do
	{
		tries = 6;
		WinOrLose = StillPlaying;

		word = Words[rand() % Words.size()];
		transform(word.begin(), word.end(), word.begin(), ::toupper);

		HiddenMessage.clear();
		HiddenMessage.resize (word.length(), Gallows::BlankFiller);


        Color(0, 8);

        std::cout << "============================== The Gallows Ver. 1.0 ==================================\n";
		std::cout << "|                                                                                    |\n";
		std::cout << "|  December 31st 1877                                                                |\n";
		std::cout << "|                                                                                    |\n";
		std::cout << "|  You have been tried for Cattle theft and Murder. It's the day of your execution   |\n";               //Intro Line
		std::cout << "|  and you're hoping for a miracle... Anything to get you out of being hanged. You   |\n";
		std::cout << "|  never thought for a moment about pulling the trigger but here you are. Now your   |\n";
		std::cout << "|  here. Suddenly, a cloaked figure offers you one last chance for freedom and save  |\n";
		std::cout << "|  yourself from the hangman's noose.                                                |\n";
		std::cout << "|                                                                                    |\n";
		std::cout << "======================================================================================\n\n";

		//std::cout << "Your word has been chosen. The word is " << word.length() << " letters long!\n\n";             //Length of the word
        std::cout << "|  My hat here contains " << Words.size() << " strips of magic paper. Each piece contains a hidden word.\n";
        std::cout << "|  Please place your hand in the hat and grab one piece of magic paper.\n\n";
        std::cout << "|  Your hand ruffles through the hat and you finally grab one.\n\n";
        std::cout << "|  While you can not read the word, you can tell it has " << word.length() << " characters in it.\n";

		while (WinOrLose == StillPlaying) {

			std::cout << "\n\n\n\tHidden Word: ";
			EnhanceHiddenMessage(HiddenMessage);
			std::cout << "\n\n";
			Color(0, 8);

			std::cout << "|  What letter, number, or character do you think is in the hidden word? ";
			std::cin >> letter;

			letter = toupper(letter);

			std::cout << "\n\n";

			LetterMatches = letterFill(letter, word, HiddenMessage);


			if(LetterMatches == 0)
			{
				Color(0, 14);
				std::cout << "*** THE CHARACTER \"" << letter << "\" DID NOT APPEAR WHEN YOU SPOKE IT. ***\n";
				tries--;

				std::cout << "*** YOU HAVE " << tries << " CHANCE";
				if(tries>1) std::cout << "S";
				std::cout << " LEFT. BE CAREFUL OR YOU WILL DIE! ***\n\n";
                Color(0, 6);

			} else if(LetterMatches > 0) {

				std::cout << "|  YOU SPOKE THE CHARACTER \"" << letter << "\" AND THERE ";

				if(LetterMatches == 1)
				{
					std::cout << "IS ";
				} else {
					std::cout << "ARE ";
				}

				std::cout << LetterMatches << " OF THEM IN YOUR LIFE SAVING WORD.\n\n";

			} else if(LetterMatches < 0) {
				Color(0, 14);
				std::cout << "*** THE CHARACTER \"" << letter << "\" ALREADY APPEARS IN YOUR WORD " << LetterMatches << "TIME";
				if(LetterMatches > 1) std::cout << "'S";
				std::cout << ". ***\n*** PLEASE TRY AGAIN! ***\n\n";
                Color(0, 6);
			}

			if (word == HiddenMessage) WinOrLose = Win;
			if (tries < 1) WinOrLose = Lose;
		}

		if(WinOrLose == Win)
		{
			std::cout << "\n\n|  YOU HAVE DONE WELL! NOW THAT YOU HAVE GUESSED MY SECRET WORD \"";     //When player wins
			EnhanceHiddenMessage(word);
			Color(0, 8);
			std::cout << "\",\n|  YOU SHALL LIVE ANOTHER DAY. YOU ARE FREE TO LEAVE THE GALLOWS THIS TIME, BUT YOU MAY\n|  NOT BE SO LUCKY IF YOU COME BACK AGAIN. NOW GO!\n\n";
		} else {

			PrintExecutionMessage();

			std::cout << "\n\n|  The word to your freedom was: ";
			EnhanceHiddenMessage(word);
			Color(0,8);
			std::cout << "\n";
		}

		do
		{
			std::cout << "\n\n|  Would you like to play again (Y / N)?: ";
			std::cin >> letter;

			letter = toupper(letter);

			if(letter == 'N') Replay = false;

		} while ((letter != 'Y') && (letter != 'N'));

		ClearScreen();

	} while(Replay == true);

	std::cout << "|  Thank you for playing!\n|  Don't come back now you hear!!!\n\n";

	return 0;
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



void Color(UINT8 Foreground)
{
    Gallows::CurrentWinColor = (Gallows::CurrentWinColor & 0xF0) | (Foreground & 0x0F);
    SetConsoleTextAttribute(hstdout, Gallows::CurrentWinColor);
}



void Color(uint8_t ForegroundColor, uint8_t BackgroundColor)
{
    HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );

    Gallows::CurrentWinColor = ((ForegroundColor & 0x0F) << 4) | (BackgroundColor & 0x0F);
    SetConsoleTextAttribute(hstdout, Gallows::CurrentWinColor);
}



void ClearScreen()
{
    DWORD cellCount;
    DWORD counter;

    cellCount = csbiEX.dwSize.X * csbiEX.dwSize.Y;

    FillConsoleOutputAttribute(hstdout, Gallows::CurrentWinColor, cellCount, Gallows::CursorCoords, &counter);
    FillConsoleOutputCharacter(hstdout, (TCHAR) ' ', cellCount, Gallows::CursorCoords, &counter);
    SetConsoleCursorPosition(hstdout, Gallows::CursorCoords );
}



void SetPalette(UINT8 PaletteNum)
{
    std::size_t i;

    csbiEX.cbSize = sizeof(csbiEX);
	GetConsoleScreenBufferInfoEx(hstdout, &csbiEX);

    for(i = 0; i < 16; i++)
    {
        csbiEX.ColorTable[i] = Gallows::ColorTable[PaletteNum][i];
    }

  	csbiEX.dwSize = Gallows::NewScreenBufferDimensions;
	csbiEX.srWindow = Gallows::NewsrWindow;
	csbiEX.dwMaximumWindowSize = Gallows::MaximumWindowSize;

    SetConsoleScreenBufferInfoEx(hstdout, &csbiEX);
}


void EnhanceHiddenMessage(std::string HiddenMessage)
{
    for(unsigned int Iterator1 = 0; Iterator1 < HiddenMessage.length(); Iterator1++)
    {
        HiddenMessage[Iterator1] == Gallows::BlankFiller ? Color(0, 4) : Color(0, 15);
        std::cout << HiddenMessage[Iterator1];
    }
}



std::string Get_File_Contents(const std::string FileName)
{
    std::ifstream CurrentFile(FileName, std::ios::in | std::ios::binary);

    if (CurrentFile)
    {
        std::string FileContents;

        CurrentFile.seekg(0, std::ios::end);

        FileContents.resize(CurrentFile.tellg());

        CurrentFile.seekg(0, std::ios::beg);
        CurrentFile.read(&FileContents[0], FileContents.size());
        CurrentFile.close();

        return(FileContents);
    } else {
        std::cout << "ERROR: Unable to load the dictionary file \"" << FileName << "\"!\n\n";
        exit(EXIT_FAILURE);
    }
}



std::vector<std::string_view> SplitStringToVector(std::string_view StringToConvert, std::string_view Delimeters = " ")
{
    std::vector<std::string_view> OutputVector;
    size_t FirstFind = 0;

    while (FirstFind < StringToConvert.size())
    {
        const auto SecondFind = StringToConvert.find_first_of(Delimeters, FirstFind);

        if (FirstFind != SecondFind) OutputVector.push_back(StringToConvert.substr(FirstFind, SecondFind - FirstFind));
        if (SecondFind == std::string_view::npos) break;

        FirstFind = SecondFind + 1;
    }

    return OutputVector;
}



void PrintExecutionMessage()
{
    std::cout << "\n\n|  Sorry, Walter! You lost and were hanged...pity\n\n";       //When player dies

    std::cout <<   "     -----------------------------------------------------------------\n";
    std::cout <<   "     |                                                               |\n";
    std::cout <<   "     |                                                               |\n";
    std::cout << R"(     |     __   __                  _    _                           |)" << "\n";
    std::cout << R"(     |     \ \ / /                 | |  | |                          |)" << "\n";
    std::cout << R"(     |      \ V /   ___   _   _    | |  | |  ___  _ __   ___         |)" << "\n";
    std::cout << R"(     |       \ /   / _ \ | | | |   | |/\| | / _ \| '__| / _ \        |)" << "\n";
    std::cout << R"(     |       | |  | (_) || |_| |   \  /\  /|  __/| |   |  __/        |)" << "\n";
    std::cout << R"(     |       \_/   \___/  \__,_|    \/  \/  \___||_|    \___|        |)" << "\n";
    std::cout <<   "     |                                                               |\n";
    std::cout << R"(     |      _____                           _              _  _      |)" << "\n";
    std::cout << R"(     |     |  ___|                         | |            | || |     |)" << "\n";
    std::cout << R"(     |     | |__  __  __  ___   ___  _   _ | |_   ___   __| || |     |)" << "\n";
    std::cout << R"(     |     |  __| \ \/ / / _ \ / __|| | | || __| / _ \ / _` || |     |)" << "\n";
    std::cout << R"(     |     | |___  >  < |  __/| (__ | |_| || |_ |  __/| (_| ||_|     |)" << "\n";
    std::cout << R"(     |     \____/ /_/\_\ \___| \___| \__,_| \__| \___| \__,_|(_)     |)" << "\n";
    std::cout <<   "     |                                                               |\n";
    std::cout <<   "     |                                                               |\n";
    std::cout <<   "     |                                                               |\n";
    std::cout <<   "     -----------------------------------------------------------------\n";

}
