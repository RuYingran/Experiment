# 本次实验全部由茹莹然完成  
## 已完成项目：
1.优化sm3,sm3生日攻击和Rho攻击，sm3长度扩展攻击  
2.优化sm4  
3.SHA256  
## 未完成项目：
sm2,区块链，比特币，以太坊  
# 优化sm3
## 利用SIMD指令集优化sm3
### 实验简介
SIMD 指令集：同时对数据向量中的每一个分别执行相同的操作从而实现空间上的并行性的技术。一次获取多个操作数后，存放于一个大型寄存器，再进行运算，从而达到一条指令完成对多个对象计算的效果，实现加速。
### 实验结果
![image](https://user-images.githubusercontent.com/109326479/181879938-b9f01335-15ad-43ea-bc5c-b9f81938cdc9.png)
### 参考文献
https://zhuanlan.zhihu.com/p/468605906
https://blog.csdn.net/AAAA202012/article/details/123983364
https://www.cnblogs.com/0211ji/p/15799530.html
https://blog.csdn.net/qq_40662424/article/details/121637732
https://github.com/guanzhi/GmSSL/blob/develop/src/sm3.c

## 多线程优化sm3
### 实验简介
多线程：在单个程序中同时运行多个线程来完成处理工作从而缩短时间。
### 实验结果
![image](https://user-images.githubusercontent.com/109326479/181880252-ef6c1e19-a491-4983-8638-52d3a3bd97dc.png)
### 参考文献
https://blog.csdn.net/calmuse/article/details/98493554
https://blog.51cto.com/u_14411234/3130169
https://wenku.baidu.com/view/d4062f0ebe64783e0912a21614791711cc79799a.html

## 循环展开优化sm3 
### 实验简介
循环展开：利用CPU指令级并行，来降低循环的开销，同时也有利于指令流水线的高效调度。
### 实验结果
![image](https://user-images.githubusercontent.com/109326479/181880302-19748c64-a248-4caf-8eb0-34cfe29a5ee6.png)
### 参考文献
https://www.cnblogs.com/0211ji/p/15799530.html
https://blog.csdn.net/qq_40662424/article/details/121637732
https://github.com/guanzhi/GmSSL/blob/develop/src/sm3.c

# sm3_birthday_attack
## 实验原理  
利用生日攻击的原理产生碰撞，找到两个任意的消息计算其hash值，寻找前nbit相同的两个消息即可认为找到碰撞。可实现最长16bit生日攻击。 
## 实验结果
### 4bit  
![image](https://user-images.githubusercontent.com/109326479/180223699-83c34883-fa25-4475-b345-7342b2ceeeb8.png)
### 8bit
![image](https://user-images.githubusercontent.com/109326479/180224614-eb8a8ffd-7ded-4cdb-9e2e-cad99b67fb0c.png)
### 12bit
![image](https://user-images.githubusercontent.com/109326479/180224734-966b0c19-a3d4-4f86-87ac-5b9ebb86a056.png)
### 16bit
![image](https://user-images.githubusercontent.com/109326479/180226840-990968f5-a1fa-437a-b2c6-2051b1743105.png)
### 参考文献
https://blog.csdn.net/Metal1/article/details/79887252

# sm3_Rho_attack
## 实验原理
![image](https://user-images.githubusercontent.com/109326479/180592779-dac469c6-4e47-4c04-8a09-f3b295c947a0.png)  
f函数为2*x+1,不断计算f函数结果产生环从而产生碰撞。可实现最长24bitRho攻击。
## 实验结果
### 8bit
![image](https://user-images.githubusercontent.com/109326479/180592882-f8f230da-19bc-4a24-bd0a-842465532cfc.png)
### 16bit
![image](https://user-images.githubusercontent.com/109326479/180592886-1ba3f06c-be1d-4b9c-9ac0-a149acc5a701.png)
### 24bit
![image](https://user-images.githubusercontent.com/109326479/180592983-e85c953a-7746-4f30-8911-bd3e995d788b.png)
### 参考文献
https://wenku.baidu.com/view/caa7919924c52cc58bd63186bceb19e8b9f6ec9b?fr=xueshu

# sm3_length_extension_attack
## 实验原理
对mes1+padding+mes2进行hash计算hash_value1,后对mes1进行hash，将得到的值作为IV对mes2+padding+length进行hash计算hash_value2,得到的hash值相同则代表可实现长度扩展攻击。  
## 实验结果
![image](https://user-images.githubusercontent.com/109326479/180593319-8a4847fe-1cba-4079-8c8d-88df3ae3a676.png)
# 优化sm4
## 利用SIMD指令集优化sm4  
### 实验简介
加密标准长度的明文：由于在 SM4 算法中，各组织之间具有连环的关系，无法将 32 组加密过程分开，且在各组加密之中使用的是自定义函数，在 AVX 指令集中无法进行使用其内置函数，故将重点放在下面的加密多组标准长度时进行加速。
### 实验结果
![image](https://user-images.githubusercontent.com/109326479/181880717-1cef199b-11c9-4f32-8042-f7f9b496d554.png)
### 参考文献
https://blog.csdn.net/m0_51565722/article/details/124330892
https://blog.csdn.net/qq_41610725/article/details/116795599
https://zhuanlan.zhihu.com/p/416172020
https://blog.csdn.net/AAAA202012/article/details/123983364

## 多线程优化sm4  
### 实验简介
通过同时运行多个线程处理加密操作，控制采用不同的线程数执行大量的加密操作，从而最大限度地利用 CPU 资源。
### 实验结果
![image](https://user-images.githubusercontent.com/109326479/181882671-1d624f54-8e06-41f4-ac85-aa5f191b0980.png)
### 参考文献
https://blog.csdn.net/calmuse/article/details/98493554
https://blog.51cto.com/u_14411234/3130169
https://wenku.baidu.com/view/d4062f0ebe64783e0912a21614791711cc79799a.html

## 流水线优化sm4  
### 实验简介
流水线的基本思想是每个阶段一次处理一条指令的特定部分，将部分结果传递给下一个阶段。每轮密钥和加密都要依赖于前几轮的结果，因此不能将密钥和加密各自进行轮与轮之间的流水线加速。而密钥生成不被加密影响，但加密需要用到密钥生成的结果，因此考虑令密钥生成算法先行一轮，而后进行加密运算，在进行加密计算的同时，还可以添加另一个线程进行下一轮轮密钥的生成计算，以此减少所需时间。因此需要实现每轮的加密和密钥生成函数，同时由于 SM4 算法密钥需要预先进行计算，因此需要额外附加一个密钥初始化函数。
### 实验结果
加密1000次所花费时间：
![image](https://user-images.githubusercontent.com/109326479/181887647-149021d5-0c7b-4f3a-9374-70ca7c18ee42.png)
### 参考文献
https://zhuanlan.zhihu.com/p/81344978
https://blog.csdn.net/qq_20880415/article/details/106080960
https://www.cnblogs.com/Jr1Preg/p/12306533.html

# SHA256
## 实验简介
SHA256是一种密码散列函数，对于任意长度的消息，SHA256都会产生一个256bit长的哈希值，称做消息摘要。这个摘要相当于是个长度为32个字节的数组，通常用长度为64的十六进制字符串来表示。通常可分为：常量的初始化、信息预处理、逻辑运算。
## 实验结果
![image](https://user-images.githubusercontent.com/109326479/181890848-951b91ff-1931-4d9a-8b3d-13c2280faaab.png)
## 参考文献
https://blog.csdn.net/u011583927/article/details/80905740
https://zhuanlan.zhihu.com/p/94619052
