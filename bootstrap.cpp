#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <random>

#define B 10000
#define N 100
#define W 0.01
#define ALPHA 0.05

int main(void){
  int d[N] = {};  //真の分布からの観測データ,全て0で初期化
//  int tmp[N];       //リサンプリングデータを一時格納する配列
  double b[B / N];   //リサンプリングデータを用いて計算した推定量を格納する
  int psum = 0;
  double bsum = 0;
  double min = 1.1;
  double max = -1;
  int k;
  int fnum;
  double *freqs;
  double p;
  double a;
  double la;
  double ua;

  //リサンプリング，推定量の計算
  for(int i = 0; i < 55; i++) d[i] = 1; //55回分は表を示す 1 で初期化
  std::mt19937 mt(2424);      //mtは決定的な乱数で，初期シードを2424とした
  for(int i = 0; i < B / N; i++){
    psum = 0;
    for(int j = 0; j < N; j++) {
      psum += d[mt() % N];  //d[]上の一様分布からリサンプリング
    }
    b[i] = (double)psum / (double)N;  //リサンプリングデータから推定量を計算
    if(min > b[i]) min = b[i];
    if(max < b[i]) max = b[i];
    bsum += b[i];
  }
  //ヒストグラムをつくる
//  min = (int)(min * 10) / 10;
//  max = (int)((max + 0.9) * 10) / 10;
  fnum = (double)((max - min) / W);
//  printf("%f - %f / %f = %d\n", max, min, W, fnum);
  freqs = (double*)(malloc(sizeof(double) * fnum));
  for(int i = 0; i < fnum; i++) freqs[i] = 0;
  std::sort(b, b + B / N);
  p = min + W;
  for(int i = 0, j = 0; i < N; i++){
    if(b[i] > p){
      j++;
      i--;
      p += W;
    }else{
      freqs[j]++;
    }
  }
  for (int i = 0; i < fnum; i++) {
    freqs[i] /= B / N;
  }
  //信頼区間の端をとる
  a = 0;
  k = 0;
//  printf("la\n");
  while(a < ALPHA / 2){
    a += freqs[k];
    k++;
//    printf("[%d]: %f\n", k, a);
  }
  la = min + k * W + W / 2;
  a = 0;
  k = 0;
//  printf("ua\n");
  while(a < 1.0 - ALPHA / 2){
    a += freqs[k];
    k++;
//    printf("[%d]: %f\n", k, a);
  }
  ua = min + k * W + W / 2;
  printf("%8.2f %% %7.2f %%\n", ALPHA / 2 * 100, (1.0 - ALPHA / 2) * 100);
  printf("%8.4f %8.4f\n", la, ua);
  return 0;
}
