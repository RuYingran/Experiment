# 优化sm3
1.利用SIMD指令集优化sm3(基于GMSSL开源代码实现）  
2.多线程优化sm3  
3.循环展开优化sm3  
# sm3_birthday_attack
## 实验原理  
利用生日攻击的原理产生碰撞，找到两个任意的消息计算其hash值，寻找前nbit相同的两个消息即可认为找到碰撞。  
## 实验结果
### 4bit  
![image](https://user-images.githubusercontent.com/109326479/180223699-83c34883-fa25-4475-b345-7342b2ceeeb8.png)
### 8bit
![image](https://user-images.githubusercontent.com/109326479/180224614-eb8a8ffd-7ded-4cdb-9e2e-cad99b67fb0c.png)
### 12bit
![image](https://user-images.githubusercontent.com/109326479/180224734-966b0c19-a3d4-4f86-87ac-5b9ebb86a056.png)
### 16bit
![image](https://user-images.githubusercontent.com/109326479/180226840-990968f5-a1fa-437a-b2c6-2051b1743105.png)
# sm3_Rho_attack
