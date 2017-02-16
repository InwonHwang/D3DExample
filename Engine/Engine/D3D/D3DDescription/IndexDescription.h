#pragma once

struct INDEX
{
#ifdef _USE_INDEX16
	WORD	_0, _1, _2;		/// WORD, 16∫Ò∆Æ ¿Œµ¶Ω∫
#else
	DWORD	_0, _1, _2;		/// WORD, 32∫Ò∆Æ ¿Œµ¶Ω∫
#endif
};