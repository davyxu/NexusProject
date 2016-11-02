#pragma once


namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Stream
	*  @{
	*/

	
	/**
		Ini段对象
	*/
	class IniSection
	{
	public:
		/// 添加数据
		void AddData( const WString& Key, const WString& Data );		

		/// 获取段名
		const WString& GetName( ){ return mName; }

		/// 读取数据
		bool Read( const WString& Key, WString& Data );

		/// 读取数据数组
		bool Read( const WString& Key, std::vector<WString>& DataArray );

		/// 保存内容到文件
		void DumpContent( TextFile& TFile );

		/// 遍历第一个数据
		bool GetFirstPair( WString& Key, WString& Data );

		/// 遍历下一个数据
		bool GetNextPair( WString& Key, WString& Data );

		/// 搜索第一个Key
		void SetSearchKey( const WString& Key );

		/// 搜索下一个Key的数据
		bool GetNextPairData( WString& Data );

	private:
		friend class IniFile;

		IniSection( const WString& Name );
		

		WString mName;

		typedef std::multimap<WString, WString> SettingsMultiMap;

		SettingsMultiMap mSettingsMultiMap;
		SettingsMultiMap::iterator mIterator;
	};


	/** 
		Ini格式配置文件,支持读写
		@remark
			请配合PropertySerilizer使用本类
	*/
	class IniFile
	{
	public:
		IniFile( );

		IniFile( const IniFile& Other );

		virtual ~IniFile( );

		/** 
			从内存流解析ini
			@param
				stream 来源内存流
			@param
				copymemory 是否拷贝来源内存流,避免crash
			@return
				文件是否打开正确
		*/
		bool Parse( DataStream* stream, bool copymemory = false );

		/** 
			从文件解析ini
			@param
				filename 打开的文件名
			@return
				文件是否打开正确
		*/
		bool Parse( const wchar_t* filename );

		/** 
			读取ini内容
			@param
				section 要读取的段
			@param
				key 要读取的key
			@param
				Result 返回的内容
			@return
				输入条件的条目是否存在
		*/
		bool Read( const WString& Section, const WString& Key, WString& Data );
		/** 
			获取段对象
			@param
				Section 段名
			@return
				段对象
		*/
		IniSection* QuerySection( const WString& Section );

		/**
			将Ini内容写入到数据流
		*/
		bool Dump( DataStream* Stream );

		/**
			将Ini内容写入到文件
		*/
		bool Dump( const wchar* FileName );


		/// 添加一个段
		IniSection* AddSection( const WString& Section );

		/**
			遍历第一个段
		*/
		IniSection* GetFirstSection( );

		/**
			遍历下一个段
		*/
		IniSection* GetNextSection( );

		/// 复制拷贝
		IniFile& operator = ( const IniFile& Other );

	private:
		bool ParseContent( TextFile& TFile );
		void DumpContent( TextFile& TFile );

	private:		
		typedef std::map<WString, IniSection*> SectionMap;
		SectionMap mSectionMap;
		SectionMap::iterator mIterator;

	};

	/** @} */
	/** @} */
}