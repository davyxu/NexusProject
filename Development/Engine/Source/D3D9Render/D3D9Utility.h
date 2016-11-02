#pragma once

namespace NekoEngine
{
	//class FD3DRefCountedObject
	//{
	//public:
	//	FD3DRefCountedObject(): NumRefs(0) {}
	//	virtual ~FD3DRefCountedObject() { Assert(!NumRefs); }
	//	void AddRef()
	//	{
	//		NumRefs++;
	//	}
	//	void Release()
	//	{
	//		if(--NumRefs == 0)
	//		{
	//			delete this;
	//		}
	//	}
	//private:
	//	int NumRefs;
	//};


	//template<typename ReferencedType>
	//class TD3DRef
	//{
	//	typedef ReferencedType* ReferenceType;
	//public:

	//	TD3DRef():
	//	  Reference(NULL)
	//	  {}

	//	  TD3DRef(ReferencedType* InReference,bool bAddRef = true )
	//	  {
	//		  Reference = InReference;
	//		  if(Reference && bAddRef)
	//		  {
	//			  Reference->AddRef();
	//		  }
	//	  }

	//	  TD3DRef(const TD3DRef& Copy)
	//	  {
	//		  Reference = Copy.Reference;
	//		  if(Reference)
	//		  {
	//			  Reference->AddRef();
	//		  }
	//	  }

	//	  ~TD3DRef()
	//	  {
	//		  if(Reference)
	//		  {
	//			  Reference->Release();
	//		  }
	//	  }

	//	  TD3DRef& operator=(ReferencedType* InReference)
	//	  {
	//		  ReferencedType* OldReference = Reference;
	//		  Reference = InReference;
	//		  if(Reference)
	//		  {
	//			  Reference->AddRef();
	//		  }
	//		  if(OldReference)
	//		  {
	//			  OldReference->Release();
	//		  }
	//		  return *this;
	//	  }

	//	  TD3DRef& operator=(const TD3DRef& InPtr)
	//	  {
	//		  return *this = InPtr.Reference;
	//	  }

	//	  bool operator==(const TD3DRef& Other) const
	//	  {
	//		  return Reference == Other.Reference;
	//	  }

	//	  ReferencedType* operator->() const
	//	  {
	//		  return Reference;
	//	  }

	//	  operator ReferenceType() const
	//	  {
	//		  return Reference;
	//	  }

	//	  ReferencedType** GetInitReference()
	//	  {
	//		  *this = NULL;
	//		  return &Reference;
	//	  }

	//	  // RHI reference interface.

	//	  friend bool IsValidRef(const TD3DRef& Ref)
	//	  {
	//		  return Ref.Reference != NULL;
	//	  }

	//	  void Release()
	//	  {
	//		  *this = NULL;
	//	  }

	//private:
	//	ReferencedType* Reference;
	//};



}
