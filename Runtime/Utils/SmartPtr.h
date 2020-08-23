#pragma once

template<typename T>
class SmartPtr
{
public:
	SmartPtr(T*ptr=nullptr) :mPtr(ptr) { if (nullptr!=mPtr){ mPtr->retain(); }}
	~SmartPtr() { if (mPtr != nullptr) { mPtr->release(); } }
	T*mPtr;
	T*Release() { if (mPtr != nullptr) { mPtr->release(); }return mPtr; }
	T* operator ->() { return mPtr; }
	T* operator ->() const { return mPtr; }
	T& operator*() {  return *mPtr;}
	bool operator==(T*ptr) { return mPtr == ptr; }
	bool operator!=(T*ptr) { return mPtr != ptr; }
	bool operator==(SmartPtr<T>&r) { return mPtr == r.mPtr; }
	void operator=(SmartPtr<T>&r) {
		if (r.mPtr!=nullptr){
			r.mPtr->retain();
		}
		if (mPtr!=nullptr){
			mPtr->release();
		}
		mPtr = r.mPtr;
	}

	void operator=(T*ptr) { 
		if (ptr != nullptr) {
			ptr->retain();
		}
		if (mPtr != nullptr)
			mPtr->release(); 
		mPtr = ptr;
	}
};
