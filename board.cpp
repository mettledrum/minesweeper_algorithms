//Andrew Hoyle

//implementation of board class

#include "board.h"
#include "tile.h"

board::~board()
{
    //goes through and clears dynamic grid of vector<vector<int> >
    //get those columns
    for(int i=0; i<Row; ++i)
    {
        this->grid[i].clear();
    }
    
    //get the vector of rows
    this->grid.clear();
    
    //other data members, (not that important)
    //make sure win check ocurrs AFTER destructor is called b/c
    // the unClickedCount is set to 0
    unClickedCount=0;
    Row=0;
    Column=0;
    mineCount=0;
}

board::board(const board& b)
{
    //get data members from b
    this->Row=b.Row;
    this->Column=b.Column;
    this->mineCount=b.mineCount;
    
    this->unClickedCount=b.unClickedCount;
    
    //resize the board to the dim. given
    this->grid.resize(Row);
    for( int i = 0; i < Row; ++i)
    {
        this->grid[i].resize(Column);
    }
    
    //copy tile values over
    for(int i=0; i<Row; ++i)
    {
        for(int j=0; j<Column; ++j)
        {
            //uses overloaded = in tile
            grid[i][j]=b.grid[i][j];
        }
    }
}

board::board(int mc, int row, int col)
{
    //resize the board to the dim. given
    this->grid.resize(row);
    for( int i = 0; i < row; ++i)
    {
        this->grid[i].resize(col);
    }

    //get data members from params
    this->Row=row;
    this->Column=col;
    this->mineCount=mc;
    
    //account for edges and mines
    //when this value is 0 after subtracting mineCount,
    // winCheck will be true
    this->unClickedCount=row*col-(2*row)-(2*col)+4;
    
    //discovered, question, flag, edge, mine == false for all, 
    for(int i=0; i<Row; ++i)
    {
        for(int j=0; j<Column; ++j)
        {
            this->grid[i][j].flag=false;
            this->grid[i][j].question=false;
            this->grid[i][j].discovered=false;
            this->grid[i][j].edge=false;
            this->grid[i][j].mine=false;
            
            this->grid[i][j].visited=false;
        }
    }
    
    //make edges=true, and discovered=true, ALL EDGES ARE discovered!!!
    //top/bottom rows
    for(int j=0; j<col; ++j)
    {
        this->grid[0][j].edge=true;
        this->grid[0][j].discovered=true;
        
        this->grid[row-1][j].edge=true;
        this->grid[row-1][j].discovered=true;
    }
    
    //left/right cols
    for(int i=0; i<row; ++i)
    {
        this->grid[i][0].edge=true;
        this->grid[i][0].discovered=true;
        
        this->grid[i][col-1].edge=true;
        this->grid[i][col-1].discovered=true;
    }
    
    //---mines
    
    //for mine selection
    std::srand(std::time(NULL));
    
    //add mines to the grid, change mine to true IF not already mine,
    // AND if not edge
    for(int count=0; count < this->mineCount;)
    {
        //randomly pick mine
        int randI = std::rand() % row;
        int randJ = std::rand() % col;
        
        if(this->grid[randI][randJ].mine==false && this->grid[randI][randJ].edge==false)
        {
            this->grid[randI][randJ].mine=true;
            ++count;
        }
    }
    
    //update the tiles' adjacent matrices
    this->updateAllTiles();
}

void board::updateAllTiles()
{
    for(int i=0; i<Row; ++i)
    {
        for(int j=0; j<Column; ++j)
        {
            if(this->grid[i][j].edge != true)
            {
                //should only be done by constructor?
                this->grid[i][j].adjMines=countMines(i,j);
                
                this->grid[i][j].adjQuestions=countQuestions(i,j);
                this->grid[i][j].adjFlags=countFlags(i,j);
                this->grid[i][j].adjUnknowns=countUnknowns(i,j);
            }
        }
    }
}

int board::countMines(int i, int j)
{
    int tempMineCount=0;
    
    //looks at 8 surrounding tiles
    for(int I=i-1; I<i-1+3; ++I)
    {
        for(int J=j-1; J<j-1+3; ++J)
        {
            if(this->grid[I][J].mine==true && !(i==I && j==J) )
                ++tempMineCount;
        }
    }
    return tempMineCount;
}

int board::countQuestions(int i, int j)
{
    int tempQCount=0;
    
    //looks at 8 surrounding tiles by making 3x3 grid and ignoring itself
    for(int I=i-1; I<i-1+3; ++I)
    {
        for(int J=j-1; J<j-1+3; ++J)
        {
            //don't count yourself!
            if(this->grid[I][J].question==true && !(i==I && j==J) )
                ++tempQCount;
        }
    }
    return tempQCount;
}

int board::countFlags(int i, int j)
{
    int tempFlagCount=0;
    
    //looks at 8 surrounding tiles
    for(int I=i-1; I<i-1+3; ++I)
    {
        for(int J=j-1; J<j-1+3; ++J)
        {
            if(this->grid[I][J].flag==true && !(i==I && j==J) )
                ++tempFlagCount;
        }
    }
    return tempFlagCount;
}

int board::countUnknowns(int i, int j)
{
    int tempUndiscoveredCount=0;
    
    //looks at 8 surrounding tiles
    for(int I=i-1; I<i-1+3; ++I)
    {
        for(int J=j-1; J<j-1+3; ++J)
        {
            //cannot include flags in the unknowns count
            if(this->grid[I][J].discovered==false && this->grid[I][J].flag==false && !(i==I && j==J) )
                ++tempUndiscoveredCount;
        }
    }
    
    return tempUndiscoveredCount;
}

void board::displayBoard()
{
    //things to display
    char EDGE = '#';
    char UNCLICKED = '+';
    char FLAG = 'F';
    char QUEST = '?';
    char MINE = '*';
    
    //the grid and row numbers
    for(int i=0; i<Row; ++i)
    {
        for(int j=0; j<Column; ++j)
        {
            if(this->grid[i][j].edge==true)
            {
                std::cout << EDGE;
                continue;
            }
            
            //updated to not print 0s, just spaces
            else if(this->grid[i][j].discovered==true && this->grid[i][j].adjMines!=0)
            {
                std::cout << this->grid[i][j].adjMines;
                continue;
            }
            
            else if(this->grid[i][j].discovered==true && this->grid[i][j].adjMines==0)
            {
                std::cout << ' ';
                continue;
            }
            
            else if(this->grid[i][j].flag==true)
            {
                std::cout << FLAG;
                continue;
            }
            
            else if(this->grid[i][j].question==true)
            {
                std::cout << QUEST;
                continue;
            }
            
            else
                std::cout << UNCLICKED;
        }
        
        std::cout << std::endl;
    } 
}

void board::displayBoardCheat()
{
    //things to display
    char EDGE = '#';
    char UNCLICKED = '+';
    char FLAG = 'F';
    char QUEST = '?';
    char MINE = '*';
    
    //the grid and row numbers
    for(int i=0; i<Row; ++i)
    {
        for(int j=0; j<Column; ++j)
        {
            if(this->grid[i][j].edge==true)
            {
                std::cout << EDGE;
                continue;
            }
            
            //updated to not print 0s, just spaces
            else if(this->grid[i][j].adjMines!=0 && this->grid[i][j].mine==false)
            {
                std::cout << this->grid[i][j].adjMines;
                continue;
            }
            
            else if(this->grid[i][j].adjMines==0 && this->grid[i][j].mine==false)
            {
                std::cout << ' ';
                continue;
            }
            
            else
            {
                std::cout << MINE;
                continue;
            }
        } 
        
        std::cout << std::endl;
    }
}

void board::deathChecker(bool& DC, int& curI, int& curJ, const clickType& curCL)
{
    if(this->grid[curI][curJ].mine==true && curCL==click)
        DC=true;
}

void board::winChecker(bool& WC)
{
    //update the unClickedCount
    this->updateUnClickedCount();
    
    if( (this->unClickedCount-this->mineCount)==0 )
        WC=true;
}

//you DON'T need to subtract edges because they are .discovered=true
void board::updateUnClickedCount()
{
    //reset value
    this->unClickedCount=0;
    
    for(int i=0; i<Row; ++i)
    {
        for(int j=0; j<Column; ++j)
        {
            if(this->grid[i][j].discovered==false)
                ++this->unClickedCount;
        }
    }
}

//exp bool set to true for algorithms' analyses
void board::updateClickedTile(int& curI, int& curJ, bool& exp, const clickType& curCL)
{
    //if tile was clicked on
    if(curCL==click)
    {
        //uncover the tile's mask, decrement unClickedCount
        this->grid[curI][curJ].discovered=true;

        //call expansion
        if(this->grid[curI][curJ].adjMines==0)
        {
            this->expandZeros(curI, curJ);
            exp=true;
        }
    }
    
    //if flag
    if(curCL==flag)
        this->grid[curI][curJ].flag=true;
    
    //if ?
    else
        this->grid[curI][curJ].question=true;
}

//ALL edges are .discovered==true 
//.visited is used for recursion help
void board::expandZeros(int curI, int curJ)
{
    //mark .visted=true
    //prevents infinite loop so it doesn't go back to itself
    this->grid[curI][curJ].visited=true;
    
    //up
    this->grid[curI-1][curJ].discovered=true;
    if(this->grid[curI-1][curJ].adjMines==0 && this->grid[curI-1][curJ].edge==false && this->grid[curI-1][curJ].visited!=true)
        this->expandZeros(curI-1, curJ);
        
    //up right
    this->grid[curI-1][curJ+1].discovered=true;
    if(this->grid[curI-1][curJ+1].adjMines==0 && this->grid[curI-1][curJ+1].edge==false && this->grid[curI-1][curJ+1].visited!=true)
        this->expandZeros(curI-1, curJ+1);
    
    //right
    this->grid[curI][curJ+1].discovered=true;
    if(this->grid[curI][curJ+1].adjMines==0 && this->grid[curI][curJ+1].edge==false && this->grid[curI][curJ+1].visited!=true)
        this->expandZeros(curI, curJ+1);
    
    //down right
    this->grid[curI+1][curJ+1].discovered=true;
    if(this->grid[curI+1][curJ+1].adjMines==0 && this->grid[curI+1][curJ+1].edge==false && this->grid[curI+1][curJ+1].visited!=true)
        this->expandZeros(curI+1, curJ+1);
    
    //down
    this->grid[curI+1][curJ].discovered=true;
    if(this->grid[curI+1][curJ].adjMines==0 && this->grid[curI+1][curJ].edge==false && this->grid[curI+1][curJ].visited!=true)
        this->expandZeros(curI+1, curJ);
    
    //down left
    this->grid[curI+1][curJ-1].discovered=true;
    if(this->grid[curI+1][curJ-1].adjMines==0 && this->grid[curI+1][curJ-1].edge==false && this->grid[curI+1][curJ-1].visited!=true)
        this->expandZeros(curI+1, curJ-1);
    
    //left
    this->grid[curI][curJ-1].discovered=true;
    if(this->grid[curI][curJ-1].adjMines==0 && this->grid[curI][curJ-1].edge==false && this->grid[curI][curJ-1].visited!=true)
        this->expandZeros(curI, curJ-1);
    
    //up left
    this->grid[curI-1][curJ-1].discovered=true;
    if(this->grid[curI-1][curJ-1].adjMines==0 && this->grid[curI-1][curJ-1].edge==false && this->grid[curI-1][curJ-1].visited!=true)
        this->expandZeros(curI-1, curJ-1);
}

