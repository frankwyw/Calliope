#ifndef __NONMOVEABLE__HPP__
#define __NONMOVEABLE__HPP__

namespace honoka
{
	class nonmoveable
	{
	protected:
		noncopyable() {};
		~noncopyable() {};
	private:
		noncopyable(const noncopyable&&) = delete;
		noncopyable&& operator =(const noncopyable&&) = delete;
	};
}

#endif
