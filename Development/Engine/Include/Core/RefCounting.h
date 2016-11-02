#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Basic
	*  @{
	*/

	/**
		引用计数对象
		@remark
			需要进行引用计数对象请派生此类
	*/
	class RefObject
	{
	public:
		RefObject(): NumRefs(0) {}
		virtual ~RefObject() {  }

		/**
			增加引用
		*/
		void AddRef()
		{
			NumRefs++;
		}

		/** 
			移除引用
			@return
				移除引用后剩下的引用
		*/
		int RemoveRef()
		{
			--NumRefs;

			RefObject_OnDecrease( NumRefs );

			if( NumRefs == 0)
			{
				RefObject_OnDestroy( );

				return 0;
			}

			return NumRefs;
		}

		/** 
			获取引用数量
			@return
				引用数量
		*/
		int GetRefCount( ){ return NumRefs; }

		/** 
			对象销毁通知
			@remark
				重载此函数将在对象即将销毁前的通知
		*/
		virtual void RefObject_OnDestroy(  )
		{
			delete this;
		}

		/** 
			对象引用减少通知
			@remark
				使用此函数可以当作垃圾回收触发通知
			@param
				RefCount 剩下的引用计数
		*/
		virtual void RefObject_OnDecrease( int RefCount ){ }

	private:
		int NumRefs;
	};

	/** 
		智能指针对象
		@remark
			本模板类模板参数类需要派生RefObject对象类，进而实现自动AddRef/RemoveRef机制
	*/
	template<typename ReferencedType>
	class TRefPtr
	{
		typedef ReferencedType* ReferenceType;
	public:

		TRefPtr():
			Reference(null)
		{}

		/** 
			构造函数
			@remark
				传入引用类指针
		*/
		TRefPtr(ReferencedType* InReference,bool bAddRef = true)
		{
			Reference = InReference;
			if(Reference && bAddRef)
			{
				Reference->AddRef();
			}
		}

		/** 
			拷贝构造函数
		*/
		TRefPtr(const TRefPtr& Copy)
		{
			Reference = Copy.Reference;
			if(Reference)
			{
				Reference->AddRef();
			}
		}

		/** 
			析构函数
		*/
		~TRefPtr()
		{
			if(Reference)
			{
				Reference->RemoveRef();
			}
		}

		/** 
			释放智能指针的对象引用
		*/
		void Release( )
		{
			*this = null;
		}

		/** 
			验证智能指针有效性
			@return
				智能指针有引用时，返回true，否则false
		*/
		bool Valid( ) const
		{
			return Reference != null;
		}

		/** 
			验证智能指针是否为空
			@return
				智能指针为空时，返回true，否则false
		*/
		bool Empty( ) const
		{
			return Reference == null;
		}

		/** 
			智能指针拷贝引用
			@param 
				InReference 传入引用对象指针
			@return
				返回本对象引用
		*/
		TRefPtr& operator=(ReferencedType* InReference)
		{
			ReferencedType* OldReference = Reference;
			Reference = InReference;
			if(Reference)
			{
				Reference->AddRef();
			}
			if(OldReference)
			{
				OldReference->RemoveRef();
			}
			return *this;
		}
		
		/** 
			智能指针拷贝引用
			@param 
				InReference 传入引用对象引用
			@return
				返回本对象引用

		*/
		TRefPtr& operator=(const TRefPtr& InPtr)
		{
			return *this = InPtr.Reference;
		}

		/** 
			智能指针比较
			@param
				Other 另外一个智能指针
			@return
				智能指针是否相等
		*/
		bool operator==(const TRefPtr& Other) const
		{
			return Reference == Other.Reference;
		}

		/** 
			智能指针比较
			@param
				Other 另外一个智能指针指针
			@return
				智能指针是否相等
		*/
		bool operator==(ReferencedType* Other) const
		{
			return Reference == Other;
		}

		/** 
			获取引用对象
			@return
				返回引用类型
		*/
		ReferencedType* operator->() const{	return Reference;}

		/** 
			获取引用对象
			@return
				返回引用类型
		*/
		ReferencedType* GetPtr( ) const{return Reference;}

		/** 
			获取引用对象
			@return
				返回引用类型
		*/
		ReferencedType*& GetPtr( ){	return Reference;}

		/** 
			转换引用对象类型
			@return
				返回引用类型
		*/
		template<typename TClass>
		TClass* CastTo( ) const{return dynamic_cast<TClass*>( Reference );}

		/** 
			获取引用对象
			@return
				返回引用类型
		*/
		operator ReferenceType() const{	return Reference;}

	private:
		ReferencedType* Reference;
	};

	/** @} */
	/** @} */
}