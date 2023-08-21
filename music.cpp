#include <iostream>
#include <string>
#include <cctype>
using namespace std;

bool isProperPiece(string piece);
bool isPlayablePiece(string piece, int& badBeat);
char transformNote(int octave, char noteLetter, char accidentalSign);
string transformBeat(string beat);
int transformPiece(string piece, string& instructions, int& badBeat);

int main()
{
    for (;;)
    {
        string instructions = "";
        int badBeat = 0;
        int test;

        cout << "Enter a piece:";
        string music;
        getline(cin, music);
        test = transformPiece(music, instructions, badBeat);
        switch (test) {
        case 1:
            cout << "This is not a proper piece!" << endl;
            break;
        case 2:
            cout << "Your piece is proper, but not playable!" << endl;
            cout << "The error is in beat " << badBeat << "." << endl;
            break;
        case 0:
            cout << "Here's your transformed piece: " << endl;
            cout << instructions << endl;
            break;
        default:
            cout << "Something went wrong." << endl;
            break;
        }
    }
}
int transformPiece(string piece, string& instructions, int& badBeat)
{
    string beat = "";
    if (!isProperPiece(piece))
        return 1;
    else if (isProperPiece(piece))
    {
        if (!isPlayablePiece(piece, badBeat))
            return 2;
    }
    if (piece.empty())
    {
        instructions = "";
        return 0;
    }
    else if (piece == "/")
    {
        instructions += " ";
        return 0;
    }
    instructions = "";
    //concateenates to empty beat variable until finds a '/' and calls transformBeat with beat as argument once it finds a '/'
    //also sets instructions for consecutive '/' as one beat
    for (int i = 0; i < piece.size(); i++)
    {
        if (beat == "" && piece.at(i) != '/')
        {
            for (int j = i; j < piece.size() && piece.at(j) != '/'; j++)
            {
                beat += piece.at(j);
            }
            instructions += transformBeat(beat);
            i += beat.size() - 1;
            beat = "";
        }
        else if (piece.at(i) == '/' && i != piece.size() - 1)
        {
            if (piece.at(i + 1) == '/')
                instructions += ' ';
        }
    }
    return 0;
}
bool isPlayablePiece(string piece, int& badBeat)
{
    //beatCounter is used to return the beat where the first nonplayable note was found
    int beatCounter = 1;
    for (int k = 0; k != piece.size(); k++)
    {
        if (piece.at(k) == '/')
            beatCounter++;
        //Checks if there are characters besides uppercase letters, accidentals, or numbers between 1 and 7
        //(with exception for 1 and 7 in specific situations)
        else if (!isupper(piece.at(k)))
        {
            if (piece.at(k) != '#' && piece.at(k) != 'b')
            {
                if (piece.at(k) < '2')
                {
                    if (piece.at(k) != '1')
                    {
                        badBeat = beatCounter;
                        return false;
                    }
                    //if finds a '1' then will check the characters before it for a '#' and 'B'
                    else if (piece.at(k) == '1')
                    {
                        if (piece.size() - k < 1)
                        {
                            badBeat = beatCounter;
                            return false;
                        }
                        else if (piece.at(k - 1) != '#' || piece.at(k - 2) != 'B')
                        {
                            badBeat = beatCounter;
                            return false;
                        }
                    }
                }
                else if (piece.at(k) > '6')
                {
                    if (piece.at(k) != '7')
                    {
                        badBeat = beatCounter;
                        return false;
                    }
                    //if finds a '7' then will check the characters before it for a 'b' and 'C'
                    else if (piece.at(k) == '7')
                    {
                        if (piece.at(k - 1) != 'C')
                        {
                            if (piece.size() - k < 1)
                            {
                                badBeat = beatCounter;
                                return false;
                            }
                            else if (piece.at(k - 1) != 'b' || piece.at(k - 2) != 'C')
                            {
                                badBeat = beatCounter;
                                return false;
                            }
                        }
                    }
                }
                //if finds a '2' then will check the characters before it for a 'b' and 'C'
                //and will return false if it does find both
                else if (piece.at(k) == '2')
                {
                    if (piece.at(k - 1) == 'b' && piece.at(k - 2) == 'C')
                    {
                        badBeat = beatCounter;
                        return false;
                    }
                }
            }
        }
    }
    return true;
}


string transformBeat(string beat)
{
    int noteCount = 0;
    string temp = "";
    string note = "";

    for (int j = 0; j < beat.size(); j++)
    {
        if (isupper(beat.at(j)))
            noteCount++;
    }
    if (noteCount > 1)
        temp += '[';

    for (int i = 0; i < beat.size(); i++)
    {
        if (note == "" && isupper(beat.at(i)))
        {
            note += beat.at(i);
            //creates note by concatenating until next uppercase letter
            for (int j = i + 1; j < beat.size() && !isupper(beat.at(j)); ++j)
            {
                if (!isupper(beat.at(j)))
                    note += beat.at(j);
            }
            //specific intructions for transformNote depending on size of the note
            if (note.size() == 1)
            {
                temp += transformNote(4, note.at(0), ' ');
            }
            else if (note.size() == 2) {
                if (note.at(1) == '#' || note.at(1) == 'b')
                {
                    temp += transformNote(4, note.at(0), note.at(1));
                }
                else
                {
                    temp += transformNote(note.at(1) - '0', note.at(0), ' ');
                }
            }
            else if (note.size() == 3)
            {
                temp += transformNote(note.at(2) - '0', note.at(0), note.at(1));
            }

            note = "";
        }
    }
    if (noteCount > 1)
        temp += ']';

    return temp;
}

bool isProperPiece(string piece)
{
    //check to see if string is empty
    if (piece == "")
    {
        return true;
    }

    //iterates through every index of piece
    for (int k = 0; k != piece.size(); k++)
    {
        //checks to see if the last character is a slash. if not, return false
        if (piece.at(piece.size() - 1) != '/')
        {
            return false;
        }

        //checks to see if the character is not a /, #, or b
        if (piece.at(k) != '/' && piece.at(k) != '#' && piece.at(k) != 'b')
        {
            //since it is not a /, #, or b, check to see if it is a letter (less than A or greater than G)
            //however, if the character is less than A, it could be a number
            if (piece.at(k) < 'A')
            {
                //check to see if it is a number. if not, return false
                if (piece.at(k) < '0' || piece.at(k) > '9')
                {
                    return false;
                }
                //if it is a number, check to see if there is a letter or accidental before it.
                else if (piece.at(k) > '0' && piece.at(k) < '9')
                {
                    //if the number is the first in the string, return false
                    if (k == 0)
                    {
                        return false;
                    }
                    //checks to see if the one before is a letter
                    if (piece.at(k - 1) < 'A' || piece.at(k - 1) > 'G')
                    {
                        //checks to see if the one before is an accidental
                        if (piece.at(k - 1) != '#' && piece.at(k - 1) != 'b')
                        {
                            return false;
                        }
                    }
                }
            }
            //if it is not a /, #, or b, check to see if it is a letter greater than G
            //if so, return false.
            else if (piece.at(k) > 'G')
            {
                return false;
            }
        }
        //checks to see if it is a # or b
        if (piece.at(k) == '#' || piece.at(k) == 'b')
        {
            //if the # or b is the first in the string, return false
            if (k == 0)
            {
                return false;
            }
            //if the char before is not a letter, return false
            if (piece.at(k - 1) < 'A' || piece.at(k - 1) > 'G')
            {
                return false;
            }
        }
    }
    //if it passes all the false checks, return true
    return true;
}

char transformNote(int octave, char noteLetter, char accidentalSign)
{
    // This check is here solely to report a common CS 31 student error.
    if (octave > 9)
    {
        cerr << "** transformNote was called with first argument = "
            << octave << endl;
    }
    int note;

    // Convert Cb, C, C#/Db, D, D#/Eb, ..., B, B#
    //      to -1, 0,   1,   2,   3, ...,  11, 12

    switch (noteLetter)
    {
    case 'C':  note = 0; break;
    case 'D':  note = 2; break;
    case 'E':  note = 4; break;
    case 'F':  note = 5; break;
    case 'G':  note = 7; break;
    case 'A':  note = 9; break;
    case 'B':  note = 11; break;
    default:   return ' ';
    }
    switch (accidentalSign)
    {
    case '#':  note++; break;
    case 'b':  note--; break;
    case ' ':  break;
    default:   return ' ';
    }

    // Convert ..., A#1, B1, C2, C#2, D2, ...
    //      to ..., -2,  -1, 0,   1,  2, ...

    int sequenceNumber = 12 * (octave - 2) + note;

    string keymap = "1!2@34$5%6^78*9(0qQwWeErtTyYuiIoOpPasSdDfgGhHjJklLzZxcCvVbBnm";
    if (sequenceNumber < 0 || sequenceNumber >= keymap.size())
        return ' ';
    return keymap.at(sequenceNumber);
}


