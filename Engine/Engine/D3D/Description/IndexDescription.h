#pragma once

struct INDEX
{
#ifdef _USE_INDEX16
	WORD	_0, _1, _2;		/// WORD, 16��Ʈ �ε���
#else
	DWORD	_0, _1, _2;		/// WORD, 32��Ʈ �ε���
#endif
};