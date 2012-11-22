//Andrew Hoyle

//so all files can see this enumeration that indicates the
// nature of the next "click" to be played

#ifndef MINESWEEPERENUM_H
#define	MINESWEEPERENUM_H

//forward declaration
class board;

// shows next click type
enum clickType {click, flag, question};

//function pointer to attach algorithm to playTheGame()
typedef void (*Algo)(board&, bool&, int&, int&, clickType&);

#endif	/* MINESWEEPERENUM_H */

