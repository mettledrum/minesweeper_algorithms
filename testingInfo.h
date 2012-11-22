//Andrew Hoyle

//holds the values for heuristics... playTheGame() and mainMine()
// take it by reference and updates times and win values

#ifndef TESTINGINFO_H
#define	TESTINGINFO_H

struct testingInfo
{
    testingInfo()
    {
        tilesOnBoard=0;
        minesOnBoard=0;
        numberOfWins=0;
        totalGames=0;
    }
    
    float tilesOnBoard;
    float minesOnBoard;
    float numberOfWins;
    float totalGames;
};


#endif	/* TESTINGINFO_H */

