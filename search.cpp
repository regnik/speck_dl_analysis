#include<cstdint>
#include<chrono>
#include<random>
#include<time.h>
#include<bitset>
#include<iostream>
#include<fstream>
#include<iomanip>
using namespace std;
#define ROTR
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define ROTL16(x,r)(((x)<<(r))|(x>>(16-(r))))
#define ROTR16(x,r)(((x)>>(r))|(x<<(16-(r))))
#define ER16(x,y,k)(x=ROTR16(x,7),x+=y,x^=k,y=ROTL16(y,2),y^=x)
#define DR16(x,y,k)(y^=x,y=ROTR16(y,2),x^=k,x-=y,x=ROTL16(x,7))
//speck 32/64 16*2bit-input 64bit-masterkey 22rounds

void Speck3264KeySchedule(u16 K[], u16 rk[]) {//key schedule
 u16 i, D = K[3], C = K[2], B = K[1], A = K[0];
 for (i = 0; i < 21;) {
  rk[i] = A; ER16(B, A, i++);
  rk[i] = A; ER16(C, A, i++);
  rk[i] = A; ER16(D, A, i++);
 }
 rk[21] = A;
}

void Speck3264Encrypt(u16 Pt[], u16 Ct[], u16 rk[], u16 r) {//encrypt
 u16 i;
 Ct[0] = Pt[0]; Ct[1] = Pt[1];
 for (i = 0; i < r;)
  ER16(Ct[1], Ct[0], rk[i++]);
}

u16 bit_dif[16] = { 0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000 };
u32 counter[101][33] = { 0 };
u32 p = 0;
u16 K[4] = { 0 };
u16 rk[22] = { 0 };
std::random_device rd;  
std::mt19937 gen(rd()); // random engine
int main() {
 ofstream fout;
 fout.open("7r_fullbitL00.txt");
#pragma omp parallel for
 for (int k = 0; k < 100; k++) {//mixed with different key
  K[0] = gen();
  K[1] = gen();
  K[2] = gen();
  K[3] = gen();
  Speck3264KeySchedule(K, rk);
  for (u32 d = 0; d < 268435456; d++) {//total data 
   u16 Pt[2] = { 0 }, Ct[2] = { 0 };
   u16 Pt_d[2] = { 0 }, Ct_d[2] = { 0 };
   p = gen();
   Pt[0] = p & 0xffff;
   Pt[1] = p >> 16;
   Speck3264Encrypt(Pt, Ct, rk, 7);
   bitset<16> bs1(Ct[1]);
   for (int i = 0; i < 32; i++) {
    if (i < 16) {
     Pt_d[0] = Pt[0] ^ bit_dif[i];
     Pt_d[1] = Pt[1];
    }
    else {
     Pt_d[0] = Pt[0];
     Pt_d[1] = Pt[1] ^ bit_dif[i - 16];
    }//set difference
    Speck3264Encrypt(Pt_d, Ct_d, rk, 7);
    bitset<16> bs3(Ct_d[1]);
    int value = bs1[0] ^ bs3[0];//set difference on the least significant bit of left input branch
    if (value == 0) counter[k][i]++;
			}
		}
		fout << "第" << k << "次:" << endl;
		for (int j = 0; j < 32; j++) {
			fout << counter[k][j] << endl;
			double a = counter[k][j];
			double b = 268435456;
			double c = (a / b) - 0.5;
			fout << j << "比特差分偏差为:" << c << endl;
		}
	}
	fout.close();
	return 0;
}
