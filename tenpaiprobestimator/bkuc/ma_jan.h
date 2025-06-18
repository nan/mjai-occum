#ifndef MA_JAN
#define MA_JAN
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include "fstream"
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>
//#include <string>
#include <cstdint>
#include <vector>
#define merusennu_N 624
#define merusennu_M 397
#define merusennu_MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_merusennu_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_merusennu_MASK 0x7fffffffUL /* least significant r bits */

//extern int *shanten_list;
//extern long long *uke_list;
//extern long long *uke_list1;
//boost::unordered_map<int,int> shanten_list;



class BKUC1681MajhanUtil {
public:

	int *shanten_list;
	
	//long long *uke_list;
	boost::unordered_map<std::string, int> uke_list;
	
	//long long uke_list[99999999];
	//76578816
	//99999999

	//long long *uke_list1;
	//boost::unordered_map<int,int> shanten_list;


	//static 
	unsigned long mt[merusennu_N]; /* the array for the state vector  */
	//static 
	int mti;

	BKUC1681MajhanUtil() {
		mti = merusennu_N + 1; /* mti==merusennu_N+1 means mt[merusennu_N] is not initialized */
	}

	//inline 
	int toInt(string s) { int v; istringstream sin(s); sin >> v; return v; }
	//inline 
	long long tolonglong(string s) { long long v; istringstream sin(s); sin >> v; return v; }
	double xor01(void) {
		static uint32_t x = 123456789;
		static uint32_t y = 362436069;
		static uint32_t z = 521288629;
		static uint32_t w = 88675123;
		uint32_t t;

		t = x ^ (x << 11);
		x = y; y = z; z = w;
		w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
		return (double)w / (double)0xffffffff;
	}

	uint32_t xor32(void) {
		static uint32_t x = 123456789;
		static uint32_t y = 362436069;
		static uint32_t z = 521288629;
		static uint32_t w = 88675123;
		uint32_t t;

		t = x ^ (x << 11);
		x = y; y = z; z = w;
		return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
	}

	void init_genrand(unsigned long s)
	{
		mt[0] = s & 0xffffffffUL;
		for (mti = 1; mti<merusennu_N; mti++) {
			mt[mti] =
				(1812433253UL * (mt[mti - 1] ^ (mt[mti - 1] >> 30)) + mti);
			mt[mti] &= 0xffffffffUL;
		}
	}
	void init_by_array(unsigned long init_key[], int key_length)
	{
		int i, j, k;
		init_genrand(19650218UL);
		i = 1; j = 0;
		k = (merusennu_N>key_length ? merusennu_N : key_length);
		for (; k; k--) {
			mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1664525UL))
				+ init_key[j] + j; /* non linear */
			mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
			i++; j++;
			if (i >= merusennu_N) { mt[0] = mt[merusennu_N - 1]; i = 1; }
			if (j >= key_length) j = 0;
		}
		for (k = merusennu_N - 1; k; k--) {
			mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1566083941UL))
				- i; /* non linear */
			mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
			i++;
			if (i >= merusennu_N) { mt[0] = mt[merusennu_N - 1]; i = 1; }
		}

		mt[0] = 0x80000000UL; /* merusennu_MSB is 1; assuring non-zero initial array */
	}
	unsigned long genrand_int32(void)
	{
		unsigned long y;
		static unsigned long mag01[2] = { 0x0UL, merusennu_MATRIX_A };
		/* mag01[x] = x * merusennu_MATRIX_A  for x=0,1 */

		if (mti >= merusennu_N) { /* generate merusennu_N words at one time */
			int kk;

			if (mti == merusennu_N + 1)   /* if init_genrand() has not been called, */
				init_genrand(5489UL); /* a default initial seed is used */

			for (kk = 0; kk<merusennu_N - merusennu_M; kk++) {
				y = (mt[kk] & UPPER_merusennu_MASK) | (mt[kk + 1] & LOWER_merusennu_MASK);
				mt[kk] = mt[kk + merusennu_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
			}
			for (; kk<merusennu_N - 1; kk++) {
				y = (mt[kk] & UPPER_merusennu_MASK) | (mt[kk + 1] & LOWER_merusennu_MASK);
				mt[kk] = mt[kk + (merusennu_M - merusennu_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
			}
			y = (mt[merusennu_N - 1] & UPPER_merusennu_MASK) | (mt[0] & LOWER_merusennu_MASK);
			mt[merusennu_N - 1] = mt[merusennu_M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

			mti = 0;
		}

		y = mt[mti++];

		/* Tempering */

		y ^= (y >> 11);
		y ^= (y << 7) & 0x9d2c5680UL;
		y ^= (y << 15) & 0xefc60000UL;
		y ^= (y >> 18);

		return y;
	}

	long genrand_int31(void)
	{
		return (long)(genrand_int32() >> 1);
	}

	double genrand_real1(void)
	{
		return genrand_int32()*(1.0 / 4294967295.0);
		/* divided by 2^32-1 */
	}

	double genrand_real2(void)
	{
		return genrand_int32()*(1.0 / 4294967296.0);
		/* divided by 2^32 */
	}

	double genrand_real3(void)
	{
		return (((double)genrand_int32()) + 0.5)*(1.0 / 4294967296.0);
		/* divided by 2^32 */
	}

	double genrand_res53(void)
	{
		unsigned long a = genrand_int32() >> 5, b = genrand_int32() >> 6;
		return(a*67108864.0 + b)*(1.0 / 9007199254740992.0);
	}

	int randam(int flag)
	{
		int i;
		if (flag == 1)
		{
			unsigned long init[4] = { 0x123, 0x234,0x345,0x456 }, length = 4;
			init_by_array(init, length);
		}
		i = genrand_int32();
		return i;
	}

	int tenpai(int aite[], int *mati)
	{
		int i, j, k, copy[9][34], koutu_suu = 0;
		int check = 0, l = 0, n = 0, agari;


		for (i = 0; i<34; i++)
		{
			copy[0][i] = aite[i];
			copy[1][i] = aite[i];
		}

		for (n = 0; n<34; n++)//一種類ずつ足す
		{
			copy[0][n]++;
			copy[1][n]++;
			for (i = 0; i<34; i++) /*頭を決める*/
			{
				if (copy[0][i] >= 2)
				{
					koutu_suu = 0;
					check = 1;
					copy[1][i] = copy[0][i] - 2;

					for (j = 0; j<34; j++) /*刻子を決める*/
					{
						//	if(n==0&&i==0)//printf("koutu=%d",koutu_suu);
						if (copy[1][j] >= 3 && koutu_suu == 0)
						{
							for (k = 0; k<34; k++)
							{
								copy[2][k] = copy[1][k];
							}

							copy[2][j] = copy[2][j] - 3;
							koutu_suu++;
							check = 2;
						}//if(copy[1][j]>=3 && koutu_suu==0)

						else if (copy[2][j] >= 3 && koutu_suu == 1)
						{
							for (k = 0; k<34; k++)
							{
								copy[3][k] = copy[1][k];
								copy[4][k] = copy[2][k];
							}
							copy[3][j] = copy[3][j] - 3;
							copy[4][j] = copy[4][j] - 3;
							koutu_suu++;
							check = 4;
						}//else if(copy[2][j]>=3 && koutu_suu==1)

						else if (copy[4][j] >= 3 && koutu_suu == 2)
						{
							for (k = 0; k<34; k++)
							{
								copy[5][k] = copy[1][k];
								copy[6][k] = copy[2][k];
								copy[7][k] = copy[3][k];
								copy[8][k] = copy[4][k];
							}
							copy[5][j] = copy[5][j] - 3;
							copy[6][j] = copy[6][j] - 3;
							copy[7][j] = copy[7][j] - 3;
							copy[8][j] = copy[8][j] - 3;
							koutu_suu++;
							check = 8;
						}//else if(copy[4][j]>=3 && koutu_suu==2)

						else if (copy[8][j] >= 3 && koutu_suu == 3)
						{
							for (k = 0; k<l; k++)
							{
								if (mati[k] == n)
								{
									goto owari;
								}
							}
							mati[l] = n;
							l++;
							goto owari;
						}
					}//for(j=0;j<=43;j++) 刻子を決める

					for (j = 1; j <= check; j++) /*順子をきめる*/
					{
						for (k = 0; k<25; k++)//ソウズの７以下
						{
							if (copy[j][k] >= 1 && copy[j][k + 1] >= 1 && copy[j][k + 2] >= 1 && k != 7 && k != 8 && k != 16 && k != 17)
							{
								copy[j][k] = copy[j][k] - 1;
								copy[j][k + 1] = copy[j][k + 1] - 1;
								copy[j][k + 2] = copy[j][k + 2] - 1;
								k--;
							}
						}

						agari = 1;

						for (k = 0; k<34; k++)
						{
							if (copy[j][k] != 0)
							{
								goto agarenai;
							}
						}

						if (agari == 1)
						{
							for (j = 0; j<l; j++)
							{
								if (mati[j] == n)
								{
									goto owari;
								}
							}
							mati[l] = n;
							l++;
							goto owari;
						}

					agarenai:;
					}//for(j=1;j<=check;j++) /*順子をきめる*/

				owari:;
					for (k = 0; k<34; k++)
					{
						copy[1][k] = copy[0][k];
					}
				}//if(copy[0][i] >= 2)
			}//for(i=0;i<34;i++) /*頭を決める*/

			copy[0][n]--;
			copy[1][n]--;


		}//一種類ずつ足す

		return l;

	}//int tenpai(int aite[],int *mati)

	int kumi(int hai[], int *mentu)
	{
		int i, j, k, n = 0, flag = 0;

		for (i = 0; i<34; i++)//刻子の種類
		{

			if (hai[i] == 0)
			{
				j = 4;
			}

			else if (hai[i] == 1)
			{
				j = 1;
			}

			else
			{
				j = 0;
			}

			for (k = 0; k<j; k++)
			{
				mentu[n + k] = i;
			}

			n += j;
		}

		for (i = 0; i<25; i++)//順子の種類
		{

			if (i != 7 && i != 8 && i != 16 && i != 17)
			{
				j = (4 - hai[i])*(4 - hai[i + 1])*(4 - hai[i + 2]);
				flag = 1;
			}

			else
			{
				flag = 0;
			}

			for (k = 0; k<j; k++)
			{
				mentu[n + k] = i + 34;

			}

			if (flag)
			{
				n += j;
			}
		}

		return n;
	}
	double kikenhai_check(int hai[], int genbutu[], int kiruhai, int kotae[])
	{
		int i, j, n, hai2[34] = { 0 };//初期値はhai[]=みえてる枚数　のち牌空間
		int aite[34] = { 0 };//相手の手
		int count1, flag = 1, mentu[10000] = { 0 }, mati[13] = { 0 }, kosuu;
		if (kiruhai != -1)
		{
			if (genbutu[kiruhai] == 1)
			{
				return 0.0;
			}
		}
		for (i = 0; i<34; i++)
		{
			kotae[i] = 0;
		}
		for (count1 = 0; count1<10000; count1++)
		{
			////printf("%d  ",count1);
			for (i = 0; i<34; i++)
			{
				hai2[i] = hai[i];
			}
			flag = 1;


			i = 0;
			while (i<4)
			{
				j = kumi(hai, mentu);;
				j = (randam(0) & 0xefffffff) % j;
				j = mentu[j];
				i++;

				if (j<34)
				{
					aite[j] += 3;
					hai2[j] += 3;
				}

				else
				{
					j -= 34;
					aite[j]++;
					aite[j + 1]++;
					aite[j + 2]++;
					hai2[j]++;
					hai2[j + 1]++;
					hai2[j + 2]++;
				}
			}//while(i<4)

			while (flag)
			{
				flag = 1;
				j = (randam(0) & 0xefffffff) % 34;//頭

				if (hai2[j] <= 2)//見えてる枚数2枚以下じゃないと頭にならない
				{
					aite[j] += 2;
					for (i = 0; i<2; i++)
					{
						flag = 0;
						hai2[j]++;
					}

				}//if(hai[l]<=2)
			}//while(flag)

			flag = 1;
			while (flag)
			{
				n = 0;
				flag = 1;
				i = (randam(0) & 0xefffffff) % 34;
				if (aite[i] != 0)
				{
					flag = 0;
					aite[i]--;
					kosuu = tenpai(aite, mati);//何待ちか調べる

					for (j = 0; j<kosuu; j++)
					{
						if (genbutu[mati[j]])//フリテンかどうかそうならやり直し
						{
							////printf("out¥n");
							n = 1;
							flag = 1;
						}

					}//for(i=0;i<j;i++)	

					if (flag)
					{
						count1--;
						flag = 0;
					}
					aite[i]++;
				}//if(aite[i]!=0)
			}//while(flag)

			if (n == 0)
			{
				for (i = 0; i<kosuu; i++)
				{
					kotae[mati[i]]++;
				}
			}

			////printf("¥n");

			for (i = 0; i<34; i++)
			{
				aite[i] = 0;
			}

		}//	for(count1=0;count1<5000;count1++)
		i = 0;
		for (j = 0; j<34; j++)
		{
			////printf("%d ",kotae[j]);
			i += kotae[j];
			if (j % 9 == 8)
			{
				//	//printf("¥n");
			}
		}
		////printf("¥n");
		/*for(j=0;j<34;j++)
		{
		k[j]=(double) kotae[j]/(double) i;
		}*/
		return (double)kotae[kiruhai] / (double)i;
	}
	//inline 
	int shantensuu(int hai[], int fu_ro)
	{
		int j_m = 0, j_t = 0;
		int tmp = 0, val = 14;
		int shanten[3][9] = { { 0 } };
		int m[3] = { 0 };

		for (int iro = 0; iro<3; iro++)
		{
			int j = hai[iro * 9];
			for (int i = 1 + iro * 9; i<9 + iro * 9; ++i) { j <<= 3; j |= hai[i]; }

			int kv = shanten_list[j];
			m[iro] = (kv & 0x00000003);
			kv >>= 2;
			for (int i = 0; i<m[iro]; i++)
			{
				shanten[iro][i * 3] = (kv & 0x00000001); kv >>= 1;//頭
				shanten[iro][i * 3 + 1] = (kv & 0x00000007); kv >>= 3;//ターツ
				shanten[iro][i * 3 + 2] = (kv & 0x00000007); kv >>= 3;//メンツ

																	  //cout<<shanten[iro][i*3]<<" "<<shanten[iro][i*3+1]<<" "<<shanten[iro][i*3+2]<<endl;
			}
		}

		/*int j=hai[27];
		for(int i=28;i<34;++i)
		{
		j<<=3;
		j|=hai[i];
		}
		j|=(1<<27);
		int kv=shanten_list2[j];
		j_t=(kv&0x00000007);kv>>=3;
		j_m=(kv&0x00000007);
		*/
		for (int i = 27; i<34; ++i)
		{
			if (hai[i] == 0) { continue; }
			else if (hai[i] == 2) { ++j_t; }
			else if (hai[i] >= 3) { ++j_m; }
		}
		//cout<<j_t<<j_m<<endl;
		for (int k = 0; k<m[0]; ++k)
		{
			for (int kk = 0; kk<m[1]; ++kk)
			{
				for (int kkk = 0; kkk<m[2]; ++kkk)
				{
					int head_ex = (shanten[0][k * 3] + shanten[1][kk * 3] + shanten[2][kkk * 3] + j_t >= 1);
					int mentu = shanten[0][k * 3 + 2] + shanten[1][kk * 3 + 2] + shanten[2][kkk * 3 + 2] + j_m + fu_ro;
					int tarts = shanten[0][k * 3 + 1] + shanten[1][kk * 3 + 1] + shanten[2][kkk * 3 + 1] + j_t + shanten[0][k * 3] + shanten[1][kk * 3] + shanten[2][kkk * 3] - head_ex;
					//cout<<shanten[0][k*3+1]<<shanten[1][kk*3+1]<<shanten[2][kkk*3+1]<<j_t<<shanten[0][k*3]<<shanten[1][kk*3]<<shanten[2][kkk*3]<<head_ex<<endl;
					if (mentu + tarts>4)
					{
						tmp = 8 - 2 * mentu - 4 + mentu - head_ex;
					}//4-mentu-head
					else
					{
						tmp = 8 - 2 * mentu - tarts - head_ex;
					}
					if (val>tmp) { val = tmp; }
				}
			}
		}
		return val;
	}

	int shantensuu_all(int hai[], int fu_ro)
	{
		int i = 14, j = 0;
		j = shantensuu(hai, fu_ro);
		if (j<i) { i = j; }
		j = titoi_shanten(hai, fu_ro);
		if (j<i) { i = j; }
		j = kokusi_shanten(hai, fu_ro);
		if (j<i) { i = j; }
		return i;
	}
	int mentusuu(int hai[])
	{
		int val = 0;
		int m[3] = { 0 };

		for (int iro = 0; iro<3; iro++)
		{
			int j = hai[iro * 9];
			for (int i = 1 + iro * 9; i<9 + iro * 9; ++i) { j <<= 3; j |= hai[i]; }

			int kv = shanten_list[j];
			m[iro] = (kv & 0x00000003);
			kv >>= 6;
			int mentu = 0;
			for (int i = 0; i<m[iro]; i++)
			{
				if (mentu<(kv & 0x00000007)) { mentu = (kv & 0x00000007); }
				kv >>= 7;//メンツ
			}
			val += mentu;
		}
		for (int i = 27; i<34; ++i) { if (hai[i] >= 3) { ++val; } }

		return val;
	}

	/*int ryanmensuu(int hai[],int mieteruhai[],int flag)
	{
	int i,j,k=0;
	int copy[34]={0};
	int copy_mieteruhai[34]={0};
	j=mentusuu(hai);
	if(flag==0)
	{
	for(i=0;i<24;i++)
	{
	if(hai[i+1]>0 && hai[i+2]>0 && i%9<=5 && mieteruhai[i]+mieteruhai[i+3]<4)
	{
	hai[i]++;
	if(mentusuu(hai)>j)
	{
	hai[i]--;
	hai[i+3]++;
	if(mentusuu(hai)>j)
	{
	hai[i+3]--;

	hai[i+1]--;
	hai[i+2]--;
	if(mentusuu(hai)==j)
	{
	k++;
	mieteruhai[i]++;
	i--;
	}
	else
	{
	hai[i+1]++;
	hai[i+2]++;
	}
	}
	else{hai[i+3]--;}
	}
	else{hai[i]--;}
	}
	}
	}
	else
	{
	for(i=0;i<34;i++)
	{
	copy[i]=hai[i];
	copy_mieteruhai[i]=mieteruhai[i];
	}

	for(i=0;i<24;i++)
	{
	if(copy[i+1]>0 && copy[i+2]>0 && i%9<=5 &&copy_mieteruhai[i]+copy_mieteruhai[i+3]<4)
	{
	copy[i]++;
	if(mentusuu(copy)>j)
	{
	copy[i]--;
	copy[i+3]++;
	if(mentusuu(copy)>j)
	{
	copy[i+3]--;

	copy[i+1]--;
	copy[i+2]--;
	if(mentusuu(copy)==j)
	{
	k++;
	copy_mieteruhai[i]++;
	i--;
	}
	else
	{
	copy[i+1]++;
	copy[i+2]++;
	}
	}
	else
	{copy[i+3]--;}
	}
	else
	{copy[i]--;}
	}
	}
	}
	return k;
	}
	int ryanmen_mentusuu(int hai[],int mieteruhai[],int *n,int *m)
	{
	int i,j,k=0,l=0;
	int copy[34]={0};
	int copy_mieteruhai[34]={0};

	for(i=0;i<34;i++)
	{
	copy[i]=hai[i];
	copy_mieteruhai[i]=mieteruhai[i];
	}
	j=mentusuu(copy);
	k=ryanmensuu(copy,copy_mieteruhai,0);
	if(*n!=-1)
	{
	*n=j+k;
	}
	if(*m!=-1)
	{
	*m=kantyansuu(copy,copy_mieteruhai,j);
	}

	for(i=0;i<27;i++)
	{
	if(copy[i+1]>0 && copy[i+2]>0 && i%9<=5 && copy_mieteruhai[i]+copy_mieteruhai[i+3]<4)
	{
	copy[i+1]--;
	copy[i+2]--;
	int men=mentusuu(copy);
	if(ryanmensuu(copy,copy_mieteruhai,1)+1>j-men)
	{
	j=men;
	copy_mieteruhai[i]++;
	l=l+1+ryanmensuu(copy,copy_mieteruhai,0);
	i--;
	}
	else
	{
	copy[i+1]++;
	copy[i+2]++;
	}
	}
	}
	//j=mentusuu(copy);
	return j+k+l;
	}*/
	int kantyansuu(int hai[], int mieteruhai[])
	{
		int i, j = 0, k;
		int copy[34] = { 0 };
		int copy_mieteruhai[34] = { 0 };
		k = mentusuu(hai);

		for (i = 0; i<34; i++)
		{
			copy[i] = hai[i];
			copy_mieteruhai[i] = mieteruhai[i];
		}
		for (i = 0; i<34; i++)
		{
			if (copy_mieteruhai[i]<4)
			{
				copy[i]++;
				if (mentusuu(copy)>k + j)
				{
					j++;
					copy_mieteruhai[i]++;
					i--;
				}
				else
				{
					copy[i]--;
				}
			}
		}
		return j;
	}
	int ryanmensuu(int hai[], int mieteruhai[], int flag)
	{
		int i, j, k = 0;
		int copy[34] = { 0 };
		int copy_mieteruhai[34] = { 0 };
		j = mentusuu(hai);
		if (flag == 0)
		{
			for (i = 0; i<24; i++)
			{
				if (i != 6 && i != 7 && i != 8 && i != 15 && i != 16 && i != 17)
				{
					hai[i]++;
					if (mentusuu(hai)>j)
					{
						hai[i]--;
						hai[i + 3]++;
						if (mentusuu(hai)>j)
						{
							hai[i + 3]--;
							if (hai[i + 1]>0 && hai[i + 2]>0)
							{
								hai[i + 1]--;
								hai[i + 2]--;
								if (mentusuu(hai) == j)
								{
									hai[i + 1]++;
									hai[i + 2]++;
									if (8 - mieteruhai[i] - mieteruhai[i + 3]>4)
									{
										k++;
										mieteruhai[i]++;
										hai[i + 1]--;
										hai[i + 2]--;
										i--;
									}
								}
								else
								{
									hai[i + 1]++;
									hai[i + 2]++;
								}
							}

						}
						else
						{
							hai[i + 3]--;
						}
					}
					else
					{
						hai[i]--;
					}
				}
			}
		}
		else
		{
			for (i = 0; i<34; i++)
			{
				copy[i] = hai[i];
				copy_mieteruhai[i] = mieteruhai[i];
			}

			for (i = 0; i<24; i++)
			{
				if (i != 6 && i != 7 && i != 8 && i != 15 && i != 16 && i != 17)
				{
					copy[i]++;
					if (mentusuu(copy)>j)
					{
						copy[i]--;
						copy[i + 3]++;
						if (mentusuu(copy)>j)
						{
							copy[i + 3]--;
							if (copy[i + 1]>0 && copy[i + 2]>0)
							{
								copy[i + 1]--;
								copy[i + 2]--;
								if (mentusuu(copy) == j)
								{
									copy[i + 1]++;
									copy[i + 2]++;
									if (8 - copy_mieteruhai[i] - copy_mieteruhai[i + 3]>4)
									{
										k++;
										copy_mieteruhai[i]++;
										copy[i + 1]--;
										copy[i + 2]--;
										i--;
									}
								}
								else
								{
									copy[i + 1]++;
									copy[i + 2]++;
								}
							}

						}
						else
						{
							copy[i + 3]--;
						}
					}
					else
					{
						copy[i]--;
					}
				}
			}
		}
		return k;
	}
	int ryanmen_mentusuu(int hai[], int mieteruhai[], int *n, int *m)
	{
		int i, j, k = 0, l = 0;
		int copy[34] = { 0 };
		int copy_mieteruhai[34] = { 0 };

		for (i = 0; i<34; i++)
		{
			copy[i] = hai[i];
			copy_mieteruhai[i] = mieteruhai[i];
		}
		j = mentusuu(copy);
		k = ryanmensuu(copy, copy_mieteruhai, 0);
		if (*n != -1)
		{
			*n = j + k;
		}
		if (*m != -1)
		{
			*m = kantyansuu(copy, copy_mieteruhai);
		}

		for (i = 0; i<27; i++)
		{
			if (i != 0 && i != 7 && i != 8 && i != 9 && i != 16 && i != 17 && i != 18 && i != 25 && i != 26)
			{
				if (copy[i]>0 && copy[i + 1]>0)
				{

					copy[i]--;
					copy[i + 1]--;

					if (ryanmensuu(copy, copy_mieteruhai, 1) + 1>j - mentusuu(copy) && (8 - copy_mieteruhai[i - 1] - copy_mieteruhai[i + 2]>4))
					{
						j = mentusuu(copy);
						copy_mieteruhai[i - 1]++;
						l = l + 1 + ryanmensuu(copy, copy_mieteruhai, 0);
						i--;

					}
					else
					{
						copy[i]++;
						copy[i + 1]++;
					}
				}
			}
		}
		j = mentusuu(copy);
		return j + k + l;
	}
	int ryanmen_block(int hai[], int *ry, int *bl)
	{
		*ry = 0; *bl = 0;
		for (int iro = 0; iro<3; iro++)
		{
			int j = hai[iro * 9];
			for (int i = 1 + iro * 9; i<9 + iro * 9; ++i) { j <<= 3; j |= hai[i]; }

			int kv = shanten_list[j];
			//cout<<"bl"<<kv<<" "<<j<<" ";
			kv >>= 23;
			*bl += (kv & 0x00000007);
			//cout<<(kv&0x00000007)<<" ";
			kv >>= 3;
			*ry += (kv & 0x00000007);
			//cout<<(kv&0x00000007)<<endl;
		}
		for (int i = 27; i<34; ++i)
		{
			if (hai[i] == 0) { continue; }
			else if (hai[i] >= 3) { *ry += 1; *bl += 1; }
			else if (hai[i] >= 1) { *bl += 1; }
		}

		return 0;
	}
	int titoi_shanten(int hai[], int fu_ro)
	{
		int j = 0, k = 0;
		if (!fu_ro)
		{
			j = 0;
			for (int i = 0; i<34; i++)
			{
				if (hai[i] == 0) { continue; }
				else if (hai[i] >= 2) { k++; j++; }
				else if (hai[i] == 1) { k++; }
			}
			if (k <= 7) { k = 7 - k; }
			else { k = 0; }

			return 6 - j + k;
		}
		else
		{
			return 7;
		}
	}
	int kokusi_shanten(int hai[], int fu_ro)
	{
		int j = 0;
		if (!fu_ro)
		{
			j = (hai[0] >= 1) + (hai[8] >= 1) + (hai[9] >= 1) + (hai[17] >= 1) + (hai[18] >= 1) + (hai[26] >= 1) + (hai[27] >= 1) + (hai[28] >= 1) + (hai[29] >= 1) + (hai[30] >= 1) + (hai[31] >= 1) + (hai[32] >= 1) + (hai[33] >= 1);

			if (hai[0]>1 || hai[8]>1 || hai[9]>1 || hai[17]>1 || hai[18]>1 || hai[26]>1 || hai[27]>1 || hai[28]>1 || hai[29]>1 || hai[30]>1 || hai[31]>1 || hai[32]>1 || hai[33]>1)
			{
				j++;
			}
			return 13 - j;
		}
		else
		{
			return 14;
		}
	}
	int mentu_kousei(int hai[], int fu_ro, int fu_ro_bu[], int agarihai, int ron_tumo, int ri_ti, int ippatu, int bafuu, int jifuu, int haitei, int dora[], int aka, int oya, int kan, int rinshan_kaiho, int tyankan, int tenho, int han_fu[])
	{
		int i, j, k, copy[17][34] = { { 0 } }, koutu_suu = 0;
		int check = 0, max = 0, m = 0, agari;
		int a[17] = { 0 };
		int atama;
		int menzen_bu[17][4] = { { 0 } };//順子は先頭数値、刻子は数字＋34  fu_ro_bu カンは+68 暗カンは+102

		for (i = 0; i<34; i++)
		{
			copy[0][i] = hai[i];
			copy[1][i] = hai[i];
		}
		for (i = 0; i<34; i++) /*頭を決める*/
		{
			if (copy[0][i] >= 2)
			{
				koutu_suu = 0;
				check = 1;
				copy[1][i] = copy[0][i] - 2;
				atama = i;
				a[1] = 0;
				for (j = 0; j<34; j++) /*刻子を決める*/
				{
					//	if(n==0&&i==0)//printf("koutu=%d",koutu_suu);
					if (copy[1][j] >= 3 && koutu_suu == 0)
					{
						for (k = 0; k<34; k++)
						{

							copy[2][k] = copy[1][k];
						}
						copy[2][j] = copy[2][j] - 3;
						menzen_bu[2][0] = j + 34;

						a[2] = 1;
						koutu_suu++;
						check = 2;
					}//if(copy[1][j]>=3 && koutu_suu==0)

					else if (copy[2][j] >= 3 && koutu_suu == 1)
					{
						for (k = 0; k<34; k++)
						{
							copy[3][k] = copy[1][k];
							copy[4][k] = copy[2][k];
						}
						copy[3][j] = copy[3][j] - 3;
						copy[4][j] = copy[4][j] - 3;
						menzen_bu[3][0] = j + 34;
						menzen_bu[4][0] = menzen_bu[2][0];
						menzen_bu[4][1] = j + 34;
						a[3] = 1;
						a[4] = 2;
						koutu_suu++;
						check = 4;
					}//else if(copy[2][j]>=3 && koutu_suu==1)

					else if (copy[4][j] >= 3 && koutu_suu == 2)
					{
						for (k = 0; k<34; k++)
						{
							copy[5][k] = copy[1][k];
							copy[6][k] = copy[2][k];
							copy[7][k] = copy[3][k];
							copy[8][k] = copy[4][k];
						}
						copy[5][j] = copy[5][j] - 3;
						copy[6][j] = copy[6][j] - 3;
						copy[7][j] = copy[7][j] - 3;
						copy[8][j] = copy[8][j] - 3;
						menzen_bu[5][0] = j + 34;
						menzen_bu[6][0] = menzen_bu[2][0];
						menzen_bu[6][1] = j + 34;
						menzen_bu[7][0] = menzen_bu[3][0];
						menzen_bu[7][1] = j + 34;
						menzen_bu[8][0] = menzen_bu[4][0];
						menzen_bu[8][1] = menzen_bu[4][1];
						menzen_bu[8][2] = j + 34;
						a[5] = 1;
						a[6] = 2;
						a[7] = 2;
						a[8] = 3;
						koutu_suu++;
						check = 8;
					}//else if(copy[4][j]>=3 && koutu_suu==2)

					else if (copy[8][j] >= 3 && koutu_suu == 3)
					{
						for (k = 0; k<34; k++)
						{
							copy[9][k] = copy[1][k];
							copy[10][k] = copy[2][k];
							copy[11][k] = copy[3][k];
							copy[12][k] = copy[4][k];
							copy[13][k] = copy[5][k];
							copy[14][k] = copy[6][k];
							copy[15][k] = copy[7][k];
							copy[16][k] = copy[8][k];
						}
						copy[9][j] = copy[9][j] - 3;
						copy[10][j] = copy[10][j] - 3;
						copy[11][j] = copy[11][j] - 3;
						copy[12][j] = copy[12][j] - 3;
						copy[13][j] = copy[13][j] - 3;
						copy[14][j] = copy[14][j] - 3;
						copy[15][j] = copy[15][j] - 3;
						copy[16][j] = copy[16][j] - 3;
						menzen_bu[9][0] = j + 34;
						menzen_bu[10][0] = menzen_bu[2][0];
						menzen_bu[10][1] = j + 34;
						menzen_bu[11][0] = menzen_bu[3][0];
						menzen_bu[11][1] = j + 34;
						menzen_bu[12][0] = menzen_bu[4][0];
						menzen_bu[12][1] = menzen_bu[4][1];
						menzen_bu[12][2] = j + 34;
						menzen_bu[13][0] = menzen_bu[5][0];
						menzen_bu[13][1] = j + 34;
						menzen_bu[14][0] = menzen_bu[6][0];
						menzen_bu[14][1] = menzen_bu[6][1];
						menzen_bu[14][2] = j + 34;
						menzen_bu[15][0] = menzen_bu[7][0];
						menzen_bu[15][1] = menzen_bu[7][1];
						menzen_bu[15][2] = j + 34;
						menzen_bu[16][0] = menzen_bu[8][0];
						menzen_bu[16][1] = menzen_bu[8][1];
						menzen_bu[16][2] = menzen_bu[8][2];
						menzen_bu[16][3] = j + 34;
						a[9] = 1;
						a[10] = 2;
						a[11] = 2;
						a[12] = 3;
						a[13] = 2;
						a[14] = 3;
						a[15] = 3;
						a[16] = 4;
						check = 16;
					}
				}//for(j=0;j<=43;j++) 刻子を決める

				for (j = 1; j <= check; j++) /*順子をきめる*/
				{
					/*if(j==9 && i==6)
					{
					for(k=0;k<34;k++)
					{
					if(k%9==0)
					{
					//printf("¥n");
					}
					//printf("%d ",copy[9][k]);
					}
					}*/
					for (k = 0; k<25; k++)//ソウズの７以下
					{
						if (copy[j][k] >= 1 && copy[j][k + 1] >= 1 && copy[j][k + 2] >= 1 && k != 7 && k != 8 && k != 16 && k != 17)
						{
							copy[j][k] = copy[j][k] - 1;
							copy[j][k + 1] = copy[j][k + 1] - 1;
							copy[j][k + 2] = copy[j][k + 2] - 1;
							menzen_bu[j][a[j]] = k;
							a[j]++;
							k--;
						}
					}

					agari = 1;
					for (k = 0; k<34; k++)
					{
						if (copy[j][k] != 0)
						{
							agari = 0;
						}
					}
					if (agari == 1)
					{
						/*printf("atama%d ",atama);
						for(k=0;k<4-fu_ro-kan;k++)
						{
						printf("me%d ",menzen_bu[j][k]);
						}
						for(k=0;k<fu_ro+kan;k++)
						{
						printf("fu%d ",fu_ro_bu[k]);
						}
						//printf("¥n");*/
						m = yaku_check(fu_ro, atama, menzen_bu[j], fu_ro_bu, agarihai, ron_tumo, ri_ti, ippatu, bafuu, jifuu, haitei, dora, aka, oya, kan, rinshan_kaiho, tyankan, tenho, han_fu);
						if (max <= m)
						{
							max = m;
						}
					}
				}//for(j=1;j<=check;j++) /*順子をきめる*/

				for (k = 0; k<34; k++)
				{
					copy[1][k] = copy[0][k];
				}
			}//if(copy[0][i] >= 2)
		}//for(i=0;i<34;i++) /*頭を決める*/

		return max;

	}//int tenpai(int aite[],int *mati)
	int mentu_kousei2(int hai[], int fu_ro, int fu_ro_bu[], int agarihai, int ron_tumo, int ri_ti, int ippatu, int bafuu, int jifuu, int haitei, int dora[], int aka, int oya, int kan, int rinshan_kaiho, int tyankan, int tenho, int han_fu[])
	{
		int menzen_bu[4] = { 0 }, part = 0;
		int copy[34] = { 0 };
		int max = 0;
		int sanrenkou = 0;
		int j = 0;
		for (j = 0; j <= 24; j++)
		{
			if (j % 9 <= 6 && hai[j] >= 3 && hai[j + 1] >= 3 && hai[j + 2] >= 3)
			{
				sanrenkou = 1;
			}
		}
		if (sanrenkou == 1)
		{
			max = mentu_kousei(hai, fu_ro, fu_ro_bu, agarihai, ron_tumo, ri_ti, ippatu, bafuu, jifuu, haitei, dora, aka, oya, kan, rinshan_kaiho, tyankan, tenho, han_fu);
		}
		else
		{
			int i = 0;
			for (i = 0; i<34; i++)
			{
				if (hai[i] >= 2)
				{
					for (j = 0; j<34; j++)
					{
						copy[j] = hai[j];
					}
					copy[i] -= 2;
					part = 0;
					if (shantensuu(copy, fu_ro + kan) == 0)
					{
						for (j = 0; j<27; j++)
						{
							if (copy[j] >= 3) { copy[j] -= 3; menzen_bu[part] = j + 34; part++; j--; }
							else if (copy[j]>0) { copy[j]--; copy[j + 1]--; copy[j + 2]--; menzen_bu[part] = j; part++; j--; }
						}
						for (j = 27; j<34; j++) { if (copy[j] == 3) { copy[j] -= 3; menzen_bu[part] = j + 34; part++; } }
						////printf("%d %d %d %d¥n",menzen_bu[0],menzen_bu[1],menzen_bu[2],menzen_bu[3]);
						////printf("%d %d %d %d¥n",fu_ro_bu[0],fu_ro_bu[1],fu_ro_bu[2],fu_ro_bu[3]);

						int m = yaku_check(fu_ro, i, menzen_bu, fu_ro_bu, agarihai, ron_tumo, ri_ti, ippatu, bafuu, jifuu, haitei, dora, aka, oya, kan, rinshan_kaiho, tyankan, tenho, han_fu);
						if (max<m) { max = m; }
					}
					copy[i] += 2;
				}
			}
		}
		return max;

	}//int tenpai(int aite[],int *mati)

	int pinfu(int check, int fu_ro, int atama, int menzen_bu[], int agarihai, int bafuu, int jifuu, int kan, int flag)
	{
		int i = 0;
		if (fu_ro != 0 || atama == bafuu || atama == jifuu || atama >= 31 || kan != 0 || check == 4)//門前 頭がオタ風
		{
			return 0;
		}
		for (i = 0; i<4; i++)//順子
		{
			if (menzen_bu[i] >= 34)
			{
				return 0;
			}
		}
		if (menzen_bu[check] == agarihai - 1 || (menzen_bu[check] % 9 == 0 && agarihai % 9 == 2) || (menzen_bu[check] % 9 == 6 && agarihai % 9 == 6))//両面
		{
			return 0;
		}
		if (flag == 1)
		{
			//printf("平和 ");
		}
		return 1;
	}//平和
	int tannyao(int fu_ro, int menzen_bu[], int fu_ro_bu[], int atama, int kan)
	{
		int i = 0, j = 0;

		if (atama % 9 == 0 || atama % 9 == 8 || atama >= 27)
		{
			return 0;
		}

		for (i = 0; i<4 - fu_ro - kan; i++)
		{
			if (menzen_bu[i] >= 34)
			{
				j = menzen_bu[i] - 34;
				if (j % 9 == 0 || j % 9 == 8 || j >= 27)
				{
					return 0;
				}
			}
			else
			{
				if (menzen_bu[i] % 9 == 0 || menzen_bu[i] % 9 == 6)
				{
					return 0;
				}
			}
		}
		j = 0;
		for (i = 0; i<fu_ro + kan; i++)
		{
			if (fu_ro_bu[i] >= 34)
			{
				j = fu_ro_bu[i] - 34;
				if (j >= 34)
				{
					j -= 34;
					if (j >= 34)
					{
						j -= 34;
					}
				}

				if (j % 9 == 0 || j % 9 == 8 || j >= 27)
				{
					return 0;
				}
			}
			else
			{
				if (fu_ro_bu[i] % 9 == 0 || fu_ro_bu[i] % 9 == 6)
				{
					return 0;
				}
			}
		}
		//printf("断幺九 ");
		return 1;

	}//タンヤオ
	int i_pe_ko(int fu_ro, int menzen_bu[], int kan)
	{
		int i = 0, j = -1, k = 0;
		if (fu_ro != 0)
		{
			return 0;
		}
		for (i = 0; i<4 - kan; i++)
		{
			if (menzen_bu[i]<34)
			{
				if (j == menzen_bu[i])
				{
					j = -1;
					k++;
				}
				else
				{
					j = menzen_bu[i];
				}
			}
		}
		if (k <= 1)
		{
			if (k == 1)
			{
				//printf("一盃口 ");
			}
			return k;
		}
		else
		{
			//printf("二盃口 ");
			return 3;
		}
	}//イーペーコー（リャンペー）
	int yakuhai(int fu_ro, int menzen_bu[], int fu_ro_bu[], int bafuu, int jifuu, int kan)
	{
		int i = 0, j = 0, k = 0;
		for (i = 0; i<4 - fu_ro - kan; i++)
		{
			if (menzen_bu[i] == 34 + bafuu)
			{
				//printf("場風 ");
				k++;
			}
			if (menzen_bu[i] == 34 + jifuu)
			{
				//printf("自風 ");
				k++;
			}
			else if (menzen_bu[i] >= 65)
			{
				if (menzen_bu[i] == 65)
				{
					//printf("白 ");
				}
				else if (menzen_bu[i] == 66)
				{
					//printf("撥 ");
				}
				else
				{
					//printf("中 ");
				}
				k++;

			}
		}
		for (i = 0; i<fu_ro + kan; i++)
		{
			j = fu_ro_bu[i];

			if (j % 34 == bafuu)
			{
				//printf("場風 ");
				k++;
			}
			if (j % 34 == jifuu)
			{
				//printf("自風 ");
				k++;
			}
			else if (j % 34 >= 31)
			{
				if (fu_ro_bu[i] % 34 == 31)
				{
					//printf("白 ");
				}
				else if (fu_ro_bu[i] % 34 == 32)
				{
					//printf("撥 ");
				}
				else
				{
					//printf("中 ");
				}
				k++;
			}
		}
		return k;
	}//役牌
	int toitoi(int check, int fu_ro, int menzen_bu[], int fu_ro_bu[], int ron_tumo, int kan, int flag)
	{
		int i = 0;
		for (i = 0; i<4 - fu_ro - kan; i++)
		{
			if (menzen_bu[i]<34) { return 0; }
		}
		for (i = 0; i<fu_ro + kan; i++)
		{
			if (fu_ro_bu[i]<34) { return 0; }
		}

		if (fu_ro == 0 && (ron_tumo == 1 || check == 4))
		{
			//printf("四暗刻 ");
			return 13;//四暗刻
		}
		else
		{
			if (flag == 1)
			{
				//printf("対々和 ");
			}
			return 2;
		}

	}//対対
	int sannannko(int fu_ro, int menzen_bu[], int ron_tumo, int check, int kan)
	{
		int i, j = 0;
		j += kan;
		for (i = 0; i<4 - fu_ro - kan; i++)
		{
			if (check == i && menzen_bu[check] >= 34 && ron_tumo == 1)
			{
				j++;
			}
			else if (check != i && menzen_bu[i] >= 34)
			{
				j++;
			}
		}
		if (j >= 3)
		{
			//printf("三暗刻 ");
			return 2;
		}
		else
		{
			return 0;
		}
	}//三暗刻
	int tyannta(int fu_ro, int menzen_bu[], int fu_ro_bu[], int atama, int kan, int flag)
	{
		int i = 0, j = 0;
		if (flag >= 2)
		{
			return 0;
		}
		if (atama <= 26 && (atama % 9 >= 1 && atama % 9 <= 7))
		{
			return 0;
		}
		for (i = 0; i<4 - fu_ro - kan; i++)
		{
			if (menzen_bu[i] >= 34)
			{
				j = menzen_bu[i] - 34;
				if (j <= 26 && (j % 9 >= 1 && j % 9 <= 7))
				{
					return 0;
				}
			}
			else
			{
				if (!(menzen_bu[i] % 9 == 0 || menzen_bu[i] % 9 == 6))
				{
					return 0;
				}
			}
		}

		for (i = 0; i<fu_ro + kan; i++)
		{
			if (fu_ro_bu[i] >= 34)
			{
				j = fu_ro_bu[i] - 34;
				if (j >= 34)
				{
					j -= 34;
					if (j >= 34)
					{
						j -= 34;
					}
				}
				if (j <= 26 && (j % 9 >= 1 && j % 9 <= 7))
				{
					return 0;
				}
			}
			else
			{
				if (!(fu_ro_bu[i] % 9 == 0 || fu_ro_bu[i] % 9 == 6))
				{
					return 0;
				}
			}
		}
		if (fu_ro == 0)
		{
			if (flag == 1)
			{
				//printf("混全帯幺九 ");
			}
			return 2;
		}
		else
		{
			if (flag == 1)
			{
				//printf("混全帯幺九 ");
			}
			return 1;
		}
	}//チャンタ
	int junntyann(int fu_ro, int menzen_bu[], int fu_ro_bu[], int atama, int kan, int flag)
	{

		if (tyannta(fu_ro, menzen_bu, fu_ro_bu, atama, kan, 0) == 0)
		{
			return 0;
		}

		int i = 0, j = 0;
		if (atama >= 27)
		{
			return 0;
		}
		for (i = 0; i<4 - fu_ro - kan; i++)
		{
			if (menzen_bu[i] >= 34)
			{
				j = menzen_bu[i] - 34;
				if (j >= 27)
				{
					return 0;
				}
			}
		}
		for (i = 0; i<fu_ro + kan; i++)
		{
			if (fu_ro_bu[i] >= 34)
			{
				j = fu_ro_bu[i] - 34;
				if (j % 34 >= 27)
				{
					return 0;
				}
			}
		}
		if (fu_ro == 0)
		{
			if (flag == 1)
			{
				//printf("純全帯幺九 ");
			}
			return 3;
		}
		else
		{
			if (flag == 1)
			{
				//printf("純全帯幺九 ");
			}
			return 2;
		}
		return 0;
	}//純チャンタ
	int honrou(int check, int fu_ro, int menzen_bu[], int fu_ro_bu[], int atama, int kan, int ron_tumo, int flag)
	{

		if (tyannta(fu_ro, menzen_bu, fu_ro_bu, atama, kan, 0) >= 1 && toitoi(check, fu_ro, menzen_bu, fu_ro_bu, ron_tumo, kan, 0) >= 2)
		{
			if (flag == 1)
			{
				//printf("混老頭 ");
			}
			return 2;
		}
		return 0;
	}//混老頭
	int tinrou(int check, int fu_ro, int menzen_bu[], int fu_ro_bu[], int atama, int kan, int ron_tumo, int flag)
	{
		if (junntyann(fu_ro, menzen_bu, fu_ro_bu, atama, kan, 0) >= 1 && toitoi(check, fu_ro, menzen_bu, fu_ro_bu, ron_tumo, kan, 0) >= 2)
		{
			// printf("清老頭 ");
			return 13;
		}
		return 0;
	}//清老頭
	int honnitu(int fu_ro, int menzen_bu[], int fu_ro_bu[], int atama, int kan, int flag)
	{
		int i = 0, j = 0, k = -1;
		if (flag == 2)
		{
			return 0;
		}
		for (i = 0; i<4 - fu_ro - kan; i++)//上がり牌探し
		{
			j = menzen_bu[i];
			if (j >= 34)
			{
				j -= 34;
			}

			if (k == -1 && j <= 26)
			{
				k = j / 9;
			}
			else if (k >= 0)
			{
				if (!(j >= 27 || (j >= 9 * k && j <= 9 * k + 8)))
				{
					return 0;
				}
			}
		}
		for (i = 0; i<fu_ro + kan; i++)//上がり牌探し
		{
			j = fu_ro_bu[i];
			if (j >= 34)
			{
				j -= 34;
				if (j >= 34)
				{
					j -= 34;
					if (j >= 34)
					{
						j -= 34;
					}
				}
			}


			if (k == -1 && j <= 26)
			{
				k = j / 9;
			}
			else if (k >= 0)
			{
				if (!(j >= 27 || (j >= 9 * k && j <= 9 * k + 8)))
				{
					return 0;
				}
			}
		}

		if (k != -1)
		{
			if (!(atama >= 27 || (atama >= 9 * k && atama <= 9 * k + 8)))
			{
				return 0;
			}
		}

		if (fu_ro == 0)
		{
			if (flag == 1)
			{
				//printf("混一色 ");
			}
			return 3;
		}
		else
		{
			if (flag == 1)
			{
				//printf("混一色 ");
			}
			return 2;
		}
		return 0;
	}//混一
	int tinnitu(int fu_ro, int menzen_bu[], int fu_ro_bu[], int atama, int kan, int flag)
	{
		if (honnitu(fu_ro, menzen_bu, fu_ro_bu, atama, kan, 0) == 0)
		{
			return 0;
		}
		if (atama >= 27)
		{
			return 0;
		}

		int i = 0, j = 0;
		for (i = 0; i<4 - fu_ro - kan; i++)//上がり牌探し
		{
			j = menzen_bu[i];
			if (j >= 34)
			{
				j -= 34;
			}
			if (j >= 27)
			{
				return 0;
			}
		}
		for (i = 0; i<fu_ro + kan; i++)//上がり牌探し
		{
			j = fu_ro_bu[i];
			if (j >= 34)
			{
				j -= 34;
			}
			if (j >= 34)
			{
				j -= 34;
			}
			if (j >= 34)
			{
				j -= 34;
			}
			if (j >= 27)
			{
				return 0;
			}
		}


		if (fu_ro == 0)
		{
			if (flag == 1)
			{
				//printf("清一色 ");
			}
			return 6;
		}
		else
		{
			if (flag == 1)
			{
				//printf("清一色 ");
			}
			return 5;
		}
		return 0;
	}//清一
	int sanshoku_dou_jun(int fu_ro, int menzen_bu[], int fu_ro_bu[], int kan)
	{
		int i, a[3][9] = { { 0 } };
		for (i = 0; i<4 - fu_ro - kan; i++)
		{
			if (menzen_bu[i] <= 26)
			{
				a[menzen_bu[i] / 9][menzen_bu[i] % 9] = 1;
			}
		}
		for (i = 0; i<fu_ro + kan; i++)
		{
			if (fu_ro_bu[i] <= 26)
			{
				a[fu_ro_bu[i] / 9][fu_ro_bu[i] % 9] = 1;
			}
		}
		for (i = 0; i<9; i++)
		{
			if (a[0][i] >= 1 && a[1][i] >= 1 && a[2][i] >= 1)
			{
				if (fu_ro == 0)
				{
					//printf("三色同順 ");
					return 2;
				}
				else
				{
					// printf("三色同順 ");
					return 1;
				}
			}
		}
		return 0;
	}//三色同順
	int sanshoku_dou_kou(int fu_ro, int menzen_bu[], int fu_ro_bu[], int kan)
	{
		int i, j, a[3][9] = { { 0 } };
		for (i = 0; i<9; i++)
		{
			for (j = 0; j<3; j++) { a[j][i] = 0; }
		}
		for (i = 0; i<4 - fu_ro - kan; i++)
		{
			if (menzen_bu[i] >= 34)
			{
				j = menzen_bu[i] - 34;
				if (j<27)
				{

					a[j / 9][j % 9] = 1;
				}
			}
		}
		for (i = 0; i<fu_ro + kan; i++)
		{

			if (fu_ro_bu[i] >= 34)
			{
				j = fu_ro_bu[i] % 34;
				if (j<27)
				{
					a[j / 9][j % 9] = 1;
				}
			}
		}
		for (i = 0; i<9; i++)
		{
			if (a[0][i] >= 1 && a[1][i] >= 1 && a[2][i] >= 1)
			{
				//printf("三色同刻 ");
				return 2;
			}
		}
		return 0;
	}//三色同刻
	int ittuu(int fu_ro, int menzen_bu[], int fu_ro_bu[], int kan)
	{
		int i, j, a[3][9] = { { 0 } };
		for (i = 0; i<4 - fu_ro - kan; i++)
		{
			if (menzen_bu[i] <= 27)
			{
				j = menzen_bu[i];
				a[j / 9][j % 9] = 1;
			}
		}
		for (i = 0; i<fu_ro + kan; i++)
		{
			if (fu_ro_bu[i] <= 27)
			{
				j = fu_ro_bu[i];
				a[j / 9][j % 9] = 1;
			}
		}
		for (i = 0; i<3; i++)
		{
			if (a[i][0] >= 1 && a[i][3] >= 1 && a[i][6] >= 1)
			{
				if (fu_ro == 0)
				{
					//printf("一気通貫 ");
					return 2;
				}
				else
				{
					//printf("一気通貫 ");
					return 1;
				}
			}
		}
		return 0;
	}//一通
	int shou_sann_genn(int fu_ro, int menzen_bu[], int fu_ro_bu[], int atama, int kan, int flag)
	{
		int i = 0, j = 0, k = 0;
		for (i = 0; i<4 - fu_ro - kan; i++)
		{
			if (menzen_bu[i] >= 65)
			{
				k++;
			}
		}
		for (i = 0; i<fu_ro + kan; i++)
		{
			j = fu_ro_bu[i];
			if (j % 34 >= 31)
			{
				k++;
			}
		}
		if (k >= 3)
		{
			// printf("大三元");
			return 13;
		}
		else if (k == 2)
		{
			if (atama >= 31 && flag == 1)
			{
				//printf("小三元 ");
				return 2;
			}
		}
		return 0;
	}//小三元
	int tyuurenn(int fu_ro, int menzen_bu[], int fu_ro_bu[], int atama, int kan, int flag)
	{
		int i, j, a[9] = { 0 };
		if (tinnitu(fu_ro, menzen_bu, fu_ro_bu, atama, kan, 0) >= 6)
		{
			for (i = 0; i<4; i++)
			{
				j = menzen_bu[i];
				if (j >= 34)
				{
					j -= 34;
					a[j % 9] += 3;
				}
				else
				{
					a[j % 9]++;
					a[j % 9 + 1]++;
					a[j % 9 + 2]++;
				}
			}
			a[atama % 9] += 2;
			if (a[0]<3 || a[8]<3)
			{
				return 0;
			}
			for (i = 1; i <= 7; i++)
			{
				if (a[i] == 0)
				{
					return 0;
				}
			}
			//printf("九蓮宝燈 ");
			return 13;
		}
		return 0;
	}
	int tu_i_so(int fu_ro, int menzen_bu[], int fu_ro_bu[], int atama, int kan)
	{
		int i, j;
		if (atama<27)
		{
			return 0;
		}
		for (i = 0; i<4 - fu_ro - kan; i++)
		{
			if (menzen_bu[i] % 34<27)
			{
				return 0;
			}
		}
		for (i = 0; i<fu_ro + kan; i++)
		{
			j = fu_ro_bu[i];
			if (j<34)
			{
				return 0;
			}

			if (j % 34<27)
			{
				return 0;
			}
		}
		//printf("字一色 ");
		return 13;
	}//字一色
	int dai_su_si(int fu_ro, int menzen_bu[], int fu_ro_bu[], int atama, int kan)
	{
		int i, j, k = 0;

		for (i = 0; i<4 - fu_ro - kan; i++)
		{
			if (menzen_bu[i] >= 27 + 34 && menzen_bu[i] <= 30 + 34)
			{
				k++;
			}
		}
		for (i = 0; i<fu_ro + kan; i++)
		{
			j = fu_ro_bu[i];
			if (j % 34 >= 27 && j % 34 <= 30)
			{
				k++;
			}
		}
		if (k >= 4)
		{
			//printf("大四喜 ");
			return 13;
		}
		else if (k == 3 && (atama <= 30 && atama >= 27))
		{
			//printf("小四喜 ");
			return 13;
		}
		return 0;
	}//大四喜
	int ryuu_i_so(int fu_ro, int menzen_bu[], int fu_ro_bu[], int atama, int kan)
	{
		int i, j;
		if (!(atama == 19 || atama == 20 || atama == 21 || atama == 23 || atama == 25 || atama == 32))
		{
			return 0;
		}
		for (i = 0; i<4 - fu_ro - kan; i++)
		{
			j = menzen_bu[i];
			if (j >= 34)
			{
				j -= 34;
				if (!(j == 19 || j == 20 || j == 21 || j == 23 || j == 25 || j == 32))
				{
					return 0;
				}
			}
			else
			{
				if (j != 19)
				{
					return 0;
				}
			}
		}

		for (i = 0; i<fu_ro + kan; i++)
		{
			j = fu_ro_bu[i];
			if (j >= 34)
			{
				j -= 34;
				if (j >= 34)
				{
					j -= 34;
					if (j >= 34)
					{
						j -= 34;
					}
				}
				if (!(j == 19 || j == 20 || j == 21 || j == 23 || j == 25 || j == 32))
				{
					return 0;
				}
			}
			else
			{
				if (j != 19)
				{
					return 0;
				}
			}
		}
		//printf("緑一色 ");
		return 13;
	}//緑一色
	int su_kann_tu(int fu_ro, int menzen_bu[], int fu_ro_bu[], int atama, int kan, int flag)
	{
		int i, j = 0;
		for (i = 0; i<fu_ro + kan; i++)
		{
			if (fu_ro_bu[i] >= 68)
			{
				j++;
			}
		}
		if (j == 3 && flag == 1)
		{
			//printf("三槓子 ");
			return 2;
		}
		else if (j == 4)
		{
			//printf("四槓子");
			return 13;
		}
		else
		{
			return 0;
		}
	}//四槓子
	int ri_ti_check(int ri_ti)
	{
		if (ri_ti == 2)
		{
			//printf("両立直 ");
			return 2;
		}
		else if (ri_ti == 1)
		{
			//printf("立直 ");
			return 1;
		}
		return 0;
	}
	int tumo_check(int fu_ro, int ron_tumo)
	{
		if (fu_ro == 0 && ron_tumo == 1)
		{
			//printf("門前清自摸和 ");
			return 1;
		}
		return 0;
	}
	int ippatu_check(int ippatu)
	{
		if (ippatu>0)
		{
			//printf("一発 ");
			return 1;
		}
		return 0;
	}
	int haitei_check(int haitei, int ron_tumo, int rinshan_kaiho)
	{
		if (haitei>0 && rinshan_kaiho == 0)
		{
			if (ron_tumo == 0)
			{
				//printf("河底撈魚 ");
			}
			else
			{
				//	printf("海底摸月 ");
			}
			return 1;
		}
		return 0;
	}
	int rinshan_kaiho_check(int rinshan_kaiho)
	{
		if (rinshan_kaiho>0)
		{
			//printf("嶺上開花 ");
			return 1;
		}
		return 0;
	}
	int tyankan_check(int tyankan)
	{
		if (tyankan>0)
		{
			//printf("槍槓 ");
			return 1;
		}
		return 0;
	}
	int tenho_check(int tenho, int ron_tumo, int oya)
	{
		if (tenho == 1 && ron_tumo == 1)
		{
			if (oya == 1)
			{
				//printf("天和 ");
			}
			else
			{
				//printf("地和 ");
			}
			return 13;
		}
		return 0;
	}
	int tu_i_so_ti_toi(int hai[])
	{
		int i = 0;
		for (i = 27; i<34; i++)
		{
			if (hai[i] != 2)
			{
				return 0;
			}
		}
		//printf("字一色 ");
		return 13;
	}//字牌チートイ
	int tanyao_ti_toi(int hai[])
	{
		int i = 0;
		for (i = 0; i<34; i++)
		{
			if (i % 9 == 0 || i % 9 == 8 || i >= 27)
			{
				if (hai[i] == 2)
				{
					return 0;
				}
			}
		}
		//printf("断幺九 ");
		return 1;
	}//断幺九チートイ
	int honrou_ti_toi(int hai[])
	{
		int i = 0;
		for (i = 0; i<34; i++)
		{
			if (!(i % 9 == 0 || i % 9 == 8 || i >= 27))
			{
				if (hai[i] == 2)
				{
					return 0;
				}
			}
		}
		//printf("混老頭 ");
		return 2;
	}//混老頭チートイ
	int honitu_ti_toi(int hai[])
	{
		int i = 0, j = 0, a[3] = { 0 };
		for (i = 0; i<34; i++)
		{
			if (i >= 0 && i <= 8 && a[0] == 0 && hai[i] >= 2)
			{
				a[0]++;
			}
			else if (i >= 9 && i <= 17 && a[1] == 0 && hai[i] >= 2)
			{
				a[1]++;
			}
			else if (i >= 18 && i <= 26 && a[2] == 0 && hai[i] >= 2)
			{
				a[2]++;
			}
			else if (i >= 27 && j == 0 && hai[i] >= 1)
			{
				j++;
			}
		}

		if (a[0] + a[1] + a[2] == 1 && j == 1)
		{
			//printf("混一色 ");
			return 3;
		}
		else if (a[0] + a[1] + a[2] == 1 && j == 0)
		{
			//printf("清一色 ");
			return 6;
		}
		return 0;
	}//混一色清一色
	int dora_check(int fu_ro, int atama, int menzen_bu[], int fu_ro_bu[], int kan, int dora[], int aka)
	{
		int i, k = 0;
		int hai[34] = { 0 };

		hai[atama] += 2;
		for (i = 0; i<4 - fu_ro - kan; i++)
		{
			if (menzen_bu[i] >= 34)
			{
				hai[menzen_bu[i] % 34] += 3;
			}
			else
			{
				hai[menzen_bu[i]]++;
				hai[menzen_bu[i] + 1]++;
				hai[menzen_bu[i] + 2]++;
			}
		}
		for (i = 0; i<fu_ro + kan; i++)
		{
			if (fu_ro_bu[i] >= 68)
			{
				hai[fu_ro_bu[i] % 34] += 4;
			}
			else if (fu_ro_bu[i] >= 34)
			{
				hai[fu_ro_bu[i] % 34] += 3;
			}
			else
			{
				hai[fu_ro_bu[i]]++;
				hai[fu_ro_bu[i] + 1]++;
				hai[fu_ro_bu[i] + 2]++;
			}
		}
		for (i = 0; i<5; i++)
		{
			if (dora[i] != -1)
			{
				k += hai[get_kind_of_dora(dora[i])];
			}
		}
		return k + aka;
	}
	int tensuu(int hansuu, int fu, int oya, int ron_tumo)
	{
		int oya_tumo[4][10] = { { 5,5,7,8,10,12,13,15,16,18 },
		{ 7,10,13,16,20,23,26,29,32,36 },
		{ 13,20,26,32,39,40,40,40,40,40 },
		{ 26,39,40,40,40,40,40,40,40,40 } };
		int oya_ron[4][10] = { { 15,15,20,24,29,34,39,44,48,53 },
		{ 20,29,39,48,58,68,77,87,96,106 },
		{ 39,58,77,96,116,120,120,120,120 },
		{ 77,116,120,120,120,120,120,120,120 } };
		int oya_ron2[4] = { 120,180,240,360 };
		int ko_tumo_ko[4][10] = { { 3,3,4,4,5,6,7,8,8,8 },
		{ 4,5,7,8,10,12,13,15,16,18 },
		{ 7,10,13,16,20,20,20,20,20,20 },
		{ 13,20,20,20,20,20,20,20,20,20 } };
		int ko_tumo_oya[4][10] = { { 5,5,7,8,10,12,13,15,16,16 },
		{ 7,10,13,16,20,23,26,29,32,36 },
		{ 13,20,26,32,39,40,40,40,40,40 },
		{ 26,39,40,40,40,40,40,40,40,40 } };
		int ko_ron[4][10] = { { 0,10,13,16,20,23,26,29,32,36 },
		{ 13,20,26,32,39,45,52,58,64,71 },
		{ 26,39,52,64,77,80,80,80,80,80 },
		{ 52,77,80,80,80,80,80,80,80,80 } };
		int ko_ron2[4] = { 80,120,160,240 };
		if (fu >= 120 && hansuu <= 5) { hansuu = 5; }
		if (oya == 1)
		{
			if (hansuu<5)
			{
				hansuu--;
				fu -= 20;
				fu /= 10;
				if (ron_tumo == 0)
				{
					return oya_ron[hansuu][fu];
				}
				else
				{
					return oya_tumo[hansuu][fu] * 3;
				}
			}
			else if (hansuu >= 13)
			{
				hansuu /= 13;
				return hansuu * 480;
			}
			else
			{
				if (hansuu == 5)
				{
					hansuu = 0;
				}
				else if (hansuu == 6 || hansuu == 7)
				{
					hansuu = 1;
				}
				else if (hansuu == 8 || hansuu == 9 || hansuu == 10)
				{
					hansuu = 2;
				}
				else
				{
					hansuu = 3;
				}
				return oya_ron2[hansuu];
			}
		}//oya==0
		else
		{
			if (hansuu<5)
			{
				hansuu--;
				fu -= 20;
				fu /= 10;
				if (ron_tumo == 0)
				{
					return ko_ron[hansuu][fu];
				}
				else
				{
					return ko_tumo_ko[hansuu][fu] * 2 + ko_tumo_oya[hansuu][fu];
				}
			}
			else if (hansuu >= 13)
			{
				hansuu /= 13;
				return hansuu * 320;
			}
			else
			{
				if (hansuu == 5)
				{
					hansuu = 0;
				}
				else if (hansuu == 6 || hansuu == 7)
				{
					hansuu = 1;
				}
				else if (hansuu == 8 || hansuu == 9 || hansuu == 10)
				{
					hansuu = 2;
				}
				else
				{
					hansuu = 3;
				}
				return ko_ron2[hansuu];
			}
		}//oya==1
		return 0;
	}
	int yaku_check(int fu_ro, int atama, int menzen_bu[], int fu_ro_bu[], int agarihai, int ron_tumo, int ri_ti, int ippatu, int bafuu, int jifuu, int haitei, int dora[], int aka, int oya, int kan, int rinshan_kaiho, int tyankan, int tenho, int han_fu[])
		//鳴きの数、頭、門前,鳴いた部分、上がった牌、ロンかツモ、リーチ、一発、場風，自風、海底、どら,親
	{
		int i = 0, j = 0, k, flag, max = 0;
		int check = 0;
		int hansuu = 0;
		int fu = 20;
		hansuu = tenho_check(tenho, ron_tumo, oya);
		if (shou_sann_genn(fu_ro, menzen_bu, fu_ro_bu, atama, kan, 0) >= 13)
		{
			//printf("大三元 ");
			hansuu += 13;
		}
		if (su_kann_tu(fu_ro, menzen_bu, fu_ro_bu, atama, kan, 0) >= 13)
		{
			//printf("四槓子 ");
			hansuu += 13;
		}
		if (tinrou(check, fu_ro, menzen_bu, fu_ro_bu, atama, kan, ron_tumo, 0) >= 13)
		{
			//printf("清老頭 ");
			hansuu += 13;
		}
		if (atama == agarihai) { check = 4; }
		if (toitoi(check, fu_ro, menzen_bu, fu_ro_bu, ron_tumo, kan, 0) >= 13)
		{
			//printf("四暗刻 ");
			hansuu += 13;
		}
		check = 0;
		hansuu += tyuurenn(fu_ro, menzen_bu, fu_ro_bu, atama, kan, 0);
		hansuu += tu_i_so(fu_ro, menzen_bu, fu_ro_bu, atama, kan);
		hansuu += dai_su_si(fu_ro, menzen_bu, fu_ro_bu, atama, kan);
		hansuu += ryuu_i_so(fu_ro, menzen_bu, fu_ro_bu, atama, kan);
		if (hansuu<13)
		{
			for (i = 0; i <= 4 - fu_ro - kan; i++)//上がり牌探し
			{
				check = 5;
				hansuu = 0;
				if (atama == agarihai && i == 4 - fu_ro - kan)
				{
					check = 4;
				}
				else if ((menzen_bu[i] == agarihai + 34 || menzen_bu[i] == agarihai || menzen_bu[i] + 1 == agarihai || menzen_bu[i] + 2 == agarihai) && i != 4 - fu_ro - kan)//頭待ち//刻子//順子
				{
					check = i;
				}

				if (check<5)//約判定
				{
					////printf("check=%d¥n",check);
					////printf("f %d %d¥n",fu_ro,kan);
					fu = 20;
					hansuu += ri_ti_check(ri_ti);
					hansuu += tumo_check(fu_ro, ron_tumo);
					hansuu += ippatu_check(ippatu);
					hansuu += haitei_check(haitei, ron_tumo, rinshan_kaiho);
					hansuu += tannyao(fu_ro, menzen_bu, fu_ro_bu, atama, kan);

					hansuu += rinshan_kaiho_check(rinshan_kaiho);
					hansuu += tyankan_check(tyankan);
					hansuu += pinfu(check, fu_ro, atama, menzen_bu, agarihai, bafuu, jifuu, kan, 1);
					hansuu += i_pe_ko(fu_ro, menzen_bu, kan);
					hansuu += yakuhai(fu_ro, menzen_bu, fu_ro_bu, bafuu, jifuu, kan);

					hansuu += toitoi(check, fu_ro, menzen_bu, fu_ro_bu, ron_tumo, kan, 1);
					hansuu += sannannko(fu_ro, menzen_bu, ron_tumo, check, kan);


					flag = 1;
					if (junntyann(fu_ro, menzen_bu, fu_ro_bu, atama, kan, 0) >= 2)
					{
						hansuu += junntyann(fu_ro, menzen_bu, fu_ro_bu, atama, kan, 1);
						flag = 2;
					}

					if (honrou(check, fu_ro, menzen_bu, fu_ro_bu, atama, kan, ron_tumo, 0) >= 2)
					{
						hansuu += honrou(check, fu_ro, menzen_bu, fu_ro_bu, atama, kan, ron_tumo, 1);
						flag = 2;
					}
					hansuu += tyannta(fu_ro, menzen_bu, fu_ro_bu, atama, kan, flag);

					flag = 1;
					if (tinnitu(fu_ro, menzen_bu, fu_ro_bu, atama, kan, 0) >= 5)
					{
						hansuu += tinnitu(fu_ro, menzen_bu, fu_ro_bu, atama, kan, 1);
						flag = 2;
					}

					hansuu += honnitu(fu_ro, menzen_bu, fu_ro_bu, atama, kan, flag);
					hansuu += sanshoku_dou_kou(fu_ro, menzen_bu, fu_ro_bu, kan);
					hansuu += sanshoku_dou_jun(fu_ro, menzen_bu, fu_ro_bu, kan);
					hansuu += ittuu(fu_ro, menzen_bu, fu_ro_bu, kan);
					hansuu += shou_sann_genn(fu_ro, menzen_bu, fu_ro_bu, atama, kan, 1);
					hansuu += su_kann_tu(fu_ro, menzen_bu, fu_ro_bu, atama, kan, 1);

					if (pinfu(check, fu_ro, atama, menzen_bu, agarihai, bafuu, jifuu, kan, 0) == 1)
					{
						if (ron_tumo == 0) { fu = 30; }
						else { fu = 20; }
					}
					else
					{
						if (fu_ro == 0 && ron_tumo == 0) { fu += 10; }
						else if (ron_tumo == 1) { fu += 2; }//上がり方
						if (atama == jifuu) { fu += 2; }
						if (atama == bafuu) { fu += 2; }
						else if (atama >= 31) { fu += 2; }//頭
						if (check == 4) { fu += 2; }
						else if (menzen_bu[check]<27 && (menzen_bu[check] == agarihai - 1 || (menzen_bu[check] % 9 == 0 && agarihai % 9 == 2) || (menzen_bu[check] % 9 == 6 && agarihai % 9 == 6)))
						{
							fu += 2;
						}//待ちの種類
						for (j = 0; j<4 - fu_ro - kan; j++)
						{
							if (menzen_bu[j] >= 34)
							{
								k = menzen_bu[j] - 34;
								if ((ron_tumo == 1 || (ron_tumo == 0 && k != agarihai) || j != check) && (k % 9 == 0 || k % 9 == 8 || k >= 27))
								{
									fu += 8;
								}
								else if ((ron_tumo == 0 && k == agarihai) && j == check && k<27 && k % 9 >= 1 && k % 9 <= 7)
								{
									fu += 2;
								}
								else
								{
									fu += 4;
								}
							}
						}
						for (j = 0; j<fu_ro + kan; j++)
						{
							if (fu_ro_bu[j] >= 34 && fu_ro_bu[j]<68)
							{
								k = fu_ro_bu[j] - 34;
								if (k % 9 == 0 || k % 9 == 8 || k >= 27)
								{
									fu += 4;
								}
								else
								{
									fu += 2;
								}
							}
							else if (fu_ro_bu[j] >= 68 && fu_ro_bu[j]<102)
							{
								k = fu_ro_bu[j] - 68;
								if (k % 9 == 0 || k % 9 == 8 || k >= 27)
								{
									fu += 16;
								}
								else
								{
									fu += 8;
								}
							}
							else if (fu_ro_bu[j] >= 102)
							{
								k = fu_ro_bu[j] - 102;
								if (k % 9 == 0 || k % 9 == 8 || k >= 27)
								{
									fu += 32;
								}
								else
								{
									fu += 16;
								}
							}
						}//for(j=0;j<fu_ro+kan;j++)
						if (fu % 10 != 0) { fu /= 10; fu++; fu *= 10; }
						if (fu == 20) { fu = 30; }

					}//else (pinfu(check,fu_ro,atama,menzen_bu,agarihai,bafuu,jifuu,kan)==1)
					if (hansuu>0)
					{
						hansuu += dora_check(fu_ro, atama, menzen_bu, fu_ro_bu, kan, dora, aka);
						//printf("%d翻 %d符¥n",hansuu,fu);
						j = tensuu(hansuu, fu, oya, ron_tumo);
						if (max<j || (j == max && han_fu[0]<hansuu))
						{
							max = j;
							han_fu[0] = hansuu;
							han_fu[1] = fu;
						}
					}
				}//if(check<5)
			}//for(i=0;i<=4-fu_ro;i++)//上がり牌探し
		}//if(hansuu>=13)
		else
		{
			han_fu[0] = hansuu;
			han_fu[1] = 0;
			max = tensuu(hansuu, fu, oya, ron_tumo);
		}
		return max;
	}
	int yaku(int hai[], int fu_ro_bu[], int agarihai, int ron_tumo, int ri_ti, int ippatu, int bafuu, int jifuu, int haitei, int dora[], int aka, int rinshan_kaiho, int tyankan, int tenho, int han_fu[])
	{
		int i = 0, j = 0, tokushu = 0, fu_ro = 0, kan = 0, hansuu = 0, oya = 0;
		if (jifuu == 27) { oya = 1; }
		fu_ro = 4;
		for (i = 0; i<4; i++) { if (fu_ro_bu[i] == -1) { fu_ro = i; i = 4; } }

		for (i = 0; i<fu_ro; i++)
		{
			if (fu_ro_bu[i] >= 102)
			{
				kan++;
			}
		}
		fu_ro -= kan;
		//hai_check(hai);//printf("¥n");
		//cout<<"aaa"<<shantensuu_all(hai);
		if (shantensuu_all(hai, fu_ro) == -1)
		{
			tokushu = 0;
			if (titoi_shanten(hai, fu_ro) == -1)
			{
				tokushu = 1;
				if (shantensuu(hai, fu_ro) == -1)
				{
					tokushu = 0;
				}

				if (tokushu == 1)
				{
					hansuu += tenho_check(tenho, ron_tumo, oya);
					hansuu += tu_i_so_ti_toi(hai);
					if (hansuu<13)
					{
						hansuu = 2;
						hansuu += ri_ti_check(ri_ti);
						hansuu += tumo_check(fu_ro, ron_tumo);
						hansuu += ippatu_check(ippatu);
						hansuu += haitei_check(haitei, ron_tumo, rinshan_kaiho);
						hansuu += tanyao_ti_toi(hai);
						//printf("七対子 ");
						hansuu += honrou_ti_toi(hai);
						hansuu += honitu_ti_toi(hai);
						for (j = 0; j<5; j++)
						{
							if (dora[j] != -1 && hai[get_kind_of_dora(dora[j])]>0)
							{
								hansuu += 2;
								////printf("ドラ*2 ");
							}
						}
						hansuu += aka;
						if (hansuu<6) { hansuu--; }
					}
					////printf("%d",hansuu);
					han_fu[0] = hansuu;
					han_fu[1] = 50;
					j = tensuu(hansuu, 50, oya, ron_tumo);
				}
			}
			else if (kokusi_shanten(hai, fu_ro) == -1)
			{
				tokushu = 2;
				hansuu = 13;
				han_fu[0] = hansuu;
				han_fu[1] = 0;
				hansuu += tenho_check(tenho, ron_tumo, oya);
				//printf("国士無双 ");
				//printf("%d ",hansuu);
				j = tensuu(hansuu, 30, oya, ron_tumo);
			}
			if (tokushu == 0)
			{
				j = mentu_kousei2(hai, fu_ro, fu_ro_bu, agarihai, ron_tumo, ri_ti, ippatu, bafuu, jifuu, haitei, dora, aka, oya, kan, rinshan_kaiho, tyankan, tenho, han_fu);
			}
		}
		if (j == 0)
		{
			//printf("役なし¥n");
		}
		else
		{
			//printf("¥n得点=%d",j*100);
		}
		return j;
	}
	int hai_check(int hai[])
	{
		int i = 0;
		for (i = 0; i<34; i++)
		{
			if (i % 9 == 0)
			{
				printf("¥n");
			}
			printf("%d ", hai[i]);
		}
		printf("¥n");
		return 0;
	}
	int ukihai_x(int hai[], int x)//x足したけどまわりに牌があるか？国士のシャンテンに注意
	{
		if (hai[x]>0) { return 1; }
		if (x >= 27) { return 0; }

		if (x % 9 == 0 && (hai[x + 1]>0 || hai[x + 2]>0)) { return 1; }
		else if (x % 9 == 8 && (hai[x - 1]>0 || hai[x - 2]>0)) { return 1; }
		else if (x % 9 == 1 && (hai[x - 1]>0 || hai[x + 1]>0 || hai[x + 2]>0)) { return 1; }
		else if (x % 9 == 7 && (hai[x + 1]>0 || hai[x - 1]>0 || hai[x - 2]>0)) { return 1; }
		else if (x % 9 >= 2 && x % 9 <= 6 && (hai[x + 2]>0 || hai[x + 1]>0 || hai[x - 1]>0 || hai[x - 2]>0)) { return 1; }

		return 0;

	}
	int ukihai_check(int hai[], int *x)
	{
		int i, j;
		j = 0;//浮き牌なし
		for (i = 0; i<9; i++)
		{
			if (i == 0 && hai[0] == 1 && hai[1] == 0 && hai[2] == 0)
			{
				*x = i;
				j++;
			}
			else if (i == 1 && hai[1] == 1 && hai[0] == 0 && hai[2] == 0 && hai[3] == 0)
			{
				*x = i;
				j++;
			}
			else if (i >= 2 && i <= 6 && hai[i] == 1 && hai[i - 1] == 0 && hai[i - 2] == 0 && hai[i + 1] == 0 && hai[i + 2] == 0)
			{
				*x = i;
				j++;
			}
			else if (i == 7 && hai[i] == 1 && hai[i - 1] == 0 && hai[i - 2] == 0 && hai[i + 1] == 0)
			{
				*x = i;
				j++;
			}
			else if (i == 8 && hai[i] == 1 && hai[i - 1] == 0 && hai[i - 2] == 0)
			{
				*x = i;
				j++;
			}
		}
		for (i = 9; i<18; i++)
		{
			if (i == 9 && hai[9] == 1 && hai[10] == 0 && hai[11] == 0)
			{
				*x = i;
				j++;
			}
			else if (i == 10 && hai[10] == 1 && hai[11] == 0 && hai[12] == 0 && hai[9] == 0)
			{
				*x = i;
				j++;
			}

			else if (i >= 11 && i <= 15 && hai[i] == 1 && hai[i - 1] == 0 && hai[i - 2] == 0 && hai[i + 1] == 0 && hai[i + 2] == 0)
			{
				*x = i;
				j++;
			}
			else if (i == 16 && hai[i] == 1 && hai[i - 1] == 0 && hai[i - 2] == 0 && hai[i + 1] == 0)
			{
				*x = i;
				j++;
			}

			else if (i == 17 && hai[i] == 1 && hai[i - 1] == 0 && hai[i - 2] == 0)
			{
				*x = i;
				j++;
			}
		}

		for (i = 18; i<27; i++)
		{
			if (i == 18 && hai[18] == 1 && hai[19] == 0 && hai[20] == 0)
			{
				*x = i;
				j++;
			}
			else if (i == 19 && hai[19] == 1 && hai[20] == 0 && hai[21] == 0 && hai[18] == 0)
			{
				*x = i;
				j++;
			}

			else if (hai[i] == 1 && hai[i - 1] == 0 && hai[i - 2] == 0 && hai[i + 1] == 0 && hai[i + 2] == 0)
			{
				*x = i;
				j++;
			}
			else if (i == 25 && hai[i] == 1 && hai[i - 1] == 0 && hai[i - 2] == 0 && hai[i + 1] == 0)
			{
				*x = i;
				j++;
			}

			else if (i == 26 && hai[i] == 1 && hai[i - 1] == 0 && hai[i - 2] == 0)
			{
				*x = i;
				j++;
			}
		}
		for (i = 27; i<34; i++)
		{
			if (hai[i] == 1)
			{
				*x = i;
				j++;
			}
		}
		return j;
	}
	int tumo(int a[])
	{
		int i = 0, j, l = 0;
		int kakuritu = 0;
		int haikakuritu[136] = { 0 };
		double k = 0.0;
		for (i = 0; i <= 33; i++)
		{
			j = 4 - a[i];
			while (j>0)
			{
				kakuritu++;
				haikakuritu[l] = i;
				l++;
				j--;
			}
		}

		////printf("%d¥n",randam()&0x0000ffff);
		k = (randam(0) & 0x0fffffff) / 268435455.0;
		k *= kakuritu;
		j = (int)k;
		////printf("%d¥n",j);
		return haikakuritu[j];
	}
	int shanten_maisuu(int hai[], int mieteruhai[], int x, int n, int fu_ro)//シャンテン数を落とす枚数を数える
	{
		int i, j = 0, m;
		for (i = 0; i<34; i++)
		{
			if (ukihai_x(hai, i) == 1 && mieteruhai[i] != 4 && x != i)
			{
				hai[i]++;
				m = shantensuu(hai, fu_ro);
				if (m + 1 == n)
				{
					j += 4 - mieteruhai[i];
				}//if(m+1==n)
				hai[i]--;
			}
		}
		return j;
	}
	int henka(int hai[], int mieteruhai[], int *n, int x, int fu_ro)//シャンテン数を落とす枚数を増やすものの枚数 nは増えた総数
	{
		int i, j, k = 0, l, m = 0;
		*n = 0;
		m = shantensuu(hai, fu_ro);
		l = shanten_maisuu(hai, mieteruhai, x, m, fu_ro);
		for (i = 0; i<34; i++)
		{
			if (mieteruhai[i] != 4 && x != i)
			{
				hai[i]++;
				mieteruhai[i]++;
				if (shantensuu(hai, fu_ro) == m)
				{
					for (j = 0; j<34; j++)
					{
						if (hai[j]>0)
						{
							hai[j]--;
							if (shantensuu(hai, fu_ro) == m && shanten_maisuu(hai, mieteruhai, x, m, fu_ro)>l)
							{
								*n += shanten_maisuu(hai, mieteruhai, x, m, fu_ro) - l;
								k += 4 - mieteruhai[i];
							}
							hai[j]++;
						}
					}
				}
				hai[i]--;
				mieteruhai[i]--;
			}
		}//for(i=0;i<34;i++)

		return k;
	}
	int ukeire_check(int hai[], int mieteruhai[], int x, int fu_ro)//シャンテン数を落とす種類の枚数*さらにシャンテン数を落とす枚数の総和
	{
		int i, j = 0, k = 0, m, n;
		int a, b;
		n = shantensuu(hai, fu_ro);

		if (n == 0)
		{
			return 0;
		}
		else
		{
			for (i = 0; i<34; i++)
			{
				if (mieteruhai[i] != 4 && x != i)
				{
					hai[i]++;
					m = 4 - mieteruhai[i];
					mieteruhai[i]++;
					j = 0;
					if (shantensuu(hai, fu_ro) == n - 1)
					{
						////printf(" %d ",i);
						for (a = 0; a<34; a++)
						{
							if (hai[a]>0)
							{
								hai[a]--;
								if (shantensuu(hai, fu_ro) == n - 1)
								{
									b = shanten_maisuu(hai, mieteruhai, x, n - 1, fu_ro);

									if (j<b)
									{
										j = b;
									}
								}
								hai[a]++;
							}
						}
						k += m*j;
						////printf("%d¥n",k);			
					}
					mieteruhai[i]--;
					hai[i]--;
				}
			}
		}
		return k;
	}
	int tarts_over_check(int hai[])
	{
		int i, j = 0, k = 0, flag = 0;//flag=0開始してない1開始中
		for (i = 0; i<9; i++)
		{
			if (i == 8)
			{
				j += hai[i];
				flag = 3;
			}
			if (hai[i]>0 && flag == 0)
			{
				flag = 1;
				j += hai[i];
			}
			else if (hai[i]>0 && (flag == 1 || flag == 2))
			{

				flag = 1;
				j += hai[i];
			}
			else if (hai[i] == 0 && flag == 1)
			{
				flag = 2;
			}
			else if ((hai[i] == 0 && flag == 2) || flag == 3)
			{
				flag = 0;
				if (j>0)
				{
					if (j <= 3)
					{
						k++;
					}
					else if (j <= 6)
					{
						k += 2;
					}
					else if (j <= 9)
					{
						k += 3;
					}
					else if (j <= 12)
					{
						k += 4;
					}
					else if (j <= 14)
					{
						k += 5;
					}
				}
				j = 0;
			}
		}//for
		flag = 0;
		j = 0;
		for (i = 9; i<18; i++)
		{
			if (i == 17)
			{
				j += hai[i];
				flag = 3;
			}
			if (hai[i]>0 && flag == 0)
			{
				flag = 1;
				j += hai[i];
			}
			else if (hai[i]>0 && (flag == 1 || flag == 2))
			{
				flag = 1;
				j += hai[i];
			}
			else if (hai[i] == 0 && flag == 1)
			{
				flag = 2;
			}
			else if ((hai[i] == 0 && flag == 2) || flag == 3)
			{
				//	//printf("i=%d",i);
				flag = 0;
				if (j>0)
				{
					if (j <= 3)
					{
						k++;
					}
					else if (j <= 6)
					{
						k += 2;
					}
					else if (j <= 9)
					{
						k += 3;
					}
					else if (j <= 12)
					{
						k += 4;
					}
					else if (j <= 14)
					{
						k += 5;
					}
				}
				j = 0;
			}
		}
		flag = 0;
		j = 0;
		for (i = 18; i<27; i++)
		{
			if (i == 26)
			{
				j += hai[i];
				flag = 3;
			}
			if (hai[i]>0 && flag == 0)
			{
				flag = 1;
				j += hai[i];
			}
			else if (hai[i]>0 && (flag == 1 || flag == 2))
			{
				flag = 1;
				j += hai[i];
			}
			else if (hai[i] == 0 && flag == 1)
			{
				flag = 2;
			}
			else if ((hai[i] == 0 && flag == 2) || flag == 3)
			{
				flag = 0;
				if (j>0)
				{
					if (j <= 3)
					{
						k++;
					}
					else if (j <= 6)
					{
						k += 2;
					}
					else if (j <= 9)
					{
						k += 3;
					}
					else if (j <= 12)
					{
						k += 4;
					}
					else if (j <= 14)
					{
						k += 5;
					}
				}
				j = 0;
			}
		}
		for (i = 27; i<34; i++)
		{
			if (hai[i] >= 2)
			{
				k++;
			}
		}
		return k;
	}
	/*
	int tarts_count_min(int hai[])
	{
	int i,j,k=0,l=0,m=14,n=0;
	int shuntu=0;
	int flag=0;
	int a[4]={0};
	int* p;
	long key;
	int shanten_manzu[4]={0},shanten_souzu[4]={0},shanten_pinzu[4]={0};

	for(i=0;i<34;i++)//頭あり
	{

	l=0;

	if(hai[i]>=2)
	{
	hai[i]-=2;
	for(j=27;j<34;j++)
	{
	if(hai[j]>=3)
	{
	k++;//コウツの数
	}

	if(hai[j]==2)
	{
	l++;//コウツの候補
	}
	}
	flag=0;
	while(flag<=6)
	{
	if((hai[flag]>=1 && (hai[flag+1]>=1 || hai[flag+2]>=1) ) || hai[flag]>=2)
	{
	break;
	}
	else
	{
	flag++;
	}
	}
	key=0;
	for(j=flag;j<=8;j++)
	{
	if(j==0)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	else if(j==1)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	if(j>=2 && j<=6)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	else if(j==7)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}

	else
	{
	key=key*10+hai[j];
	}
	}
	else if(j==8)
	{
	if(hai[j]==1 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	}

	p = search( key );
	if(p==NULL)
	{
	shanten_manzu[0]=0;
	shanten_manzu[1]=0;
	shanten_manzu[2]=0;
	shanten_manzu[3]=0;

	}
	else
	{
	shanten_manzu[0]=*p/1000;
	shanten_manzu[1]=(*p-shanten_manzu[0]*1000)/100;
	shanten_manzu[2]=(*p-shanten_manzu[0]*1000-shanten_manzu[1]*100)/10;
	shanten_manzu[3]=(*p-shanten_manzu[0]*1000-shanten_manzu[1]*100-shanten_manzu[2]*10);
	}
	flag=9;
	while(flag<=15)
	{
	if((hai[flag]>=1 && (hai[flag+1]>=1 || hai[flag+2]>=1) ) || hai[flag]>=2)
	{
	break;
	}
	else
	{
	flag++;
	}
	}

	key=0;
	for(j=flag;j<=17;j++)
	{
	if(j==9)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	else if(j==10)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	if(j>=11 && j<=15)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	else if(j==16)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}

	else
	{
	key=key*10+hai[j];
	}

	}
	else if(j==17)
	{
	if(hai[j]==1 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	}

	p = search( key );
	if(p==NULL)
	{
	shanten_pinzu[0]=0;
	shanten_pinzu[1]=0;
	shanten_pinzu[2]=0;
	shanten_pinzu[3]=0;

	}
	else
	{
	shanten_pinzu[0]=*p/1000;
	shanten_pinzu[1]=(*p-shanten_pinzu[0]*1000)/100;
	shanten_pinzu[2]=(*p-shanten_pinzu[0]*1000-shanten_pinzu[1]*100)/10;
	shanten_pinzu[3]=(*p-shanten_pinzu[0]*1000-shanten_pinzu[1]*100-shanten_pinzu[2]*10);
	}

	flag=18;
	while(flag<=24)
	{
	if((hai[flag]>=1 && (hai[flag+1]>=1 || hai[flag+2]>=1) ) || hai[flag]>=2)
	{
	break;
	}
	else
	{
	flag++;
	}
	}
	key=0;
	for(j=flag;j<=26;j++)
	{
	if(j==18)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	else if(j==19)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	if(j>=20 && j<=24)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	else if(j==25)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}

	else
	{
	key=key*10+hai[j];
	}
	}
	else if(j==26)
	{
	if(hai[j]==1 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	}
	p = search( key );
	if(p==NULL)
	{
	shanten_souzu[0]=0;
	shanten_souzu[1]=0;
	shanten_souzu[2]=0;
	shanten_souzu[3]=0;

	}
	else
	{
	shanten_souzu[0]=*p/1000;
	shanten_souzu[1]=(*p-shanten_souzu[0]*1000)/100;
	shanten_souzu[2]=(*p-shanten_souzu[0]*1000-shanten_souzu[1]*100)/10;
	shanten_souzu[3]=(*p-shanten_souzu[0]*1000-shanten_souzu[1]*100-shanten_souzu[2]*10);
	}

	for(j=0;j<8;j++)
	{
	if(j%2==0)
	{
	a[0]=0;
	}
	else
	{
	a[0]=2;
	}

	if(j%4==0 || j%4==1)
	{
	a[1]=0;
	}
	else
	{
	a[1]=2;
	}

	if(j<=3)
	{
	a[2]=0;
	}
	else
	{
	a[2]=2;
	}

	if(shanten_manzu[a[0]]+shanten_manzu[a[0]+1]+shanten_pinzu[a[1]]+shanten_pinzu[a[1]+1]+shanten_souzu[a[2]]+shanten_souzu[a[2]+1]+k+l>4)
	{
	a[3]=4-shanten_manzu[a[0]]-shanten_pinzu[a[1]]-shanten_souzu[a[2]]-k;
	}

	else
	{
	a[3]=shanten_manzu[a[0]+1]+shanten_pinzu[a[1]+1]+shanten_souzu[a[2]+1]+l;
	}

	shuntu=7-2*(shanten_manzu[a[0]]+shanten_pinzu[a[1]]+shanten_souzu[a[2]]+k)-a[3];//シャンテン数計算
	if(m>shuntu)
	{
	m=shuntu;
	n=shanten_manzu[a[0]]+shanten_manzu[a[0]+1]+shanten_pinzu[a[1]]+shanten_pinzu[a[1]+1]+shanten_souzu[a[2]]+shanten_souzu[a[2]+1]+k+l+1;
	}
	else if(m==shuntu)
	{
	if(n<shanten_manzu[a[0]]+shanten_manzu[a[0]+1]+shanten_pinzu[a[1]]+shanten_pinzu[a[1]+1]+shanten_souzu[a[2]]+shanten_souzu[a[2]+1]+k+l+1)
	{
	n=shanten_manzu[a[0]]+shanten_manzu[a[0]+1]+shanten_pinzu[a[1]]+shanten_pinzu[a[1]+1]+shanten_souzu[a[2]]+shanten_souzu[a[2]+1]+k+l+1;
	}
	}
	if(m<-1)
	{
	m=-1;
	}
	}
	k=0;
	l=0;
	hai[i]+=2;
	}//if(hai2[i]>=2)頭あり

	}//for(i=0;i<34;i++)

	k=0;
	l=0;
	//頭なし
	for(j=27;j<34;j++)
	{
	if(hai[j]>=3)
	{
	k++;//コウツの数
	}

	if(hai[j]==2)
	{
	l++;//コウツの候補
	}
	}

	flag=0;
	while(flag<=6)
	{
	if((hai[flag]>=1 && (hai[flag+1]>=1 || hai[flag+2]>=1) ) || hai[flag]>=2)
	{
	break;
	}
	else
	{
	flag++;
	}
	}
	key=0;
	for(j=flag;j<=8;j++)
	{
	if(j==0)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	else if(j==1)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	if(j>=2 && j<=6)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	else if(j==7)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}

	else
	{
	key=key*10+hai[j];
	}
	}
	else if(j==8)
	{
	if(hai[j]==1 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	}

	p = search( key );
	//	//printf("%ld¥n",key);
	if(p==NULL)
	{
	shanten_manzu[0]=0;
	shanten_manzu[1]=0;
	shanten_manzu[2]=0;
	shanten_manzu[3]=0;

	}
	else
	{
	shanten_manzu[0]=*p/1000;
	shanten_manzu[1]=(*p-shanten_manzu[0]*1000)/100;
	shanten_manzu[2]=(*p-shanten_manzu[0]*1000-shanten_manzu[1]*100)/10;
	shanten_manzu[3]=(*p-shanten_manzu[0]*1000-shanten_manzu[1]*100-shanten_manzu[2]*10);
	}
	flag=9;
	while(flag<=15)
	{
	if((hai[flag]>=1 && (hai[flag+1]>=1 || hai[flag+2]>=1) ) || hai[flag]>=2)
	{
	break;
	}
	else
	{
	flag++;
	}
	}
	key=0;
	for(j=flag;j<=17;j++)
	{
	if(j==9)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	else if(j==10)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	if(j>=11 && j<=15)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	else if(j==16)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}

	else
	{
	key=key*10+hai[j];
	}
	}
	else if(j==17)
	{
	if(hai[j]==1 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	}

	p = search( key );
	if(p==NULL)
	{
	shanten_pinzu[0]=0;
	shanten_pinzu[1]=0;
	shanten_pinzu[2]=0;
	shanten_pinzu[3]=0;

	}
	else
	{
	shanten_pinzu[0]=*p/1000;
	shanten_pinzu[1]=(*p-shanten_pinzu[0]*1000)/100;
	shanten_pinzu[2]=(*p-shanten_pinzu[0]*1000-shanten_pinzu[1]*100)/10;
	shanten_pinzu[3]=(*p-shanten_pinzu[0]*1000-shanten_pinzu[1]*100-shanten_pinzu[2]*10);
	}
	flag=18;
	while(flag<=24)
	{
	if((hai[flag]>=1 && (hai[flag+1]>=1 || hai[flag+2]>=1) ) || hai[flag]>=2)
	{
	break;
	}
	else
	{
	flag++;
	}
	}
	key=0;
	for(j=flag;j<=26;j++)
	{
	if(j==18)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	else if(j==19)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	if(j>=20 && j<=24)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	else if(j==25)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}

	else
	{
	key=key*10+hai[j];
	}
	}
	else if(j==26)
	{
	if(hai[j]==1 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	}

	p = search( key );
	if(p==NULL)
	{
	shanten_souzu[0]=0;
	shanten_souzu[1]=0;
	shanten_souzu[2]=0;
	shanten_souzu[3]=0;

	}
	else
	{
	shanten_souzu[0]=*p/1000;
	shanten_souzu[1]=(*p-shanten_souzu[0]*1000)/100;
	shanten_souzu[2]=(*p-shanten_souzu[0]*1000-shanten_souzu[1]*100)/10;
	shanten_souzu[3]=(*p-shanten_souzu[0]*1000-shanten_souzu[1]*100-shanten_souzu[2]*10);
	}

	for(j=0;j<8;j++)
	{
	if(j%2==0)
	{
	a[0]=0;
	}
	else
	{
	a[0]=2;
	}

	if(j%4==0 || j%4==1)
	{
	a[1]=0;
	}
	else
	{
	a[1]=2;
	}

	if(j<=3)
	{
	a[2]=0;
	}
	else
	{
	a[2]=2;
	}

	if(shanten_manzu[a[0]]+shanten_manzu[a[0]+1]+shanten_pinzu[a[1]]+shanten_pinzu[a[1]+1]+shanten_souzu[a[2]]+shanten_souzu[a[2]+1]+k+l>4)
	{
	a[3]=4-shanten_manzu[a[0]]-shanten_pinzu[a[1]]-shanten_souzu[a[2]]-k;
	}

	else
	{
	a[3]=shanten_manzu[a[0]+1]+shanten_pinzu[a[1]+1]+shanten_souzu[a[2]+1]+l;
	}
	////printf("%d %d %d %d ¥n",shanten_manzu[0],shanten_manzu[1],shanten_manzu[2],shanten_manzu[3]);
	shuntu=8-2*(shanten_manzu[a[0]]+shanten_pinzu[a[1]]+shanten_souzu[a[2]]+k)-a[3];//シャンテン数計算
	if(m>shuntu)
	{
	m=shuntu;
	n=shanten_manzu[a[0]]+shanten_manzu[a[0]+1]+shanten_pinzu[a[1]]+shanten_pinzu[a[1]+1]+shanten_souzu[a[2]]+shanten_souzu[a[2]+1]+k+l;
	}
	if(m>=shuntu)
	{
	if(n<shanten_manzu[a[0]]+shanten_manzu[a[0]+1]+shanten_pinzu[a[1]]+shanten_pinzu[a[1]+1]+shanten_souzu[a[2]]+shanten_souzu[a[2]+1]+k+l)
	{
	n=shanten_manzu[a[0]]+shanten_manzu[a[0]+1]+shanten_pinzu[a[1]]+shanten_pinzu[a[1]+1]+shanten_souzu[a[2]]+shanten_souzu[a[2]+1]+k+l;
	}
	}
	if(m<-1)
	{
	m=0;
	}
	}


	return n;

	}*/
	int tarts_count_max(int hai[])
	{
		int i, j = 0, k = 0, flag = 0;//flag=0開始してない1開始中
		for (i = 0; i<9; i++)
		{
			if ((hai[i] == 0 && flag == 0) || i == 8)
			{
				j += hai[i];
				flag = 1;
				if (j>0)
				{
					if (j <= 3)
					{
						k++;
					}
					else if (j <= 6)
					{
						k += 2;
					}
					else if (j <= 9)
					{
						k += 3;
					}
					else if (j <= 12)
					{
						k += 4;
					}
					else if (j <= 14)
					{
						k += 5;
					}
				}
				j = 0;
			}
			else if (hai[i]>0)
			{
				j += hai[i];
				flag = 1;
			}
			else if (hai[i] == 0 && flag == 1)
			{
				flag = 0;
			}

		}//for
		flag = 0;
		j = 0;
		for (i = 9; i<18; i++)
		{
			if ((hai[i] == 0 && flag == 0) || i == 17)
			{
				j += hai[i];
				flag = 1;
				if (j>0)
				{
					if (j <= 3)
					{
						k++;
					}
					else if (j <= 6)
					{
						k += 2;
					}
					else if (j <= 9)
					{
						k += 3;
					}
					else if (j <= 12)
					{
						k += 4;
					}
					else if (j <= 14)
					{
						k += 5;
					}
				}
				j = 0;
			}
			else if (hai[i]>0)
			{
				j += hai[i];
				flag = 1;
			}
			else if (hai[i] == 0 && flag == 1)
			{
				flag = 0;
			}

		}//for
		flag = 0;
		j = 0;
		for (i = 18; i<27; i++)
		{
			if ((hai[i] == 0 && flag == 0) || i == 26)
			{
				j += hai[i];
				flag = 1;
				if (j>0)
				{
					if (j <= 3)
					{
						k++;
					}
					else if (j <= 6)
					{
						k += 2;
					}
					else if (j <= 9)
					{
						k += 3;
					}
					else if (j <= 12)
					{
						k += 4;
					}
					else if (j <= 14)
					{
						k += 5;
					}
				}
				j = 0;
			}
			else if (hai[i]>0)
			{
				j += hai[i];
				flag = 1;
			}
			else if (hai[i] == 0 && flag == 1)
			{
				flag = 0;
			}

		}//for
		flag = 0;
		j = 0;
		for (i = 27; i<34; i++)
		{
			if (hai[i] >= 1)
			{
				k++;
			}
		}
		return k;
	}
	int matitori(int hai[], int matitori[], int fu_ro)
	{
		int i = 0, j = 0;
		for (i = 0; i<34; i++)
		{
			if (hai[i]>0)
			{
				hai[i]--;
				if (shantensuu(hai, fu_ro) == 0)
				{
					matitori[j] = i;
					j++;
				}
				hai[i]++;
			}
		}
		return j;
	}
	double kitai_houjuu_ten(int dora, int kiri_dora, int ri_ti, int fu_ro, int fu_ro_bu[], int fu_ro_aka[], int kaze, int mieterudora)
	{
		int i, j, hai[34] = { 0 };
		if (ri_ti == 1)
		{
			if (kaze == 27)
			{
				double oya_ri_ti[8][3] = { { 104.77,121,138.52 },{ 99.56,114.68,141 },{ 94.03,107.54,126 },{ 88.47,101.01,121 },{ 83.22,92.93,105 },{ 78.55,89.52,120 },{ 73.25,85,0 },{ 66.44,0,0 } };
				return oya_ri_ti[mieterudora][kiri_dora];
			}
			else
			{
				double ko_ri_ti[8][3] = { { 71.53,82.03,103.67 },{ 68.06,79.65,103.1 },{ 64.22,75.27,92.17 },{ 60.52,72.11,87.45 },{ 57.08,67.39,80 },{ 53.53,64.67,75 },{ 49.34,52.1,0 },{ 43.08,0,0 } };
				return ko_ri_ti[mieterudora][kiri_dora];
			}
		}
		else if (ri_ti == 0 && fu_ro == 0)
		{
			if (kaze == 27)
			{
				double oya_dama[8][3] = { { 89.99,112.56,125.83 },{ 83.99,102.02,122.42 },{ 76.81,92.91,106 },{ 68.98,84.11,100 },{ 62.98,80.48,95 },{ 55.8,64.04,0 },{ 49.9,58.14,0 },{ 45.88,0,0 } };
				return oya_dama[mieterudora][kiri_dora];
			}
			else
			{
				double ko_dama[8][3] = { { 55.52,70.88,90.24 },{ 51.37,66.78,88.93 },{ 46.4,63.3,76.14 },{ 41.17,57.57,68.78 },{ 37.01,50.25,63 },{ 33.34,45.25,0 },{ 29.83,41,0 },{ 28.59,0,0 } };
				return ko_dama[mieterudora][kiri_dora];
			}
		}
		else
		{
			for (i = 0; i<fu_ro; i++)
			{
				j = fu_ro_bu[i];
				if (j>34)
				{
					j -= 34;
					hai[j] += 3;
				}
				else
				{
					hai[j]++;
					hai[j + 1]++;
					hai[j + 2]++;
				}
			}
			j = 0;
			for (i = 0; i<fu_ro; i++)
			{
				if (fu_ro_aka[i] >= 34)
				{
					j++;
				}
			}
			j += hai[dora];
			j += kiri_dora;
			if (kaze == 27)
			{
				double oya_fu_ro[8][8] = { { 59.25,77.83,98.66,129.77,150.46,180,180,240 },{ 55.29,72.29,93.44,128.66,146.31,180,180,0 },
				{ 51.07,67.07,89.31,128.21,141.88,180,0,0 },{ 47.16,62.97,82.38,127.59,136.95,0,0,0 },
				{ 44.45,59.53,77.94,126.07,0,0,0,0 },{ 42.69,57.17,69.25,0,0,0,0,0 },{ 41.61,55.84,0,0,0,0,0,0 },{ 41.26,0,0,0,0,0,0,0 } };
				return oya_fu_ro[mieterudora][j];
			}
			else
			{
				double ko_fu_ro[8][8] = { { 35.72,48.98,64.38,85.2,98.5,121.33,120,160 },{ 33.04,45.55,61.61,84.04,94.46,120.59,120,0 },
				{ 30.27,41.51,58.05,84.2,91.24,120,0 },{ 27.71,37.67,54.74,84.04,88.2,0,0,0 },
				{ 26.05,35.34,50.89,82.88,0,0,0,0 },{ 25.02,32.56,45.18,0,0,0,0,0 },
				{ 24.78,30.88,0,0,0,0,0,0 },{ 25.22,0,0,0,0,0,0,0 } };
				return ko_fu_ro[mieterudora][j];
			}
		}
	}
	double tenpai_prob(int junme, int fu_ro, int tedasi, int ri_ti)
	{
		if (ri_ti == 1)
		{
			return 1.0;
		}
		else if (ri_ti == 0 && fu_ro == 0)
		{
			double houjuu[18] = { 0,0.0000023,0.0000182,0.0000466,0.000135822,0.000302712,0.000548742,0.000898523,0.001284274,0.001891844,0.002573561,0.003118234,0.003754068,0.004288421,0.004942179,0.005509689,0.005767812,0.008819527 };
			return houjuu[junme];
		}
		else
		{
			if (tedasi>10)
			{
				tedasi = 10;
			}
			if (fu_ro == 1)
			{
				double fu_ro1[18][11] = { { 0.007841126,0.017057902,0.017057902,0.017057902,0.017057902,0.017057902,0.017057902,0.017057902,0.017057902,0.017057902,0.017057902 },
				{ 0.014866204,0.030496357,0.050533069,0.050533069,0.050533069,0.050533069,0.050533069,0.050533069,0.050533069,0.050533069,0.050533069 },
				{ 0.029085098,0.054914259,0.085914716,0.126279107,0.126279107,0.126279107,0.126279107,0.126279107,0.126279107,0.126279107,0.126279107 },
				{ 0.052757695,0.093426818,0.133249176,0.180168913,0.224398721,0.224398721,0.224398721,0.224398721,0.224398721,0.224398721,0.224398721 },
				{ 0.083303233,0.143142616,0.198873818,0.252813109,0.311839434,0.40209,0.40209,0.40209,0.40209,0.40209,0.40209 },
				{ 0.124156614,0.20257714,0.275533792,0.335349087,0.397423022,0.462955267,0.502736358,0.502736358,0.502736358,0.502736358,0.502736358 },
				{ 0.169244444,0.265600028,0.35111946,0.422426071,0.486075206,0.546204054,0.588747424,0.626826366,0.626826366,0.626826366,0.626826366 },
				{ 0.221697786,0.327923727,0.419654028,0.495463741,0.560533148,0.615113148,0.656081496,0.696646243,0.709285982,0.709285982,0.709285982 },
				{ 0.277632577,0.391213417,0.482623076,0.563219958,0.625376253,0.677986458,0.715744004,0.742705955,0.760363389,0.760363389,0.760363389 },
				{ 0.324997459,0.440431251,0.530926479,0.60755423,0.668814058,0.718697435,0.754228008,0.776141245,0.789640266,0.793464988,0.793464988 },
				{ 0.370278943,0.490214678,0.581004485,0.652963383,0.707635931,0.753392637,0.786397231,0.806209662,0.820495488,0.831615237,0.842840888 },
				{ 0.396761134,0.51440831,0.601998362,0.669356597,0.722404584,0.764805114,0.79521193,0.815873817,0.830958853,0.842354886,0.857448567 },
				{ 0.435314411,0.550626123,0.636735653,0.695963535,0.743102154,0.781728799,0.808455885,0.829930547,0.845170987,0.856752685,0.866054459 },
				{ 0.491765857,0.591797994,0.668889968,0.723472517,0.764960319,0.797704724,0.82168046,0.838922553,0.852477015,0.861403566,0.868136996 },
				{ 0.568293482,0.657885646,0.72614837,0.770706631,0.803279831,0.831659712,0.851882409,0.866833713,0.877455927,0.885931598,0.888445502 },
				{ 0.644683477,0.722833958,0.775681804,0.815885892,0.842242083,0.863843055,0.879419556,0.889482901,0.897392674,0.905049937,0.90818231 },
				{ 0.707925011,0.777945637,0.820771612,0.850216275,0.871178374,0.889137421,0.899763228,0.907681393,0.913536695,0.919950887,0.922926164 },
				{ 0.75111276,0.812555912,0.849180303,0.874965606,0.892678812,0.906240095,0.914477399,0.919114167,0.923312393,0.928357631,0.931760643 } };
				return fu_ro1[junme][tedasi];
			}
			else if (fu_ro == 2)
			{
				double fu_ro2[18][11] = { { 0.06234414,0.06234414,0.06234414,0.06234414,0.06234414,0.06234414,0.06234414,0.06234414,0.06234414,0.06234414,0.06234414 },
				{ 0.098387097,0.17180087,0.17180087,0.17180087,0.17180087,0.17180087,0.17180087,0.17180087,0.17180087,0.17180087,0.17180087 },
				{ 0.132953653,0.229453372,0.295879805,0.295879805,0.295879805,0.295879805,0.295879805,0.295879805,0.295879805,0.295879805,0.295879805 },
				{ 0.190180879,0.290733419,0.385216011,0.444472299,0.444472299,0.444472299,0.444472299,0.444472299,0.444472299,0.444472299,0.444472299 },
				{ 0.24454508,0.363704683,0.46586728,0.556398251,0.638114363,0.638114363,0.638114363,0.638114363,0.638114363,0.638114363,0.638114363 },
				{ 0.314340589,0.448180321,0.555435414,0.647238426,0.70393225,0.769725084,0.769725084,0.769725084,0.769725084,0.769725084,0.769725084 },
				{ 0.374620468,0.51796878,0.62214559,0.703818468,0.767003862,0.804283244,0.869522163,0.869522163,0.869522163,0.869522163,0.869522163 },
				{ 0.435791062,0.578826661,0.678644241,0.752096986,0.803116603,0.842493283,0.878290264,0.878290264,0.878290264,0.878290264,0.878290264 },
				{ 0.488972163,0.626597125,0.72071752,0.782680514,0.827973553,0.862996901,0.89791926,0.89791926,0.89791926,0.89791926,0.89791926 },
				{ 0.542069506,0.67297736,0.759233948,0.816939676,0.85366834,0.883984062,0.901521355,0.915589733,0.929658111,0.929658111,0.929658111 },
				{ 0.58577474,0.710821203,0.78902848,0.83914792,0.873024343,0.89643548,0.912304237,0.923997005,0.930330588,0.930330588,0.930330588 },
				{ 0.632188751,0.745437636,0.815262071,0.860312968,0.891108524,0.909306021,0.919913504,0.934698703,0.947137046,0.947137046,0.947137046 },
				{ 0.667583173,0.772590403,0.835733686,0.873935155,0.901603456,0.916761148,0.928130357,0.939130813,0.946984902,0.946984902,0.946984902 },
				{ 0.69924812,0.795003203,0.852996925,0.884883021,0.909876723,0.923043348,0.93370401,0.94139908,0.948724195,0.954756643,0.954756643 },
				{ 0.706784387,0.803653585,0.858437227,0.887855433,0.910012245,0.92199431,0.933646612,0.939176061,0.943370816,0.953134468,0.953134468 },
				{ 0.723104056,0.812251043,0.863512484,0.889521468,0.911664795,0.923267031,0.934137535,0.938711317,0.943285099,0.949729974,0.952014976 },
				{ 0.756797583,0.83418017,0.873949967,0.899048425,0.917058565,0.929355876,0.939569484,0.944933139,0.948775013,0.953348673,0.955142955 },
				{ 0.782034346,0.848910172,0.882485689,0.905545101,0.92353651,0.936220028,0.944881505,0.949892278,0.953688317,0.955893635,0.957811303 } };
				return fu_ro2[junme][tedasi];
			}
			else if (fu_ro == 3)
			{
				double fu_ro3[18][11] = { { 0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3 },
				{ 0.405940594,0.405940594,0.405940594,0.405940594,0.405940594,0.405940594,0.405940594,0.405940594,0.405940594,0.405940594,0.405940594 },
				{ 0.534743202,0.689828802,0.689828802,0.689828802,0.689828802,0.689828802,0.689828802,0.689828802,0.689828802,0.689828802,0.689828802 },
				{ 0.612483745,0.74165583,0.948331166,0.948331166,0.948331166,0.948331166,0.948331166,0.948331166,0.948331166,0.948331166,0.948331166 },
				{ 0.659038902,0.789666855,0.851014022,0.851014022,0.851014022,0.851014022,0.851014022,0.851014022,0.851014022,0.851014022,0.851014022 },
				{ 0.703308431,0.814874269,0.870761282,0.870761282,0.870761282,0.870761282,0.870761282,0.870761282,0.870761282,0.870761282,0.870761282 },
				{ 0.7431645,0.847886118,0.884393449,0.904794605,0.904794605,0.904794605,0.904794605,0.904794605,0.904794605,0.904794605,0.904794605 },
				{ 0.78553616,0.874143588,0.909156274,0.934508012,0.934508012,0.934508012,0.934508012,0.934508012,0.934508012,0.934508012,0.934508012 },
				{ 0.814303639,0.895236677,0.922397539,0.942574179,0.957686237,0.957686237,0.957686237,0.957686237,0.957686237,0.957686237,0.957686237 },
				{ 0.838975297,0.911654014,0.939525069,0.958578815,0.96813755,0.975218094,0.975218094,0.975218094,0.975218094,0.975218094,0.975218094 },
				{ 0.849227491,0.921598295,0.94914484,0.967400539,0.974531671,0.97998917,0.993329723,0.993329723,0.993329723,0.993329723,0.993329723 },
				{ 0.866263441,0.93257909,0.958391667,0.974597018,0.983064678,0.986828083,0.993414042,0.993414042,0.993414042,0.993414042,0.993414042 },
				{ 0.866004963,0.9316067,0.956228288,0.97133995,0.980685619,0.983904682,0.989269788,0.989269788,0.989269788,0.989269788,0.989269788 },
				{ 0.875675676,0.928597516,0.951446311,0.967630874,0.979619439,0.984148453,0.985589503,0.992794751,0.992794751,0.992794751,0.992794751 },
				{ 0.867924528,0.91745283,0.947340599,0.963430971,0.973995357,0.982962475,0.988286702,0.994143351,0.994143351,0.994143351,0.994143351 },
				{ 0.87704918,0.917596791,0.948678703,0.965042015,0.974655461,0.983394957,0.989621848,0.991697479,0.991697479,0.991697479,0.991697479 },
				{ 0.894459103,0.93224535,0.957813142,0.970117643,0.974714928,0.983467453,0.992767011,0.994833579,0.994833579,0.994833579,0.994833579 },
				{ 0.924778761,0.954867257,0.969227675,0.978593165,0.982160971,0.98635839,0.993937062,0.99514965,0.99514965,0.99514965,0.99514965 } };
				return fu_ro3[junme][tedasi];
			}
			else
			{
				return 1.0;
			}
		}
		return 0.0;
	}
	int genbutu_check(int mati[], int shurui, int sutehai[], int sutehai_suu, int pre_genbutu[], int pre_genbutu_suu, int hai[])
	{
		int i, j = 0;
		for (i = 0; i<34; i++)
		{
			hai[i] = 0;
		}
		for (i = 0; i<sutehai_suu; i++)
		{
			hai[sutehai[i]]++;
		}
		for (i = 0; i<pre_genbutu_suu - 1; i++)
		{
			hai[pre_genbutu[i]]++;
		}
		for (i = 0; i<34; i++)
		{
			if (hai[i]>0)
			{
				hai[i] = 1;
			}
		}
		for (i = 0; i<shurui; i++)
		{
			if (hai[mati[shurui]]>0)
			{
				j = 1;

			}
		}
		return j;

	}
	int hai_count(int count[], int what, int dora, int dora_hyouji, int hai[], int tehai_aka, int fu_ro_bu0[], int fu_ro_bu1[], int fu_ro_bu2[], int fu_ro_bu3[], int fu_ro_aka0[], int fu_ro_aka1[], int fu_ro_aka2[], int fu_ro_aka3[], int sutehai0[], int sutehai1[], int sutehai2[], int sutehai3[], int sutehai_suu[])
	{
		//what=-1ドラの数 他その数字
		int i = 0, j = 0;
		int dora_maisuu = 0;
		for (i = 0; i<34; i++)
		{
			count[i] = hai[i];
		}
		////printf("%d %d %d %d¥n",sutehai_suu[0],sutehai_suu[1],sutehai_suu[2],sutehai_suu[3]);
		for (i = 0; i<sutehai_suu[0]; i++)
		{
			count[sutehai0[i]]++;
		}
		for (i = 0; i<sutehai_suu[1]; i++)
		{
			count[sutehai1[i]]++;
		}
		for (i = 0; i<sutehai_suu[2]; i++)
		{
			count[sutehai2[i]]++;
		}
		for (i = 0; i<sutehai_suu[3]; i++)
		{
			count[sutehai3[i]]++;
		}
		i = 0;
		while (fu_ro_bu0[i] != -1)
		{
			j = fu_ro_bu0[i];
			if (fu_ro_aka0[i] >= 34)
			{
				dora_maisuu++;
			}
			if (j>34)
			{
				j -= 34;
				count[j] += 2;
			}
			else
			{
				count[j]++;
				count[j + 1]++;
				count[j + 2]++;
				if (fu_ro_aka0[i] >= 34)
				{
					count[fu_ro_aka0[i] - 34]--;
				}
				else
				{
					count[fu_ro_aka0[i]]--;
				}
			}
			i++;
		}
		i = 0;
		while (fu_ro_bu1[i] != -1)
		{
			j = fu_ro_bu1[i];
			if (fu_ro_aka1[i] >= 34)
			{
				dora_maisuu++;
			}
			if (j>34)
			{
				j -= 34;
				count[j] += 2;
			}
			else
			{
				count[j]++;
				count[j + 1]++;
				count[j + 2]++;
				if (fu_ro_aka1[i] >= 34)
				{
					count[fu_ro_aka1[i] - 34]--;
				}
				else
				{
					count[fu_ro_aka1[i]]--;
				}
			}
			i++;
		}
		i = 0;
		while (fu_ro_bu2[i] != -1)
		{
			j = fu_ro_bu2[i];
			if (fu_ro_aka2[i] >= 34)
			{
				dora_maisuu++;
			}
			if (j>34)
			{
				j -= 34;
				count[j] += 2;
			}
			else
			{
				count[j]++;
				count[j + 1]++;
				count[j + 2]++;
				if (fu_ro_aka2[i] >= 34)
				{
					count[fu_ro_aka2[i] - 34]--;
				}
				else
				{
					count[fu_ro_aka2[i]]--;
				}
			}
			i++;
		}
		i = 0;
		while (fu_ro_bu3[i] != -1)
		{
			j = fu_ro_bu3[i];
			if (fu_ro_aka3[i] >= 34)
			{
				dora_maisuu++;
			}
			if (j>34)
			{
				j -= 34;
				count[j] += 2;
			}
			else
			{
				count[j]++;
				count[j + 1]++;
				count[j + 2]++;
				if (fu_ro_aka3[i] >= 34)
				{
					count[fu_ro_aka3[i] - 34]--;
				}
				else
				{
					count[fu_ro_aka3[i]]--;
				}
			}
			i++;
		}
		j = dora_hyouji;
		if (j>34)
		{
			dora_maisuu++;
			j -= 34;
		}
		count[j]++;
		if (tehai_aka >= 4)
		{
			tehai_aka -= 4;
			dora_maisuu++;
		}
		if (tehai_aka >= 2)
		{
			tehai_aka -= 2;
			dora_maisuu++;
		}
		if (tehai_aka >= 1)
		{
			tehai_aka -= 1;
			dora_maisuu++;
		}
		dora_maisuu += count[dora];
		//hai_check(count);
		if (what == -1)
		{
			return dora_maisuu;
		}
		else
		{
			return count[what];
		}
	}
	/*
	int atama_count_min(int hai[])
	{
	int i,j,k=0,l=0,m=14,n=0;
	int shuntu=0;
	int flag=0,flag2=0;
	int a[4]={0};
	int* p;
	long key;
	int shanten_manzu[4]={0},shanten_souzu[4]={0},shanten_pinzu[4]={0};

	for(i=0;i<34;i++)//頭あり
	{

	l=0;

	if(hai[i]>=2)
	{
	hai[i]-=2;
	for(j=27;j<34;j++)
	{
	if(hai[j]>=3)
	{
	k++;//コウツの数
	}

	if(hai[j]==2)
	{
	l++;//コウツの候補
	}
	}
	flag=0;
	while(flag<=6)
	{
	if((hai[flag]>=1 && (hai[flag+1]>=1 || hai[flag+2]>=1) ) || hai[flag]>=2)
	{
	break;
	}
	else
	{
	flag++;
	}
	}
	key=0;
	for(j=flag;j<=8;j++)
	{
	if(j==0)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	else if(j==1)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	if(j>=2 && j<=6)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	else if(j==7)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}

	else
	{
	key=key*10+hai[j];
	}
	}
	else if(j==8)
	{
	if(hai[j]==1 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	}

	p = search( key );
	if(p==NULL)
	{
	shanten_manzu[0]=0;
	shanten_manzu[1]=0;
	shanten_manzu[2]=0;
	shanten_manzu[3]=0;

	}
	else
	{
	shanten_manzu[0]=*p/1000;
	shanten_manzu[1]=(*p-shanten_manzu[0]*1000)/100;
	shanten_manzu[2]=(*p-shanten_manzu[0]*1000-shanten_manzu[1]*100)/10;
	shanten_manzu[3]=(*p-shanten_manzu[0]*1000-shanten_manzu[1]*100-shanten_manzu[2]*10);
	}
	flag=9;
	while(flag<=15)
	{
	if((hai[flag]>=1 && (hai[flag+1]>=1 || hai[flag+2]>=1) ) || hai[flag]>=2)
	{
	break;
	}
	else
	{
	flag++;
	}
	}

	key=0;
	for(j=flag;j<=17;j++)
	{
	if(j==9)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	else if(j==10)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	if(j>=11 && j<=15)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	else if(j==16)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}

	else
	{
	key=key*10+hai[j];
	}

	}
	else if(j==17)
	{
	if(hai[j]==1 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	}

	p = search( key );
	if(p==NULL)
	{
	shanten_pinzu[0]=0;
	shanten_pinzu[1]=0;
	shanten_pinzu[2]=0;
	shanten_pinzu[3]=0;

	}
	else
	{
	shanten_pinzu[0]=*p/1000;
	shanten_pinzu[1]=(*p-shanten_pinzu[0]*1000)/100;
	shanten_pinzu[2]=(*p-shanten_pinzu[0]*1000-shanten_pinzu[1]*100)/10;
	shanten_pinzu[3]=(*p-shanten_pinzu[0]*1000-shanten_pinzu[1]*100-shanten_pinzu[2]*10);
	}

	flag=18;
	while(flag<=24)
	{
	if((hai[flag]>=1 && (hai[flag+1]>=1 || hai[flag+2]>=1) ) || hai[flag]>=2)
	{
	break;
	}
	else
	{
	flag++;
	}
	}
	key=0;
	for(j=flag;j<=26;j++)
	{
	if(j==18)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	else if(j==19)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}

	}

	if(j>=20 && j<=24)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j+2]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	else if(j==25)
	{
	if(hai[j]==1 && hai[j+1]==0 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}

	else
	{
	key=key*10+hai[j];
	}
	}
	else if(j==26)
	{
	if(hai[j]==1 && hai[j-1]==0 && hai[j-2]==0)
	{
	key=key*10;
	}
	else
	{
	key=key*10+hai[j];
	}
	}
	}
	p = search( key );
	if(p==NULL)
	{
	shanten_souzu[0]=0;
	shanten_souzu[1]=0;
	shanten_souzu[2]=0;
	shanten_souzu[3]=0;

	}
	else
	{
	shanten_souzu[0]=*p/1000;
	shanten_souzu[1]=(*p-shanten_souzu[0]*1000)/100;
	shanten_souzu[2]=(*p-shanten_souzu[0]*1000-shanten_souzu[1]*100)/10;
	shanten_souzu[3]=(*p-shanten_souzu[0]*1000-shanten_souzu[1]*100-shanten_souzu[2]*10);
	}

	for(j=0;j<8;j++)
	{
	if(j%2==0)
	{
	a[0]=0;
	}
	else
	{
	a[0]=2;
	}

	if(j%4==0 || j%4==1)
	{
	a[1]=0;
	}
	else
	{
	a[1]=2;
	}

	if(j<=3)
	{
	a[2]=0;
	}
	else
	{
	a[2]=2;
	}

	if(shanten_manzu[a[0]]+shanten_manzu[a[0]+1]+shanten_pinzu[a[1]]+shanten_pinzu[a[1]+1]+shanten_souzu[a[2]]+shanten_souzu[a[2]+1]+k+l>4)
	{
	a[3]=4-shanten_manzu[a[0]]-shanten_pinzu[a[1]]-shanten_souzu[a[2]]-k;
	}

	else
	{
	a[3]=shanten_manzu[a[0]+1]+shanten_pinzu[a[1]+1]+shanten_souzu[a[2]+1]+l;
	}

	shuntu=7-2*(shanten_manzu[a[0]]+shanten_pinzu[a[1]]+shanten_souzu[a[2]]+k)-a[3];//シャンテン数計算
	if(m>shuntu)
	{
	m=shuntu;
	n=1;
	flag2=1;
	}
	else if(m==shuntu && flag2==0)
	{
	n++;
	flag2=1;
	}
	if(m<-1)
	{
	m=-1;
	}
	}
	k=0;
	l=0;
	hai[i]+=2;
	}//if(hai2[i]>=2)頭あり
	flag2=0;
	}//for(i=0;i<34;i++)
	return n;
	}*/

	int tokuten_sim(int jibun, int simotya, int toimen, int kamitya, int kyoku, int honba, int ri_ti_bou, int oya, int tonnan, double junibunnpu[])
	{
		int i, loop1, loop2, temp = 1, kyoku1, oya1, honba1, ri_ti_bou1, all = 0, flag = 0;
		int joutai[1000];
		int tokuten[4];
		int shuusi[6][9];
		int hansuu[1000];
		int para1, para2;
		long juni[4] = { 0 };
		int moto[4] = { 0 };
		double heikinjuni;
		unsigned long k;

		tokuten[0] = jibun;
		tokuten[1] = simotya;
		tokuten[2] = toimen;
		tokuten[3] = kamitya;
		kyoku1 = kyoku;
		oya = oya % 4;
		oya1 = oya;
		honba1 = honba;
		ri_ti_bou1 = ri_ti_bou;
		all = tokuten[0] + tokuten[1] + tokuten[2] + tokuten[3];
		for (i = 0; i <= 3; i++)
		{
			moto[(100 - kyoku + oya + i) % 4] = i;
		}
		for (i = 0; i <= 209; i++)
		{
			joutai[i] = 0;/*上がり*/
		}
		for (i = 210; i <= 337; i++)
		{
			joutai[i] = 1;/*放銃*/
		}
		for (i = 338; i <= 574; i++)
		{
			joutai[i] = 2;/*つもられる*/
		}

		for (i = 575; i <= 624; i++)
		{
			joutai[i] = 3;/*流局テンパイ*/
		}
		for (i = 625; i <= 724; i++)
		{
			joutai[i] = 4;/*ノーテン*/
		}
		for (i = 725; i <= 999; i++)
		{
			joutai[i] = 5;/*何もなし*/
		}
		for (i = 0; i <= 134; i++)
		{
			hansuu[i] = 0;/*1翻*/
		}
		for (i = 135; i <= 360; i++)
		{
			hansuu[i] = 1;/*２翻*/
		}
		for (i = 361; i <= 545; i++)
		{
			hansuu[i] = 2;/*３翻３０符*/
		}
		for (i = 546; i <= 615; i++)
		{
			hansuu[i] = 3;/*３翻40符 or 4翻20符*/
		}
		for (i = 616; i <= 909; i++)
		{
			hansuu[i] = 4;/*満貫*/
		}
		for (i = 910; i <= 985; i++)
		{
			hansuu[i] = 5;/*ハネ満*/
		}
		for (i = 986; i <= 997; i++)
		{
			hansuu[i] = 6;/*倍満*/
		}

		i = 998;
		hansuu[i] = 7;/*三倍満*/
		i = 999;
		hansuu[i] = 8;/*役満*/


		shuusi[0][0] = 10; shuusi[0][1] = 20; shuusi[0][2] = 40; shuusi[0][3] = 52; shuusi[0][4] = 80; shuusi[0][5] = 120; shuusi[0][6] = 160; shuusi[0][7] = 240; shuusi[0][8] = 320;
		shuusi[1][0] = -12; shuusi[1][1] = -23; shuusi[1][2] = -47; shuusi[1][3] = -60; shuusi[1][4] = -93; shuusi[1][5] = -140; shuusi[1][6] = -186; shuusi[1][7] = -266; shuusi[1][8] = -373;
		shuusi[2][0] = -4; shuusi[2][1] = -7; shuusi[2][2] = -13; shuusi[2][3] = -17; shuusi[2][4] = -27; shuusi[2][5] = -40; shuusi[2][6] = -53; shuusi[2][7] = -80; shuusi[2][8] = -107;
		shuusi[3][0] = 15; shuusi[3][1] = 30; shuusi[3][2] = 60; shuusi[3][3] = 78; shuusi[3][4] = 120; shuusi[3][5] = 180; shuusi[3][6] = 240; shuusi[3][7] = 360; shuusi[3][8] = 480;
		shuusi[4][0] = -10; shuusi[4][1] = -20; shuusi[4][2] = -40; shuusi[4][3] = -52; shuusi[4][4] = -80; shuusi[4][5] = -120; shuusi[4][6] = -160; shuusi[4][7] = -240; shuusi[4][8] = -320;
		shuusi[5][0] = -5; shuusi[5][1] = -10; shuusi[5][2] = -20; shuusi[5][3] = -26; shuusi[5][4] = -40; shuusi[5][5] = -60; shuusi[5][6] = -80; shuusi[5][7] = -120; shuusi[5][8] = -160;

		for (loop2 = 1; loop2 <= 1000; loop2++)
		{
			////printf("%d¥n",loop2);

			for (loop1 = 1; loop1 <= 1000; loop1++)
			{
				while (kyoku1<tonnan || (!(10 * tokuten[0]>3 * all || 10 * tokuten[1]>3 * all || 10 * tokuten[2]>3 * all || 10 * tokuten[3]>3 * all) && kyoku1<tonnan + 4))
				{
					for (i = 1; i <= 4; i++)
					{

						k = randam(0);
						k = k & 0xEFFFFFFF;
						k = k % 1000;
						para1 = joutai[k];

						if (para1 <= 2)
						{
							k = randam(0);
							k = k & 0xEFFFFFFF;
							k = k % 1000;
							para2 = hansuu[k];

							if (para1 == 0) { temp = 3 * honba1 + 10 * ri_ti_bou1; flag = 1; }
							else if (para1 == 1) { temp = -3 * honba1; }
							else { temp = -1 * honba1; }
							if (i == 4) { para1 += 3; }
							tokuten[(oya1 + i) % 4] += shuusi[para1][para2] + temp;
							if (i == 4 && flag == 1)
							{
								ri_ti_bou1 = 0;
							}
						}

						else if (para1 == 3)
						{
							tokuten[(oya1 + i) % 4] += 14;
						}
						else if (para1 == 4)
						{
							tokuten[(oya1 + i) % 4] -= 14;
						}
						else
						{
							/*何も無い*/
						}

					}//for(i=1;i<=4;i++)
					if (tokuten[0]<0 || tokuten[1]<0 || tokuten[2]<0 || tokuten[3]<0)
					{
						break;
					}

					if (kyoku1 == tonnan - 1 && para1 == 3)
					{
						temp = 0;
						for (i = 0; i<4; i++)
						{
							if (i != oya1 && tokuten[oya1]<tokuten[i])
							{
								temp = 1;
							}
						}
						if (temp == 0)
						{
							break;
						}
					}
					kyoku1++;
					oya1 = (oya1 + 1) % 4;

					if (para1 == 3)
					{
						honba1++;
						kyoku1--;
						oya1 = (oya1 + 3) % 4;
					}
					else if (para1 == 4) { honba1++; }
					else { honba1 = 0; }
					all = tokuten[0] + tokuten[1] + tokuten[2] + tokuten[3];
				}//while(kyoku1<tonnan)

				temp = 1;
				for (i = 1; i<4; i++)
				{
					if (tokuten[0]<tokuten[i] || (tokuten[0] == tokuten[i] && moto[0]>moto[i]))
					{
						temp++;
					}
				}

				juni[temp - 1]++;
				temp = 1;
				/*//printf("%d %d %d %d¥n",tokuten[0],tokuten[1],tokuten[2],tokuten[3]);*/
				tokuten[0] = jibun;
				tokuten[1] = simotya;
				tokuten[2] = toimen;
				tokuten[3] = kamitya;
				kyoku1 = kyoku;
				oya1 = oya;
				honba1 = honba;
				flag = 0;
				ri_ti_bou1 = ri_ti_bou;
				all = tokuten[0] + tokuten[1] + tokuten[2] + tokuten[3];
			}
		}
		for (i = 1; i <= 4; i++)
		{
			junibunnpu[i] = (double)juni[i - 1] / 1000.0 / 1000.0;
		}

		////printf("%d %d %d %d¥n",juni[0],juni[1],juni[2],juni[3]);

		heikinjuni = (juni[0] + 2 * juni[1] + 3 * juni[2] + 4 * juni[3]) / 1000.0;
		heikinjuni = heikinjuni / 1000.0;
		junibunnpu[0] = heikinjuni;
		////printf("%f¥n",heikinjuni);
		return 0;
	}

	int saisoku_tenpai_ver4(int hai[], int mieteruhai[], int junme, int simulation, int fu_ro)
	{

		int i, j, k = 0, l = 0, m = 14, n = 0, shuntu = 0;
		int loop = 0, main_loop = 0;
		int hai2[34] = { 0 };
		int copy[16][34] = { { 0 } };//13
		int copy2[476][34] = { { 0 } };
		int **sim_tumo;
		int **copy3;
		if (simulation >= 10000) { simulation = 10000; }

		sim_tumo = (int**)malloc(sizeof(int*)*simulation);
		int mati[13] = { 0 };
		int flag;
		int a[4] = { 0 };
		int agatta[14] = { 0 };
		int kitta[14] = { 0 };
		int kouho[14] = { 0 };
		int agarijun[34] = { 0 };
		for (i = 0; i<simulation; i++)
		{
			sim_tumo[i] = (int*)malloc(sizeof(int) * 40);
			if (sim_tumo[i] == NULL)
			{
				exit(1);
			}
		}
		copy3 = (int**)malloc(sizeof(int*) * 6664);
		for (i = 0; i<6664; i++)
		{
			copy3[i] = (int*)malloc(sizeof(int) * 34);
			if (copy3[i] == NULL)
			{
				exit(1);
			}
		}


		m = shantensuu(hai, fu_ro);
		//	//printf("sfg %d",m);
		if (m <= 1)//1?
		{
			for (i = 0; i<simulation; i++)
			{
				for (j = 0; j<34; j++)
				{
					copy[0][j] = mieteruhai[j];
				}

				for (j = 0; j<28 - junme; j++)
				{
					k = tumo(copy[0]);
					sim_tumo[i][j] = k;
					copy[0][sim_tumo[i][j]]++;
				}
			}
			i = 0;
			j = 0;
			k = 0;
			while (i<34)
			{
				loop = hai[i];
				while (loop>0)
				{
					for (j = 0; j<34; j++)
					{
						copy[k][j] = hai[j];
					}
					kitta[k] = i;
					copy[k][i]--;
					k++;
					loop--;
				}
				i++;
			}

			for (i = 0; i<14 - 3 * fu_ro; i++)
			{
				if (shantensuu(copy[i], fu_ro) == 0)
				{

					j = tenpai(copy[i], mati);
					for (m = 0; m<simulation; m++)
					{
						flag = 0;
						for (shuntu = 0; shuntu<28 - junme; shuntu++)
						{
							if (flag)
							{
								shuntu = 27;
							}

							for (l = 0; l<j; l++)
							{
								if (sim_tumo[m][shuntu] == mati[l])
								{
									flag = 1;
								}
							}
						}//for(shuntu=0;shuntu<18;shuntu++)

						if (flag)
						{
							agatta[i]++;
						}
					}//for(m=0;m<1000;m++)
				}//if(shantensuu(copy[i])==0)
			}//for(i=0;i<k;i++)

			j = 0;
			l = 0;
			n = 0;
			for (j = 0; j<14 - 3 * fu_ro; j++)
			{
				l = 0;
				while (l<34)
				{
					i = 0;
					while (i<34)
					{
						copy2[n][i] = copy[j][i];
						i++;
					}
					copy2[n][l % 34]++;
					n++;
					l++;
				}
			}
			j = 0;
			n = 0;
			for (i = 0; i<(14 - 3 * fu_ro) * 34; i++)
			{
				j = 0;
				while (j<34)
				{
					loop = copy2[i][j];
					while (loop>0)
					{
						for (m = 0; m<34; m++)
						{
							copy3[n][m] = copy2[i][m];
						}
						copy3[n][j]--;
						n++;
						loop--;
					}
					j++;
				}
			}
			for (i = 0; i<(14 - 3 * fu_ro)*(14 - 3 * fu_ro) * 34; i++)
			{
				if (shantensuu(copy3[i], fu_ro) == 0)
				{
					for (j = 0; j<34; j++)
					{
						if (hai[j]<copy3[i][j])
						{
							a[0] = j;
							j = 34;
						}
					}
					j = tenpai(copy3[i], mati);

					for (m = 0; m<simulation; m++)
					{
						flag = 0;
						for (shuntu = 0; shuntu<28 - junme; shuntu++)
						{
							if (flag == 0)
							{
								if (sim_tumo[m][shuntu] == a[0])
								{

									flag = 1;
								}
							}
							else if (flag)
							{
								for (l = 0; l<j; l++)
								{
									if (sim_tumo[m][shuntu] == mati[l])
									{
										flag = 2;
									}
								}
							}
							else
							{
								shuntu = 27;
							}
						}//for(shuntu=0;shuntu<18;shuntu++)

						if (flag == 2)
						{
							//if(i%(14-3*fu_ro)!=13)//printf("%d %d ¥n",i%(14-3*fu_ro),kouho[i%(14-3*fu_ro)]);
							kouho[i % (14 - 3 * fu_ro)]++;
						}
					}//for(m=0;m<simulation;m++)
				}//	if(shantensuu(copy3[i])==0)
				if (i % (14 - 3 * fu_ro) == 14 - 3 * fu_ro - 1)
				{
					m = kouho[0];
					flag = 0;
					for (j = 0; j<14 - 3 * fu_ro; j++)
					{
						if (m<kouho[j])
						{
							m = kouho[j];
							flag = j;
						}
					}

					agatta[i / ((14 - 3 * fu_ro) * 34)] += kouho[flag];
					for (j = 0; j<14 - 3 * fu_ro; j++)
					{
						kouho[j] = 0;
					}
				}
			}//	for(i=0;i<n;i++)
			m = agatta[0];
			flag = 0;
			for (i = 0; i<(14 - 3 * fu_ro); i++)
			{
				//printf("%d %d¥n",kitta[i],agatta[i]);
				if (m<agatta[i])
				{////printf("%d¥n",i);
					m = agatta[i];
					flag = i;
				}
				agatta[i] = 0;
			}

			//hai[kitta[flag]]--;
			////printf("%d¥n",kitta[flag]);
			for (i = 0; i<6664; i++)
			{
				free(copy3[i]);
			}
			for (i = 0; i<simulation; i++)
			{
				free(sim_tumo[i]);
			}
			free(copy3);
			free(sim_tumo);
			return kitta[flag];
		}
		else
		{
			m = 0;
			shuntu = 0;
			for (i = 0; i<34; i++)
			{
				if (hai[i] >= 2)
				{
					shuntu++;//トイツの数
				}
			}
			for (i = 0; i<simulation; i++)
			{
				for (j = 0; j <= 34; j++)
				{
					copy[0][j] = mieteruhai[j];
				}
				for (j = 0; j<40; j++)
				{
					k = tumo(copy[0]);
					sim_tumo[i][j] = k;
					////printf("%d¥n",k);
					copy[0][k]++;
				}
			}
			while (m<34)
			{
				if (hai[m]>0)
				{
					hai[m]--;
					for (main_loop = 0; main_loop<simulation; main_loop++)
					{
						for (j = 0; j<34; j++)
						{
							hai2[j] = hai[j];
						}
						i = 0;
						flag = 1;
						while (flag)
						{
							n = shantensuu(hai2, fu_ro);
							if (n == -1)
							{
								flag = 0;
							}
							while (n>-1)
							{
								j = sim_tumo[main_loop][i];
								hai2[j]++;
								agarijun[m]++;
								i++;
								n--;
								if (i == 40 - junme)
								{
									flag = 0;
									break;
								}
								if (m == j && shuntu >= 2)
								{
									////printf("ok");
									hai2[j]--;
								}
							}
						}//while(flag)
					}//for(main_loop=0;mainloop<1;main_loop++)
					hai[m]++;
					m++;
				}//if(hai[m]>0)
				else
				{
					m++;
				}
			}//while(m<34)
			a[0] = 9999999;
			for (i = 0; i<34; i++)
			{
				if (a[0]>agarijun[i] && agarijun[i] != 0)
				{
					a[0] = agarijun[i];
					m = i;
				}
				////printf("%d¥n",agarijun[i]);
				agarijun[i] = 0;
			}
			a[0] = 0;
			//hai[m]--;
			////printf("%d¥n",m);
			for (i = 0; i<6664; i++)
			{
				free(copy3[i]);
			}
			for (i = 0; i<simulation; i++)
			{
				free(sim_tumo[i]);
			}
			free(copy3);
			free(sim_tumo);
			return m;
		}
		return 0;
	}


	void init(string filename)
	{
		ifstream ifs1(filename.c_str());
		if (!ifs1)
		{
			cout << filename << "は開けません" << endl; while (1) {}
		}

		string str;
		vector<string> result;
		shanten_list = (int*)malloc(sizeof(int)*(0x07ffffff));
		if (shanten_list == NULL) { cout << "メモリ足りない" << endl; while (1) {}exit(1); }
		while (getline(ifs1, str))
		{
			boost::split(result, str, boost::is_space());
			if (result[2] == "0") { shanten_list[(toInt(result[0]))] = toInt(result[1]); }
		}
		ifs1.close();
		randam(1);
	}
	string IntToString(int number)
	{
		std::ostringstream ss;
		ss << number;
		return ss.str();
	}
	double string2double(const std::string& str) {
		double rt;
		std::stringstream ss;
		ss << str;
		ss >> rt;
		return rt;
	}

	std::string DoubletoString(double number)
	{
		std::ostringstream ss;
		ss << number;
		return ss.str();
	}

	string LongToString(long long number)
	{
		std::ostringstream ss;
		ss << number;
		return ss.str();
	}


	int get_kind_of_dora(int i)
	{
		if (i<27 && i % 9 != 8)
		{
			return i + 1;
		}
		else if (i<27 && i % 9 == 8)
		{
			return i - 8;
		}
		else if (i == 27) { return 28; }
		else if (i == 28) { return 29; }
		else if (i == 29) { return 30; }
		else if (i == 30) { return 27; }

		else if (i == 31) { return 32; }
		else if (i == 32) { return 33; }
		else if (i == 33) { return 31; }

		return -1;
	}
	int block(int hai[])
	{
		int i, j = 0, k = 0, flag = 0;
		for (i = 0; i<27; i++)
		{
			if (i % 9 == 8)
			{
				flag = 0;
				j += hai[i];
				k += j / 3;
				if (j % 3 != 0) { k++; }
				j = 0;
			}
			else if (hai[i] == 0 && j >= 1)
			{
				if (flag == 0)
				{
					flag = 1;
				}
				else if (flag == 1)
				{
					flag = 0;
					k += j / 3;
					if (j % 3 != 0) { k++; }
					j = 0;
				}
			}
			else if (hai[i]>0)
			{
				flag = 0;
				j += hai[i];
			}
		}
		for (i = 27; i<34; i++)
		{
			if (hai[i]>0)
			{
				k++;
			}
		}
		return k;
	}


	int deagari(int tensa, int sekijun, int oya, int aite_oya, int ri_ti_bou, int honba)//自分または対象のプレイヤが他からロンする
	{
		int honto_tensa = 0;
		int honto_sekijun = sekijun;
		int honto_oya = oya;
		int flag = -1;//1:勝ってる-1:負けてる
		if ((tensa >= 0 && sekijun == 0) || tensa>0)//勝ってるとき
		{
			flag = 1;
			honto_oya = aite_oya;
			honto_sekijun = 1 - honto_sekijun;
			honto_tensa = tensa - 10 * ri_ti_bou - 3 * honba;
		}
		else//負けてるとき
		{
			flag = -1;
			honto_oya = oya;
			honto_tensa = tensa + 10 * ri_ti_bou + 3 * honba;
			honto_tensa *= -1;
		}
		int sa[2][10] = { { 10,13,20,26,39,52,77,80,120,1000 },
		{ 15,20,29,39,58,77,116,120,180,1000 } };//跳満以上は無視

		for (int i = 0; i<10; i++)
		{
			if (honto_tensa<sa[honto_oya][i] || (honto_tensa == sa[honto_oya][i] && honto_sekijun == 0)) { return (i + 1)*flag; }
		}
		return 0;
	}
	int tsumo_agari(int tensa, int sekijun, int oya, int aite_oya, int ri_ti_bou, int honba)//自分または対象のプレイヤがツモ
	{
		int honto_tensa = 0;
		int honto_sekijun = sekijun;
		int kati_oya = oya;
		int make_oya = aite_oya;
		int flag = -1;//1:勝ってる-1:負けてる
		if ((tensa >= 0 && sekijun == 0) || tensa>0)//勝ってるとき
		{
			flag = 1;
			make_oya = aite_oya;
			kati_oya = oya;
			honto_sekijun = 1 - honto_sekijun;
			honto_tensa = tensa - 10 * ri_ti_bou - 4 * honba;
		}
		else//負けてるとき
		{
			flag = -1;
			kati_oya = aite_oya;
			make_oya = oya;
			honto_tensa = tensa + 10 * ri_ti_bou + 4 * honba;
			honto_tensa *= -1;
		}

		int oya_ko = 0;
		if (kati_oya == 0 && make_oya == 0) { oya_ko = 0; }
		else if (kati_oya == 1 && make_oya == 0) { oya_ko = 1; }
		else if (kati_oya == 0 && make_oya == 1) { oya_ko = 2; }

		int sa[3][10] = { { 14,19,25,34,50,65,99,100,150,1000 },
		{ 16,22,30,40,60,78,119,120,180,1000 },
		{ 20,28,40,52,80,108,156,160,240,1000 } };//跳満以上は無視

		for (int i = 0; i<10; i++)
		{
			if (honto_tensa<sa[oya_ko][i] || (honto_tensa == sa[oya_ko][i] && honto_sekijun == 0)) { return (i + 1)*flag; }
		}
		return 0;
	}
	int chokugeki(int tensa, int sekijun, int oya, int aite_oya, int ri_ti_bou, int honba)//自分と対象のプレイヤがロンしあう
	{
		int honto_tensa = 0;
		int honto_sekijun = sekijun;
		int honto_oya = oya;
		int flag = -1;//1:勝ってる-1:負けてる
		if ((tensa >= 0 && sekijun == 0) || tensa>0)//勝ってるとき
		{
			flag = 1;
			honto_oya = aite_oya;
			honto_sekijun = 1 - honto_sekijun;
			honto_tensa = tensa - 10 * ri_ti_bou - 6 * honba;
		}
		else//負けてるとき
		{
			flag = -1;
			honto_oya = oya;
			honto_tensa = tensa + 10 * ri_ti_bou + 6 * honba;
			honto_tensa *= -1;
		}
		int sa[2][10] = { { 20,26,40,52,78,104,154,160,240,1000 },
		{ 30,40,58,78,116,154,232,240,360,1000 } };//跳満以上は無視

		for (int i = 0; i<10; i++)
		{
			if (honto_tensa<sa[honto_oya][i] || (honto_tensa == sa[honto_oya][i] && honto_sekijun == 0)) { return (i + 1)*flag; }
		}
		return 0;
	}
	int waki_tsumo(int tensa, int sekijun, int oya, int aite_oya, int ri_ti_bou, int honba)//脇がツモ
	{
		int honto_tensa = 0;
		int honto_sekijun = sekijun;
		int kati_oya = oya;
		int make_oya = aite_oya;
		int flag = -1;//1:勝ってる-1:負けてる
		if ((tensa >= 0 && sekijun == 0) || tensa>0)//勝ってるとき
		{
			flag = 1;
			kati_oya = oya;
			make_oya = aite_oya;
			honto_sekijun = 1 - honto_sekijun;
		}
		else//負けてるとき
		{
			flag = -1;
			make_oya = oya;
			kati_oya = aite_oya;
			honto_tensa = tensa*(-1);
		}
		if (make_oya == 1) { return 11 * flag; }//負けてるほうが親
												//else if(make_oya==0 && kati_oya==0){return 0;}//どっちも子
		int sa[10] = { 2,3,5,6,10,13,19,20,30,1000 };//跳満以上は無視

		for (int i = 0; i<10; i++)
		{
			if (honto_tensa<sa[i] || (honto_tensa == sa[i] && honto_sekijun == 0)) { return (i + 1)*flag; }
		}
		return 0;
	}
	int ryuukyoku(int tensa, int sekijun)//流局したとき
	{
		int honto_tensa = 0;
		int honto_sekijun = sekijun;
		int flag = -1;//1:勝ってる-1:負けてる

		if ((tensa >= 0 && sekijun == 0) || tensa>0)//勝ってるとき
		{
			flag = 1;
			honto_sekijun = 1 - honto_sekijun;
			honto_tensa = tensa;
		}
		else//負けてるとき
		{
			flag = -1;
			honto_tensa = tensa;
			honto_tensa *= -1;
		}
		int sa[4] = { 0,30,40,1000 };
		for (int i = 0; i<4; i++)
		{
			if (honto_tensa<sa[i] || (honto_tensa == sa[i] && honto_sekijun == 0)) { return (i + 1)*flag; }
		}
		return 0;
	}
	int trance37to34(int i)
	{
		if (i<5) { return i; }

		else if (i<15) { return i - 1; }

		else if (i<25) { return i - 2; }

		else { return i - 3; }

		return i;
	}
	int trance34to37(int i)
	{
		if (i <= 4) {}
		else if (i >= 5 && i <= 13) { return i + 1; }
		else if (i >= 14 && i <= 22) { return i + 2; }
		else { return i + 3; }
		return i;
	}
	int check_nakeru(int hai[], int hai_kind, int dare)
	{
		int i = 0;
		i = hai_kind;
		if (hai_kind == -1) { return 0; }
		if (hai[hai_kind] >= 2)
		{
			return 1;
		}
		if (hai_kind<27 && dare == 3)
		{
			if (hai_kind % 9 == 0 && hai[hai_kind + 1]>0 && hai[hai_kind + 2]>0) { return 1; }
			else if (hai_kind % 9 == 8 && hai[hai_kind - 1]>0 && hai[hai_kind - 2]>0) { return 1; }
			else if (hai_kind % 9 == 1 && ((hai[hai_kind + 1]>0 && hai[hai_kind + 2]>0) || (hai[hai_kind - 1]>0 && hai[hai_kind + 1]>0))) { return 1; }
			else if (hai_kind % 9 == 7 && ((hai[hai_kind - 1]>0 && hai[hai_kind - 2]>0) || (hai[hai_kind - 1]>0 && hai[hai_kind + 1]>0))) { return 1; }
			else if (hai_kind % 9 >= 2 && hai_kind % 9 <= 6 && ((hai[hai_kind - 1]>0 && hai[hai_kind - 2]>0) || (hai[hai_kind - 1]>0 && hai[hai_kind + 1]>0) || (hai[hai_kind + 1]>0 && hai[hai_kind + 2]>0))) { return 1; }
		}
		return 0;

	}
	int hai_kind_string_to_33(string s, int *aka)
	{
		*aka = 0;
		if ((signed)s.size() == 2)
		{
			if (s.substr(1, 1) == "M")
			{
				*aka = 1;
			}
			if (s.substr(1, 1) == "P")
			{
				*aka = 2;
			}
			if (s.substr(1, 1) == "S")
			{
				*aka = 4;
			}

			if (s.substr(1, 1) == "m" || s.substr(1, 1) == "M")
			{
				return  toInt(s.substr(0, 1)) - 1;
			}
			else if (s.substr(1, 1) == "p" || s.substr(1, 1) == "P")
			{
				return  toInt(s.substr(0, 1)) + 8;
			}
			else if (s.substr(1, 1) == "s" || s.substr(1, 1) == "S")
			{
				return  toInt(s.substr(0, 1)) + 17;
			}
		}
		else if ((signed)s.size() == 1)
		{
			if (s == "w") { return 27; }
			else if (s == "x") { return 28; }
			else if (s == "y") { return 29; }
			else if (s == "z") { return 30; }
			else if (s == "h") { return 31; }
			else if (s == "i") { return 32; }
			else if (s == "j") { return 33; }
		}
		return -1;
	}
	int hai_kind_string_to_37(string s, int *aka)
	{
		*aka = 0;
		int i = 0;
		if ((signed)s.size() == 2)
		{
			if (s.substr(1, 1) == "M")
			{
				*aka = 1; i = 1;
			}
			if (s.substr(1, 1) == "P")
			{
				*aka = 2; i = 1;
			}
			if (s.substr(1, 1) == "S")
			{
				*aka = 4; i = 1;
			}
			if (toInt(s.substr(0, 1)) >= 6) { i++; }
			if (s.substr(1, 1) == "m" || s.substr(1, 1) == "M")
			{
				return  toInt(s.substr(0, 1)) - 1 + i;
			}
			else if (s.substr(1, 1) == "p" || s.substr(1, 1) == "P")
			{
				return  toInt(s.substr(0, 1)) + 9 + i;
			}
			else if (s.substr(1, 1) == "s" || s.substr(1, 1) == "S")
			{
				return  toInt(s.substr(0, 1)) + 19 + i;
			}
		}
		else if ((signed)s.size() == 1)
		{
			if (s == "w") { return 30; }
			else if (s == "x") { return 31; }
			else if (s == "y") { return 32; }
			else if (s == "z") { return 33; }
			else if (s == "h") { return 34; }
			else if (s == "i") { return 35; }
			else if (s == "j") { return 36; }
		}
		return -1;
	}

};

#endif//ma_jan