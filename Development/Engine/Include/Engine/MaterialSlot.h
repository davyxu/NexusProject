#pragma once

namespace NekoEngine
{
	class MaterialNode;
	class MaterialDescriptor;

	/** 
		材质节点连接槽
	*/
	class MaterialSlot
	{
	public:
		MaterialSlot( );

		/// 获取槽绑定的节点
		MaterialNode* GetNode( ){ return mNode; }

		/// 获取槽的名称
		const wchar* GetName( ){ return mName; }

		/// 这个槽没有连接任何节点，使用默认值
		bool IsFree( );	


		/**
			将槽与材质节点绑定
			@param
				Name 槽名称
			@param
				Node 绑定的节点
			@param
				Type 槽Shader值类型
			@param
				DefaultValue 默认生成代码
		*/
		void Bind( const wchar* Name, MaterialNode* Node, eMaterialValueType Type = MVT_None, const char* DefaultValue = "None" );

		/// 编译本槽
		void Compile(MaterialCompiler* Compiler);

		/// 合并代码
		void CombineCode( MaterialCompiler* Compiler );

		/// 取得结果
		MaterialMaskedValue GetResult( );

		/// 获取合并后的结果值
		const char* GetCombinedCodeResult( ){ return mCodeResult.c_str(); }

		/// 获取合并后的代码体
		const char* GetCombinedCodeBody( ){ return mCodeBody.c_str(); }

		
		/**
			连接到材质节点
			@param
				NodeID 要连接的材质节点
			@param
				Mask 连接分量
			@remark
				只支持1个输入节点,如果有多个输入,请使用Add操作
		*/
		void Connect( dword NodeID , eMaterialValueMask Mask = MVM_RGB );

		/// 将输入值作为值类型
		void SolveInputMaskAsValueType( );

		/// 断开与节点连接
		void Disconnect( MaterialNode* NodeDeleteSource );

		friend BinarySerializer& operator <<( NekoEngine::BinarySerializer& Ser, MaterialSlot*& Data )
		{			
			Ser << Data->mLinkedNodeID
				<< Data->mInputMask;

			return Ser;
		}

		/// 获取与这个槽连接的节点
		MaterialNode* GetLinkedNode( );
		
	private:
		friend class MaterialNode;

		// 对自己
		const wchar*		mName;	
		eMaterialValueType	mValueType;
		const char*			mDefaultValue;
		AString				mCodeBody;
		AString				mCodeResult;
		bool				mCompileFinish;
		MaterialNode*		mNode;

		// 对其他节点输出
		dword				mLinkedNodeID;
		eMaterialValueMask	mInputMask;
		dword				mInputResult;
		MaterialDescriptor*			mMaterial;

	};


	/** 
		材质节点
	*/
	class MaterialNode : public RTTIObject
	{
		DECLARE_RTTI_CLASS( MaterialNode, RTTIObject, ClassFlag_Intrinsic| ClassFlag_Serializable )

	public:
		MaterialNode( );

		virtual ~MaterialNode( );

		/// 获取输出的代码索引
		dword GetOutputCodeIndex( ) { return mOutputResult; }

		/// 编译代码,编译所有的输入槽
		virtual void Compile( MaterialCompiler* Compiler );	

		/// 合并代码
		virtual void CombineCode( MaterialCompiler* Compiler);

		/// 是否为最终节点
		virtual bool IsTerminateNode( ){ return false; }

		/// 材质节点的名称
		virtual WString GetName( ){ return L"MaterialNode"; }

		/// 获取节点ID
		dword GetID( ){ return mID; }

		/// 遍历节点上的槽
		MaterialSlot* GetSlot( dword Index );

		/// 获取槽的数量
		dword GetSlotCount( );

		/// 断开这个节点的所有连接
		void Disconnect( );

		/// 通过槽绑定到Node时的名称找到实例
		MaterialSlot* GetSlotByName( const wchar* Name );

	public:
		/// 节点初始化
		virtual void Init(  MaterialCompiler* Compiler, MaterialDescriptor* Mat  );

		virtual void Serialize( BinarySerializer& Ser );

		void AddOutputSlot( MaterialSlot* Slot );

		void RemoveOutputSlot( MaterialSlot* Slot );

		static void StaticInitProperty( );

	private:
		friend class MaterialSlot;

		void AddSlot( MaterialSlot* Slot );
		

	protected:
		friend class MaterialDescriptor;
		dword mID;

		typedef std::vector<MaterialSlot*> SlotArray;
		SlotArray			mSlotArray;
		dword				mOutputResult;// 由派生类在Compile中决定这个代码的生成
		bool				mCompileFinish;
		MaterialDescriptor*	mMaterial;

		typedef std::list<MaterialSlot*> SlotList;

		// 对外连接的槽
		SlotList			mOutputSlotList;

		/// 为编辑器保存准备
		Vector2				mScenePos;

	};

}
