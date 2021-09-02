#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<string>
using namespace std;
#define u16 uint16_t

const int keynum = 4096;

double dat[16][keynum];

void initdata()
{
	for (int i = 0; i<16; i++)
		for (int j = 0; j<keynum; j++)
			dat[i][j] = 0.0;
}
int main() {
	for (int i = 0; i < 16; i++) {
		ifstream fin;
		string filename = to_string(i);
		string houzui = ".txt";
		filename += houzui;
		fin.open(filename);
		u16 K[4];
		initdata();
		for (int j = 0; j < keynum; j++) {
			fin >> K[0];
			fin >> K[1];
			fin >> K[2];
			fin >> K[3];
			int site;
			for (int k = 0; k < 16; k++ ) {
				fin >> site;
				fin >> dat[k][j];
			}
		}
		fin.close();
		cout << "load"<<filename<<"finished" << endl;
		int maxtime[16];
		for (int j = 0; j<16; j++) maxtime[j] = 0;
		for (int i = 0; i<keynum; i++)
		{
			double maxbias_key = -100.0;
			int maxsite_key = -1;
			for (int j = 0; j<16; j++)
				if (dat[j][i]>maxbias_key)
				{
					maxbias_key = dat[j][i];
					maxsite_key = j;
				}
			maxtime[maxsite_key]++;
		}
		for (int j = 0; j<16; j++)
		{
			if (maxtime[j] == 0) continue;
			double sum = 0.0;
			double maxbias_j = -100.0;
			double minbias_j = 0.0;
			for (int i = 0; i<keynum; i++)
			{
				sum = sum + dat[j][i];
				if (dat[j][i]>maxbias_j) maxbias_j = dat[j][i];
				if (dat[j][i]<minbias_j) minbias_j = dat[j][i];
			}
			printf("j:%d times:%d average:%.2f max:%.2f min:%.2f\n", j, maxtime[j], sum / keynum, maxbias_j, minbias_j);
		}
	}
	system("pause");
	return 0;
}
