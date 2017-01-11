#pragma once

typedef struct tagIndex
{
#ifdef _USE_INDEX16
	unsigned short	_0, _1, _2;		/// WORD, 16∫Ò∆Æ ¿Œµ¶Ω∫
#else
	unsigned long	_0, _1, _2;		/// WORD, 32∫Ò∆Æ ¿Œµ¶Ω∫
#endif
}INDEX, *LPINDEX;
