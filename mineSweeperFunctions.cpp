//Andrew Hoyle

/* Holds the functions to play the game, and display the
 * results to the outfile.  The 3 algorithms are found here. */

#include "mineSweeperFunctions.h"

void displayTileInfo(const std::list<tileInfo>& TIs)
{
    std::list<tileInfo>::const_iterator b=TIs.begin(), e=TIs.end();
    
    while(b!=e)
    {
        std::cout << *b++ << " ; ";
    }
    std::cout << '\n';
}

float outerRisksCalc(board& b, const int ii, const int jj, const int minEQmax)
{
    float tileRiskNum = b.getGrid()[ii][jj].adjMines - b.getGrid()[ii][jj].adjFlags - minEQmax;

    float tileRiskDenom = b.getGrid()[ii][jj].adjUnknowns - minEQmax;

    float tileRisk = tileRiskNum / tileRiskDenom;
    
    return tileRisk;
}

int minFunc(const int size, const int mineX, const int mineY)
{
    int theMin=size;
    
    if(theMin>mineX)
        theMin=mineX;
    
    if(theMin>mineY)
        theMin=mineY;
    
    return theMin;
}

int maxFunc(const int mineX, const int mineY)
{
    int theMax=0;
    
    if(mineX>theMax)
        theMax=mineX;
    
    if(mineY>theMax)
        theMax=mineY;
    
    return theMax;
}

void compareOuters(std::list<tileInfo>& oX, std::list<tileInfo>& oY, std::list<tileInfo>& iXY)
{
    //adds to innerXY if there are similarities
    std::list<tileInfo>::iterator bX=oX.begin(), eX=oX.end();
    while(bX!=eX)
    {
        std::list<tileInfo>::iterator bY=oY.begin(), eY=oY.end();
        
        while(bY!=eY)
        {
            if( (*bY)==(*bX) )
            {
                //add to innerXY vector
                iXY.push_back(*bY);
                
                //mark tile for deletion after this loop
                (*bY).risk=999;
                (*bX).risk=999;
            }
            ++bY;
        }
        ++bX;
    }
    
    //deletion loops
    bX=oX.begin();
    eX=oX.end();
    while(bX!=eX)
    {
        if((*bX).risk==999)
            bX=oX.erase(bX);
        else
            ++bX;
    }
    
    std::list<tileInfo>::iterator bY=oY.begin(), eY=oY.end();
    while(bY!=eY)
    {
        if((*bY).risk==999)
            bY=oY.erase(bY);
        else
            ++bY;
    }
}

void populateOuter(board& B, std::list<tileInfo>& outer, const int eye, const int jay)
{
    //check all 8 borders for being UNdiscovered,
    // change curI and curJ and return if they are
    for(int I=eye-1; I<eye-1+3; ++I)
    {
        for(int J=jay-1; J<jay-1+3; ++J)
        {
            if( B.getGrid()[I][J].discovered==false &&
                B.getGrid()[I][J].flag==false && 
                B.getGrid()[I][J].question==false && 
                !(eye==I && jay==J) )
            {
                //fill outer with TIs
                tileInfo tempTI;
                tempTI.row=I;
                tempTI.col=J;
                tempTI.risk=0;
                
                outer.push_back(tempTI);
            }
        }
    }
}

void theSmartOne(board& b1, bool& exp, int& curI, int& curJ, clickType& curCL)
{   
    //pick randomly until sea expansion-----------------------------------------
    if(exp==false)
    {
        std::cout << "dumb 1st level called" << std::endl;
        dumbRandomSelectTile(b1, exp, curI, curJ, curCL);
        return;
    }
    
    //level2 stuff here: cookieCutter()-----------------------------------------
    
    std::cout << "2nd level called\n";
    
    //GOTTA HAVE ALL 1 and 0 clicked or flagged first!
        //analyze tile by tile on board and click on first
    // one found with 0% risk
    for(int i=1; i<b1.getRow(); ++i)
    {
        for(int j=1; j<b1.getCol(); ++j)
        {            
            //we only care about discovered tiles with undiscovered neighbors
            if(b1.getGrid()[i][j].adjUnknowns!=0 && b1.getGrid()[i][j].discovered==true)
            {
                //calculating the risk
                float tileRiskNum = b1.getGrid()[i][j].adjMines - b1.getGrid()[i][j].adjFlags;
                float tileRiskDenom = b1.getGrid()[i][j].adjUnknowns;
                float tileRisk = tileRiskNum / tileRiskDenom;

                std::cout << "tileRisk around "<< i << ", " << j << ": " << tileRisk << std::endl;
                
                // 2) NO mines in all surrounding tiles---
                if(tileRisk==0)
                {
                    //find an undiscovered tile and point curI and curJ to it
                    lookAroundClicks(b1, i, j, curI, curJ);
                    curCL=click;
                    
                    std::cout << "selected row: " << curI << std::endl;
                    std::cout << "selected col: " << curJ << std::endl;
                    
                    return;
                }

                // 3) ALL MINES in surrounding tiles---
                if(tileRisk==1)
                {
                    //find an undiscovered tile and point curI and curJ to it
                    lookAroundClicks(b1, i, j, curI, curJ);
                    curCL=flag;
    
                    std::cout << "selected row: " << curI << std::endl;
                    std::cout << "selected col: " << curJ << std::endl;
    
                    return;
                }
            }
        }
    }
    
    //3rd level, looking at two tiles: X, and Y---------------------------------
    
    std::cout << "3rd level called\n";
    
    //X:i,j
    for(int i=1; i<b1.getRow(); ++i)
    {
        for(int j=1; j<b1.getCol(); ++j)
        {
            //Y:I,J
            for(int I=i-2; I<=i+2; ++I)
            {
                for(int J=j-2; J<=j+2; ++J)
                {
                    //if there is potentially overlap b/w Y and X
                    if( I>0 && 
                        J>0 && 
                        I<b1.getRow() &&
                        J<b1.getCol() &&
                        !(j==J && i==I) && 
                        b1.getGrid()[I][J].discovered==true && 
                        b1.getGrid()[I][J].flag==false && 
                        b1.getGrid()[I][J].edge==false &&
                        b1.getGrid()[I][J].adjMines!=0 &&
                        b1.getGrid()[i][j].discovered==true && 
                        b1.getGrid()[i][j].flag==false && 
                        b1.getGrid()[i][j].edge==false &&
                        b1.getGrid()[i][j].adjMines!=0 )
                    {
                        std::cout << "acceptable X and Y:\n";
                        std::cout << "X: (" << i << ", " << j << ")\n";
                        std::cout << "Y: (" << I << ", " << J << ")\n";
                        
                        //holds values for algorithms
                        std::list<tileInfo> outerX, outerY, innerXY;
                        
                        //populate outers, very similar to 3x3
                        // lookAroundClicks() function, only it
                        // populates vector
                        populateOuter(b1, outerX, i, j);
                        populateOuter(b1, outerY, I, J);
                        
                        std::cout << "\touterX before: ";
                        displayTileInfo(outerX);
                        std::cout << "\touterY before: ";
                        displayTileInfo(outerY);
                        
                        //compare the outers, populate inner if there's overlap
                        // also, remove similar values from outers if that's
                        // the case
                        compareOuters(outerX, outerY, innerXY);
                        
                        std::cout << "\touterX AFTER: ";
                        displayTileInfo(outerX);
                        std::cout << "\touterY AFTER: ";
                        displayTileInfo(outerY);
                        std::cout << "\tinnerXY AFTER: ";
                        displayTileInfo(innerXY);
                        
                        //only do next part if there's some overlap and at least
                        // one of the outers is not empty
                        if ( !innerXY.empty() && !(outerX.empty() && outerY.empty()) )
                        {
                            
                            std::cout << "\t\tmade it to min ?= max comparison\n";
                            
                            //to hold the values to compare
                            int minVal, maxVal;
                            
                            minVal=minFunc(innerXY.size(), 
                                    b1.getGrid()[i][j].adjMines-b1.getGrid()[i][j].adjFlags, 
                                    b1.getGrid()[I][J].adjMines-b1.getGrid()[I][J].adjFlags);
                            
                            maxVal=maxFunc(b1.getGrid()[i][j].adjMines-b1.getGrid()[i][j].adjFlags-outerX.size(),
                                    b1.getGrid()[I][J].adjMines-b1.getGrid()[I][J].adjFlags-outerY.size());
                            
                            //are these values similar? Then you can redo
                            // the calcRisks part for each outerX and outerY tile
                            if( minVal==maxVal && minVal>0 )
                            {
                                //look at outerX
                                float xRisk=outerRisksCalc(b1, i, j, minVal);
                                
                                //look at outerY
                                float yRisk=outerRisksCalc(b1, I, J, minVal);
                                
                                std::cout << "\t\t\tmin == max! == " << minVal << std::endl;
                                std::cout << "\t\t\txRisk: " << xRisk << "\n";
                                std::cout << "\t\t\tyRisk: " << yRisk << "\n";
                                std::cout << "\t\t\tif x==0 or 1, and !outerX.empty() going to click: " << outerX.front() << std::endl;
                                std::cout << "\t\t\tif y==0 or 1, and !outerY.empty() going to click: " << outerY.front() << std::endl;
                                
                                //four possible scenarios for a sure pick:
                                //1) outerX has mine 100%
                                if(xRisk==1 && !outerX.empty())
                                {
//                                    pauseGame();
                                    
                                    curI=outerX.front().row;
                                    curJ=outerX.front().col;
                                    curCL=flag;
                                    return;
                                }
                                
                                //2) outerX has mine 0%
                                if(xRisk==0 && !outerX.empty())
                                {
                                    curI=outerX.front().row;
                                    curJ=outerX.front().col;
                                    curCL=click;
                                    return;
                                }
                                
                                //3) outerY has mine 100%
                                if(yRisk==1 && !outerY.empty())
                                {
//                                    pauseGame();
                                    
                                    curI=outerY.front().row;
                                    curJ=outerY.front().col;
                                    curCL=flag;
                                    return;
                                }
                                //4) outerY has mine 0%
                                if(yRisk==0 && !outerY.empty())
                                {
                                    curI=outerY.front().row;
                                    curJ=outerY.front().col;
                                    curCL=click;
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    // 2-at-a-time part didn't find any 100% scenarios:
    // 4) 5) sort risks, pick lowest--------------------------------------------
    std::cout << "deuces wild! you gotta guess again, no 100% or 0%" << std::endl;
    
    //will hold the info of a tile to be stuffed into PQ if it
    // meets the appropriate conditions
    tileInfo theRiskTile;
    float tileRiskNum1, tileRiskDenom1, tileRisk1;
    
    //will hold the min values
    std::priority_queue<tileInfo, std::vector<tileInfo>, compareMe> minRisksPQ;
    
    //populate PQ with tiles whose risks are b/w 0 and 1
    for(int i=1; i<b1.getRow(); ++i)
    {
        for(int j=1; j<b1.getCol(); ++j)
        {
            //we only care about discovered tiles with undiscovered neighbors
            if(b1.getGrid()[i][j].adjUnknowns!=0 && b1.getGrid()[i][j].discovered==true)
            {
                //calculating the risk
                tileRiskNum1 = b1.getGrid()[i][j].adjMines - b1.getGrid()[i][j].adjFlags;
                tileRiskDenom1 = b1.getGrid()[i][j].adjUnknowns;
                tileRisk1 = tileRiskNum1 / tileRiskDenom1;
                
                //put 'em in the PQ
                std::cout << "PQ now has tileRisk around: " << i << ", " << j << ": " << tileRisk1 << std::endl;

                theRiskTile.row=i;
                theRiskTile.col=j;
                theRiskTile.risk=tileRisk1;

                //stuff value into the PQ
                minRisksPQ.push(theRiskTile);
            }
        }
    }
    
    //if there are no non-flags bordering an undiscovered
    if(minRisksPQ.empty())
    {
        std::cout << "Oh snap, the PQ is empty on a rare case!!!\n";
        dumbRandomSelectTile(b1, exp, curI, curJ, curCL);
        return;
    }
    
    //get values to pass to lookAroundClicks()
    tileInfo safestTile = minRisksPQ.top();
    int PQi=safestTile.row;
    int PQj=safestTile.col;
        
    //take the top off the PQ and click on an undiscovered tile around it
    // (undiscovered tile also is not a question or a flag)
    lookAroundClicks(b1, PQi, PQj, curI, curJ);
    curCL=click;
    
    //I don't know if the curI and curJ have been updated by lookAroundClicks()
    std::cout << "PQ has picked: " << curI << ", " << curJ << " with risk of: " << safestTile.risk << std::endl;
}

void oneAtATimeThenCalcRisksTile(board& b1, bool& exp, int& curI, int& curJ, clickType& curCL)
{
    // 1) toward beginning of game when sea expansion has NOT-------------------
    // been uncovered yet.
    if(exp==false)
    {
        std::cout << "dumb part called" << std::endl;
        dumbRandomSelectTile(b1, exp, curI, curJ, curCL);
        return;
    }
    
    // 2) and 3) calculating mine risks-----------------------------------------
    
    std::cout << "intelligent part called" << std::endl;
    
    //will hold the risk values of each tile in following loop
    float tileRisk, tileRiskNum, tileRiskDenom;
    
    //analyze tile by tile on board and click on first
    // one found with 0% risk
    for(int i=1; i<b1.getRow(); ++i)
    {
        for(int j=1; j<b1.getCol(); ++j)
        {            
            //we only care about discovered tiles with undiscovered neighbors
            if(b1.getGrid()[i][j].adjUnknowns!=0 && b1.getGrid()[i][j].discovered==true)
            {
                //debugging
//              std::cout << "adjUnknowns of " << i << ", " << j << ": " << b1.getGrid()[i][j].adjUnknowns << std::endl;
//              std::cout << "adjMines of " << i << ", " << j << ": " << b1.getGrid()[i][j].adjMines << std::endl;
//              std::cout << i << ", " << j << " discovered?: " << b1.getGrid()[i][j].discovered << std::endl;
                
                //calculating the risk
                tileRiskNum = b1.getGrid()[i][j].adjMines - b1.getGrid()[i][j].adjFlags;
                tileRiskDenom = b1.getGrid()[i][j].adjUnknowns;
                tileRisk = tileRiskNum / tileRiskDenom;

                std::cout << "tileRisk around "<< i << ", " << j << ": " << tileRisk << std::endl;
                
                // 2) NO mines in all surrounding tiles---
                if(tileRisk==0)
                {
                    //find an undiscovered tile and point curI and curJ to it
                    lookAroundClicks(b1, i, j, curI, curJ);
                    curCL=click;
                    
                    std::cout << "selected row: " << curI << std::endl;
                    std::cout << "selected col: " << curJ << std::endl;
                    
                    return;
                }

                // 3) ALL MINES in surrounding tiles---
                if(tileRisk==1)
                {
                    //find an undiscovered tile and point curI and curJ to it
                    lookAroundClicks(b1, i, j, curI, curJ);
                    curCL=flag;
                    return;
                }
            }
        }
    }
    
    // 4) 5) sort risks, pick lowest--------------------------------------------
    std::cout << "fiddlesticks, you gotta guess again, no 100% or 0%" << std::endl;
    
    //will hold the info of a tile to be stuffed into PQ if it
    // meets the appropriate conditions
    tileInfo theRiskTile;
    
    //will hold the min values
    std::priority_queue<tileInfo, std::vector<tileInfo>, compareMe> minRisksPQ;
    
    //populate PQ with tiles whose risks are b/w 0 and 1
    for(int i=1; i<b1.getRow(); ++i)
    {
        for(int j=1; j<b1.getCol(); ++j)
        {
            //we only care about discovered tiles with undiscovered neighbors
            if(b1.getGrid()[i][j].adjUnknowns!=0 && b1.getGrid()[i][j].discovered==true)
            {
                //calculating the risk
                tileRiskNum = b1.getGrid()[i][j].adjMines - b1.getGrid()[i][j].adjFlags;
                tileRiskDenom = b1.getGrid()[i][j].adjUnknowns;
                tileRisk = tileRiskNum / tileRiskDenom;
                
                //put 'em in the PQ
                std::cout << "PQ now has tileRisk around: " << i << ", " << j << ": " << tileRisk << std::endl;

                theRiskTile.row=i;
                theRiskTile.col=j;
                theRiskTile.risk=tileRisk;

                //stuff value into the PQ
                minRisksPQ.push(theRiskTile);
                }
            }
        }
    
    //if there are no non-flags bordering an undiscovered
    if(minRisksPQ.empty())
    {
        std::cout << "Oh snap, the PQ is empty due to a rare case!!!\n";
        dumbRandomSelectTile(b1, exp, curI, curJ, curCL);
        return;        
    }
    
    //get values to pass to lookAroundClicks().. PW will never be empty
    tileInfo safestTile = minRisksPQ.top();
    int PQi=safestTile.row;
    int PQj=safestTile.col;
        
    //take the top off the PQ and click on an undiscovered tile around it
    // (undiscovered tile also is not a question or a flag)
    lookAroundClicks(b1, PQi, PQj, curI, curJ);
    curCL=click;
    
    //I don't know if the curI and curJ have been updated by lookAroundClicks()
    std::cout << "PQ has picked: " << curI << ", " << curJ << " with risk of: " << safestTile.risk << std::endl;
}

void oneAtATimeThenGuessTile(board& b1, bool& exp, int& curI, int& curJ, clickType& curCL)
{
    // 1) toward beginning of game when sea expansion has NOT-------------------
    // been uncovered yet.
    if(exp==false)
    {
        std::cout << "dumb part called" << std::endl;
        dumbRandomSelectTile(b1, exp, curI, curJ, curCL);
        return;
    }
    
    // 2) and 3) calculating mine risks-----------------------------------------
    
    std::cout << "intelligent part called" << std::endl;
    
    //will hold the risk values of each tile in following loop
    float tileRisk, tileRiskNum, tileRiskDenom;
    
    //analyze tile by tile on board and click on first
    // one found with 0% risk
    for(int i=1; i<b1.getRow(); ++i)
    {
        for(int j=1; j<b1.getCol(); ++j)
        {            
            //we only care about discovered tiles with undiscovered neighbors
            if(b1.getGrid()[i][j].adjUnknowns!=0 && b1.getGrid()[i][j].discovered==true)
            {
                //debugging
//              std::cout << "adjUnknowns of " << i << ", " << j << ": " << b1.getGrid()[i][j].adjUnknowns << std::endl;
//              std::cout << "adjMines of " << i << ", " << j << ": " << b1.getGrid()[i][j].adjMines << std::endl;
//              std::cout << i << ", " << j << " discovered?: " << b1.getGrid()[i][j].discovered << std::endl;
                
                //calculating the risk
                tileRiskNum = b1.getGrid()[i][j].adjMines - b1.getGrid()[i][j].adjFlags;
                tileRiskDenom = b1.getGrid()[i][j].adjUnknowns;
                tileRisk = tileRiskNum / tileRiskDenom;

                std::cout << "tileRisk around "<< i << ", " << j << ": " << tileRisk << std::endl;
                
                // 2) NO mines in all surrounding tiles---
                if(tileRisk==0)
                {
                    //find an undiscovered tile and point curI and curJ to it
                    lookAroundClicks(b1, i, j, curI, curJ);
                    curCL=click;
                    return;
                }

                // 3) ALL MINES in surrounding tiles---
                if(tileRisk==1)
                {
                    //find an undiscovered tile and point curI and curJ to it
                    lookAroundClicks(b1, i, j, curI, curJ);
                    curCL=flag;
                    return;
                }
            }
        }
    }
    
    // 4) 5) back to 1)---------------------------------------------------------
    //no 100% or 0% were found, so gotta guess now, so set the
    // expansion to false again so part 1) activates again
    std::cout << "gosh darn it, you gotta guess again, no 100% or 0%" << std::endl;
    exp=false;
    
    std::cout << "dumb part called" << std::endl;
    dumbRandomSelectTile(b1, exp, curI, curJ, curCL);    
}

//remember, edges are DISCOVERED
// MUST check for flag and question because they are UNDISCOVERED
void lookAroundClicks(board& B, const int i, const int j, int& curI, int& curJ)
{
    //check all 8 borders for being UNdiscovered,
    // change curI and curJ and return if they are
    for(int I=i-1; I<i-1+3; ++I)
    {
        for(int J=j-1; J<j-1+3; ++J)
        {
            if( B.getGrid()[I][J].discovered==false &&
                B.getGrid()[I][J].flag==false &&
                B.getGrid()[I][J].question==false && 
                !(i==I && j==J) )
            {
                curI=I;
                curJ=J;
                return;
            }
        }
    }
}

//the stupidest technique of all, but must check for flags and questions 
// b/c it is integrated into the more intelligent algorithms
void dumbRandomSelectTile(board& b, bool& notUsed, int& curI, int& curJ, clickType& curCl)
{
    //for tile selection
    std::srand(std::time(NULL));
    
    //will hold random selection of x/y coordinates
    int randI, randJ;
    
    //don't select edge or discovered mine!
    //all edges are discovered=true, so edge check works with
    // just .discovered=true check, also should NOT even pick an
    // edge ever because of rand()% values 
    do
    {
        randI = std::rand() % (b.getRow()-2);
        randJ = std::rand() % (b.getCol()-2);
        ++randI;
        ++randJ;
    } while(b.getGrid()[randI][randJ].discovered!=false || 
            b.getGrid()[randI][randJ].flag!=false || 
            b.getGrid()[randI][randJ].question!=false);
    
    std::cout << "selected row: " << randI << std::endl;
    std::cout << "selected col: " << randJ << std::endl;
    
    curI=randI;
    curJ=randJ;
    curCl=click;

    return;
}

void pauseGame()
{
    std::cout << std::endl <<"paused, press key and <ENTER>: ";
    
    char c;
    std::cin >> c;
    
    std::cout << std::endl;
}

//board is now made in main, NOT in playTheGame()
void playTheGame(board& b1, Algo TheAlgorithmFromPointer, testingInfo& ti)
{   
    //show behind the undiscovered mask
    std::cout << "\n\nboard behind the scenes:" << std::endl;
    b1.displayBoardCheat();
    
    //terminates the game loop
    bool deathCheck=false;
    bool winCheck=false;
    
    //used by algorithms to go to next phase after
    // a sea expansion has been found
    bool expansion=false;
    
    //current move from user or algorithm that is played
    int curI, curJ;
    clickType curClick;
    
    //game begins
    while(deathCheck==false && winCheck==false)
    {             
        //shows board
        b1.displayBoard();
        std::cout << std::endl;
        
        //where algorithm/user picks next tile
        TheAlgorithmFromPointer(b1, expansion, curI, curJ, curClick);      
        
        //check for death click, curClick must be "click" enum
        b1.deathChecker(deathCheck, curI, curJ, curClick);
        //jump out of loop if true
        if(deathCheck==true)
            continue;
        
        //curClick==click
            //update tile clicked to discovered, 0s expansion takes place if 0 clicked
            // zerosExpansion() is a sub function.  bool expansion is turned true
            // if there's a sea expansion, used by algorithms
        //curClick==question
            //tile marked to question=true
        //curClick==flag
            //tile marked to flag=true
        b1.updateClickedTile(curI, curJ, expansion, curClick);
        
        //board updated
        b1.updateAllTiles();
        
        //win check - are all tiles discovered?
        // based off of board's unClickedCount data member-mineCount == 0?
        // also calls updateUnClickedCount
        b1.winChecker(winCheck);
    }
    
    if(winCheck==true)
    {
        std::cout << "You Won!\n";
        b1.displayBoard();
        b1.displayBoardCheat();
        
        ++ti.numberOfWins;
    }
    
    else
    {
        std::cout << "You Blew Up at: " << curI << ", " << curJ << std::endl;
        b1.displayBoard();
        b1.displayBoardCheat();
        
//        if(expansion==true)
//            pauseGame();
    }
    
    return;
}

