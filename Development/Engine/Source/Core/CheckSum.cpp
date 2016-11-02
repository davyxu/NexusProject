#include "CorePCH.h"

#pragma warning (disable:4244) // warning C4244: 'initializing' : conversion from 'wchar' to 'NekoEngine::byte', possible loss of data

namespace NekoEngine
{
	const dword CRC32_POLY = 0x04c11db7;
	dword GCRCTable[256];

	#define UpperChar(c) ((c<'a' || c>'z') ? (c) : (c+'A'-'a'))


	bool CheckSum::mInitialized = false;

	void CheckSum::StaticInit( )
	{
		for( dword iCRC = 0; iCRC<256; iCRC++ )
			for( dword c = iCRC<<24, j=8; j!=0; j-- )
				GCRCTable[iCRC] = c = c & 0x80000000 ? (c << 1) ^ CRC32_POLY : (c << 1);

		BLZStaticInit( );

		mInitialized = true;
	}

	#define AUTOINIT if ( !mInitialized ) CheckSum::StaticInit( )
		

	dword CheckSum::MemCRC( const void* InData, int Length, dword CRC )
	{
		AUTOINIT;
		
		_byte* Data = (_byte*)InData;
		CRC = ~CRC;
		for( int i=0; i<Length; i++ )
			CRC = (CRC << 8) ^ GCRCTable[(CRC >> 24) ^ Data[i]];
		return ~CRC;
	}


	dword CheckSum::StrHashNoCase(const char* Data)
	{
		AUTOINIT;

		dword Hash=0;

		while( *Data )
		{
			char Ch = UpperChar(*Data);
			_byte  B  = Ch;
			Hash     = ((Hash >> 8) & 0x00FFFFFF) ^ GCRCTable[(Hash ^ B) & 0x000000FF];
			Data++;
		}

		return Hash;
	}

	dword CheckSum::StrHashNoCase(const wchar* Data)
	{
		AUTOINIT;

		dword Hash=0;

		while( *Data )
		{
			wchar Ch = UpperChar(*Data);
			_byte  B  = Ch;
			Hash     = ((Hash >> 8) & 0x00FFFFFF) ^ GCRCTable[(Hash ^ B) & 0x000000FF];
			B        = Ch>>8;
			Hash     = ((Hash >> 8) & 0x00FFFFFF) ^ GCRCTable[(Hash ^ B) & 0x000000FF];
			Data++;
		}

		return Hash;
	}

	dword CheckSum::StrHash(const wchar* Data)
	{
		AUTOINIT;

		dword Hash=0;

		while( *Data )
		{
			wchar Ch = *Data;
			_byte  B  = Ch;
			Hash     = ((Hash >> 8) & 0x00FFFFFF) ^ GCRCTable[(Hash ^ B) & 0x000000FF];
			B        = Ch>>8;
			Hash     = ((Hash >> 8) & 0x00FFFFFF) ^ GCRCTable[(Hash ^ B) & 0x000000FF];
			Data++;
		}

		return Hash;
	}

	dword CheckSum::StrHash(const char* Data)
	{
		AUTOINIT;

		dword Hash=0;

		while( *Data )
		{
			char Ch = *Data;
			_byte  B  = Ch;
			Hash     = ((Hash >> 8) & 0x00FFFFFF) ^ GCRCTable[(Hash ^ B) & 0x000000FF];
			Data++;
		}

		return Hash;
	}


	CheckSum::BLZBuffer CheckSum::mBLZBuffer;

	void CheckSum::BLZStaticInit( )
	{
		dword seed   = 0x00100001;
		dword index1 = 0;
		dword index2 = 0;
		int i;

		memset(mBLZBuffer, 0, sizeof(mBLZBuffer));

		/* Initialize the decryption buffer. */
		for (index1 = 0; index1 < 0x100; index1++) 
		{
			for(index2 = index1, i = 0; i < 5; i++, index2 += 0x100) 
			{
				dword temp1, temp2;
				seed  = (seed * 125 + 3) % 0x2AAAAB;
				temp1 = (seed & 0xFFFF) << 0x10;

				seed  = (seed * 125 + 3) % 0x2AAAAB;
				temp2 = (seed & 0xFFFF);

				mBLZBuffer[index2] = (temp1 | temp2);
			}
		}
	}

	dword CheckSum::BLZStrHash(const char* str, dword type ) 
	{
		AUTOINIT;

		dword seed1 = 0x7FED7FED;
		dword seed2 = 0xEEEEEEEE;
		dword ch;			/* One key character */

		/* Prepare seeds */
		while (*str != 0) 
		{
			ch = UpperChar(*str);
			Assert( ((type<<8) + ch) >=0 );
			Assert( ((type<<8) + ch) <BLZBUFFER_SIZE );
			seed1 = mBLZBuffer[(type<<8) + ch] ^ (seed1 + seed2);
			seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
			str++;
		}

		return seed1;
	}

	dword CheckSum::BLZStrHash(const wchar* str, dword type ) 
	{
		AUTOINIT;

		dword seed1 = 0x7FED7FED;
		dword seed2 = 0xEEEEEEEE;
		dword ch;			/* One key character */

		/* Prepare seeds */
		while (*str != 0) 
		{
			wchar c = UpperChar(*str);
			ch  = c & 0xff;
			Assert( ((type<<8) + ch) >=0 );
			Assert( ((type<<8) + ch) <BLZBUFFER_SIZE );
			seed1 = mBLZBuffer[(type<<8) + ch] ^ (seed1 + seed2);
			seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;

			ch    = c>>8;

			Assert( ((type<<8) + ch) >=0 );
			Assert( ((type<<8) + ch) <BLZBUFFER_SIZE );
			seed1 = mBLZBuffer[(type<<8) + ch] ^ (seed1 + seed2);
			seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
			str++;
		}

		return seed1;
	}
}