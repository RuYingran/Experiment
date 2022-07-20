#pragma once
#include<iostream>
# include <immintrin.h>
using namespace std;

#define sm3_digest_BYTES 32
#define sm3_block_BYTES 64
#define sm3_hmac_BYTES sm3_digest_BYTES
#define rol(x,j) ((x<<j)|(x>>(32-j)))
#define P0(x) (x^rol(x,9)^rol(x,17))
#define P1(x) (x^rol(x,15)^rol(x,23))
#define FF0(x,y,z) x^y^z
#define GG0(x,y,z) x^y^z
#define FF1(x,y,z)(x&y) | x&z | (y&z)
#define GG1(x,y,z)(x&y) | ((~x)&z)

#define FULL_UNROLL(A,B,C,D,E,F,G,H,x)       \
    if(j<16) T_[j]=0x79CC4519;else T_[j]= 0x7AB79D8A;\
    SS1 = rol((rol(A, 12) + E + rol(T[j],j)), 7);\
	SS2 = SS1 ^ rol(A, 12);                  \
	TT1 = FF##x(A, B, C) + D + SS2 + W1[j];   \
	TT2 = GG##x(E, F, G) + H + SS1 + W[j];    \
	B = rol(B, 9);                           \
	H = TT1;                                 \
	F = rol(F, 19);                          \
	D = P0(TT2);                             \
	j++

#define FULL_UNROLL_8(A, B, C, D, E, F, G, H, x)\
	FULL_UNROLL(A, B, C, D, E, F, G, H, x);		\
	FULL_UNROLL(H, A, B, C, D, E, F, G, x);		\
	FULL_UNROLL(G, H, A, B, C, D, E, F, x);		\
	FULL_UNROLL(F, G, H, A, B, C, D, E, x);		\
	FULL_UNROLL(E, F, G, H, A, B, C, D, x);		\
	FULL_UNROLL(D, E, F, G, H, A, B, C, x);		\
	FULL_UNROLL(C, D, E, F, G, H, A, B, x);		\
	FULL_UNROLL(B, C, D, E, F, G, H, A, x)

# define _mm_rotl_epi32(X,i) \
	_mm_xor_si128(_mm_slli_epi32((X),(i)), _mm_srli_epi32((X),32-(i)))

typedef struct sm3_ctx_t {
	uint32_t digest[sm3_digest_BYTES / sizeof(uint32_t)];
	int nblocks;  //number of blocks that have been processed
	uint8_t block[sm3_block_BYTES * 4];
	int num;
}sm3_ctx;

void sm3_init(sm3_ctx* ctx);
void sm3_update(sm3_ctx* ctx, const uint8_t* data, size_t data_len);
void sm3_final(sm3_ctx* ctx, uint8_t* digest);
void sm3(const uint8_t* message, size_t mlen, uint8_t res[sm3_block_BYTES]);
static void sm3_compress(uint32_t digest[sm3_block_BYTES / sizeof(uint32_t)], const uint8_t block[sm3_block_BYTES]);

uint64_t byte_swap64(uint64_t i)
{
	uint64_t j;
	j = (i << 56);
	j += (i << 40) & UINT64_C(0x00FF000000000000);
	j += (i << 24) & UINT64_C(0x0000FF0000000000);
	j += (i << 8) & UINT64_C(0x000000FF00000000);
	j += (i >> 8) & UINT64_C(0x00000000FF000000);
	j += (i >> 24) & UINT64_C(0x0000000000FF0000);
	j += (i >> 40) & UINT64_C(0x000000000000FF00);
	j += (i >> 56);
	return j;
}
uint32_t byte_swap32(uint32_t i)
{
	uint32_t j;
	j = (i << 24);
	j += (i << 8) & 0x00FF0000;
	j += (i >> 8) & 0x0000FF00;
	j += (i >> 24);
	return j;
}

void sm3_init(sm3_ctx* ctx) {
	ctx->digest[0] = 0x7380166F;
	ctx->digest[1] = 0x4914B2B9;
	ctx->digest[2] = 0x172442D7;
	ctx->digest[3] = 0xDA8A0600;
	ctx->digest[4] = 0xA96F30BC;
	ctx->digest[5] = 0x163138AA;
	ctx->digest[6] = 0xE38DEE4D;
	ctx->digest[7] = 0xB0FB0E4E;

	ctx->nblocks = 0;
	ctx->num = 0;
}

void sm3_update(sm3_ctx* ctx, const uint8_t* data, size_t dlen) {
	if (ctx->num) {
		unsigned int left = sm3_block_BYTES - ctx->num;
		if (dlen < left) {
			memcpy(ctx->block + ctx->num, data, dlen);
			ctx->num += dlen;
			return;
		}
		else {
			memcpy(ctx->block + ctx->num, data, left);
			sm3_compress(ctx->digest, ctx->block);
			ctx->nblocks++;
			data += left;
			dlen -= left;
		}
	}
	while (dlen >= sm3_block_BYTES) {
		sm3_compress(ctx->digest, data);
		ctx->nblocks++;
		data += sm3_block_BYTES;
		dlen -= sm3_block_BYTES;
	}
	ctx->num = dlen;
	if (dlen) {
		memcpy(ctx->block, data, dlen);
	}
}

void sm3_final(sm3_ctx* ctx, uint8_t* digest) {
	size_t i;
	uint32_t* pdigest = (uint32_t*)(digest);
	uint64_t* count = (uint64_t*)(ctx->block + sm3_block_BYTES - 8);

	ctx->block[ctx->num] = 0x80;

	if (ctx->num + 9 <= sm3_block_BYTES) {
		memset(ctx->block + ctx->num + 1, 0, sm3_block_BYTES - ctx->num - 9);
	}
	else {
		memset(ctx->block + ctx->num + 1, 0, sm3_block_BYTES - ctx->num - 1);
		sm3_compress(ctx->digest, ctx->block);
		memset(ctx->block, 0, sm3_block_BYTES - 8);
	}

	count[0] = (uint64_t)(ctx->nblocks) * 512 + (ctx->num << 3);
	count[0] = byte_swap64(count[0]);

	sm3_compress(ctx->digest, ctx->block);
	for (i = 0; i < sizeof(ctx->digest) / sizeof(ctx->digest[0]); i++) {
		pdigest[i] = byte_swap32(ctx->digest[i]);
	}
}

static void sm3_compress(uint32_t digest[sm3_block_BYTES / sizeof(uint32_t)], const uint8_t block[sm3_block_BYTES]) {
	int j;
	uint32_t W[68],W1[64];
	__m128i X, Y, R,Q,Z;
	__m128i M = _mm_setr_epi32(0, 0, 0, 0xffffffff);//按反向顺序设置4个带符号的32位整数值
	__m128i V = _mm_setr_epi8(3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12);//反向顺序设置16个带符号的8位整数值
	//const uint32_t* pblock = (const uint32_t*)(block);

	uint32_t A = digest[0], B = digest[1], C = digest[2], D = digest[3];
	uint32_t E = digest[4], F = digest[5], G = digest[6], H = digest[7];

	uint32_t SS1, SS2, TT1, TT2, T[64];
	//for (j = 0; j < 16; j++) W[j] = byte_swap32(pblock[j]);
	for (j = 0; j < 16; j += 4) {
		X = _mm_loadu_si128((__m128i*)(block + j * 4)); //每次处理4个，将数据转换为128bit并存入W数组中
		X = _mm_shuffle_epi8(X, V);
		_mm_storeu_si128((__m128i*)(W + j), X);
	}
	//for (j = 16; j < 68; j++) W[j] = P1(W[j - 16] ^ W[j - 9] ^ rol(W[j - 3], 15)) ^ rol(W[j - 13], 7) ^ W[j - 6];
	for (j = 16; j < 68; j += 4) {
		X = _mm_loadu_si128((__m128i*)(W + j - 3));   //X = (W[j - 3], W[j - 2], W[j - 1], 0) 
		X = _mm_andnot_si128(M, X);

		X = _mm_rotl_epi32(X, 15);
		Y = _mm_loadu_si128((__m128i*)(W + j - 9));
		X = _mm_xor_si128(X, Y);
		Y = _mm_loadu_si128((__m128i*)(W + j - 16));
		X = _mm_xor_si128(X, Y);

		Y = _mm_rotl_epi32(X, (23 - 15));  //计算P1(x) (x^rol(x,15)^rol(x,23))
		Y = _mm_xor_si128(Y, X);
		Y = _mm_rotl_epi32(Y, 15);
		X = _mm_xor_si128(X, Y);

		Y = _mm_loadu_si128((__m128i*)(W + j - 13));
		Y = _mm_rotl_epi32(Y, 7);
		X = _mm_xor_si128(X, Y);
		Y = _mm_loadu_si128((__m128i*)(W + j - 6));
		X = _mm_xor_si128(X, Y);

		R = _mm_shuffle_epi32(X, 0);  // W[j + 3] ^= P1(rol32(W[j + 1], 15)) 
		R = _mm_and_si128(R, M);
		Y = _mm_rotl_epi32(R, 15);
		Y = _mm_xor_si128(Y, R);
		Y = _mm_rotl_epi32(Y, 9);
		R = _mm_xor_si128(R, Y);
		R = _mm_rotl_epi32(R, 6);
		X = _mm_xor_si128(X, R);

		_mm_storeu_si128((__m128i*)(W + j), X);
	}
	//for (j = 0; j < 64; j+=4) W1[j] = W[j] ^ W[j + 4];
	for (j = 0; j < 64; j+=4)
	{
		Q = _mm_loadu_si128((__m128i*)(W + j)); //W数组中数据已知，每次4个并行计算W1[j]数组中的值从而提升速度
		Z = _mm_loadu_si128((__m128i*)(W + j+4));
		Z = _mm_xor_si128(Q, Z);
		_mm_storeu_si128((__m128i*)(W1 + j), Z);

	}
	for (j = 0; j < 16; j++)
	{
		T[j] = 0x79CC4519;
		SS1 = rol((rol(A, 12) + E + rol(T[j], j)), 7);
		SS2 = SS1 ^ rol(A, 12);
		TT1 = FF0(A, B, C) + D + SS2 + W1[j];
		TT2 = GG0(E, F, G) + H + SS1 + W[j];
		D = C;
		C = rol(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = rol(F, 19);
		F = E;
		E = P0(TT2);
	}
	for (j = 16; j < 64; j++)
	{
		T[j] = 0x7AB79D8A;
		SS1 = rol(rol(A, 12) + E + rol(T[j], j), 7);
		SS2 = SS1 ^ rol(A, 12);
		TT1 = FF1(A, B, C) + D + SS2 + W1[j];
		TT2 = GG1(E, F, G) + H + SS1 + W[j];
		D = C;
		C = rol(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = rol(F, 19);
		F = E;
		E = P0(TT2);
	}
	
	/*j = 0;
	FULL_UNROLL_8(A, B, C, D, E, F, G, H, 0);
	FULL_UNROLL_8(A, B, C, D, E, F, G, H, 0);
	FULL_UNROLL_8(A, B, C, D, E, F, G, H, 1);
	FULL_UNROLL_8(A, B, C, D, E, F, G, H, 1);
	FULL_UNROLL_8(A, B, C, D, E, F, G, H, 1);
	FULL_UNROLL_8(A, B, C, D, E, F, G, H, 1);
	FULL_UNROLL_8(A, B, C, D, E, F, G, H, 1);
	FULL_UNROLL_8(A, B, C, D, E, F, G, H, 1);
	*/
	digest[0] ^= A;
	digest[1] ^= B;
	digest[2] ^= C;
	digest[3] ^= D;
	digest[4] ^= E;
	digest[5] ^= F;
	digest[6] ^= G;
	digest[7] ^= H;
}

void sm3(const uint8_t* message, size_t mlen, uint8_t res[sm3_block_BYTES])
{
	sm3_ctx ctx;
	sm3_init(&ctx);
	sm3_update(&ctx, message, mlen);
	sm3_final(&ctx, res);
}

