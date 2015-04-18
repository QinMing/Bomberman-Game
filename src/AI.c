/*===================�ļ�AI.c=======================
���ļ�Ϊ��Ϸ���˹��������棬��GamePlay()�������ñ��ļ��е�AI_OP()�ļ�ʵ��Ƕ�� 
������������3�ֲ�ͬ�Ѷȵ�AI����ȫ�ֱ���gamelevel�������ɹ��������ѡ�� 
----------------------------------------------------*/ 

static char q[15001][2]; /*����˳��ʹ�ÿ�������������ö�����Ϊ�洢���ݽṹ*/
static char pt[1000];  /*��¼����Ŀ���·��*/ 
static int open,closed,pth;  /*���׶�βָ���·���ĳ���*/
static const int s[2][4]={{-1,1,0,0},{0,0,-1,1}};  /*�����ߵĲ���Ҳ�������������ĸ�����*/
struct pos { 
	int x,y;
} p1,cur,goal; /*��¼������ǰλ�õı���*/
static int Location[2],nx,ny,nx1,ny1;  /*AIĿǰ�ڵ�ͼ��λ�ú���һ���λ��*/

/*=====================================================
����ԭ�ͣ�int danger(int cx,int cy,int t);
���ܣ��жϱ��Ϊt��ը����(cx,cy)���Ƿ�����в 
��ڣ�AI�ĸ������(cx,cy)����ը�����t 
���ڣ�����0��1��0��ʾ����в��1��ʾ����в 
������ϵ���˺�����AI_Danger�������ã�Ϊ���ж�������ֻ��AI�Ѷ�ΪCrazyʱ��ʹ�� 
-------------------------------------------------------*/ 
int danger(int cx,int cy,int t)  
{
	int deltax,deltay,whs,pw;
	deltax=abs(cx-bombi[t]);
	deltay=abs(cy-bombj[t]);
	whs=whosebomb[t];
	pw=bomb_power[whs];
	if ((deltax==0&&deltay<=pw)||(deltax<=pw&&deltay==0)||(deltax==0&&deltay==0))return 1;
	return 0;
}

/*=====================================================
����ԭ�ͣ�int potential(int cx,int cy,int j,int t)�� 
���ܣ��ж������(cx,cy)��ͬ�л�ͬ�е�ը���Ƿ���Ǳ����в 
��ڣ�AI�ĸ������(cx,cy)��ը�����кŻ��к�j��ͬ��tȡ0��ͬ��tȡ1 
���ڣ�����0��1��0��ʾ���ը������Ǳ����в��1��ʾ ��Ǳ����в 
������ϵ���˺�����AI_Danger�������ã�Ϊ���ж�������ֻ��AI�Ѷ�ΪCrazyʱ��ʹ�� 
-------------------------------------------------------*/
int potential(int cx,int cy,int j,int t)  
{
	int i;
	if (t==0) {
		if (j>=cy)
			for (i=cy+1;i<j;i++) if (a[i][cx]!=0&&a[i][cx]!=5) return 0;
			else for (i=j+1;i<cy;i++) if (a[i][cx]!=0&&a[i][cx]!=5) return 0;
	}
	if (t==1) {
		if (j>=cx)
			for (i=cx+1;i<j;i++) if (a[cy][i]!=0&&a[cy][i]!=5) return 0;
			else for (i=j+1;i<cx;i++) if (a[cy][i]!=0&&a[cy][i]!=5) return 0;
	}
	return 1;
} 

/*=====================================================
����ԭ�ͣ�int AI_danger(int cx,int cy)�� 
���ܣ��ж������(cx,cy)���Ƿ���Σ�� 
��ڣ�AI�ĸ������(cx,cy)
���ڣ�����0��1��0��ʾAI�ڸõ���Σ�գ�1��ʾ��Σ�� 
������ϵ���˺�����AI_Search()�������ã������ж�����·���Ƿ�ȫ 
-------------------------------------------------------*/
int AI_danger(int cx,int cy)   
{
	int i,j,zds;
	short bombx[16],bomby[16];
	zds=-1;
	if (gamelevel<2) {
		for (j=0;j<15;j++) if (a[j][cx]==5) return 1;
		for (j=0;j<15;j++) if (a[cy][j]==5) return 1;   
	}
	if (gamelevel==2) {
		for (j=0;j<15;j++) if (a[j][cx]==5&&potential(cx,cy,j,0)) {zds++; bombx[zds]=cx; bomby[zds]=j;}
		for (j=0;j<15;j++) if (a[cy][j]==5&&potential(cx,cy,j,1)) {zds++; bombx[zds]=j; bomby[zds]=cy;}
	}
	j=0;
	while (j<=zds) {
		for (i=0;i<MAXBOMB;i++) 
			if (bombi[i]==bombx[j]&&bombj[i]==bomby[j]) if (danger(cx,cy,i)) return 1;
		j++;
	}
	return 0;
}

/*=====================================================
����ԭ�ͣ�int cov(int x,int y)�� 
���ܣ��ж�(x,y)���Ƿ���Ե��� 
��ڣ�����(x,y)
���ڣ�����0��1��0��ʾ�޷����1��ʾ���Ե��� 
������ϵ���˺�����AI_OP()�������ã������жϵ�ǰҪ�ߵ���һ���Ƿ�����ߵ���
Ҳ����˵���ж���������·���Ƿ�ͨ��Ҳ����û�б���������ĳЩ�¶��� 
-------------------------------------------------------*/
int cov(int x,int y)    
{
	if (a[y][x]==7||a[y][x]==0||a[y][x]>=30) return 1;
	return 0;
}

/*=====================================================
����ԭ�ͣ�int AI_Search(int t)�� 
���ܣ�����һ������ָ��Ŀ���·��������ֵΪ0��ʾ��Χû��Ҫ�ҵĶ�����1Ϊ�ҵ��ˣ����һ᷵��һ������õ��·����Ϣ��pt[]������ 
��ڣ�t=0Ϊը����1Ϊ���֣�2Ϊ���ߣ�3Ϊש�飬4Ϊ��ܵ�,5Ϊ�жϸõ����ը�����Ƿ���Զ�� 
���ڣ�����0��1��0��ʾû��������·����1��ʾ����������·�� �����Ұ�·������pt[]��
������ϵ���˺�����AI_OP()�������ã���������ָ����Ŀ�����Ծ��� 
-------------------------------------------------------*/
int AI_Search(int t)  
{
	int i,j,k;
	char ok[MAP_SIZE][MAP_SIZE]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},   /*�жϵ�ͼ�ϵĵ��Ƿ񱻷��ʹ�*/
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},  
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	short fat[MAP_SIZE][MAP_SIZE]={{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},  
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}, /*��¼·�����߷���0��1��2��3���߲���Ϊ-1*/
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},   
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},  
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},  
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};

	cur.x=Location[0]; cur.y=Location[1];   /*�����ûʲô��˵�ģ�����һ��������������㷨*/ 
	p1.x=q[0][0]=cur.x; p1.y=q[0][1]=cur.y;
	open=-1; closed=0; pth=-1; 
	while (open!=closed)
	{
		open=(open+1)%15000;
		cur.x=q[open][0]; cur.y=q[open][1]; ok[cur.y][cur.x]=1;
		switch (t) {
		case 0:return AI_danger(cur.x,cur.y);
		case 1:
			if (a[cur.y][cur.x]==8) {
			goal.x=cur.x; goal.y=cur.y;
			while (cur.x!=Location[0]||cur.y!=Location[1])
			{
				pth++; pt[pth]=fat[cur.y][cur.x];
				cur.x=cur.x-s[0][pt[pth]];
				cur.y=cur.y-s[1][pt[pth]];
			}
			return 1;
			   }
			   for (i=0;i<4;i++)
				   if ((a[cur.y+s[1][i]][cur.x+s[0][i]]==0||a[cur.y+s[1][i]][cur.x+s[0][i]]==8||a[cur.y+s[1][i]][cur.x+s[0][i]]==7)&&(!ok[cur.y+s[1][i]][cur.x+s[0][i]])&&(!(AI_danger(cur.x+s[0][i],cur.y+s[1][i]))))
				   {
					   if ((closed+1)%15000!=open){
						   closed=(closed+1)%15000;
						   q[closed][0]=cur.x+s[0][i]; 
						   q[closed][1]=cur.y+s[1][i]; 
						   fat[q[closed][1]][q[closed][0]]=i;
					   }
				   } break;
		case 2: 
			if (a[cur.y][cur.x]>=30&&a[cur.y][cur.x]<=43) {
			goal.x=cur.x; goal.y=cur.y;
			while (cur.x!=Location[0]||cur.y!=Location[1])
			{
				pth++; pt[pth]=fat[cur.y][cur.x];
				cur.x=cur.x-s[0][pt[pth]];
				cur.y=cur.y-s[1][pt[pth]];
			}
			return 1;
				}
				for (i=0;i<4;i++)
					if ((!ok[cur.y+s[1][i]][cur.x+s[0][i]])&&(!(AI_danger(cur.x+s[0][i],cur.y+s[1][i])))) 
						if (a[cur.y+s[1][i]][cur.x+s[0][i]]==0||(a[cur.y+s[1][i]][cur.x+s[0][i]]>=30&&a[cur.y+s[1][i]][cur.x+s[0][i]]<=43)||a[cur.y+s[1][i]][cur.x+s[0][i]]==7) {
							if ((closed+1)%15000!=open){
								closed=(closed+1)%15000;
								q[closed][0]=cur.x+s[0][i]; 
								q[closed][1]=cur.y+s[1][i]; 
								fat[q[closed][1]][q[closed][0]]=i;
							}
						}

						break;
		case 3:
			if (a[cur.y][cur.x]==1) {
			goal.x=cur.x; goal.y=cur.y;
			while (cur.x!=Location[0]||cur.y!=Location[1])
			{
				pth++; pt[pth]=fat[cur.y][cur.x];
				cur.x=cur.x-s[0][pt[pth]];
				cur.y=cur.y-s[1][pt[pth]];
			}
			return 1;
			   }
			   for (i=0;i<4;i++)
				   if ((a[cur.y+s[1][i]][cur.x+s[0][i]]==0||a[cur.y+s[1][i]][cur.x+s[0][i]]==1||a[cur.y+s[1][i]][cur.x+s[0][i]]==7)&&(!ok[cur.y+s[1][i]][cur.x+s[0][i]])&&(!(AI_danger(cur.x+s[0][i],cur.y+s[1][i])))) {
					   if ((closed+1)%15000!=open){
						   closed=(closed+1)%15000;
						   q[closed][0]=cur.x+s[0][i]; q[closed][1]=cur.y+s[1][i]; fat[q[closed][1]][q[closed][0]]=i;
					   }
				   }
				   break;
		case 4:
			if (!AI_danger(cur.x,cur.y)) {
			goal.x=cur.x; goal.y=cur.y;
			while (cur.x!=Location[0]||cur.y!=Location[1])
			{
				pth++; pt[pth]=fat[cur.y][cur.x];
				cur.x=cur.x-s[0][pt[pth]];
				cur.y=cur.y-s[1][pt[pth]];
			}
			return 1;
			   }
			   for (i=0;i<4;i++)
				   if ((a[cur.y+s[1][i]][cur.x+s[0][i]]==0||a[cur.y+s[1][i]][cur.x+s[0][i]]==7)&&(!ok[cur.y+s[1][i]][cur.x+s[0][i]])) {
					   if ((closed+1)%15000!=open){
						   closed=(closed+1)%15000;
						   q[closed][0]=cur.x+s[0][i]; q[closed][1]=cur.y+s[1][i]; fat[q[closed][1]][q[closed][0]]=i;
					   }
				   }
				   break;
		case 5:
			if (!AI_danger(cur.x,cur.y)&&cur.x!=p1.x&&cur.y!=p1.y) return 1;
			for (i=0;i<4;i++)
				if ((a[cur.y+s[1][i]][cur.x+s[0][i]]==0||a[cur.y+s[1][i]][cur.x+s[0][i]]==7)&&(!ok[cur.y+s[1][i]][cur.x+s[0][i]]))
				{
					if ((closed+1)%15000!=open){
						closed=(closed+1)%15000;
						q[closed][0]=cur.x+s[0][i]; q[closed][1]=cur.y+s[1][i]; fat[q[closed][1]][q[closed][0]]=i;
					}
				}
				break;
		} 
	}
	return 0;
}

/*=====================================================
����ԭ�ͣ�void calc_pos(void)�� 
���ܣ�����AI��ǰ���� 
������ϵ���˺�����AI_OP()�������ã�ÿ�μ�����·��ǰ�ȼ���AI��ǰ������ 
-------------------------------------------------------*/
void calc_pos(void)   /*����AI��ǰ�ĸ������*/
{
	Location[0]=xy_to_ij(px[0]+15);
	Location[1]=xy_to_ij(py[0]+15);
}


/*=====================================================
����ԭ�ͣ�void AI_Move(int t)�� 
���ܣ�����AI���ƶ� 
��ڣ��ƶ��Ĳ���t,1Ϊ���ը�����ԣ�2Ϊ�������֣�3ΪѰ�ҵ��ߣ�4ΪѰ�ҿ���ը����ǽ 
���ڣ���
������ϵ���˺�����AI_OP()�������ã����Կ���AI���ƶ� 
-------------------------------------------------------*/
void AI_Move(int t)  
{
	int state=0,can,i,px,py;
	if (t==1) {  /*����AI��Σ�յ����Σ���ʱ��������һ��û��Σ�յ�λ�ã�Ȼ���ߵ���λ���Զ��ը��*/
		if ((p1.x==goal.x||p1.y==goal.y)&&(pth<0)) state=AI_Search(4);
		if (state) exroad=1; 
		if (pth>=0) {
			switch (pt[pth])
			{
			case 0:handle_direction[0]=3; nx=Location[0]+s[0][0]; ny=Location[1]+s[1][0]; break;
			case 1:handle_direction[0]=4; nx=Location[0]+s[0][1]; ny=Location[1]+s[1][1]; break;
			case 2:handle_direction[0]=1; nx=Location[0]+s[0][2]; ny=Location[1]+s[1][2]; break;
			case 3:handle_direction[0]=2; nx=Location[0]+s[0][3]; ny=Location[1]+s[1][3]; break;
			}
			pth--;
		}
		else {
			exroad=0;
		}
		return ;
	}
	if (t==2) { /*��������ǵ�AI����û��ը����в����ʱ��ʼ�Ҷ��֣�����ҵ��������ƶ����������Խ�������*/
		if (pth>0) {
			switch (pt[pth])
			{
			case 0:handle_direction[0]=3; nx=Location[0]+s[0][0]; ny=Location[1]+s[1][0]; break;
			case 1:handle_direction[0]=4; nx=Location[0]+s[0][1]; ny=Location[1]+s[1][1]; break;
			case 2:handle_direction[0]=1; nx=Location[0]+s[0][2]; ny=Location[1]+s[1][2]; break;
			case 3:handle_direction[0]=2; nx=Location[0]+s[0][3]; ny=Location[1]+s[1][3]; break;
			}
			pth--;
		}
		else {
			exroad=0;
			ai_sbb=1; 
		}
		return ;
	}
	if (t==3) {/*Ѱ�Ҹ������������ĵ��ߣ������ƶ���ȥ��*/
		if (pth>=0) {
			switch (pt[pth])
			{
			case 0:handle_direction[0]=3; nx=Location[0]+s[0][0]; ny=Location[1]+s[1][0]; break;
			case 1:handle_direction[0]=4; nx=Location[0]+s[0][1]; ny=Location[1]+s[1][1]; break;
			case 2:handle_direction[0]=1; nx=Location[0]+s[0][2]; ny=Location[1]+s[1][2]; break;
			case 3:handle_direction[0]=2; nx=Location[0]+s[0][3]; ny=Location[1]+s[1][3]; break;
			}
			pth--;
		}
		else exroad=0;
		return ;
	}
	if (t==4) {/*��һ������ը����ש�飬����ը��*/
		if (pth>0) {
			switch (pt[pth])
			{
			case 0:handle_direction[0]=3; nx=Location[0]+s[0][0]; ny=Location[1]+s[1][0]; break;
			case 1:handle_direction[0]=4; nx=Location[0]+s[0][1]; ny=Location[1]+s[1][1]; break;
			case 2:handle_direction[0]=1; nx=Location[0]+s[0][2]; ny=Location[1]+s[1][2]; break;
			case 3:handle_direction[0]=2; nx=Location[0]+s[0][3]; ny=Location[1]+s[1][3]; break;
			}
			pth--;
		}
		else {
			goal.x=Location[0]; goal.y=Location[1];
			can=AI_Search(5);
			if (can) {exroad=0; ai_sbb=1; return;}
			else {
				px=Location[0]; py=Location[1];
				for (i=0;i<4;i++)
					if ((a[py+s[1][i]][px+s[0][i]]==0||a[py+s[1][i]][px+s[0][i]]==7||(a[py+s[1][i]][px+s[0][i]]>=30&&a[py+s[1][i]][px+s[0][i]]<=43))&&(!(AI_danger(px+s[0][i],py+s[1][i])))) break;
				switch (i)
				{
				case 0:handle_direction[0]=3; nx=Location[0]+s[0][0]; ny=Location[1]+s[1][0]; break;
				case 1:handle_direction[0]=4; nx=Location[0]+s[0][1]; ny=Location[1]+s[1][1]; break;
				case 2:handle_direction[0]=1; nx=Location[0]+s[0][2]; ny=Location[1]+s[1][2]; break;
				case 3:handle_direction[0]=2; nx=Location[0]+s[0][3]; ny=Location[1]+s[1][3]; break;
				}
				exroad=0;
			}
		}
		return ;
	}
}

/*=====================================================
����ԭ�ͣ�void AI_OP(int b)�� 
���ܣ�AI�����Ľӿڣ�����ͳ��AI��ȫ������ 
��ڣ�bΪAI���ڿ��Էŵ�ը���� 
���ڣ���
������ϵ���˺�����GamePlay()�б����ã�ΪAI����� 
-------------------------------------------------------*/
void AI_OP(int b)  
{
	static int state,celue;
	calc_pos();  /*�ȼ�������*/
	if (handle_direction[0]!=0) {
		switch (handle_direction[0])
		{
		case 1:nx1=Location[0]+s[0][2]; ny1=Location[1]+s[1][2]; break;
		case 2:nx1=Location[0]+s[0][3]; ny1=Location[1]+s[1][3]; break;
		case 3:nx1=Location[0]+s[0][0]; ny1=Location[1]+s[1][0]; break;
		case 4:nx1=Location[0]+s[0][1]; ny1=Location[1]+s[1][1]; break;
		}    /*������һ����Ҫ�ߵ��ĵ�����*/
		if (!cov(nx1,ny1)) {AI_Reset(); return ;}   /*�ж�·����һ���Ƿ���Ե��������Ծ��ߣ������һ�����ɴ˵�����ܱ���������ĳЩ�µ�ס�ˣ���Ҫ����AI���¼���·��*/
		if (Location[0]!=nx||Location[1]!=ny) return ;
		else {handle_direction[0]=0; return ;}  /*�����һ��ɴ��һֱ�����ƶ�ֱ��������һ��Ϊֹ*/
	}                     
	if ((px[0]-10)%30!=0||(py[0]-10)%30!=0) return ;  /*�ж��Ƿ�ͣ������*/
	if (ai_sbb) {set_bomb(0); ai_sbb=0;}   /*·�����е�����ж�����·������Ƿ���Ҫ��ը�������Ҫ���ͷ�ը��*/
	if (!exroad) {  /*���AI��ʱ����ִ��һ������������·�����Ϳ�ʼ����·��*/
		celue=state=0;
		state=AI_Search(0);      /*�����������Ƿ���ը��*/
		if (state!=0) celue=1;   /*������ը����ִ�в���1*/
		else if (celue==0) {
			if (gamelevel==0) 
				if (random(3)==0) state=AI_Search(1);  /*�����ʱ���Ǽ�ģʽ����1/3�ĸ���AI��ѡ������������*/
				else ;
			else state=AI_Search(1);
		}   /*ը���Ҳ������Ҷ���*/
		if (celue==0&&state!=0) {
			celue=2;  /*�����ж��֣�ִ�в���2*/
		}else if (celue==0) {
			state=AI_Search(2);  /*�������û�ж��֣��ҵ���*/
		}
		if (celue==0&&state!=0) {
			celue=3; /*��������е��ߣ�ִ�в���3*/
		}else if (celue==0) {
			state=AI_Search(3);  /*�������Ҳ�Ҳ��������ҿ���ը����ǽ����ը��*/ 
		}
		if (celue==0&&b>0&&state!=0) celue=4; /*���������ǽ����ִ�в���4*/
		if (celue>1) exroad=1;  /*����������������ҵ�·��*/
		if (celue>=1&&celue<=4) AI_Move(celue); /*�ҵ��˶�������ʼִ�в���(������4��������Ҳ���)*/
	}
	else AI_Move(celue);  /*�����ʱ������ִ��һ��·�����ͼ���ִ��*/
	return ;
}

/*=====================================================
����ԭ�ͣ�void AI_Reset(void)�� 
���ܣ�����AI��·����Ϣ 
��ڣ���
���ڣ���
������ϵ���˺�����AI_OP()�Լ�GamePlay()�б����ã����AI��ǰ·��ʧЧ���������ˣ���Ҫ������·����Ϣ 
-------------------------------------------------------*/
void AI_Reset(void) 
{
	handle_direction[0]=0;
	ai_sbb=0;
	exroad=0;
}
