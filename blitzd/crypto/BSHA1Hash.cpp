#include "Config.h"

#include "BSHA1Hash.h"

namespace Crypto
{
	/*
	*  Define the BSHA1 circular left shift macro
	*/
#define BSHA1CircularShift(bits,word) \
	(((word) << (bits)) | ((word) >> (32-(bits))))

	/* Local Function Prototyptes */
	void BSHA1PadMessage(BSHA1Context *);
	void BSHA1ProcessMessageBlock(BSHA1Context *);

	/*
	*  BSHA1Reset
	*
	*  Description:
	*      This function will initialize the BSHA1Context in preparation
	*      for computing a new BSHA1 message digest.
	*
	*  Parameters:
	*      context: [in/out]
	*          The context to reset.
	*
	*  Returns:
	*      sha Error Code.
	*
	*/
	int BSHA1Reset(BSHA1Context *context)
	{
		if (!context)
		{
			return shaNull;
		}

		context->Length_Low             = 0;
		context->Length_High            = 0;
		context->Message_Block_Index    = 0;

		context->Intermediate_Hash[0]   = 0x67452301;
		context->Intermediate_Hash[1]   = 0xEFCDAB89;
		context->Intermediate_Hash[2]   = 0x98BADCFE;
		context->Intermediate_Hash[3]   = 0x10325476;
		context->Intermediate_Hash[4]   = 0xC3D2E1F0;

		context->Computed   = 0;
		context->Corrupted  = 0;

		return shaSuccess;
	}

	/*
	*  BSHA1Result
	*
	*  Description:
	*      This function will return the 160-bit message digest into the
	*      Message_Digest array  provided by the caller.
	*      NOTE: The first octet of hash is stored in the 0th element,
	*            the last octet of hash in the 19th element.
	*
	*  Parameters:
	*      context: [in/out]
	*          The context to use to calculate the SHA-1 hash.
	*      Message_Digest: [out]
	*          Where the digest is returned.
	*
	*  Returns:
	*      sha Error Code.
	*
	*/
	int BSHA1Result( BSHA1Context *context,
		byte Message_Digest[BSHA1HashSize])
	{
		int i;

		if (!context || !Message_Digest)
		{
			return shaNull;
		}

		if (context->Corrupted)
		{
			return context->Corrupted;
		}

		if (!context->Computed)
		{
			BSHA1PadMessage(context);
			for(i=0; i<64; ++i)
			{
				/* message may be sensitive, clear it out */
				context->Message_Block[i] = 0;
			}
			context->Length_Low = 0;    /* and clear length */
			context->Length_High = 0;
			context->Computed = 1;

		}

		for(i = 0; i < BSHA1HashSize; ++i)
		{
			Message_Digest[i] = context->Intermediate_Hash[i>>2]
			>> 8 * ( 3 - ( i & 0x03 ) );
		}

		return shaSuccess;
	}

	/*
	*  BSHA1Input
	*
	*  Description:
	*      This function accepts an array of octets as the next portion
	*      of the message.
	*
	*  Parameters:
	*      context: [in/out]
	*          The SHA context to update
	*      message_array: [in]
	*          An array of characters representing the next portion of
	*          the message.
	*      length: [in]
	*          The length of the message in message_array
	*
	*  Returns:
	*      sha Error Code.
	*
	*/
	int BSHA1Input( BSHA1Context    *context,
		const byte  *message_array,
		uint length)
	{
		if (!length)
		{
			return shaSuccess;
		}

		if (!context || !message_array)
		{
			return shaNull;
		}

		if (context->Computed)
		{
			context->Corrupted = shaStateError;

			return shaStateError;
		}

		if (context->Corrupted)
		{
			return context->Corrupted;
		}
		while(length-- && !context->Corrupted)
		{
			context->Message_Block[context->Message_Block_Index++] =
				(*message_array & 0xFF);

			context->Length_Low += 8;
			if (context->Length_Low == 0)
			{
				context->Length_High++;
				if (context->Length_High == 0)
				{
					/* Message is too long */
					context->Corrupted = 1;
				}
			}

			if (context->Message_Block_Index == 64)
			{
				BSHA1ProcessMessageBlock(context);
			}

			message_array++;
		}

		return shaSuccess;
	}

	/*
	*  BSHA1ProcessMessageBlock
	*
	*  Description:
	*      This function will process the next 512 bits of the message
	*      stored in the Message_Block array.
	*
	*  Parameters:
	*      None.
	*
	*  Returns:
	*      Nothing.
	*
	*  Comments:

	*      Many of the variable names in this code, especially the
	*      single character names, were used because those were the
	*      names used in the publication.
	*
	*
	*/
	void BSHA1ProcessMessageBlock(BSHA1Context *context)
	{
		const uint K[] =    {       /* Constants defined in SHA-1   */
			0x5A827999,
			0x6ED9EBA1,
			0x8F1BBCDC,
			0xCA62C1D6
		};
		int           t;                 /* Loop counter                */
		uint      temp;              /* Temporary word value        */
		uint      W[80];             /* Word sequence               */
		uint      A, B, C, D, E;     /* Word buffers                */

		/*
		*  Initialize the first 16 words in the array W
		*/
		memset(W, 0, sizeof(W));
		memcpy(W, context->Message_Block, context->Message_Block_Index);

		for(t = 16; t < 80; t++)
		{
			W[t] = BSHA1CircularShift(W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16],1);
		}

		A = context->Intermediate_Hash[0];
		B = context->Intermediate_Hash[1];
		C = context->Intermediate_Hash[2];
		D = context->Intermediate_Hash[3];
		E = context->Intermediate_Hash[4];

		for(t = 0; t < 20; t++)
		{
			temp =  BSHA1CircularShift(5,A) +
				((B & C) | ((~B) & D)) + E + W[t] + K[0];
			E = D;
			D = C;
			C = BSHA1CircularShift(30,B);

			B = A;
			A = temp;
		}

		for(t = 20; t < 40; t++)
		{
			temp = BSHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
			E = D;
			D = C;
			C = BSHA1CircularShift(30,B);
			B = A;
			A = temp;
		}

		for(t = 40; t < 60; t++)
		{
			temp = BSHA1CircularShift(5,A) +
				((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
			E = D;
			D = C;
			C = BSHA1CircularShift(30,B);
			B = A;
			A = temp;
		}

		for(t = 60; t < 80; t++)
		{
			temp = BSHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[3];
			E = D;
			D = C;
			C = BSHA1CircularShift(30,B);
			B = A;
			A = temp;
		}

		context->Intermediate_Hash[0] += A;
		context->Intermediate_Hash[1] += B;
		context->Intermediate_Hash[2] += C;
		context->Intermediate_Hash[3] += D;
		context->Intermediate_Hash[4] += E;

		context->Message_Block_Index = 0;
	}

	/*
	*  BSHA1PadMessage
	*

	*  Description:
	*      According to the standard, the message must be padded to an even
	*      512 bits.  The first padding bit must be a '1'.  The last 64
	*      bits represent the length of the original message.  All bits in
	*      between should be 0.  This function will pad the message
	*      according to those rules by filling the Message_Block array
	*      accordingly.  It will also call the ProcessMessageBlock function
	*      provided appropriately.  When it returns, it can be assumed that
	*      the message digest has been computed.
	*
	*  Parameters:
	*      context: [in/out]
	*          The context to pad
	*      ProcessMessageBlock: [in]
	*          The appropriate SHA*ProcessMessageBlock function
	*  Returns:
	*      Nothing.
	*
	*/

	void BSHA1PadMessage(BSHA1Context *context)
	{
		BSHA1ProcessMessageBlock(context);
	}
}
