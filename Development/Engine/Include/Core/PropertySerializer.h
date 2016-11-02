#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Reflection
	*  @{
	*/


	class TextFile;
	class IniFile;
	class IniSection;

	class PropertySerializer;

	/** 
		属性序列化错误
	*/
	class PropertySerializerErrorCatcher
	{
	public:
		PropertySerializerErrorCatcher( PropertySerializer& Ser );

		virtual ~PropertySerializerErrorCatcher( );

		/** 
			写入一个错误
		*/
		void WriteError( const WString& Section, const WString& Key );

		/// 是否有错误
		bool HasError( ){ return mErrorArray.size() > 0; }

		/// 错误数量
		dword GetErrorCount( ){return mErrorArray.size(); }

		/// 错误描述
		bool GetErrorDesc( dword Index, WString& ErrorSec, WString& ErrorKey );

	private:
		PropertySerializer& mSer;

		struct ErrorDesc 
		{
			WString mSection;
			WString mKey;
			ErrorDesc( const WString& Sec, const WString& Key )
				: mSection( Sec )
				, mKey( Key )
			{

			}
		};

		typedef std::vector<ErrorDesc> ErrorArray;
		ErrorArray mErrorArray;
	};


	/** 
		INI格式属性序列化
	*/
	class PropertySerializer
	{
	public:
		PropertySerializer( const wchar* FileName, bool IsLoading );

		virtual ~PropertySerializer( );

		/// 序列化Ini的段
		bool SerializeSection( const WString& Name );

		/// 获取当前序列化的段
		const WString& GetSection( );

		/// 当前是否为读取状态
		bool IsLoading( );

		/// 当前是否为保存状态
		bool IsSaving( );

		/// 获取错误对象
		PropertySerializerErrorCatcher* GetErrorObject( );

		/// 设置Key
		friend PropertySerializer& operator<<( PropertySerializer& Ser, const wchar_t* Key )
		{
			// 作为key
			Ser.mKey = Key;

			return Ser;
		}

		/// 使用value_泛型匹配类型，注意检查类型正确性
		template<typename TType>
		friend PropertySerializer& operator<<( PropertySerializer& Ser, const TType& Data )
		{
			PropertySerializerErrorCatcher* ErrorObj = Ser.GetErrorObject();

			if ( Ser.mSectionData == null )
			{
				if ( ErrorObj != NULL )
					ErrorObj->WriteError( L"Section not found", Ser.mKey );

				return Ser;
			}

			assert( Ser.IsSaving() );

			if ( Ser.IsSaving() )
			{
				Ser.mSectionData->AddData( mKey, value_tostringW(Data) );
			}

			return Ser;
		}

		/// 使用value_泛型匹配类型，注意检查类型正确性
		template<typename TType>
		friend PropertySerializer& operator<<( PropertySerializer& Ser, TType& Data )
		{				
			PropertySerializerErrorCatcher* ErrorObj = Ser.GetErrorObject();

			if ( Ser.mSectionData == null )
			{
				if ( ErrorObj != NULL )
					ErrorObj->WriteError( L"Section not found", Ser.mKey );

				return Ser;
			}
			if ( Ser.IsSaving() )
			{
				Ser.mSectionData->AddData( Ser.mKey, value_tostringW( Data ).c_str() );
			}
			else
			{				
				WString Result;
				if ( Ser.mSectionData->Read( Ser.mKey, Result ) )
				{
					value_parse( Result.c_str(), Data );
				}
				else 
				{

					if ( ErrorObj != NULL )
						ErrorObj->WriteError( Ser.mSectionData->GetName(), Ser.mKey );
				}

			}

			return Ser;
		}
		

		/// 使用value_泛型匹配类型，注意检查类型正确性
		template<typename TType>
		friend PropertySerializer& operator<<( PropertySerializer& Ser, std::vector<TType>& DataArray )
		{				
			PropertySerializerErrorCatcher* ErrorObj = Ser.GetErrorObject();

			if ( Ser.mSectionData == null )
			{
				if ( ErrorObj != NULL )
					ErrorObj->WriteError( L"Section not found", Ser.mKey );

				return Ser;
			}
			if ( Ser.IsSaving() )
			{
				for ( dword i = 0;i< DataArray.size();i++)
				{
					Ser.mSectionData->AddData( Ser.mKey, value_tostringW( DataArray[i] ).c_str() );
				}
			}
			else
			{	

				Ser.mSectionData->SetSearchKey( Ser.mKey );

				WString Result;
				while( Ser.mSectionData->GetNextPairData( Result ) )
				{
					TType Data;
					value_parse( Result.c_str(), Data );

					DataArray.push_back( Data );
				}
			}

			return Ser;
		}

	public:
		IniFile*		mIniFile;

		WString			mKey;

		IniSection*		mSectionData;

		WString*		mSaveFileName;

		typedef std::list<PropertySerializerErrorCatcher*> ErrorObjectStack;
		ErrorObjectStack mErrorObjectStack;
	};

	//void GameEngine::SerializeConfig( PropertySerializer& Ser )
	//{
	//	Ser.SerializeSection(L"Option" )
	//		<< L"UseThreadRendering" << mUseThreadRendering
	//		<< L"UseThreadStreaming" << mUseThreadStreaming
	//		<< L"UseShaderCache" << mUseShaderCache
	//		<< L"ShowGrid"		<< mShowGrid;


	//	Ser.SerializeSection( L"Debug" ) 
	//		<< L"CameraPosition";
	//}

	/** @} */
	/** @} */
}