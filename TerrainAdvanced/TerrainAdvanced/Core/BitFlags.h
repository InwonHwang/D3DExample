#pragma once

#pragma once

// enum ���� �÷��� ���� �������� �ʰ� ����ϵ��� �ϱ� ����
#define FLAG(x) (1<<(x)) 

template <typename T>
class BitFlags
{
private:
	T value;

public:
	BitFlags() : value(0) {}
	~BitFlags() {}

	bool check(T place) const;	// �ش� �ڸ����� ��Ʈ�� 1���� Ȯ��
	void set(T place);			// �ش� �ڸ����� ��Ʈ�� 1�� ����
	void reset(T place);		// �ش� �ڸ����� ��Ʈ�� 0���� ����
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
