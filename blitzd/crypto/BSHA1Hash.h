#ifndef _BSHA1HASH_H_
#define _BSHA1HASH_H_

namespace Crypto
{
	/*
	* If you do not have the ISO standard stdint.h header file, then you
	* must typdef the following:
	*    name              meaning
	*  uint         unsigned 32 bit integer
	*  byte          unsigned 8 bit integer (i.e., byte)
	*  int_least16_t    integer of >= 16 bits
	*
	*/

#ifndef _SHA_enum_
#define _SHA_enum_
	enum
	{
		shaSuccess = 0,
		shaNull,            /* Null pointer parameter */
		shaInputTooLong,    /* input data too long */
		shaStateError       /* called Input after Result */
	};
#endif
#define BSHA1HashSize 20

	/*
	*  This structure will hold context information for the SHA-1
	*  hashing operation
	*/
	typedef struct BSHA1Context
	{
		uint Intermediate_Hash[BSHA1HashSize/4]; /* Message Digest  */

		uint Length_Low;            /* Message length in bits      */
		uint Length_High;           /* Message length in bits      */

		/* Index into message block array   */
		ushort Message_Block_Index;
		byte Message_Block[64];      /* 512-bit message blocks      */

		int Computed;               /* Is the digest computed?         */
		int Corrupted;             /* Is the message digest corrupted? */
	} BSHA1Context;

	/*
	*  Function Prototypes
	*/

	int BSHA1Reset(  BSHA1Context *);
	int BSHA1Input(  BSHA1Context *, const byte *, uint);
	int BSHA1Result( BSHA1Context *,
		byte Message_Digest[BSHA1HashSize]);
}

#endif // _BSHA1HASH_H_
