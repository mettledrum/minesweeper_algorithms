//Andrew Hoyle

#ifndef TILE_H
#define	TILE_H

struct tile
{
    
//should make private eventually so only board can mess with tiles--------------
//    template <int, int> friend class board;
//    
//private:
//should make private eventually so only board can mess with tiles--------------

    //makes board copy constructor easier to implement
    tile& operator=(const tile& t)
    {
        edge=t.edge;
        question=t.question;
        flag=t.flag;
        mine=t.mine;
        discovered=t.discovered;
        adjMines=t.adjMines;
        visited=t.visited;
        adjUnknowns=t.adjUnknowns;
        adjFlags=t.adjFlags;
        adjQuestions=t.adjQuestions;
        
        return *this;
    }
    
    //around the grid
    bool edge;
    
    //can change with "click"
    bool question;
    bool flag;
    
    bool mine;
    bool discovered;
    
    //populated by the board class in its constructor
    int adjMines;
    
    //used by expandZeros() in board class for recursion
    bool visited;
    
    //updated by the board class
    int adjUnknowns;
    int adjFlags;
    int adjQuestions;
};



#endif	/* TILE_H */

