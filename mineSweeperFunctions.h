//Andrew Hoyle

#ifndef MINESWEEPERFUNCTIONS_H
#define	MINESWEEPERFUNCTIONS_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <list>
#include <vector>

#include "tileInfo.h"
#include "board.h"
#include "mineSweeperGlobals.h"
#include "testingInfo.h"

//-----------------level 3

//looks around the X and Y tiles and uses the info from the inner min==max
// and returns the float value... it is later checked for 1 or 0 representing
// a certain scenario of there being a mine or not
float outerRisksCalc(board& b, const int ii, const int jj, const int innerNumber);

//if the ints returned by these equations are ==, then the next phase can begin
int minFunc(const int size, const int mineX, const int mineY);
int maxFunc(const int mineX, const int mineY);

//this compares the two outers and if they have some overlap, those tileInfo
// structs are removed from the outers by marking them with 999 in the .risk
// data member.  If they overlap, that tileInfo is copied over to the innerXY
// list<tileInfo>
void compareOuters(std::list<tileInfo>& oX, std::list<tileInfo>& oY, std::list<tileInfo>& iXY);

//fills the outerX and outerY by looking at tiles around it that meet the requirements
void populateOuter(board& B, std::list<tileInfo>& outer, const int i, const int j);

// 1) randomly picks until a sea expansion is found
// 2) searches for 0% risk, 0.0f
// 3) searches for 100% mines, 1.0f
// 4) looks at tiles 2 at a time for 0% or 1&
// 5) makes PQ like oneAtATimeThenCalcRisksTile()
// 6) if PQ is empty... guess
void theSmartOne(board& b1, bool& exp, int& curI, int& curJ, clickType& curCL);

//-----------------level 2

// 1) randomly picks until a sea expansion is found
// 2) searches for 0% risk, 0.0f
// 3) searches for 100% mines, 1.0f
// 4) prints out if risk MUST be taken
// 5) finds safest tile to click, risks sorted in heap... PQ should never be empty
void oneAtATimeThenCalcRisksTile(board& b1, bool& exp, int& curI, int& curJ, clickType& curCL);//?

// 1) randomly picks until a sea expansion is found
// 2) searches for 0% risk, 0.0f
// 3) searches for 100% mines, 1.0f
// 4) prints out if risk MUST be taken
// 5) back to 1)
void oneAtATimeThenGuessTile(board& b1, bool& exp, int& curI, int& curJ, clickType& curCL);//?

//used by oneAtATimeTile() after risk to look for undiscovered
// tiles to click on when risk is at certain level.  Initially, it is
// called at 0%, then 100%, then for a decreasing amount if a guess
// MUST be taken later.
//can "click" or "flag" tiles
void lookAroundClicks(board& B, const int I, const int J, int& curI, int& curJ);//?

//-----------------level 1

//the dumbest, most dangerous algorithm, RANDOMLY picks tiles
// with reckless abandon!  Just makes sure tile has NOT been
// picked before, and that it's NOT an edge
//ALWAYS just "clicks", but is sensitive to surrounding flags and questions
void dumbRandomSelectTile(board& b1, bool& notUsed, int& curI, int& curJ, clickType& curCL);

//-----------------random functions 

//so you can view the tile info for the 3rd algorithm, DOESN'T show risk
// stuffed in the tileInfo... b/c it is not used in the looking-at-2-at-a-time
// part at all... Infact, the .risk data member of the tileInfo struct is 
// marked as 999 if there's overlap b/w the outerX and outerY to mark those
// in the list to be deleted.
void displayTileInfo(const std::list<tileInfo>& TIs);

//uses cin >> so you can look at the board being played
// later, chang it to a vew seconds pause maybe
void pauseGame();

//used to customize the PQ to have min at top based on .risk in the
// oneAtATimeThenCalcRisksTile() algorithm found also in theSmartestOne()
struct compareMe
{
    bool operator()(const tileInfo& L, const tileInfo& R)
    {
        return L.risk > R.risk;
    }
};

//-----------------the game loop

//feeds the board, ecapsulates the game, takes pointer to the 
// algorithm function pointer to know what to use to try and beat
// the game
void playTheGame(board& b, Algo TheAlgorithmFromPointer, testingInfo& ti);

#endif	/* MINESWEEPERFUNCTIONS_H */

