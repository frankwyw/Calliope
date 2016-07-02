#include <memory>
#include <mutex>

template <typename T>
struct Singleton
{
public:
	template <typename ...ARGS>
	static std::shared_ptr<T> get_instance(ARGS&&... arg)
	{
		if (instance == nullptr)
		{
			std::call_once(once, &Singleton::create<ARGS&&...>, arg...);
		}
		return instance;
	}

	static std::shared_ptr<T> get_instance()
	{
		if (instance == nullptr)
		{
			std::call_once(once, &Singleton::create0);
		}
		return instance;
	}

private:
    static void create0()
	{
        instance = std::make_shared<T>();
	}

	template <typename ...ARGS>
	static void create(ARGS&&... arg)
	{
		instance = std::make_shared<T>(std::forward<ARGS&&...>(arg...));
	}



	static std::once_flag once;
	static std::shared_ptr<T> instance;
};

template <typename T>
std::shared_ptr<T> Singleton<T>::instance = nullptr;

template <typename T>
std::once_flag Singleton<T>::once;
