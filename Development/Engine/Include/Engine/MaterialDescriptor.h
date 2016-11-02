#pragma once

namespace NekoEngine
{
	/** 
		材质描述器
		@remark
			设计阶段的材质节点整合器
			将需要的各种类型节点添加到描述器中并连接它们既可生成对应的Pixel Shader文件
	*/
	class MaterialDescriptor : public ManagedObject
	{
		DECLARE_RTTI_CLASS( MaterialDescriptor, ManagedObject, ClassFlag_Intrinsic| ClassFlag_Serializable )
	public:
		MaterialDescriptor( );

		virtual ~MaterialDescriptor( );

		/**
			添加节点
			@param
				ClassInfo 节点类型
			@return
				添加后的节点
		*/
		MaterialNode* AddNode( RTTIClass* ClassInfo );

		/// 添加节点的模板封装
		template<typename TClass>
		TClass* AddNode( )
		{
			return RTTI_CAST(TClass, AddNode( RTTI_CLASS_INFO(TClass) ) );
		}

		/// 获取材质节点
		MaterialNode* GetNode( dword ID );

		/// 移除一个材质节点
		void RemoveNode( dword ID );

		/// 清除所有材质节点
		void Clear( );

		/// 编译并生成Shader代码
		void GenerateShaderFile( const wchar* FilenName );

		/// 获取编译器
		MaterialCompiler* GetCompiler( ){ return mCompiler; }

		/// 遍历第一个节点
		MaterialNode* GetFirstNode( );

		/// 遍历下一个节点
		MaterialNode* GetNextNode( );

		/// 获取终端节点
		MaterialNode* GetTerminateNode( );

	public:
		virtual void Serialize( BinarySerializer& Ser );

	private:
		typedef dynamic_hash<dword, MaterialNode*> NodeMap;
		NodeMap				mNodeMap;
		MaterialCompiler*	mCompiler;
		dword				mAccIDGen;
		NodeMap::iterator*	mIterator;
		
	};

	typedef TRefPtr<MaterialDescriptor> MaterialDescriptorPtr;
}
