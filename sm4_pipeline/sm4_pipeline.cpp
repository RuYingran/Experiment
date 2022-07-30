#include <iostream>
#include <string>
#include <cmath>
#include<thread>
#include<Windows.h>
using namespace std;

string FK[4] = { "A3B1BAC6", "56AA3350", "677D9197", "B27022DC" };

string CK[32] = { "00070E15", "1C232A31", "383F464D", "545B6269",
				  "70777E85", "8C939AA1", "A8AFB6BD", "C4CBD2D9",
				  "E0E7EEF5", "FC030A11", "181F262D", "343B4249",
				  "50575E65", "6C737A81", "888F969D", "A4ABB2B9",
				  "C0C7CED5", "DCE3EAF1", "F8FF060D", "141B2229",
				  "30373E45", "4C535A61", "686F767D", "848B9299",
				  "A0A7AEB5", "BCC3CAD1", "D8DFE6ED", "F4FB0209",
				  "10171E25", "2C333A41", "484F565D", "646B7279" };

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

string BinToHex(string str) {//二进制转换为十六进制的函数实现
	string hex = "";
	int temp = 0;
	while (str.size() % 4 != 0) {
		str = "0" + str;
	}
	for (int i = 0; i < str.size(); i += 4) {
		temp = (str[i] - '0') * 8 + (str[i + 1] - '0') * 4 + (str[i + 2] - '0') * 2 + (str[i + 3] - '0') * 1;
		if (temp < 10) {
			hex += to_string(temp);
		}
		else {
			hex += 'A' + (temp - 10);
		}
	}
	return hex;
}

string HexToBin(string str) {//十六进制转换为二进制的函数实现
	string bin = "";
	string table[16] = { "0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111" };
	for (int i = 0; i < str.size(); i++) {
		if (str[i] >= 'A' && str[i] <= 'F') {
			bin += table[str[i] - 'A' + 10];
		}
		else if (str[i] >= 'a' && str[i] <= 'f') {
			bin += table[str[i] - 'a' + 10];
		}
		else {
			bin += table[str[i] - '0'];
		}
	}
	return bin;
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

string XOR(string str1, string str2) {//异或函数实现
	string res1 = HexToBin(str1);
	string res2 = HexToBin(str2);
	string res = "";
	for (int i = 0; i < res1.size(); i++) {
		if (res1[i] == res2[i]) {
			res += "0";
		}
		else {
			res += "1";
		}
	}
	return BinToHex(res);
}

void Key_Extension_Precedent(string& MK)//生成MK
{
	string k = "";
	string K[4] = { XOR(MK.substr(0,8),FK[0]),XOR(MK.substr(8,8),FK[1]),XOR(MK.substr(16,8),FK[2]),XOR(MK.substr(24),FK[3]) };
	for (int i = 0; i < 4; i++) k += K[i];
	MK = k;
}

void Key_Extension_Round(string& K, int i)//每一轮的密钥扩展
{
	string K_[4] = { K.substr(0,8),K.substr(8,8),K.substr(16,8),K.substr(24) };
	string k = "";
	string tmp = T2(XOR(XOR(XOR(K_[1], K_[2]), K_[3]), CK[i]));
	k += K_[1]; k += K_[2]; k += K_[3];
	k += XOR(K_[0], tmp);
	K = k;//下一轮的密钥根据这个生成，后八位就是下一轮的轮密钥
	//Sleep(1);
	//cout << K << endl;
}


void Key_Extension_31(string& K)
{
	for (int i = 1; i < 32; i++)
	{
		//for (int j = 0; j < 10; j++);
		Key_Extension_Round(K, i);
		//cout << i << "\t" << K << endl;
	}
}

void Encode_Round(string& rk, string& pln)//rk是轮密钥，应该是;pln是这次要加密的32位字
{
	string cipher[32] = { pln.substr(0,8),pln.substr(8,8),pln.substr(16,8),pln.substr(24) };
	string cip = "";
	string temp = T(XOR(XOR(XOR(cipher[1], cipher[2]), cipher[3]), rk.substr(24)));//rk后八位是轮密钥
	cip += cipher[1]; cip += cipher[2]; cip += cipher[3];
	cip += XOR(cipher[0], temp);
	pln = cip;
	//cout << pln << endl;
}

void Encode_32(string& pln, string& mk)
{
	for (int i = 0; i < 32; i++)
	{
		Encode_Round(mk, pln);
		//cout << i << "\t" << pln << endl;
	}
	//cout << "密   文：" << pln.substr(24, 8) << "  " << pln.substr(16, 8) << "  " << pln.substr(8, 8) << "  " << pln.substr(0, 8) << endl;
}


void encode_pipeline(string& MK, string& pln)
{
	Key_Extension_Precedent(MK);
	int flag = 0;
	int c = 0;
	Key_Extension_Round(MK, 0);
	for (int i = 0; i < 31; i++)
	{
		thread th1(Encode_Round, ref(MK), ref(pln));
		thread th2(Key_Extension_Round, ref(MK), i + 1);

		th1.join();
		th2.join();
	}
	/*thread th1(Encode_32,ref(pln), ref(MK));
	thread th2(Key_Extension_31, ref(MK));

	th2.join();
	th1.join();*/
	Encode_Round(MK, pln);
}

int main() {//主函数

	string str = "0123456789ABCDEFFEDCBA9876543210";
	string key = "0123456789ABCDEFFEDCBA9876543210";
	LARGE_INTEGER    a, a_, nFrequency;
	double timeAccuracy = 1000;
	QueryPerformanceFrequency(&nFrequency);
	QueryPerformanceCounter(&a);
	for (int i = 0; i < 12; i++)	encode_pipeline(key, str);
	QueryPerformanceCounter(&a_);
	printf("%.8f\n", timeAccuracy * ((double)a_.QuadPart - a.QuadPart) / nFrequency.QuadPart);
}
