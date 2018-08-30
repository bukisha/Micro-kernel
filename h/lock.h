#ifndef _LOCK_H_
#define _LOCK_H_
	
//----------------------------------------------------------------------

// Zabranjuje asinhrono preuzimanje.
#define lock asm {pushf; cli;}

// Dozvoljava asinhrono preuzimanje.
#define unlock asm popf

//----------------------------------------------------------------------

#endif

//----------------------------------------------------------------------
