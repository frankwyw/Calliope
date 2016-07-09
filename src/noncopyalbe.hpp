#ifndef __NONCOPYABLE__HPP__
#define __NONCOPYABLE__HPP__

namespace honoka
{
	class noncopyable
	{
	protected:
		noncopyable() {};
		~noncopyable() {};
	private:
		noncopyable(const noncopyable&) = delete;
		noncopyable& operator =(const noncopyable&) = delete;
	};
}

#endif
