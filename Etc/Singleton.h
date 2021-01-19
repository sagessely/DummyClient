#pragma once

template< class T >
class Singleton
{
public:
	Singleton(void) {}
	virtual ~Singleton(void) {}

	static T* GetInstance()
	{
		std::call_once(Singleton<T>::flag,
			[]() {
				Singleton<T>::instance = new T();
			}
		);
		return instance;
	}

private:
	static T* instance;
	static std::once_flag flag;
};

template < class T >
T* Singleton<T>::instance = nullptr;
template < class T >
std::once_flag Singleton<T>::flag;

#define GetSingleton(T) \
	T* p##T = Singleton<T>::GetInstance();
