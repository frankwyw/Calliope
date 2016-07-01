#include "noncopyable.hpp"

template <typename T>
struct Singleton : public noncopyable
{
public:
	static std::shared_ptr<T> get_instance()
	{
		if (instance == nullptr)
		{
			std::call_once(once, &Singleton::create);
		}
		return instance;
	}

private:
	static void create()
	{
		instance = std::make_shared<T>();
	}

	static std::once_flag once;
	static std::shared_ptr<T> instance;
};

template <typename T>
std::shared_ptr<T> Singleton<T>::instance = nullptr;

template <typename T>
std::once_flag Singleton<T>::once;
