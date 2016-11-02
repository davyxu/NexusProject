#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Reflection
	*  @{
	*/

	struct NameData
	{
		enum{ NAME_SIZE = 128 };

		dword mHashCode;
		dword mIndex;				// 全局保存的name中的索引
		NameData* mNext;			// 链接
		wchar mName[NAME_SIZE];		// 有多长保存多少Name，不是每个都是128
	};

	/// 名称构建原则
	enum NameBuildType
	{
		NameBuild_Find,		///< 在已存在的NameTable中查找，没有找到，置为None
		NameBuild_Add,		///< 如果没有找到，自动添加
	};

	enum NameConst
	{
		NameConst_None = 0, // 这个字符串不能被Name化
	};

	typedef std::vector<NameData*> NameDataArray;


	/** 
		名称表
	*/
	class NameRef
	{
	public:
		enum{ NAMECONST_RESERVED_COUNT = 1024, NAME_NO_NUMBER = -1 };

		//引擎中段使用情况
		/*
			WString -->StrHashNoCase --> HashTable  --> Array --> Index --> App
			
			NameConst <----Const位于index的前段
			NAMECONST_RESERVED_COUNT
			Dynamic added Name  <- 动态添加的Name位于Const后面
		*/
		
		/// 默认构造
		NameRef( );

		/** 
			内部构造
			@param
				Index 名称索引
			@param
				Number 名称号码
		*/
		NameRef( dword Index, dword Number );

		/** 
			常量构造
		*/
		NameRef( dword c );

		/** 
			自动解析名称构造
			@param
				name 带有标示的名称,例如Actor_0
			@param
				buildtype 构建方式
			@param
				Number 额外给定的名称索引
			@param
				SpliteName 是否需要解析名称
		*/
		NameRef( const wchar* name, NameBuildType buildtype = NameBuild_Add, dword Number = NAME_NO_NUMBER, bool SpliteName = true);

		/// 拷贝
		NameRef& operator=( const NameRef& Other );


		/// 判定相等
		bool operator==( const NameRef& Other ) const
		{
			return mIndex == Other.mIndex && mNumber == Other.mNumber;
		}

		/// 判定不等
		bool operator!=( const NameRef& Other ) const
		{
			return mIndex != Other.mIndex || mNumber != Other.mNumber;
		}

		/// For std::map
		bool operator < ( const NameRef& Other ) const
		{
			return mIndex < Other.mIndex;
		}

		/// 获得key
		dword GetIndex( ) const;

		/// 获得HashCode
		dword GetHash( ) const;

		/// 获取副索引
		dword GetNumber( ) const;

		/// 取原字符串( Number == 0 )
		const wchar* ToStringFast( ) const ;

		/// 取原字符串 自动
		WString ToStringSafe( ) const;

		/// 名称是否有效
		bool IsValid() const;

		/// 直接往表中添加
		static bool AddNameConst( const wchar* name, dword index );

		static void StaticInit( );

		static void StaticExit( );

		/// 检查名字表情况
		static bool CheckNameTable( Logger* logger );

		static void Dump( Logger* logger );
		
		friend dword GetTypeHash( const NameRef& A )
		{
			return A.GetIndex() + A.GetNumber();
		}
	private:
		void RefreshDebugView( );
		static dword GenHashIndex( dword hashcode );

	private:
		const wchar* mContent;
		dword mIndex;
		dword mNumber;// 用于作为Class的ObjectIndex
		
		static NameDataArray GNameRefArray;
		static NameData* GNameDataHash[4096];
		static bool		 GNameTableInitialized;
	};

	
	/** @} */
	/** @} */
}