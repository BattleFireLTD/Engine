#pragma once
#include <vector>
namespace Alice{
	class TTree{
	public:
		TTree* mParent;
		TTree* mChild;
		TTree* mLeftSibling;
		TTree* mRightSibling;
		TTree* mLastChild;
	public:
		TTree();
		virtual ~TTree();
		bool AppendChild(TTree*node);
		bool InsertBefore(TTree*before);
		bool InsertAfter(TTree*after);
		void RemoveChild(TTree*node);
		bool IsParent(TTree*node);
		void Clean();
		template<typename T>
		T* LeftSibling(){
			return (T*)mLeftSibling;
		}
		template<typename T>
		T* RightSibling(){
			return (T*)mRightSibling;
		}
		template<typename T>
		T* Child(){
			return (T*)mChild;
		}
		template<typename T>
		T* Parent(){
			return (T*)mParent;
		}
		template<typename T>
		T* LastChild() {
			return (T*)mLastChild;
		}
	};
}