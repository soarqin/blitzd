#ifndef _CRYPTO_CRC32_H_
#define _CRYPTO_CRC32_H_

namespace Crypto
{
	class CRC32
	{
	public:
		CRC32( );
		uint CalcData(const byte * data, uint size, uint initial = 0xFFFFFFFF);
		uint CalcFile(const char * filename);
	
	private:
		static uint crc32_table[256];
	};
}

#endif // _CRYPTO_CRC32_H_
