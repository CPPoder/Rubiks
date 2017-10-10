#ifndef MUTEXPROTECTEDCLASS_HPP
#define MUTEXPROTECTEDCLASS_HPP


#include <thread>
#include <mutex>


template <typename T> class MutexProtectedClass
{
private:
	T value;
	std::mutex mutex;

public:
	MutexProtectedClass() : value() {}
	~MutexProtectedClass() {}
	MutexProtectedClass(MutexProtectedClass const & x) : value(x.value) {}
	MutexProtectedClass& operator=(MutexProtectedClass const & x)
	{
		mutex.lock();
		value = x.value;
		mutex.unlock();
	}
	T get() const
	{
		return value;
	}
	void set(T const & value)
	{
		mutex.lock();
		this->value = value;
		mutex.unlock();
	}
};





#endif //MUTEXPROTECTEDCLASS_HPP

