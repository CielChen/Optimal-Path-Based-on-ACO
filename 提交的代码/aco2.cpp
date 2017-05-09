#include <stdlib.h> 
#include <stdio.h> 
#include <time.h> 
#include <iostream>
#include <string>
using namespace std;
#define ro 0.25  //信息素局部更新时，乘以的系数
#define initInfo 0.2  //初始化信息素，都为常数

struct MaYi   //蚂蚁数据结构
{
	int road[18];  
	int l;
	int len;
	int life;
	int flag[18];
};


int main(void)
{
	int mn = 0, key = 0;
	int direction, opti;
	double total = 0, sd[18] = { 0 };
	int bushu;
	static int count = 0;
	int i, i1, j, j1, k, k1, l, mr, minr[18], minroad[18], m = 100;
	static int minl, mini, minlen;
	int M[18][18] = { 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	                  3, 0, 1, 0, 1, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0,
					  1, 1, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					  1, 0, 1, 0, 0, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					  0, 1, 2, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
					  0, 0, 1, 2, 1, 0, 1, 0, 0, 3, 1, 0, 3, 0, 0, 0, 0, 0,
					  0, 0, 0, 2, 0, 1, 0, 1, 2, 0, 0, 0, 2, 4, 3, 0, 0, 0,
					  0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					  0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 1, 3, 0, 0,
					  0, 4, 0, 0, 1, 3, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
					  0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 2, 0, 0, 0, 0, 0,
					  0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0,                     //11、12直接写成不可达
					  0, 0, 0, 0, 0, 3, 2, 0, 0, 0, 2, 0, 0, 2, 0, 0, 1, 0,
					  0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 2, 0, 1, 2, 2, 1,
					  0, 0, 0, 0, 0, 0, 3, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0,
					  0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2, 1, 0, 0, 4,
					  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 0, 0, 0, 1,
					  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 4, 1, 0 };
	int Ant[18][7];/////到达局部最短路径的蚂蚁
	int b1, b2;
	int f1, f2;
	int find = 0;
	MaYi mayi[200];
	double P[18][18];
	time_t t;
	while (key != 1)
	{
		///////////////////////////////
		for (i = 0; i<18; i++)
		{
			for (j = 0; j<18; j++)
			{
				printf("%d ", M[i][j]);
				P[i][j] = 0.3;

			}
			printf("\n");
		}
		printf("\n");
		/*//初始化信息素：开始时想把必经点和线段的信息素加大，但效果不好
		for (i = 0; i < 18; i++){
			P[i][2] = 0.3;
			P[i][4] = 0.3;
			P[i][7] = 0.3;
			P[i][12] = 0.3;
			P[i][13] = 0.3;
			P[i][14] = 0.3;
		}
		P[2][4] = 0.5;
		P[4][2] = 0.5;
		P[13][14] = 0.5;
		P[14][13] = 0.5;*/

		//////////////////////////////////////
		///////蚂蚁初始化/////////////////////
		//////////////////////////////////////
		b1 = 0;
		b2 = 17;

		for (k = 0; k<2 * m; k++)
		{
			for (i = 0; i<18; i++)
			{
				mayi[k].flag[i] = 0;
			}
			if (k<m)
			{
				mayi[k].road[0] = b1;
				mayi[k].flag[b1] = 1;
			}
			else
			{
				mayi[k].road[0] = b2;
				mayi[k].flag[b2] = 1;
			}
			mayi[k].l = 0;
			mayi[k].len = 0;
			mayi[k].life = 1;

		}

		//////////////////////////////////////


		minl = 1000;
		minlen = 1000;

		int x = 10;
		int y = 5;
		for (opti = 0; opti<1000; opti++)
		{
			mini = minlen;

			i = b1;
			i1 = b2;
			for (i = 0; i<18; i++)
			for (k = 0; k<7; k++)
				Ant[i][k] = 0;

			for (bushu = 0; bushu < 50; bushu++)
			{
				////////////////////////
				///头蚂蚁1/////////////
				///////////////////////

				//----------------------从起点出发的蚂蚁----------------------
				for (mn = 0; mn < 100; mn++)
				{
					if (mayi[mn].life == 1)
					{
						i = mayi[mn].road[mayi[mn].l];
						total = 0;
						for (k = 0; k < 18; k++)
							sd[k] = 0;
						for (k = 0; k < 18; k++)
						{
							if (M[i][k] == 0)
							{
								P[i][k] = 0;
							}
							else if (mayi[mn].flag[k] == 0)
							{
								total += P[i][k];
								sd[k] = total;
							}
						}
						
						direction = rand() % 160;
						for (k = 0; k < 18; k++)
						if (direction < 160 * sd[k] / total)
						{
							mayi[mn].l++;
							mayi[mn].len += M[i][k];
							mayi[mn].road[mayi[mn].l] = k;

							//允许折返，但出于花费的考虑，只能折返一次
							if (k == 7)
								mayi[mn].flag[i] = 0;
							if (k == 12)
								mayi[mn].flag[i] = 0;
							if ((k == 2 && i == 4) || (i == 2 && k == 4))
								mayi[mn].flag[i] = 0;
							if ((k == 13 && i == 14) || (i == 13 && k == 14))
								mayi[mn].flag[i] = 0;

							i = k;
							mayi[mn].flag[i] = 1;

							if (Ant[i][0] == 0)
							{
								Ant[i][0] = mn;
								Ant[i][3] = mayi[mn].l;
								Ant[i][5] = mayi[mn].len;
							}
							if (Ant[i][1] != 0) Ant[i][2] = 1;
							break;
						}
						if (i == b2)
						{
							mayi[mn].life = 2;
						}
						if (k == 18)
						{
							mayi[mn].life = 0;
						}

					}
				}

			


				/////底部蚂蚁2////////
				//----------------------从终点出发的蚂蚁----------------------
				for (mn = m; mn < m + 100; mn++)
				{
					if (mayi[mn].life == 1)
					{
						i1 = mayi[mn].road[mayi[mn].l];
						total = 0;
						for (k1 = 0; k1 < 18; k1++)
							sd[k1] = 0;

						for (k1 = 0; k1 < 18; k1++)
						{
							if (M[i1][k1] == 0)
							{
								P[i1][k1] = 0;
							}
							else if (mayi[mn].flag[k1] == 0)
							{
								total += P[i1][k1];
								sd[k1] = total;

							}
						}

						direction = rand() % 160;
						for (k1 = 0; k1 < 18; k1++)
						if (direction < 160 * sd[k1] / total)
						{

							mayi[mn].l++;
							mayi[mn].len += M[i1][k1];
							mayi[mn].road[mayi[mn].l] = k1;

							//允许折返，但出于花费的考虑，只能折返一次
							if (k1 == 7)
								mayi[mn].flag[i1] = 0;
							if (k1 == 12)
								mayi[mn].flag[i1] = 0;
							if ((k1 == 2 && i1 == 4) || (i1 == 2 && k1 == 4))
								mayi[mn].flag[i1] = 0;
							if ((k1 == 13 && i1 == 14) || (i1 == 13 && k1 == 14))
								mayi[mn].flag[i1] = 0;

							i1 = k1;
							mayi[mn].flag[i1] = 1;

							if (Ant[i1][1] == 0)
							{
								Ant[i1][1] = mn;
								Ant[i1][4] = mayi[mn].l;
								Ant[i1][6] = mayi[mn].len;
							}
							if (Ant[i1][0] != 0) Ant[i1][2] = 1;
							break;
						}
						if (i1 == b1)
						{
							mayi[mn].life = 2;

						}
						if (k == 18)
						{
							mayi[mn].life = 0;

						}
					}
				}
			}

			////////////因为相遇找到最优解//////////////////////////////
			for (i = 0; i < 18; i++){
				if (Ant[i][2] == 1)
				{
					f1 = 0;
					f2 = 0;
					for (j = 0; j < Ant[i][3]; j++)
						minr[j] = mayi[Ant[i][0]].road[j];
					for (k = Ant[i][3]; k < Ant[i][3] + Ant[i][4] + 1; k++)
						minr[k] = mayi[Ant[i][1]].road[Ant[i][4] - k + Ant[i][3]];
					for (l = 0; l < (Ant[i][3] + Ant[i][4] + 1); l++){
						if (minr[l] == 7)
							count++;
						if (minr[l] == 12)
							count++;
						if (((minr[l] == 2 && minr[l - 1] == 4) || (minr[l] == 2 && minr[l + 1] == 4)) && f1 == 0){
							count++;
							f1 = 1;
						}
						if (((minr[l] == 13 && minr[l - 1] == 14) || (minr[l] == 13 && minr[l + 1] == 14)) && f2 == 0){
							count++;
							f2 = 1;
						}
					}
					//注：与aco1.cpp相比，这里还加入了Ant[i][5] + Ant[i][6]) < minlen，不仅节点数，也考虑权重
					if (count >= 4 && ((((Ant[i][5] + Ant[i][6]) < minlen)) || ((Ant[i][5] + Ant[i][6]) == minlen && Ant[i][3] + Ant[i][4] < minl))){
						minl = Ant[i][3] + Ant[i][4];
						minlen = Ant[i][5] + Ant[i][6];
						for (mr = 0; mr < minl + 1; mr++){
							minroad[mr] = minr[mr];
						}
						count = 0;
						find = 1;
					}
					else{
						count = 0;
					}
				}
			}
			/////////////头部蚂蚁找到最优解/////////////////////
			for (mn = 0; mn<100; mn++)
			{
				if ((mayi[mn].life != 0) && (mayi[mn].life != 1))
				{
					f1 = 0;
					f2 = 0;
					for (i = 0; i <= mayi[mn].l+1; i++)
						minr[i] = mayi[mn].road[i];
					for (j = 0; j < mayi[mn].l+1; j++){
						if (minr[j] == 7)
							count++;
						if (minr[j] == 12)
							count++;
						if (((minr[j] == 2 && minr[j - 1] == 4) || (minr[j] == 2 && minr[j + 1] == 4)) && f1 == 0){
							count++;
							f1 = 1;
						}
						if (((minr[j] == 13 && minr[j - 1] == 14) || (minr[j] == 13 && minr[j + 1] == 14)) && f2 == 0){
							count++;
							f2 = 1;
						}
					}
					if (count >= 4 && ((mayi[mn].len < minlen) || (mayi[mn].len == minlen && mayi[mn].l < minl))){
						minl = mayi[mn].l;
						minlen = mayi[mn].len; k = mn;
						for (mr = 0; mr < minl + 1; mr++){
							minroad[mr] = minr[mr];
						}
						count = 0;
						find = 1;
					}
					else{
						count = 0;
					}
				}
			}

			////////////底部蚂蚁找到最优解////////////////////////// 
			for (mn = m; mn<m + 100; mn++)
			{
				if ((mayi[mn].life != 0) && (mayi[mn].life != 1))
				{
					f1 = 0;
					f2 = 0;
					for (i = mayi[mn].l+1; i >= 0; i--)
						minr[i] = mayi[mn].road[i];
					for (j = 0; j < mayi[mn].l+1; j++){
						if (minr[j] == 7)
							count++;
						if (minr[j] == 12)
							count++;
						if (((minr[j] == 2 && minr[j - 1] == 4) || (minr[j] == 2 && minr[j + 1] == 4)) && f1 == 0){
							count++;
							f1 = 1;
						}
						if (((minr[j] == 13 && minr[j - 1] == 14) || (minr[j] == 13 && minr[j + 1] == 14)) && f2 == 0){
							count++;
							f2 = 1;
						}
					}
					if (count >= 4 && ((mayi[mn].len < minlen) || (mayi[mn].len == minlen && mayi[mn].l < minl))){
						minl = mayi[mn].l;
						minlen = mayi[mn].len; k = mn;
						for (mr = 0; mr < minl + 1; mr++){
							minroad[mr] = minr[minl - mr];
						}
						count = 0;
						find = 1;
					}
					else{
						count = 0;
					}
				}
			}

			////////////////////重新分配信息素//////////////////
			if (minl<mini)                                           //找到局部最优解
			{
				for (i = 0; i<18; i++)
				for (j = 0; j<18; j++)
					P[i][j] = ro*P[i][j];                         //P[i][j]重新分配值，自乘0.2
				for (k = 0; k<minl; k++)
				{
					i = minroad[k];
					P[k][i] += 1 / double(minl);                        //P[i][j]自加minl倒数调参
				}
				/*if (x > 0){
					for (i = 0; i < 18; i++){
						for (j = 0; j < 18; j++){
							printf("%0.1lf ", P[i][j]);
						}
						cout << endl;
					}
					cout << endl;
					x--;
				}*/
			}

			for (k = 0; k<2 * m; k++)
			{
				for (i = 0; i<18; i++)
				{
					mayi[k].flag[i] = 0;
				}
				if (k<m)
				{
					mayi[k].road[0] = b1;
					mayi[k].flag[b1] = 1;
				}
				else
				{
					mayi[k].road[0] = b2;
					mayi[k].flag[b2] = 1;
				}
				mayi[k].l = 0;
				mayi[k].len = 0;
				mayi[k].life = 1;

			}

		}


		////////////////////////////////////////////////////////
		cout<<"-----------------  结果2:用最少的花费，达到终点--------------------"<<endl;
		cout<<"经过的储物间数量为："<<minl+1<<endl;
		cout<<"路径花费为："<<minlen<<endl;
		cout<<"路径为：";
		//cout << "步长为:" << minl << endl;
		//cout << "路径为:";
		string path[1000];
		if (minl<1000)
		{
			for (i = 0; i <= minl; i++)
			{
				if(minroad[i]==0)
					path[i]="S";
				else if(minroad[i]==17)
					path[i]="E";
				else
					path[i]="N"+to_string(minroad[i]);
			}
		}
		for(i=0;i<=minl;i++)
			cout<<" "<<path[i];
		cout << endl;
	
		system("pause");
		return 0;
	}
}

