//Andrew Hoyle
//struct used with more intelligent algorithms to weigh your guesses

#ifndef TILEINFO_H
#define	TILEINFO_H

struct tileInfo
{
    int row;
    int col;
    float risk;
    
    bool operator==(const tileInfo& ti) const
    {
        if(row==ti.row && col==ti.col && risk==ti.risk)
            return true;
        else
            return false;
    }
    
    friend std::ostream& operator << (std::ostream& out, const tileInfo& ti)
    {
        out << "(" << ti.row << ", " << ti.col << ")";
        return out;
    }
    
};


#endif	/* TILEINFO_H */

