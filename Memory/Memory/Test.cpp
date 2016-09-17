#include <iostream>

class LinearAllocator
{
public:
    LinearAllocator(char* pBuffer, int bufferSize) :
        m_pBuffer(pBuffer),
		m_bufferSize(bufferSize),
        m_currentOffset(0)
    {
    }
    void* Alloc(size_t size)
    {
       void* pMemToReturn = m_pBuffer + m_currentOffset;
       m_currentOffset += size;
       return pMemToReturn;
    }
    void Free(void* pMem)
    {
       // We can't easily free memory in this type of allocator.
       // Therefore we just ignore this... or you could assert.
    }
private:
    char* m_pBuffer;
    int m_bufferSize;
    int m_currentOffset;
};

class Temp
{
public:
	Temp() {}
	~Temp() {}

	int _data0;
	int _data1;
	int _data2;
};

int main()
{
	char *buffer = new char[1024]; //미리 할당된 메모리 블럭

	LinearAllocator allocator(buffer, sizeof(char) * 1024);

	Temp * temp = static_cast<Temp *>(allocator.Alloc(sizeof(Temp)));

	temp->_data0 = 1;
	temp->_data1 = 2;
	temp->_data2 = 3;

	std::cout << temp->_data0 << " " << temp->_data1 << " " << temp->_data2 << std::endl;


	
	delete[] buffer;
	return 0;
}