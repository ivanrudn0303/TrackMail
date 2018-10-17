template<typename T>
class Stack
{
	uint32_t Size, Capacity, DestructCounter;
	T* Data;
public:
	Stack();
	Stack(uint32_t size);
	Stack(const std::initializer_list<T>&);
	~Stack();
	bool empty() const;
	uint32_t size() const;
	void push(const T&);
	void clear();
	uint32_t destruct();
	T& pop();
	T& pop(bool*);

};

template<typename T>
Stack<T>::Stack(): Data(nullptr), Size(0), Capacity(0)
{}

template<typename T>
Stack<T>::Stack(uint32_t size):Size(0), Capacity(2 * size), DestructCounter(0)
{
	Data = static_cast<T*>(std::malloc(Capacity * sizeof(T)));
}

template<typename T>
Stack<T>::Stack(const std::initializer_list<T>& array):Size(array.size()), Capacity(2 * array.size()), DestructCounter(array.size())
{
	Data = static_cast<T*>(std::malloc(Capacity * sizeof(T)));
	T *pointer = Data;
	for (auto i = array.begin(); i < array.end(); ++i)
		new(pointer++) T(*i);
}

template<typename T>
Stack<T>::~Stack()
{
	if (Capacity != 0)
	{
		clear();
		free(Data);
	}
}

template<typename T>
bool Stack<T>::empty() const
{
	return (Size == 0);
}

template<typename T>
uint32_t Stack<T>::size() const
{
	return Size;
}

template<typename T>
void Stack<T>::clear()
{
	Size = 0;
	for (uint32_t i = 0; i < DestructCounter; ++i)
		(Data + i)->~T();
	DestructCounter = 0;
}

template<typename T>
uint32_t Stack<T>::destruct()
{
	uint32_t i;
	for (i = Size; i < DestructCounter; ++i)
		(Data + i)->~T();
	i -= Size;
	DestructCounter = Size;
	return uint32_t(i);
}

template<typename T>
void Stack<T>::push(const T &PushData)
{
	if (Size == Capacity)
	{
		if (!(this->empty()))
			Capacity *= 2;
		else
			Capacity = 2;
		Data = static_cast<T*>(std::realloc(Data, Capacity * sizeof(T)));
	}
	if (Size == DestructCounter)
		DestructCounter++;
	else
		(Data + Size)->~T();
	new(Data + Size++) T(PushData);
}

template<typename T>
T& Stack<T>::pop()
{
	if (this->empty())
		throw(std::exception("Empty Stack\n"));
	else
		return Data[--Size];

}

template<typename T>
T& Stack<T>::pop(bool *state)
{
	if (!(this->empty()))
	{
		* state = true;
		return Data[--Size];
	}
	else
	{
		*state = false;
		return T();
	}
}
