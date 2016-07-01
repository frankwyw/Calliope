class nonmoveablr
{
protected:
	noncopyable() {};
	~noncopyable() {};
private:
	noncopyable(const noncopyable&&) = delete;
	noncopyable&& operator =(const noncopyable&&) = delete;
};
