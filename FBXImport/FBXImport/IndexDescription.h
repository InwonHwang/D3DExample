#pragma once

typedef struct tagIndex
{
#ifdef _USE_INDEX16
	unsigned short	_0, _1, _2;		/// WORD, 16��Ʈ �ε���
#else
	unsigned long	_0, _1, _2;		/// WORD, 32��Ʈ �ε���
#endif
}INDEX, *LPINDEX;
