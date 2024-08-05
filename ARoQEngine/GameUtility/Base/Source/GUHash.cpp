//////////////////////////////////////////////////////////////////////////////////
///  @file   GUHash.cpp
///  @brief  temp
///  @author toide
///  @date   2024/04/29 10:58:19
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUHash.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;

//#pragma warning(disable: 26450)
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
uint32 Hash::FNV1a_32(const void* key, const uint64 byteLength, const uint32 seed)
{
	uint32 hash = seed;
	hash ^= FNV_OFFSET_BASIS_32;

	const uint8* data = (const uint8*)key;

	for (uint64 i = 0; i < byteLength; ++i)
	{
		hash = (FNV_PRIME_32 * hash) ^ data[i];
	}

	return hash;
}

uint64 Hash::FNV1a_64(const void* key, const uint64 byteLength, const uint64 seed)
{
	uint64 hash = seed;
	hash ^= FNV_OFFSET_BASIS_64;

	const uint8* data = (const uint8*)key;

	for (uint64 i = 0; i < byteLength; ++i)
	{
		hash = (FNV_PRIME_64 * hash) ^ data[i];
	}

	return hash;
}

uint32 Hash::Murmur2_32(const void* key, const uint64 byteLength, const uint32 seed)
{
	const uint32 m = 0x5bd1e995;
	const int32  r = 24;

	const uint8* data = (const uint8*)key;
	uint32       hash = seed ^ (uint32)byteLength;

	uint64 byteRes = byteLength;
	while (byteRes >= 4)
	{
		uint32 k = *(uint32*)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		hash *= m;
		hash ^= k;

		data += 4;
		byteRes -= 4;
	}

	if (byteRes == 3) { hash ^= data[2] << 16; }
	if (byteRes >= 2) { hash ^= data[1] << 8; }
	if (byteRes >= 1) { hash ^= data[0]; }
	if (byteRes > 0)  { hash *= m; }

	hash ^= hash >> 13;
	hash *= m;
	hash ^= hash >> 15;

	return hash;
}

uint64 Hash::Murmur2_64(const void* key, const uint64 byteLength, const uint64 seed)
{
	const uint64 m = 0xc6a4a7935bd1e995;
	const int r = 47;

	uint64 hash = seed ^ (byteLength * m);

	const uint64* data = (const uint64*)key;
	const uint64* end  = data + (byteLength / 8);

	while (data != end)
	{
		uint64 k = *data++;

		k *= m;
		k ^= k >> r;
		k *= m;

		hash ^= k;
		hash *= m;
	}

	const uint8* data2   = (const uint8*)data;
	const auto   byteRes = byteLength & 7;
	if (byteRes == 7) { hash ^= int64(data2[6]) << 48; }
	if (byteRes >= 6) { hash ^= uint64(data2[5]) << 40; }
	if (byteRes >= 5) { hash ^= uint64(data2[4]) << 32; }
	if (byteRes >= 4) { hash ^= uint64(data2[3]) << 24; }
	if (byteRes >= 3) { hash ^= uint64(data2[2]) << 16; }
	if (byteRes >= 2) { hash ^= uint64(data2[1]) << 8; }
	if (byteRes >= 1) { hash ^= uint64(data2[0]) ; }
	if (byteRes > 0)  { hash *= m;}

	hash ^= hash >> r;
	hash *= m;
	hash ^= hash >> r;

	return hash;
}

uint32 Hash::XX_32(const void* key, const uint64 byteLength, const uint32 seed)
{	
	if (key == nullptr || byteLength == 0) { return 0; }

	const auto RotateLeft = [](const uint32 x, const uint8 bits) -> uint32
		{
			return (x << bits) | (x >> (32 - bits));
		};

	const uint8* data = (const uint8*)key;
	const uint8* end  = data + byteLength;

	uint32 hash = 0;

	// 128bit以上含まれている場合, 1つの32bit値に折りたたみます.
	if(byteLength >= 16) 
	{
		const uint8* limit = end - 16;
		uint32 v1 = seed + XX_PRIME1_32 + XX_PRIME2_32;
		uint32 v2 = seed + XX_PRIME2_32;
		uint32 v3 = seed + 0;
		uint32 v4 = seed - XX_PRIME1_32;

		do 
		{
			const uint32* block = (const uint32*)data;
			v1 = RotateLeft(v1 + block[0] * XX_PRIME2_32, 13) * XX_PRIME1_32;
			v2 = RotateLeft(v2 + block[1] * XX_PRIME2_32, 13) * XX_PRIME1_32;
			v3 = RotateLeft(v3 + block[2] * XX_PRIME2_32, 13) * XX_PRIME1_32;
			v4 = RotateLeft(v4 + block[3] * XX_PRIME2_32, 13) * XX_PRIME1_32;
			data += 16;
		} while (data <= limit);

		hash = RotateLeft(v1, 1) + RotateLeft(v2, 7) + RotateLeft(v3, 12) + RotateLeft(v4, 18);
	}
	else
	{
		hash = XX_PRIME5_32;
	}

	hash += (uint32)byteLength;

	// 残り4バイトになるまで, RotateLeft(17)
	while (data + 4 <= end)
	{
		hash += *(uint32*)data * XX_PRIME3_32;
		hash = RotateLeft(hash, 17) * XX_PRIME4_32;
		data += 4;
	}

	// 1byteずつ 0～3 byteを順に処理する
	while (data < end)
	{
		hash += (*data) * XX_PRIME5_32;
		hash =  RotateLeft(hash, 11) * XX_PRIME1_32;
		data++;
	}

	// ハッシュ値を出す前に全てのbitをMixします. 
	hash ^= hash >> 15;
	hash *= XX_PRIME2_32;
	hash ^= hash >> 13;
	hash *= XX_PRIME3_32;
	hash ^= hash >> 16;

	return hash;
}

uint64 Hash::XX_64(const void* key, const uint64 byteLength, const uint64 seed)
{
	if (key == nullptr || byteLength == 0) { return 0; }

	const auto RotateLeft = [](const uint64 x, const uint8 bits) -> uint64
		{
			return (x << bits) | (x >> (64 - bits));
		};

	const uint8* data = (const uint8*)key;
	const uint8* end  = data + byteLength;

	uint64 hash = 0;

	// 256bit以上含まれている場合, 1つの64bit値に折りたたみます.
	if(byteLength >= 32) 
	{
		const uint8* limit = end - 32;
		uint64 v1 = seed + XX_PRIME1_64 + XX_PRIME2_64;
		uint64 v2 = seed + XX_PRIME2_64;
		uint64 v3 = seed + 0;
		uint64 v4 = seed - XX_PRIME1_64;

		do 
		{
			const uint64* block = (const uint64*)data;
			v1 = RotateLeft(v1 + block[0] * XX_PRIME2_64, 31) * XX_PRIME1_64;
			v2 = RotateLeft(v2 + block[1] * XX_PRIME2_64, 31) * XX_PRIME1_64;
			v3 = RotateLeft(v3 + block[2] * XX_PRIME2_64, 31) * XX_PRIME1_64;
			v4 = RotateLeft(v4 + block[3] * XX_PRIME2_64, 31) * XX_PRIME1_64;
			data += 32;
		} while (data <= limit);

		hash = RotateLeft(v1, 1) + RotateLeft(v2, 7) + RotateLeft(v3, 12) + RotateLeft(v4, 18);
	}
	else
	{
		hash = XX_PRIME5_64;
	}

	hash += (uint64)byteLength;

	// 残り8バイトになるまで, RotateLeft(17)
	while (data + 8 <= end)
	{
		uint64 k1 = *(uint64*)data;
		k1 *= XX_PRIME2_64;
		k1  = RotateLeft(k1, 31);
		k1 *= XX_PRIME1_64;

		hash ^= k1;
		hash = RotateLeft(hash, 27) * XX_PRIME1_64 + XX_PRIME4_64;
		data += 8;
	}

	if (data + 4 <= end)
	{
		hash ^= (uint64)(*(uint32*)data) * XX_PRIME1_64;
		hash = RotateLeft(hash, 23) * XX_PRIME2_64 + XX_PRIME3_64;
		data += 4;
	}

	// 1byteずつ 0～3 byteを順に処理する
	while (data < end)
	{
		hash ^= (*data) * XX_PRIME5_64;
		hash =  RotateLeft(hash, 11) * XX_PRIME1_64;
		data++;
	}

	// ハッシュ値を出す前に全てのbitをMixします. 
	hash ^= hash >> 33;
	hash *= XX_PRIME2_64;
	hash ^= hash >> 29;
	hash *= XX_PRIME3_64;
	hash ^= hash >> 32;

	return hash;
}
#pragma endregion Public Function