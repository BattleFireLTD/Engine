#pragma once
namespace Alice
{
	class LinkedList{
	public:
		LinkedList() :mNext(nullptr){
		}
		virtual void PushBack(LinkedList*node){
			if (mNext==nullptr){
				mNext = node;
			}else{
				if (mNext==node){
					return;
				}
				Next<LinkedList>()->PushBack(node);
			}
		}
		virtual void Remove(LinkedList*node){
			if (mNext!=nullptr){
				if (node==mNext){
					mNext = node->Next<LinkedList>();
					node->mNext = nullptr;
				}else{
					Next<LinkedList>()->Remove(node);
				}
			}
		}
		LinkedList*mNext;
		template<typename T>
		T* Next(){
			return (T*)mNext;
		}
	};
	class DoubleLinkedList{
	public:
		DoubleLinkedList() :mPrev(nullptr),mNext(nullptr){
		}
		virtual ~DoubleLinkedList(){
			LeaveList();
		}
		//move up
		void operator<<(int nGAP){
			DoubleLinkedList*nodeToShift = this;
			DoubleLinkedList*targetPos = Prev<DoubleLinkedList>();
			while (--nGAP){
				targetPos = targetPos->Prev<DoubleLinkedList>();
			}
			nodeToShift->InsertBefore(targetPos);
		}
		//move down
		void operator>>(int nGAP){
			DoubleLinkedList*nodeToShift = this;
			DoubleLinkedList*targetPos = Next<DoubleLinkedList>();
			while (--nGAP){
				targetPos = targetPos->Next<DoubleLinkedList>();
			}
			nodeToShift->InsertAfter(targetPos);
		}
		void Append(DoubleLinkedList*node){
			if (mNext!=nullptr){
				mNext->Append(node);
			}else{
				mNext = node;
				node->mPrev = this;
			}
		}
		void InsertBefore(DoubleLinkedList*node){
			LeaveList();
			if (node->mPrev!=nullptr){
				node->mPrev->mNext = this;
			}
			mPrev = node->mPrev;
			node->mPrev = this;
			mNext = node;
		}
		void InsertAfter(DoubleLinkedList*node){
			LeaveList();
			if (node->mNext != nullptr){
				node->mNext->mPrev = this;
			}
			mNext = node->mNext;
			node->mNext = this;
			mPrev = node;
		}
		void LeaveList(){
			if (mPrev!=nullptr){
				mPrev->mNext = mNext;
			}
			if (mNext!=nullptr){
				mNext->mPrev = mPrev;
			}
			mPrev = nullptr;
			mNext = nullptr;
		}
	public:
		DoubleLinkedList*mPrev;
		DoubleLinkedList*mNext;
	public:
		template<typename T>
		T* Next(){
			return (T*)mNext;
		}
		template<typename T>
		T* Prev(){
			return (T*)mPrev;
		}
	};
}