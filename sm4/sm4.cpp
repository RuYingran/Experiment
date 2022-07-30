#include <iostream>
#include <string>
#include <cmath>
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
	cout << "轮密钥与每轮输出状态：" << endl;
	cout << endl;
	//string cipher[36] = { plain.substr(0,8),plain.substr(8,8),plain.substr(16,8),plain.substr(24) };

	unsigned cip[36] = { 0 }, tmp;
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
	cout << endl;
	sprintf_s(C1, "%08x", cip[35]);
	sprintf_s(C2, "%08x", cip[34]);
	str = string(C1) + string(C2);
	sprintf_s(C3, "%08x", cip[33]);
	sprintf_s(C4, "%08x", cip[32]);
	str += (string(C3) + string(C4));
	return str;
}
/**/
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

int main() {//主函数
	string str[8] = { "0123456789ABCDEFFEDCBA9876543210","1232534244DDFEBECBDBAA5593345567","3725343638DDBEABBCAFEF5015683450","0458627910ADDFEFDBEFCA9762439108","dafedd99d37e7c0be4423690afd1a5bd","2704bbba006bd8d0cb367d74a692018f","a28185892f84fd3f8491aebf41c22417","80103a84f1eec8626692e13c290eadfe" };
	string key[8] = { "0123456789ABCDEFFEDCBA9876543210","2ed4737abbf95f438143e095c01b052f","cb53e31aa11f2702e34822d29645d678","f8d225edd0851c235c24ebe865e1dac4","f9d921f0460c1ca8aec736e5a05a590d","11b60d7dccbbce37dcb0e26209d4d7c5","6edbaff0c9ab24513186aa27775cdf50","47e79d3015d5841b98ec21e7cf7e2a6a" };
	string cipher[8], plain[8];
	for (int i = 0; i < 8; i++)//ECB模式
	{
		cout << "第" << i << "块：" << endl;
		cout << "明    文：" << str[i].substr(0, 8) << "  " << str[i].substr(8, 8) << "  " << str[i].substr(16, 8) << "  " << str[i].substr(24, 8) << endl;
		cout << endl;
		KeyExtension(key[0]);
		cout << "加密密钥：" << key[i].substr(0, 8) << "  " << key[i].substr(8, 8) << "  " << key[i].substr(16, 8) << "  " << key[i].substr(24, 8) << endl;
		cout << endl;

		cipher[i] = encode(str[i], key[0]);
		cout << cipher << endl;
		plain[i] = decode(cipher[i], key[0]);

		cout << "明    文：" << plain[i].substr(0, 8) << "  " << plain[i].substr(8, 8) << "  " << plain[i].substr(16, 8) << "  " << plain[i].substr(24, 8) << endl;
	}
	return 0;
}
