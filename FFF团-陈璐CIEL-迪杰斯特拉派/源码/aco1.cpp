#include <stdlib.h> 
#include <stdio.h> 
#include <time.h> 
#include <iostream>
#include <string>
using namespace std;
#define ro 0.3  //信息素局部更新时，乘以的系数
#define initInfo 0.2  //初始化信息素，都为常数

struct MaYi   //蚂蚁数据结构
{
	int road[18];  //禁忌表，记录蚂蚁经过的节点
	int l;  //记录蚂蚁当前所在的节点在禁忌表中的序号
	int len;  //存储蚂蚁所走路径的花费
	int life;  //life=0表示蚂蚁无路可走；life=2表示蚂蚁顺利从起点走到了终点，或者从终点走到了起点；life=1表示蚂蚁还在路上
	int flag[18];  //flag[i]=1表示蚂蚁经过了编号为i的节点，若没有经过该节点则flag=0
};

int main(void)
{
	int mn = 0, key = 0;  //mn:蚂蚁编号
	int direction, opti;
	double total = 0, sd[18] = { 0 };
	int bushu;
	static int count = 0;
	int i, i1, j, j1, k, k1, l, mr, minr[18], minroad[18], m = 100;  //minr记录找到最优路径的蚂蚁所在最优路径上包含的节点，minroad记录最优路径的各个节点
	static int minl, mini, minlen;  //mini记录当前最优路径包含的节点数
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
	int Ant[18][7];//Ant[i][]第一个到达节点i的蚂蚁的信息（蚂蚁编号，节点i是此蚂蚁所走路径上的第几个节点，此时的花费）。既包括从起点出发的蚂蚁，又包括从终点出发的蚂蚁
	int b1, b2;
	int f1, f2;
	int find = 0;
	MaYi mayi[200];  //蚂蚁数量
	double P[18][18];  //每个节点的信息素
	time_t t;
	while (key != 1)
	{
		//--------------------初始化信息素--------------------
		cout<<"-------------地图-------------"<<endl;
		for (i = 0; i<18; i++)
		{
			for (j = 0; j<18; j++)
			{
				printf("%d ", M[i][j]);
				P[i][j] = initInfo;

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

		//////////////////////////////////////////
		//---------------蚂蚁初始化---------------
		b1 = 0;  //起点编号
		b2 = 17;  //终点编号	

		for (k = 0; k<2 * m; k++)
		{
			for (i = 0; i<18; i++)  
			{
				mayi[k].flag[i] = 0;
			}
			if (k<m)  //100只蚂蚁从起点出发
			{
				mayi[k].road[0] = b1;
				mayi[k].flag[b1] = 1;
			}
			else   //100只蚂蚁从终点出发
			{
				mayi[k].road[0] = b2;
				mayi[k].flag[b2] = 1;
			}
			mayi[k].l = 0;
			mayi[k].len = 0;
			mayi[k].life = 1;

		}

		//////////////////////////////////////


		minl = 1000;  //minl：路径经过的节点数
		minlen = 1000;  //minlen：路径的花费

		int x = 10;
		int y = 5;
		for (opti = 0; opti<500; opti++)  //迭代500次
		{
			mini = minl;

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
						i = mayi[mn].road[mayi[mn].l];  //i:蚂蚁mn当前所在的节点编号
						total = 0;
						for (k = 0; k < 18; k++)  //sd：信息启发因子，反映了路径上的信息素对蚂蚁的选择作用强度
							sd[k] = 0;
						for (k = 0; k < 18; k++)  //看邻接矩阵的第i行
						{
							if (M[i][k] == 0)  //如果节点i与节点k没有直达，则设置信息素为0
							{
								P[i][k] = 0;
							}
							else if (mayi[mn].flag[k] == 0)  //节点i与节点k直达，且之前没有经过过节点k，则把k的信息素加到total
							{
								total += P[i][k];
								sd[k] = total;  //节点k的sd
							}
						}
						
						direction = rand() % 160;  //direction：方向启发因子，表明启发信息对蚂蚁选择下一步时的影响程度
						for (k = 0; k < 18; k++)
						if (direction < 160 * sd[k] / total)
						{
							mayi[mn].l++;
							mayi[mn].len += M[i][k];  
							mayi[mn].road[mayi[mn].l] = k;

							i = k;
							mayi[mn].flag[i] = 1;

							if (Ant[i][0] == 0)  //Ant[i][0]=0，则认为是从终点出发的蚂蚁还没有到达节点i
							{
								Ant[i][0] = mn;  //Ant[i][0]：第一个到达节点i，且从起点出发的蚂蚁的编号
								Ant[i][3] = mayi[mn].l;  //Ant[i][3]：节点i在蚂蚁mn的禁忌表中的位置
								Ant[i][5] = mayi[mn].len;  //Ant[i][5]：蚂蚁mn到达节点i时已经用了的花费
							}
							if (Ant[i][1] != 0) //Ant[i][1]！=0，则已经有从终点出发的蚂蚁到达过节点i，此时将Ant[i][2]设为1
								Ant[i][2] = 1;  
							break;
						}
						if (i == b2)  //从起点出发的蚂蚁到达了终点
						{
							mayi[mn].life = 2;
						}
						if (k == 18)  //k=18，说明蚂蚁无路可走
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
						i1 = mayi[mn].road[mayi[mn].l];  //i1:蚂蚁mn当前所在的节点编号
						total = 0;
						for (k1 = 0; k1 < 18; k1++) //sd：信息启发式因子，反映了路径上的信息素对蚂蚁的选择作用强度
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

							i1 = k1;
							mayi[mn].flag[i1] = 1;

							if (Ant[i1][1] == 0)  //Ant[i1][1]=0，则认为是从起点出发的蚂蚁还没有到达节点i1
							{
								Ant[i1][1] = mn;  //Ant[i1][1]：第一个到达节点i1，且从终点出发的蚂蚁的编号
								Ant[i1][4] = mayi[mn].l;  //Ant[i1][4]：节点i1在蚂蚁mn的禁忌表中的位置
								Ant[i1][6] = mayi[mn].len;  //Ant[i1][6]：蚂蚁mn到达节点i1时已经用了的花费
							}
							if (Ant[i1][0] != 0) //Ant[i1][0]！=0，则已经有从起点出发的蚂蚁到达过节点i1，此时将Ant[i1][2]设为1
								Ant[i1][2] = 1;  //以上，Ant[i][2]=1表示，不论是从起点出发的蚂蚁，还是从终点出发的蚂蚁都能达到节点i
							break;
						}
						if (i1 == b1)  //从终点出发的蚂蚁到达了起点
						{
							mayi[mn].life = 2;

						} 
						if (k == 18)  //k=18，说明蚂蚁无路可走
						{
							mayi[mn].life = 0;

						}
					}
				}
			}

			////////////因为相遇找到最优解//////////////////////////////
			for (i = 0; i < 18; i++){
				if (Ant[i][2] == 1)  //Ant[i][2] == 1，表示节点i被访问过
				{
					f1 = 0;
					f2 = 0;
					
					//minr记录相遇的两只蚂蚁从出发到相遇所经过的节点
					for (j = 0; j < Ant[i][3]; j++)  //minr[j]记录第一个经过节点i的，且从起点出发的蚂蚁在经过节点i时走过的每个节点（包括节点i）
						minr[j] = mayi[Ant[i][0]].road[j];
					for (k = Ant[i][3]; k < Ant[i][3] + Ant[i][4] + 1; k++)  //minr[k]记录第一个经过节点i的，且从终点出发的蚂蚁在经过节点i前走过的每个节点（不包括节点i）
						minr[k] = mayi[Ant[i][1]].road[Ant[i][4] - k + Ant[i][3]];
					
					for (l = 0; l < (Ant[i][3] + Ant[i][4] + 1); l++){  //检验相遇路径中是否包含必须经过的节点和线段
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

					//注：这里只有Ant[i][3] + Ant[i][4] < minl的判断，即只比较了当次迭代的节点数与全局最优的节点数，并没有考虑权重
					if (count >= 4 && ( Ant[i][3] + Ant[i][4] < minl)){  //只有count>=4，说明经过了所有必须经过的节点和线段
						minl = Ant[i][3] + Ant[i][4];  //路径经过的节点数
						minlen = Ant[i][5] + Ant[i][6];  //路径长度
						for (mr = 0; mr < minl + 1; mr++){
							minroad[mr] = minr[mr];
						}
						count = 0;
						find = 1;  //find=1表示找到了这一轮迭代的最短路径
					}
					else{
						count = 0;
					}
				}
			}
			/////////////头部蚂蚁找到最优解/////////////////////
			//-------------从起点出发的蚂蚁找到最优解----------------------
			for (mn = 0; mn<100; mn++)
			{
				if ((mayi[mn].life != 0) && (mayi[mn].life != 1))  //即life=2，说明从起点出发的蚂蚁顺利到达终点
				{
					f1 = 0;
					f2 = 0;
					for (i = 0; i <= mayi[mn].l+1; i++)  //最优路径包含的节点
						minr[i] = mayi[mn].road[i];
					for (j = 0; j < mayi[mn].l+1; j++){  //判断是否经过了必须经过的点和线段
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
					if (count >= 4 && (mayi[mn].l < minl)){
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
			//-------------从终点出发的蚂蚁找到最优解----------------------
			for (mn = m; mn<m + 100; mn++)
			{
				if ((mayi[mn].life != 0) && (mayi[mn].life != 1))  //即life=2，说明从终点出发的蚂蚁顺利到达起点
				{
					f1 = 0;
					f2 = 0;
					for (i = mayi[mn].l+1; i >= 0; i--)  //最优路径包含的节点
						minr[i] = mayi[mn].road[i];
					for (j = 0; j < mayi[mn].l+1; j++){   //判断是否经过了必须经过的点和线段
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
							f2 = 0;
						}
					}
					if (count >= 4 && (mayi[mn].l < minl)){
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
						P[i][j] = ro*P[i][j];    //信息素P[i][j]的局部更新，自乘ro
				for (k = 0; k<minl; k++)
				{
					i = minroad[k];
					P[k][i] += 1 / double(minl);     //信息素P[i][j]的全局更新
				}
			}

			//重新初始化“struct MaYi”
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
		cout<<"-----------------  结果1:经过最少的储物间，达到终点--------------------"<<endl;
		cout<<"经过的储物间数量为："<<minl+1<<endl;
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
				//cout << " " << minroad[i];
			}
		}
		for(i=0;i<=minl;i++)
			cout<<" "<<path[i];
		cout << endl;
	
		system("pause");
		return 0;
	}
}

