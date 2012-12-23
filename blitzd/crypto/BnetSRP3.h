#ifndef _BNETSRP3_H_
#define _BNETSRP3_H_

#include <gmp.h>
#include "crypto/SHA1Hash.h"

namespace Crypto
{
	const byte SRP3_N[] = {
		0x87, 0xC7, 0x23, 0x85, 0x65, 0xF6, 0x16, 0x12,
		0xD9, 0x12, 0x32, 0xC7, 0x78, 0x6C, 0x97, 0x7E,
		0x55, 0xB5, 0x92, 0xA0, 0x8C, 0xB6, 0x86, 0x21,
		0x03, 0x18, 0x99, 0x61, 0x8B, 0x1A, 0xFF, 0xF8
	};
	
	const byte SRP3_I[] = {
		0x6c, 0x0e, 0x97, 0xed, 0x0a, 0xf9, 0x6b, 0xab,
		0xb1, 0x58, 0x89, 0xeb, 0x8b, 0xba, 0x25, 0xa4,
		0xf0, 0x8c, 0x01, 0xf8
	};
	
	const uint SRP3_g = 0x2F;
	
	class BnetSRP3
	{
	public:
		BnetSRP3(const char* username_, const byte * salt_);
		BnetSRP3(const std::string& username_, const byte * salt_);
		BnetSRP3(const char* username_, const char* password_);
		BnetSRP3(const std::string& username_, const std::string& password_);
		~BnetSRP3();
		void getVerifier(mpz_ptr result);
		void getSalt(byte * result);
		void setSalt(const byte * salt);
		void getClientSessionPublicKey(mpz_ptr result);
		void getServerSessionPublicKey(mpz_ptr result, mpz_t& v);
		void getHashedClientSecret(mpz_ptr result, mpz_t& B_);
		void getHashedServerSecret(mpz_ptr result, mpz_t& A, mpz_t& v);
		void getClientPasswordProof(byte * result, mpz_t& A, mpz_t& B_, mpz_t& K);
		void getServerPasswordProof(byte * result, mpz_t& A, const byte * M, mpz_t& K);
	
	private:
		int	init(const char* username_, const char* password_, const byte * salt_);
		void getClientPrivateKey(mpz_ptr result);
		void getScrambler(mpz_ptr result, mpz_t& B_);
		void getClientSecret(mpz_ptr result, mpz_t& B_);
		void getServerSecret(mpz_ptr result, mpz_t& A, mpz_t& v);
		void hashSecret(mpz_ptr result, mpz_t& secret);
		static mpz_t	N;
		static mpz_t	g;
		static bool srp3_inited;
		mpz_t	a;	// client session private key
		mpz_t	b;	// server session private key
		mpz_t	B;	// server public key cache
		bool B_inited;
		static gmp_randstate_t rand;
		std::string	username;
		std::string	password;
		byte raw_salt[32];
	};
}

#endif // _BNETSRP3_H_
