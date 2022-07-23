#include <iostream>
#include <string>
#include <cmath>
#include<immintrin.h>
#include<Windows.h>
using namespace std;

string Sbox[16][16] = { {"D6","90","E9","FE","CC","E1","3D","B7","16","B6","14","C2","28","FB","2C","05"},
						 {"2B","67","9A","76","2A","BE","04","C3","AA","44","13","26","49","86","06","99"},
						 {"9C","42","50","F4","91","EF","98","7A","33","54","0B","43","ED","CF","AC","62"},
						 {"E4","B3","1C","A9","C9","08","E8","95","80","DF","94","FA","75","8F","3F","A6"},
						 {"47","07","A7","FC","F3","73","17","BA","83","59","3C","19","E6","85","4F","A8"},
						 {"68","6B","81","B2","71","64","DA","8B","F8","EB","0F","4B","70","56","9D","35"},
						 {"1E","24","0E","5E","63","58","D1","A2","25","22","7C","3B","01","21","78","87"},
						 {"D4","00","46","57","9F","D3","27","52","4C","36","02","E7","A0","C4","C8","9E"},
						 {"EA","BF","8A","D2","40","C7","38","B5","A3","F7","F2","CE","F9","61","15","A1"},
						 {"E0","AE","5D","A4","9B","34","1A","55","AD","93","32","30","F5","8C","B1","E3"},
						 {"1D","F6","E2","2E","82","66","CA","60","C0","29","23","AB","0D","53","4E","6F"},
						 {"D5","DB","37","45","DE","FD","8E","2F","03","FF","6A","72","6D","6C","5B","51"},
						 {"8D","1B","AF","92","BB","DD","BC","7F","11","D9","5C","41","1F","10","5A","D8"},
						 {"0A","C1","31","88","A5","CD","7B","BD","2D","74","D0","12","B8","E5","B4","B0"},
						 {"89","69","97","4A","0C","96","77","7E","65","B9","F1","09","C5","6E","C6","84"},
						 {"18","F0","7D","EC","3A","DC","4D","20","79","EE","5F","3E","D7","CB","39","48"}
};


unsigned HexToDec(char c) {//十六进制转换为十进制的函数实现
	unsigned t;
	char s[2] = { c };
	sscanf_s(s, "%x", &t);
	return t;
}

string NLTransform(string str) {//非线性变换t函数实现
	string res = "";
	for (int i = 0; i < 4; i++) {
		res = res + Sbox[HexToDec(str[i << 1])][HexToDec(str[(i << 1) + 1])];
	}
	return res;
}

string LTransform(string str) {//线性变换L函数实现
	unsigned t1, t2, t3, t4, t5;
	sscanf_s(str.c_str(), "%x", &t1);
	char tmp[10] = { 0 };
	t2 = (t1 << 2) + (t1 >> 30);
	t3 = (t1 << 10) + (t1 >> 22);
	t4 = (t2 << 16) + (t2 >> 16);
	t5 = (t2 << 22) + (t2 >> 10);
	t1 = t1 ^ t2 ^ t3 ^ t4 ^ t5;
	sprintf_s(tmp, "%08x", t1);
	return string(tmp);
}

string L2Transform(string str) {//线性变换L'函数实现
	unsigned t1, t2, t3;
	sscanf_s(str.c_str(), "%x", &t1);
	char tmp[10] = { 0 };
	t2 = (t1 << 13) + (t1 >> 19);
	t3 = (t1 << 23) + (t1 >> 9);
	t1 = t1 ^ t2 ^ t3;
	sprintf_s(tmp, "%08x", t1);
	return string(tmp);
}

string T(string str) {//用于加解密算法中的合成置换T函数实现

	string tmp = LTransform(NLTransform(str));
	//cout << tmp << endl;
	return tmp;
}

string T2(string str) {//用于密钥扩展算法中的合成置换T函数实现

	string tmp = NLTransform(str);
	//cout << tmp << endl;
	tmp = L2Transform(tmp);
	//cout << tmp << endl;
	return tmp;
}

string KeyExtension(string MK) {//密钥扩展函数实现
	//cout << "ll" << ":" << MK << endl;

	unsigned FK[4] = { 0xA3B1BAC6, 0x56AA3350, 0x677D9197, 0xB27022DC };
	unsigned CK[32] = { 0x00070E15, 0x1C232A31, 0x383F464D, 0x545B6269,
						0x70777E85, 0x8C939AA1, 0xA8AFB6BD, 0xC4CBD2D9,
						0xE0E7EEF5, 0xFC030A11, 0x181F262D, 0x343B4249,
						0x50575E65, 0x6C737A81, 0x888F969D, 0xA4ABB2B9,
						0xC0C7CED5, 0xDCE3EAF1, 0xF8FF060D, 0x141B2229,
						0x30373E45, 0x4C535A61, 0x686F767D, 0x848B9299,
						0xA0A7AEB5, 0xBCC3CAD1, 0xD8DFE6ED, 0xF4FB0209,
						0x10171E25, 0x2C333A41, 0x484F565D, 0x646B7279 };
	unsigned K[36] = { 0 }, tmp;
	sscanf_s(MK.substr(0, 8).c_str(), "%x", &K[0]);
	sscanf_s(MK.substr(8, 8).c_str(), "%x", &K[1]);
	sscanf_s(MK.substr(16, 8).c_str(), "%x", &K[2]);
	sscanf_s(MK.substr(24).c_str(), "%x", &K[3]);

	K[0] ^= FK[0]; K[1] ^= FK[1];
	K[2] ^= FK[2]; K[3] ^= FK[3];
	char C[9];
	string ek = "", str;
	for (int i = 0; i < 32; i++) {
		tmp = K[i + 1] ^ K[i + 2] ^ K[i + 3] ^ CK[i];
		sprintf_s(C, "%08x", tmp);
		str = T2(string(C));
		sscanf_s(str.c_str(), "%x", &K[i + 4]);
		K[i + 4] ^= K[i];
		sprintf_s(C, "%08x", K[i + 4]);
		ek += string(C);
	}

	//cout << "rr" << ":" << ek << endl;
	return ek;
}

string encode(string pln, string key) {//加密函数实现
	//cout << "轮密钥与每轮输出状态：" << endl;
	//cout << endl;
	//string cipher[36] = { plain.substr(0,8),plain.substr(8,8),plain.substr(16,8),plain.substr(24) };

	unsigned cip[36] = { 0 }, tmp;
	//优化


	sscanf_s(pln.substr(0, 8).c_str(), "%x", &cip[0]);
	sscanf_s(pln.substr(8, 8).c_str(), "%x", &cip[1]);
	sscanf_s(pln.substr(16, 8).c_str(), "%x", &cip[2]);
	sscanf_s(pln.substr(24).c_str(), "%x", &cip[3]);
	char C[9], C1[9], C2[9], C3[9], C4[9];
	//string cipher = plain.substr(0,8);
	string ek = KeyExtension(key), str;
	for (int i = 0; i < 32; i++) {

		sscanf_s(ek.substr(8 * i, 8).c_str(), "%x", &tmp);
		tmp = cip[i + 1] ^ cip[i + 2] ^ cip[i + 3] ^ tmp;
		sprintf_s(C, "%08x", tmp);
		str = T(string(C));
		sscanf_s(str.c_str(), "%x", &tmp);
		cip[i + 4] = cip[i] ^ tmp;
		//cout << "rk[" + to_string(i) + "] = " + ek.substr(8 * i, 8) + "    X[" + to_string(i) + "] = " + cip[i + 4] << endl;
	}

	sprintf_s(C1, "%08x", cip[35]);
	sprintf_s(C2, "%08x", cip[34]);
	str = string(C1) + string(C2);
	sprintf_s(C3, "%08x", cip[33]);
	sprintf_s(C4, "%08x", cip[32]);
	str += (string(C3) + string(C4));
	return str;
}

string decode(string cip, string key) {//解密函数实现
	cout << "轮密钥与每轮输出状态：" << endl;
	cout << endl;

	unsigned pln[36] = { 0 }, tmp;
	sscanf_s(cip.substr(0, 8).c_str(), "%x", &pln[0]);
	sscanf_s(cip.substr(8, 8).c_str(), "%x", &pln[1]);
	sscanf_s(cip.substr(16, 8).c_str(), "%x", &pln[2]);
	sscanf_s(cip.substr(24).c_str(), "%x", &pln[3]);
	char C[9], C1[9], C2[9], C3[9], C4[9];
	string ek = KeyExtension(key), str;

	for (int i = 0; i < 32; i++) {

		sscanf_s(ek.substr(8 * (31 - i), 8).c_str(), "%x", &tmp);
		tmp = pln[i + 1] ^ pln[i + 2] ^ pln[i + 3] ^ tmp;
		sprintf_s(C, "%08x", tmp);
		str = T(string(C));
		sscanf_s(str.c_str(), "%x", &tmp);
		pln[i + 4] = pln[i] ^ tmp;
		//cout << "rk[" + to_string(i) + "] = " + ek.substr(8 * i, 8) + "    X[" + to_string(i) + "] = " + cip[i + 4] << endl;
	}
	cout << endl;
	sprintf_s(C1, "%08x", pln[35]);
	sprintf_s(C2, "%08x", pln[34]);
	str = string(C1) + string(C2);
	sprintf_s(C3, "%08x", pln[33]);
	sprintf_s(C4, "%08x", pln[32]);
	str += (string(C3) + string(C4));
	return str;
}

int new_encode(string pln1, string pln2, string pln3, string pln4, string key) {

	//同时加密 pln1, pln2 pln3, pln4
	unsigned tmp;
	unsigned cip1[36] = { 0 };
	sscanf_s(pln1.substr(0, 8).c_str(), "%x", &cip1[0]);
	sscanf_s(pln1.substr(8, 8).c_str(), "%x", &cip1[1]);
	sscanf_s(pln1.substr(16, 8).c_str(), "%x", &cip1[2]);
	sscanf_s(pln1.substr(24, 8).c_str(), "%x", &cip1[3]);
	unsigned cip2[36] = { 0 };
	sscanf_s(pln2.substr(0, 8).c_str(), "%x", &cip2[0]);
	sscanf_s(pln2.substr(8, 8).c_str(), "%x", &cip2[1]);
	sscanf_s(pln2.substr(16, 8).c_str(), "%x", &cip2[2]);
	sscanf_s(pln2.substr(24, 8).c_str(), "%x", &cip2[3]);
	unsigned cip3[36] = { 0 };
	sscanf_s(pln3.substr(0, 8).c_str(), "%x", &cip3[0]);
	sscanf_s(pln3.substr(8, 8).c_str(), "%x", &cip3[1]);
	sscanf_s(pln3.substr(16, 8).c_str(), "%x", &cip3[2]);
	sscanf_s(pln3.substr(24, 8).c_str(), "%x", &cip3[3]);
	unsigned cip4[36] = { 0 };
	sscanf_s(pln4.substr(0, 8).c_str(), "%x", &cip4[0]);
	sscanf_s(pln4.substr(8, 8).c_str(), "%x", &cip4[1]);
	sscanf_s(pln4.substr(16, 8).c_str(), "%x", &cip4[2]);
	sscanf_s(pln4.substr(24, 8).c_str(), "%x", &cip4[3]);
	char C0[9], C1[9], C2[9], C3[9];
	//string cipher = plain.substr(0,8);
	string ek = KeyExtension(key), str0, str1, str2, str3;


	__m128i M1, M2, M3, M4;
	int tmpC[4];

	for (int i = 0; i < 32; i++) {


		sscanf_s(ek.substr(8 * i, 8).c_str(), "%x", &tmp);

		M1 = _mm_set_epi32(cip1[i + 1], cip2[i + 1], cip3[i + 1], cip4[i + 1]);
		M2 = _mm_set_epi32(cip1[i + 2], cip2[i + 2], cip3[i + 2], cip4[i + 2]);
		M3 = _mm_set_epi32(cip1[i + 3], cip2[i + 3], cip3[i + 3], cip4[i + 3]);
		M4 = _mm_set1_epi32(tmp);

		__m128i N1 = _mm_xor_epi32(M1, M2);
		__m128i N2 = _mm_xor_epi32(M3, M4);
		__m128i N3 = _mm_xor_epi32(N1, N2);

		_mm_store_si128((__m128i*)(tmpC), N3);


		sprintf_s(C0, "%08x", tmpC[0]);
		sprintf_s(C1, "%08x", tmpC[1]);
		sprintf_s(C2, "%08x", tmpC[2]);
		sprintf_s(C3, "%08x", tmpC[3]);

		str0 = T(string(C0));
		str1 = T(string(C1));
		str2 = T(string(C2));
		str3 = T(string(C3));

		unsigned tmp0, tmp1, tmp2, tmp3;
		sscanf_s(str0.c_str(), "%x", &tmp0);
		sscanf_s(str1.c_str(), "%x", &tmp1);
		sscanf_s(str2.c_str(), "%x", &tmp2);
		sscanf_s(str3.c_str(), "%x", &tmp3);

		cip1[i + 4] = cip1[i] ^ tmp0;
		cip2[i + 4] = cip2[i] ^ tmp1;
		cip3[i + 4] = cip3[i] ^ tmp2;
		cip4[i + 4] = cip4[i] ^ tmp3;

	}

	//对于cip1,2,3,4分别返回
	cout << endl;
	string r0, r1, r2, r3;
	sprintf_s(C0, "%08x", cip1[35]);
	sprintf_s(C1, "%08x", cip1[34]);
	r0 = string(C0) + string(C1);
	sprintf_s(C2, "%08x", cip1[33]);
	sprintf_s(C3, "%08x", cip1[32]);
	r0 += (string(C2) + string(C3));
	cout << r0 << endl;

	sprintf_s(C0, "%08x", cip2[35]);
	sprintf_s(C1, "%08x", cip2[34]);
	r1 = string(C0) + string(C1);
	sprintf_s(C2, "%08x", cip2[33]);
	sprintf_s(C3, "%08x", cip2[32]);
	r1 += (string(C2) + string(C3));
	cout << r1 << endl;

	sprintf_s(C0, "%08x", cip3[35]);
	sprintf_s(C1, "%08x", cip3[34]);
	r2 = string(C0) + string(C1);
	sprintf_s(C2, "%08x", cip3[33]);
	sprintf_s(C3, "%08x", cip3[32]);
	r2 += (string(C2) + string(C3));
	cout << r2 << endl;

	sprintf_s(C0, "%08x", cip4[35]);
	sprintf_s(C1, "%08x", cip4[34]);
	r3 = string(C0) + string(C1);
	sprintf_s(C2, "%08x", cip4[33]);
	sprintf_s(C3, "%08x", cip4[32]);
	r3 += (string(C2) + string(C3));
	cout << r3 << endl;
	return 0;
}

int new_encode2(string pln1, string pln2, string pln3, string pln4, string pln5, string pln6, string pln7, string pln8, string key) {

	//同时加密 pln1, pln2 pln3, pln4
	unsigned tmp;
	unsigned cip1[36] = { 0 };
	sscanf_s(pln1.substr(0, 8).c_str(), "%x", &cip1[0]);
	sscanf_s(pln1.substr(8, 8).c_str(), "%x", &cip1[1]);
	sscanf_s(pln1.substr(16, 8).c_str(), "%x", &cip1[2]);
	sscanf_s(pln1.substr(24, 8).c_str(), "%x", &cip1[3]);
	unsigned cip2[36] = { 0 };
	sscanf_s(pln2.substr(0, 8).c_str(), "%x", &cip2[0]);
	sscanf_s(pln2.substr(8, 8).c_str(), "%x", &cip2[1]);
	sscanf_s(pln2.substr(16, 8).c_str(), "%x", &cip2[2]);
	sscanf_s(pln2.substr(24, 8).c_str(), "%x", &cip2[3]);
	unsigned cip3[36] = { 0 };
	sscanf_s(pln3.substr(0, 8).c_str(), "%x", &cip3[0]);
	sscanf_s(pln3.substr(8, 8).c_str(), "%x", &cip3[1]);
	sscanf_s(pln3.substr(16, 8).c_str(), "%x", &cip3[2]);
	sscanf_s(pln3.substr(24, 8).c_str(), "%x", &cip3[3]);
	unsigned cip4[36] = { 0 };
	sscanf_s(pln4.substr(0, 8).c_str(), "%x", &cip4[0]);
	sscanf_s(pln4.substr(8, 8).c_str(), "%x", &cip4[1]);
	sscanf_s(pln4.substr(16, 8).c_str(), "%x", &cip4[2]);
	sscanf_s(pln4.substr(24, 8).c_str(), "%x", &cip4[3]);
	unsigned cip5[36] = { 0 };
	sscanf_s(pln5.substr(0, 8).c_str(), "%x", &cip5[0]);
	sscanf_s(pln5.substr(8, 8).c_str(), "%x", &cip5[1]);
	sscanf_s(pln5.substr(16, 8).c_str(), "%x", &cip5[2]);
	sscanf_s(pln5.substr(24, 8).c_str(), "%x", &cip5[3]);
	unsigned cip6[36] = { 0 };
	sscanf_s(pln6.substr(0, 8).c_str(), "%x", &cip6[0]);
	sscanf_s(pln6.substr(8, 8).c_str(), "%x", &cip6[1]);
	sscanf_s(pln6.substr(16, 8).c_str(), "%x", &cip6[2]);
	sscanf_s(pln6.substr(24, 8).c_str(), "%x", &cip6[3]);
	unsigned cip7[36] = { 0 };
	sscanf_s(pln7.substr(0, 8).c_str(), "%x", &cip7[0]);
	sscanf_s(pln7.substr(8, 8).c_str(), "%x", &cip7[1]);
	sscanf_s(pln7.substr(16, 8).c_str(), "%x", &cip7[2]);
	sscanf_s(pln7.substr(24, 8).c_str(), "%x", &cip7[3]);
	unsigned cip8[36] = { 0 };
	sscanf_s(pln8.substr(0, 8).c_str(), "%x", &cip8[0]);
	sscanf_s(pln8.substr(8, 8).c_str(), "%x", &cip8[1]);
	sscanf_s(pln8.substr(16, 8).c_str(), "%x", &cip8[2]);
	sscanf_s(pln8.substr(24, 8).c_str(), "%x", &cip8[3]);
	char C0[9], C1[9], C2[9], C3[9], C4[9], C5[9], C6[9], C7[9];
	//string cipher = plain.substr(0,8);
	string ek = KeyExtension(key), str0, str1, str2, str3, str4, str5, str6, str7;


	__m256i M1, M2, M3, M4;
	int tmpC[8];

	for (int i = 0; i < 32; i++) {


		sscanf_s(ek.substr(8 * i, 8).c_str(), "%x", &tmp);

		M1 = _mm256_set_epi32(cip1[i + 1], cip2[i + 1], cip3[i + 1], cip4[i + 1], cip5[i + 1], cip6[i + 1], cip7[i + 1], cip8[i + 1]);
		M2 = _mm256_set_epi32(cip1[i + 2], cip2[i + 2], cip3[i + 2], cip4[i + 2], cip5[i + 2], cip6[i + 2], cip7[i + 2], cip8[i + 2]);
		M3 = _mm256_set_epi32(cip1[i + 3], cip2[i + 3], cip3[i + 3], cip4[i + 3], cip5[i + 3], cip6[i + 3], cip7[i + 3], cip8[i + 3]);
		M4 = _mm256_set1_epi32(tmp);

		__m256i N1 = _mm256_xor_epi32(M1, M2);
		__m256i N2 = _mm256_xor_epi32(M3, M4);
		__m256i N3 = _mm256_xor_epi32(N1, N2);

		_mm256_store_si256((__m256i*)(tmpC), N3);


		sprintf_s(C0, "%08x", tmpC[0]);
		sprintf_s(C1, "%08x", tmpC[1]);
		sprintf_s(C2, "%08x", tmpC[2]);
		sprintf_s(C3, "%08x", tmpC[3]);
		sprintf_s(C4, "%08x", tmpC[4]);
		sprintf_s(C5, "%08x", tmpC[5]);
		sprintf_s(C6, "%08x", tmpC[6]);
		sprintf_s(C7, "%08x", tmpC[7]);

		str0 = T(string(C0));
		str1 = T(string(C1));
		str2 = T(string(C2));
		str3 = T(string(C3));
		str4 = T(string(C4));
		str5 = T(string(C5));
		str6 = T(string(C6));
		str7 = T(string(C7));

		unsigned tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
		sscanf_s(str0.c_str(), "%x", &tmp0);
		sscanf_s(str1.c_str(), "%x", &tmp1);
		sscanf_s(str2.c_str(), "%x", &tmp2);
		sscanf_s(str3.c_str(), "%x", &tmp3);
		sscanf_s(str4.c_str(), "%x", &tmp4);
		sscanf_s(str5.c_str(), "%x", &tmp5);
		sscanf_s(str6.c_str(), "%x", &tmp6);
		sscanf_s(str7.c_str(), "%x", &tmp7);

		cip1[i + 4] = cip1[i] ^ tmp0;
		cip2[i + 4] = cip2[i] ^ tmp1;
		cip3[i + 4] = cip3[i] ^ tmp2;
		cip4[i + 4] = cip4[i] ^ tmp3;
		cip5[i + 4] = cip5[i] ^ tmp4;
		cip6[i + 4] = cip6[i] ^ tmp5;
		cip7[i + 4] = cip7[i] ^ tmp6;
		cip8[i + 4] = cip8[i] ^ tmp7;

	}

	//对于cip1,2,3,4分别返回
	cout << endl;
	string r0, r1, r2, r3, r4, r5, r6, r7;
	sprintf_s(C0, "%08x", cip1[35]);
	sprintf_s(C1, "%08x", cip1[34]);
	r0 = string(C0) + string(C1);
	sprintf_s(C2, "%08x", cip1[33]);
	sprintf_s(C3, "%08x", cip1[32]);
	r0 += (string(C2) + string(C3));
	cout << r0 << endl;

	sprintf_s(C0, "%08x", cip2[35]);
	sprintf_s(C1, "%08x", cip2[34]);
	r1 = string(C0) + string(C1);
	sprintf_s(C2, "%08x", cip2[33]);
	sprintf_s(C3, "%08x", cip2[32]);
	r1 += (string(C2) + string(C3));
	cout << r1 << endl;

	sprintf_s(C0, "%08x", cip3[35]);
	sprintf_s(C1, "%08x", cip3[34]);
	r2 = string(C0) + string(C1);
	sprintf_s(C2, "%08x", cip3[33]);
	sprintf_s(C3, "%08x", cip3[32]);
	r2 += (string(C2) + string(C3));
	cout << r2 << endl;

	sprintf_s(C0, "%08x", cip4[35]);
	sprintf_s(C1, "%08x", cip4[34]);
	r3 = string(C0) + string(C1);
	sprintf_s(C2, "%08x", cip4[33]);
	sprintf_s(C3, "%08x", cip4[32]);
	r3 += (string(C2) + string(C3));
	cout << r3 << endl;

	sprintf_s(C0, "%08x", cip5[35]);
	sprintf_s(C1, "%08x", cip5[34]);
	r0 = string(C0) + string(C1);
	sprintf_s(C2, "%08x", cip5[33]);
	sprintf_s(C3, "%08x", cip5[32]);
	r0 += (string(C2) + string(C3));
	cout << r0 << endl;

	sprintf_s(C0, "%08x", cip6[35]);
	sprintf_s(C1, "%08x", cip6[34]);
	r1 = string(C0) + string(C1);
	sprintf_s(C2, "%08x", cip6[33]);
	sprintf_s(C3, "%08x", cip6[32]);
	r1 += (string(C2) + string(C3));
	cout << r1 << endl;

	sprintf_s(C0, "%08x", cip7[35]);
	sprintf_s(C1, "%08x", cip7[34]);
	r2 = string(C0) + string(C1);
	sprintf_s(C2, "%08x", cip7[33]);
	sprintf_s(C3, "%08x", cip7[32]);
	r2 += (string(C2) + string(C3));
	cout << r2 << endl;

	sprintf_s(C0, "%08x", cip8[35]);
	sprintf_s(C1, "%08x", cip8[34]);
	r3 = string(C0) + string(C1);
	sprintf_s(C2, "%08x", cip8[33]);
	sprintf_s(C3, "%08x", cip8[32]);
	r3 += (string(C2) + string(C3));
	cout << r3 << endl;
	return 0;
}

int main() {//主函数


	double run_time1 = 0, run_time, run_time2 = 0;//运行时间 
	LARGE_INTEGER Frequency;//计数器频率 
	LARGE_INTEGER start_PerformanceCount;//起始计数器  
	LARGE_INTEGER end_PerformanceCount;//结束计数器  

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&start_PerformanceCount);
	string str_ = "0123456789ABCDEFFEDCBA9876543210";
	string str = "0123456789ABCDEFFEDCBA98765432100123456789ABCDEFFEDCBA98765432100123456789ABCDEFFEDCBA98765432100123456789ABCDEFFEDCBA98765432100123456789ABCDEFFEDCBA98765432100123456789ABCDEFFEDCBA98765432100123456789ABCDEFFEDCBA98765432100123456789ABCDEFFEDCBA9876543210";
	string key = "0123456789ABCDEFFEDCBA9876543210";
	for (int i = 0; i < 1; i++)
	{
		string str1 = str.substr(0 + 256 * i, 32);
		string str2 = str.substr(32 + 256 * i, 32);
		string str3 = str.substr(64 + 256 * i, 32);
		string str4 = str.substr(96 + 256 * i, 32);
		string str5 = str.substr(128 + 256 * i, 32);
		string str6 = str.substr(160 + 256 * i, 32);
		string str7 = str.substr(192 + 256 * i, 32);
		string str8 = str.substr(224 + 256 * i, 32);

		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&start_PerformanceCount);
		new_encode2(str1, str2, str3, str4, str5, str6, str7, str8, key);
		QueryPerformanceCounter(&end_PerformanceCount);
		run_time1 += (end_PerformanceCount.QuadPart - start_PerformanceCount.QuadPart) / (double)Frequency.QuadPart;

		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&start_PerformanceCount);
		cout << encode(str1, key) << endl;
		cout << encode(str2, key) << endl;
		cout << encode(str3, key) << endl;
		cout << encode(str4, key) << endl;
		cout << encode(str5, key) << endl;
		cout << encode(str6, key) << endl;
		cout << encode(str7, key) << endl;
		cout << encode(str8, key) << endl;
		QueryPerformanceCounter(&end_PerformanceCount);
		run_time2 += (end_PerformanceCount.QuadPart - start_PerformanceCount.QuadPart) / (double)Frequency.QuadPart;

	}
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&start_PerformanceCount);
	cout << endl << encode(str_, key) << endl;

	QueryPerformanceCounter(&end_PerformanceCount);
	run_time = (end_PerformanceCount.QuadPart - start_PerformanceCount.QuadPart) / (double)Frequency.QuadPart;
	printf("single encryption running time:%e\n", run_time);
	printf("ordinary running time:%e\n", run_time2);
	printf(" four sets running time:%e\n", run_time1);
}