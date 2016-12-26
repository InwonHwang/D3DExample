#pragma once

#pragma once

// enum 에서 플래그 값을 정의하지 않고 계산하도록 하기 위해
#define FLAG(x) (1<<(x)) 

template <typename T>
class BitFlags
{
private:
	T value;

public:
	BitFlags() : value(0) {}
	~BitFlags() {}

	bool check(T place) const;	// 해당 자리수의 비트가 1인지 확인
	void set(T place);			// 해당 자리수의 비트를 1로 변경
	void reset(T place);		// 해당 자리수의 비트를 0으로 변경
	void clear();
};

typedef BitFlags<unsigned char> flags8;
typedef BitFlags<unsigned short> flags16;
typedef BitFlags<unsigned int> flags32;

template<typename T>
inline bool BitFlags<T>::check(T place) const
{
	return (value & FLAG(place)) != 0;
}

template<typename T>
void BitFlags<T>::set(T place)
{
	value |= FLAG(place);
}

template<typename T>
inline void BitFlags<T>::reset(T place)
{
	value &= ~FLAG(place);
}

template<typename T>
inline void BitFlags<T>::clear()
{
	value = 0;
}
