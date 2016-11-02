#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Render
	*  @{
	*/


	/** 
		Shader参数对应表
	*/
	class ShaderParameterMap
	{
	public:
		/** 
			查找一个Shader参数对应的寄存器信息
			@param
				ParameterName 参数名称
			@param
				OutBaseRegisterIndex 返回的寄存器开始索引
			@param
				OutNumRegisters 返回的变量占用的寄存器数量
		*/
		bool FindParameterAllocation(const char* ParameterName,word& OutBaseRegisterIndex,word& OutNumRegisters) const;

		/** 
			D3D底层分析Shader时添加的内容
			@param
				ParameterName 参数名称
			@param
				OutBaseRegisterIndex 寄存器开始索引
			@param
				OutNumRegisters 变量占用的寄存器数量
		*/
		void AddParameterAllocation(const char* ParameterName,word BaseRegisterIndex,word NumRegisters);

	private:
		struct ParameterAllocation
		{
			word BaseRegisterIndex;
			word NumRegisters;
		};
		dynamic_hash<AString,ParameterAllocation> ParameterMap;
	};


	/** 
		Shader参数
		@remark
			保存有一个Shader参数设置入口
	*/
	class ShaderParameter
	{
	public:
		ShaderParameter( );

		/** 
			在Shader参数表中查询参数名称,将寄存器保存于本对象
			@param
				ParameterMap 参数表
			@param
				ParameterName 参数名
		*/
		void Bind(const ShaderParameterMap& ParameterMap,const char* ParameterName );

		/** 
			是否已经绑定
		*/
		bool IsBound() const { return mNumRegisters > 0; }

		/** 
			取得基础寄存器索引
		*/
		dword GetBaseRegisterIndex() const { return mBaseRegisterIndex; }

		/** 
			获得变量占用的寄存器数
		*/
		dword GetNumRegisters() const { return mNumRegisters; }

		/// 二进制序列化操作符重载
		friend BinarySerializer& operator <<( NekoEngine::BinarySerializer& Ser, ShaderParameter& Data )
		{
			Ser << Data.mBaseRegisterIndex
				<< Data.mNumRegisters;

			return Ser;
		}

	private:
		word mBaseRegisterIndex;
		word mNumRegisters;
	};


	/** 
		Shader编译环境
	*/
	struct ShaderCompilerEnvironment
	{
		// IncludeFileName, FileContent
		typedef dynamic_hash<AString,AString> StringPair;

		/** 
			包含文件列表
		*/
		StringPair mIncludeFileMap;

		/** 
			宏映射
		*/
		StringPair mMacroDefineMap;
	};

	/** @} */
	/** @} */
}
