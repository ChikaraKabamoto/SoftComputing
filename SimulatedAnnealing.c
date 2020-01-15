#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "SimulatedAnnealing.h"
#define MAX 2147483627


int T=1000;       /*初期温度*/
int **time_info;
double *weight_info;
int **point_combination;
int *visited_point;
int weight;
int *root,*subroot;
int  **subpoint_combination;

int main()
 {
    int point;
    printf("地点数を入力してください\n");
    scanf("%d",&point);
     
    int rand_time,i,j,r;
    int t;
    double weight,score,prescore,temperature;
    t=time(NULL);
    srand(t);
    MemoryAllocation(point);
    InitTimeArray(point);
    TimeSetting(point);
    printf("\n<各地点間の移動距離>\n");
    printf("        ");
    PrintTravelTime(point);
    weight=SettingWeight(point);
    SettingRoot(point);
    score=CalcScore(point,weight);
    prescore=score;
    printf("\n");
    printf("<初期スコア> %lf\n",score);
    temperature=T;
    int k=1,rept,R;
    double temp;

     if(point>4)  rept=point*10;   /*反復回数を設定*/
     else  rept=point+2;

     while(temperature>1)      /*温度が１℃になるまで最適解を探索*/
    {
         for(R=0;R<rept;R++)
         {
            ExchangePoint(point);            /*ルートを一部変更*/
            score=CalcScore(point,weight);   /*スコアを計算*/
            if(score-prescore<0) 
            {
                 prescore=score;             /*スコアがルート交換前より良い（低い）なら,そのルートを新しいルートにする*/ 
                 CopyRoot(point);
            }
            else                             
           {                                  /*スコアがルート交換前より悪い（高い）ときは、以下のようになる*/ 
                temp=(double)rand()/MAX;
                if(temp<exp(-(score-prescore)/temperature)) /* e^(-ΔE/T)の確率でスコアが悪くても交換後のルートを新しいルートとして採用*/ 
                {
                   prescore=score;
                   CopyRoot(point);
                 }
                else ResetPoint(point);                    /* e^(-ΔE/T)の確率にあてはまらない場合はルートは以前のままにする*/                  
            }
         }
         temperature=TemperatureChange(T,k);
         k++;
    }
   printf("<最終ルート> ");
   for(i=0;i<point;i++)
   {  
       printf("%d->",subroot[i]);
   }
   printf("%d\n",subroot[i]);
   printf("<最終スコア> %lf\n",prescore);
   return 0;
 }

  /*冷却スケジュール*/
 double TemperatureChange(int T,int k)
 {
   double r=0.9;
   return T*pow(r,k);
 }
 /*メモリ確保*/
void MemoryAllocation(int point)
{
	int i;
        time_info=(int **)malloc((point+2)*sizeof(int *));
        for(i=0;i<point+1;i++)
        {
            time_info[i]=(int *)malloc((point+2)*sizeof(int));
        }
        weight_info=(double *)malloc((point+2)*sizeof(double));
        point_combination=(int **)malloc((point+2)*sizeof(int *));
        for(i=0;i<point+1;i++)
        {
            point_combination[i]=(int *)malloc((point+2)*sizeof(int));
        }
        visited_point=(int *)malloc((point+2)*sizeof(int));
        root=(int *)malloc((point+2)*sizeof(int));
        subroot=(int *)malloc((point+2)*sizeof(int));
         subpoint_combination=(int **)malloc((point+2)*sizeof(int *));
        for(i=0;i<point+1;i++)
        {
            subpoint_combination[i]=(int *)malloc((point+2)*sizeof(int));
        }
}
 /*移動距離配列の初期化*/
void InitTimeArray(int point)
 {
   int i,j;
   for(i=0;i<point+1;i++)
    {
         for(j=0;j<point+1;j++)
         {
             time_info[i][j]=0;
         }
    }
 }
 /*移動距離の設定*/
void TimeSetting(int point)
  { 
     int i,j;
    for(i=0;i<point+1;i++)
       for(j=i+1;j<point+1;j++)
      {
            do{ *(*(time_info+i)+j)=rand()%60;
             }while(*(*(time_info+i)+j)<5);
      }
  }
 /*各地点の移動距離を表示*/
void PrintTravelTime(int point)
{
   int i,j;
   for(i=0;i<point+1;i++)
   {
      printf("地点%2d  ",i);
   }
   printf("\n");
   for(i=0;i<point+1;i++)
   {
         printf("地点%2d",i);
         for(j=0;j<point+1;j++)
         {
             printf("%8d",time_info[i][j]);
          }
         printf("\n");
   }
}
 /*各地点の荷物の重さを設定*/
 double SettingWeight(int point)
{
     double sum=0;
     int i;
    do{
          for(i=0;i<point;i++)
          {
            sum=0;
            double temp;
            temp=(double)rand()/MAX;
            temp=temp*(rand()%3)+1;
            weight_info[i+1]=(temp*300)/(2*point);
          }
          for(i=0;i<point;i++)
          {
           sum+=weight_info[i+1];
          }
      }while(sum>300);
    printf("\n全重量は%dkg\n\n",(int)sum);
    printf("<各地点の荷物の重量> ");
    for(i=1;i<point+1;i++)
    {
       printf("地点%d:%3.1lfkg ",i,weight_info[i]);
    }
    printf("\n\n");
    return sum;
}
 /*初回のルートを設定*/
void SettingRoot(int point)
{
    int now=0,before;
    int i;
    while(now==0)
    {
       now=rand()%(point+1);
    }
    point_combination[0][now]=1;
    visited_point[now]=1;
    before=now;
    printf("<初期ルート> ");
    printf("0->%d->",now);
    for(i=1;i<point;i++)
    {
       do{
           now=rand()%(point)+1;
          }while(visited_point[now]==1);
       if(i<point-1) printf("%d->",now);
       else if(i==point-1) printf("%d",now);
       point_combination[before][now]=1;
       visited_point[now]=1;
       before=now;
     }
}
  /*スコアを計算*/
double CalcScore(int point,double weight)
{
	int i,j;
	int temp;
        double score=0;
	for(i=1;i<point+1;i++)
	{
		if(point_combination[0][i]==1)
		{
			temp=i;
			break;
		}
	} 
        score=weight*time_info[0][temp];
        weight=weight-weight_info[temp];
        int k=0;
	for(i=1;i<point;i++)
	{
		for(j=1;j<point+1;j++)
		{
			if(point_combination[temp][j]==1)
                       {
                         if(temp<j) score=score+weight*time_info[temp][j];
                         else  score=score+weight*time_info[j][temp];
                        weight=weight-weight_info[j];
                        temp=j;
                        break;
                        }
		}
	}
       score=score+(double)time_info[j][0];
       return score;
}	 

 /*ルートを変更する*/
void ExchangePoint(int point)
{
   int i,j,exchange1,exchange2,next1,next2,t;
   
   for(i=0;i<point+1;i++)
   {
      root[i]=0;
   }
   
   for(i=1;i<point+1;i++)
   { 
       if(point_combination[0][i]==1) 
       {
          root[1]=i;
          t=i;
        } 
    }
  
   for(i=2;i<point+1;i++)
    {
        for(j=1;j<point+1;j++)
         {
              if(point_combination[t][j]==1)
               {
                   root[i]=j;
                   t=j;
                   break;
                }
         }
    }

   do{
         exchange1=(rand()%point)+1;
         exchange2=(rand()%point)+1;
      }while(exchange1==exchange2);

    if(exchange1>exchange2)
    {
      t=exchange1;
      exchange1=exchange2;
      exchange2=t;
     }

    next1=root[exchange1+1];
    next2=root[exchange2+1];
    point_combination[root[exchange1-1]][root[exchange1]]=0;
    point_combination[root[exchange2-1]][root[exchange2]]=0;
    point_combination[root[exchange1]][next1]=0;
    point_combination[root[exchange2]][next2]=0;

    if(abs(exchange1-exchange2)!=1)
    { 
      point_combination[root[exchange1-1]][root[exchange2]]=1;
      point_combination[root[exchange2-1]][root[exchange1]]=1;
     if(next2!=0) point_combination[root[exchange1]][next2]=1;
     if(next1!=0) point_combination[root[exchange2]][next1]=1;
    }
    else   
    {
      point_combination[root[exchange1-1]][root[exchange2]]=1;
      point_combination[root[exchange2]][root[exchange1]]=1;
     if(next2!=0) point_combination[root[exchange1]][next2]=1;
     }

    for(i=0;i<point+1;i++)
    {
       root[i]=0;
     }
  
    for(i=1;i<point+1;i++)
    { 
       if(point_combination[0][i]==1) 
       {
          root[1]=i;
          t=i;
       } 
    }
    for(i=2;i<point+1;i++)
     {
        for(j=1;j<point+1;j++)
         {
              if(point_combination[t][j]==1)
               {
                  root[i]=j;
                  t=j;
                  break;
                }
          }
      }
  for(i=0;i<point+1;i++)
   {
         for(j=0;j<point+1;j++)
         {
            subpoint_combination[i][j]=point_combination[i][j];
         }  
    }
 }
  /*ルートを復元*/
 void ResetPoint(int point)
 {
      int i,j;
      for(i=0;i<point+1;i++)
      {
          for(j=0;j<point+1;j++)
         {
            point_combination[i][j]=subpoint_combination[i][j];
          }
       }
 }
  /*ルートを退避*/
 void CopyRoot(int point)
 {
      int i;
      for(i=0;i<point+1;i++)
     {
        subroot[i]=root[i];
      }
 }

