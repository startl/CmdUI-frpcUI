// Copyright(c) 1999-2023 aslze
// Licensed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef ASL_Pointer_H
#define ASL_Pointer_H

#include <asl/atomic.h>

namespace asl {

template<class T>
struct SharedCore
{
	T*          p;
	AtomicCount rc;

	SharedCore() : p(0), rc(1) {}
	SharedCore(T* r) : p(r), rc(1) {}
	void ref() { ++rc; }
	void unref()
	{
		if (--rc <= 0)
		{
			delete p;
			p = 0;
			delete this;
		}
	}
};

template<class T2, class T>
const SharedCore<T2>* cast_(const SharedCore<T>* c)
{
	SharedCore<T2>* c2 = (SharedCore<T2>*)c;
	c2->p = c->p;
	return c2;
}


template <class T>
class Shared
{
public:
	int refcount() const { return _p->rc; }
	template<class T2> friend class Shared;
	Shared()
	{
		_p = 0;
	}
	Shared(const Shared<T>& p) : _p(p._p)
	{
		ref();
	}
	template<class T2>
	Shared(const Shared<T2>& p) : _p((SharedCore<T>*)cast_<T>(p._p)) 
	{
		ref();
	}
	Shared(T* p)
	{
		_p = new SharedCore<T>(p);
	}
	~Shared()
	{
		unref();
	}
	Shared& operator=(const Shared& r)
	{
		if (_p != r._p)
		{
			SharedCore<T>* t = _p;
			_p = r._p;
			ref();
			if (t)
				t->unref();
		}
		return *this;
	}
	template<class T2>
	Shared& operator=(const Shared<T2>& r)
	{
		*this = Shared(r);
		return *this;
	}
	Shared& operator=(T* r)
	{
		{
			SharedCore<T>* t = _p;
			_p = new SharedCore<T>(r);
			if (t)
				t->unref();
		}
		return *this;
	}
	Shared clone() const
	{
		return Shared(_p->p->clone());
	}
	T& operator*() const
	{
		return *_p->p;
	}
	T* operator->() const
	{
		return _p->p;
	}
	operator T*() const
	{
		return _p->p;
	}

	T* get() const { return _p->p; }
	
	template<class T2>
	Shared<T2> as() const
	{
		Shared<T2> sp;
		const T2* p = dynamic_cast<const T2*>(_p->p);
		if (p)
		{
			sp._p = (SharedCore<T2>*)_p;
			sp.ref();
		}
		return sp;
	}

	operator void*() const
	{
		return _p->p;
	}
	operator bool() const
	{
		return _p != 0 && _p->p != 0;
	}
	bool operator!() const
	{
		return _p == 0 || _p->p == 0;
	}
	bool operator==(T* r) const
	{
		return (_p && _p->p == r);
	}
	bool operator!=(T* r) const
	{
		return (_p == 0 || _p->p != r);
	}
	template <typename R>
	bool operator==(R r) const
	{
		return (_p == (const T*)(r) );
	}
	template <typename R>
	bool operator!=(R r) const
	{
		return (_p != (const T*)(r) );
	}
	bool operator<(const Shared &r) const
	{
		return _p < r._p;
	}
private:

	SharedCore<T>* _p;

	void ref()
	{
		if (_p)
			_p->ref();
	}

	void unref()
	{
		if (_p)
			_p->unref();
	}
};

/**
 * \deprecated This old class will disappear (use unique_ptr<T>, Shared<T>...)
 */
template <class T>
class Pointer
{
	Pointer(const Pointer& p): _p(0)
	{
	}
	Pointer& operator=(const Pointer& r)
	{
		_p = 0;
		return *this;
	}
public:
	Pointer(): _p(0)
	{
	}
	Pointer(T* p): _p(p)
	{
	}
	~Pointer()
	{
		delete _p;
	}
	Pointer& operator=(T* r)
	{
		delete _p;
		_p = r;
		return *this;
	}
	T& operator*() const
	{
		return *_p;
	}
	T* operator->() const
	{
		return _p;
	}
	operator T*() const
	{
		return _p;
	}
	operator void*() const
	{
		return _p;
	}
	operator bool() const
	{
		return _p != 0;
	}
	bool operator!() const
	{
		return _p == 0;
	}
	template <typename R>
	bool operator==(R r) const
	{
		return (_p == (const T*)(r) );
	}
	template <typename R>
	bool operator!=(R r) const
	{
		return (_p != (const T*)(r) );
	}
	bool operator<(const Pointer &r) const
	{
		return _p < r._p;
	}
private:

	T* _p;
};

template <class T>
class Pointer<T[]>
{
	Pointer(const Pointer& p): _p(0)
	{
	}
	Pointer& operator=(const Pointer& r)
	{
		_p = 0;
		return *this;
	}
public:
	Pointer(): _p(0)
	{
	}
	Pointer(T* p): _p(p)
	{
	}
	~Pointer()
	{
		delete [] _p;
	}
	Pointer& operator=(T* r)
	{
		delete [] _p;
		_p = r;
		return *this;
	}
	T& operator[](int i)
	{
		return _p[i];
	}
	const T& operator[](int i) const
	{
		return _p[i];
	}
	operator bool() const
	{
		return _p != 0;
	}
	operator T*() const
	{
		return _p;
	}
	operator void*() const
	{
		return _p;
	}
	bool operator!() const
	{
		return _p == 0;
	}
	template <typename R>
	bool operator==(R r) const
	{
		return (_p == (const T*)(r) );
	}
	template <typename R>
	bool operator!=(R r) const
	{
		return (_p != (const T*)(r) );
	}
	bool operator<(const Pointer &r) const
	{
		return _p < r._p;
	}
private:

	T* _p;
};

template <class T>
struct StaticSpace
{
	void construct() const
	{
		asl_construct((T*)_space);
	}
	void construct(const T& x) const
	{
		asl_construct_copy((T*)_space, x);
	}
	void destroy() const
	{
		asl_destroy((T*)_space);
	}

	T& operator*() { return *(T*)(_space); }
	T* operator->() { return (T*)(_space); }
	const T& operator*() const { return *(const T*)(_space); }
	const T* operator->() const { return (const T*)(_space); }
private:
	byte _space[sizeof(T)];
};

}
#endif
