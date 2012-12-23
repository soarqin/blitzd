#include "Config.h"

#include "BnetSRP3.h"

#ifdef _MSC_VER
#pragma comment(lib, "../gmp/libgmp.a")
#pragma comment(lib, "../gmp/libgcc.a")
#endif

namespace Crypto
{
	bool BnetSRP3::srp3_inited = false;
	mpz_t	BnetSRP3::N;
	mpz_t	BnetSRP3::g;
	gmp_randstate_t BnetSRP3::rand;

	BnetSRP3::BnetSRP3( const char* username_, const byte * salt_ )
	{
		init(username_, NULL, salt_);
	}

	BnetSRP3::BnetSRP3( const std::string& username_, const byte * salt_ )
	{
		init(username_.c_str(), NULL, salt_);
	}

	BnetSRP3::BnetSRP3( const char* username_, const char* password_ )
	{
		init(username_, password_, NULL);
	}

	BnetSRP3::BnetSRP3( const std::string& username_, const std::string& password_ )
	{
		init(username_.c_str(), password_.c_str(), NULL);
	}

	BnetSRP3::~BnetSRP3()
	{
		mpz_clear(a);
		mpz_clear(b);
		if(B_inited)
		{
			mpz_clear(B);
			B_inited = false;
		}
	}

	void BnetSRP3::getVerifier( mpz_ptr result )
	{
		mpz_init(result);
		mpz_t pkey;
		getClientPrivateKey(pkey);
		mpz_powm(result, g, pkey, N);
		mpz_clear(pkey);
	}

	void BnetSRP3::getSalt( byte * result )
	{
		memcpy(result, raw_salt, 32);
	}

	void BnetSRP3::setSalt( const byte * salt )
	{
		memcpy(raw_salt, salt, 32);
	}

	void BnetSRP3::getClientSessionPublicKey( mpz_ptr result )
	{
		mpz_init(result);
		mpz_powm(result, g, a, N);
	}

	void BnetSRP3::getServerSessionPublicKey( mpz_ptr result, mpz_t& v )
	{
		if (!B_inited)
		{
			mpz_init(B);
			mpz_powm(B, g, b, N);
			mpz_add(B, B, v);
			mpz_mod(B, B, N);
			B_inited = true;
		}

		mpz_init_set(result, B);
	}

	void BnetSRP3::getHashedClientSecret( mpz_ptr result, mpz_t& B_ )
	{
		mpz_t clientSecret;
		getClientSecret(clientSecret, B_);
		hashSecret(result, clientSecret);
		mpz_clear(clientSecret);
	}

	void BnetSRP3::getHashedServerSecret( mpz_ptr result, mpz_t& A, mpz_t& v )
	{
		mpz_t serverSecret;
		getServerSecret(serverSecret, A, v);
		hashSecret(result, serverSecret);
		mpz_clear(serverSecret);
	}

	void BnetSRP3::getClientPasswordProof( byte * result, mpz_t& A, mpz_t& B_, mpz_t& K )
	{
		byte usernameHash[20], proofHash[20];
		byte A_[32], B__[32], K_[40];

		SHA1Context sha;
		SHA1Reset(&sha);
		SHA1Input(&sha, (byte*) username.c_str(), username.length());
		SHA1Result(&sha, usernameHash);

		SHA1Reset(&sha);
		SHA1Input(&sha, (byte*) SRP3_I, 20);
		SHA1Input(&sha, usernameHash, 20);
		SHA1Input(&sha, (byte*) raw_salt, 32);
		mpz_export(A_, NULL, -1, 1, 0, 0, A);
		SHA1Input(&sha, (byte*) A_, 32);
		mpz_export(B__, NULL, -1, 1, 0, 0, B_);
		SHA1Input(&sha, (byte*) B__, 32);
		mpz_export(K_, NULL, -1, 1, 0, 0, K);
		SHA1Input(&sha, (byte*) K_, 40);
		SHA1Result(&sha, (byte*) proofHash);

		memcpy(result, proofHash, 20);
	}

	void BnetSRP3::getServerPasswordProof( byte * result, mpz_t& A, const byte * M, mpz_t& K )
	{
		SHA1Context sha;
		byte proofHash[20];
		byte A_[32], K_[40];

		SHA1Reset(&sha);
		mpz_export(A_, NULL, -1, 1, 0, 0, A);
		SHA1Input(&sha, (byte*) A_, 32);
		SHA1Input(&sha, M, 20);
		mpz_export(K_, NULL, -1, 1, 0, 0, K);
		SHA1Input(&sha, (byte*) K_, 40);
		SHA1Result(&sha, (byte*) proofHash);

		memcpy(result, proofHash, 20);
	}

	int BnetSRP3::init( const char* username_, const char* password_, const byte * salt_ )
	{
		if(!srp3_inited)
		{
			gmp_randinit_default(rand);
			gmp_randseed_ui(rand, (unsigned long)time(NULL));
			mpz_init2(N, 256);
			mpz_import(N, 32, -1, 1, 0, 0, SRP3_N);
			mpz_init_set_ui(g, SRP3_g);
			srp3_inited = true;
		}
		uint i;
		const char* source;

		if (!username_) {
			return -1;
		}

		uint len = strlen(username_);
		username.resize(len);
		source = username_;
		for(i = 0; i < len; ++ i)
		{
			username[i] = toupper(*(source++));
		}

		if (!((password_ == NULL) ^ (salt_ == NULL))) {
			return -1;
		}

		if (password_!=NULL) {
			len = strlen(password_);
			password.resize(len);
			source =  password_;
			for(i = 0; i < len; ++ i)
			{
				password[i] = toupper(*(source++));
			}
			mpz_init2(a, 256);
			mpz_init(b);
			mpz_urandomm(a, rand, N); /* generates the private key */

			mpz_t s;
			mpz_init2(s, 256);
			mpz_urandomb(s, rand, 256);
			mpz_export(raw_salt, NULL, 1, 4, -1, 0, s);
		} else {
			password = "";
			mpz_init(a);
			mpz_init2(b, 256);
			mpz_urandomm(b, rand, N);

			setSalt(salt_);
		}

		B_inited = false;

		return 0;
	}

	void BnetSRP3::getClientPrivateKey( mpz_ptr result )
	{
		std::string userpass;
		byte userpass_hash[20], private_value_hash[20];
		SHA1Context shac;

		userpass.resize(username.length() + 1 + password.length());
		memcpy(&userpass[0], &username[0], username.length());
		userpass[username.length()] = ':';
		memcpy(&userpass[username.length() + 1], &password[0], password.length());
		SHA1Reset(&shac);
		SHA1Input(&shac, (byte*) userpass.c_str(), (username.length() + 1 + password.length()));
		SHA1Result(&shac, userpass_hash);

		// get the SHA-1 hash of the salt and user:pass hash
		SHA1Reset(&shac);
		SHA1Input(&shac, (byte*) raw_salt, 32);
		SHA1Input(&shac, userpass_hash, 20);
		SHA1Result(&shac, private_value_hash);
		SHA1Reset(&shac);

		mpz_init2(result, 160);
		mpz_import(result, 20, -1, 1, 0, 0, private_value_hash);
	}

	void BnetSRP3::getScrambler( mpz_ptr result, mpz_t& B_ )
	{
		byte raw_B[32];
		byte hash[20];
		SHA1Context shac;

		mpz_export(raw_B, NULL, -1, 1, 0, 0, B_);
		SHA1Reset(&shac);
		SHA1Input(&shac, raw_B, 32);
		SHA1Result(&shac, hash);
		mpz_init_set_ui(result, htonl(*(uint*) hash));
	}

	void BnetSRP3::getClientSecret( mpz_ptr result, mpz_t& B_ )
	{
		mpz_t x, u;
		getClientPrivateKey(x);
		getScrambler(u, B_);
		mpz_t temp;
		mpz_init(temp);
		mpz_powm(temp, g, x, N);
		mpz_init(result);
		mpz_add(result, N, B_);
		mpz_sub(result, result, temp);
		mpz_clear(temp);
		mpz_init(temp);
		mpz_mul(temp, x, u);
		mpz_add(temp, temp, a);
		mpz_powm(result, result, temp, N);
		mpz_clear(temp);
		mpz_clear(x);
		mpz_clear(u);
	}

	void BnetSRP3::getServerSecret( mpz_ptr result, mpz_t& A, mpz_t& v )
	{
		mpz_t B_, u;
		getServerSessionPublicKey(B_, v);
		getScrambler(u, B_);
		mpz_init(result);
		mpz_powm(result, v, u, N);
		mpz_mul(result, result, A);
		mpz_mod(result, result, N);
		mpz_powm(result, result, b, N);
		mpz_clear(B_);
		mpz_clear(u);
	}

	void BnetSRP3::hashSecret( mpz_ptr result, mpz_t& secret )
	{
		int i;
		byte raw_secret[32];
		byte odd[16], even[16], hashedSecret[40];
		byte* secretPointer;
		byte* oddPointer;
		byte* evenPointer;
		byte odd_hash[20], even_hash[20];

		mpz_export(raw_secret, NULL, -1, 1, 0, 0, secret);
		secretPointer = raw_secret;
		oddPointer = odd;
		evenPointer = even;

		for(i=0; i<16; i++)
		{
			*(oddPointer++) = *(secretPointer++);
			*(evenPointer++) = *(secretPointer++);
		}

		SHA1Context ctx;

		SHA1Reset(&ctx);
		SHA1Input(&ctx, (byte*) odd, 16);
		SHA1Result(&ctx, odd_hash);

		SHA1Reset(&ctx);
		SHA1Input(&ctx, (byte*) even, 16);
		SHA1Result(&ctx, even_hash);

		secretPointer = hashedSecret;
		oddPointer = (byte*)odd_hash;
		evenPointer = (byte*)even_hash;

		for(i=0; i<20; i++)
		{
			*(secretPointer++) = *(oddPointer++);
			*(secretPointer++) = *(evenPointer++);
		}
		mpz_init2(result, 320);
		mpz_import(result, 40, -1, 1, 0, 0, hashedSecret);
	}
}
