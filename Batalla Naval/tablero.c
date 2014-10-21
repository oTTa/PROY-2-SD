 
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "tablero.h"

     
int tableroPropio[10][10];
int tableroRival [10][10];
int totalBarcos=5;
int totalBarcosEnemigos=5;
//Convierte barcos a un tablero completo
void inicializarTablero()
{
  int i;
  int j;
  for(i=0;i<10;i++)
    for(j=0;j<10;j++)
	tableroPropio[i][j]=0;
    for(i=0;i<10;i++)
    for(j=0;j<10;j++)
	tableroRival[i][j]=-1;
  /*for(i=0;i<5;i++)
    tableroPropio[((barco->b1)[i]).c1][((barco->b1)[i]).c2]=1;
  for(i=0;i<4;i++)
    tableroPropio[(barco->b2)[i].c1][(barco->b2)[i].c2]=2;
  for(i=0;i<4;i++)
    tableroPropio[(barco->b3)[i].c1][(barco->b3)[i].c2]=3;
  for(i=0;i<3;i++)
    tableroPropio[(barco->b4)[i].c1][(barco->b4)[i].c2]=4;
  for(i=0;i<3;i++)
    tableroPropio[(barco->b5)[i].c1][(barco->b5)[i].c2]=5;
  for(i=0;i<2;i++)
    tableroPropio[(barco->b6)[i].c1][(barco->b6)[i].c2]=6;
  for(i=0;i<2;i++)
    tableroPropio[(barco->b7)[i].c1][(barco->b7)[i].c2]=7;
  for(i=0;i<2;i++)
    tableroPropio[(barco->b8)[i].c1][(barco->b8)[i].c2]=8;
  for(i=0;i<2;i++)
    tableroPropio[(barco->b9)[i].c1][(barco->b9)[i].c2]=9;*/
}
int insertarBarco(coord* c,int orientacion, int barco)
{
  int respuesta;
  int cantCasillas;
  int i;
  if (barco==1)
    cantCasillas=5;
  else if (barco==2 || barco==3)
      cantCasillas=4;
  else if (barco==4 || barco==5)
      cantCasillas=3;
  else cantCasillas=2;
  if (orientacion==0)
  {
    if (((c->c2)+cantCasillas)>10)
      respuesta=1;
    else 
    {
       i=c->c2;
      
      while ((tableroPropio[c->c1][i]==0) && (i<(c->c2)+cantCasillas)){
	i++;
      }
      if (i==(c->c2)+cantCasillas)
      {
	respuesta=0;
       for(i=c->c2;i<(c->c2)+cantCasillas;i++)
	  {
	    tableroPropio[c->c1][i]=barco;
	  }
      }
      else 
	{
	  respuesta=2;
	 
	}
    }
  }
  else
    {
    if (((c->c1)+cantCasillas)>10)
      respuesta=1;
    else 
    {
       i=c->c1;
      
      while ((tableroPropio[i][c->c2]==0) && (i<(c->c1)+cantCasillas)){
	i++;
      }
      if (i==(c->c1)+cantCasillas)
      {
	respuesta=0;
	 for(i=c->c1;i<(c->c1)+cantCasillas;i++)
	  {
	    tableroPropio[i][c->c2]=barco;
	  }
      }
      else respuesta=2;
    }
  }
  return respuesta;
    
}
void mostrarTableros(){
  int i,j;
	printf("     0   1   2   3   4   5   6   7   8   9             0   1   2   3   4   5   6   7   8   9  \n");
	printf("   -----------------------------------------         -----------------------------------------\n");
	for (i=0; i<10; i++){
		printf("%i. ",i);
		printf("|");
		for (j=0; j<10; j++){ 
			
			if(tableroPropio[i][j]==0){
			printf(" 0 ");}
			else {
			printf(" %i ",tableroPropio[i][j]);}
			printf("|");
		}
		printf("      ");
		printf("%i. ",i);
		printf("|");
		for (j=0; j<10; j++){ 
			
			if(tableroRival[i][j]==0){
			printf(" 0 ");}
			else if(tableroRival[i][j]==1){
			printf(" X ");}
			else
			  printf("   ");
			printf("|");
		}
	printf("\n");
	printf("   -----------------------------------------         -----------------------------------------\n");
	}
}

int recibirAtaque(coord *c)
{
  int n;
  int respuesta;
  if (tableroPropio[c->c1][c->c2]==0)
    respuesta=0;
  else
  {
    totalBarcos--;
    n=tableroPropio[c->c1][c->c2];
    tableroPropio[c->c1][c->c2]=0;
    if (totalBarcos>0)
    {
      int i;
      i=0;
      while ((tableroPropio[i][c->c2]!=n) && (i<10))
      {
	i++;
      }
      if (i<10)
      {
	respuesta=1;
      }
      else
      {
	i=0;
	 while ((tableroPropio[c->c1][i]!=n) && (i<10))
	{
	  i++;
	}
      if (i<10)
      {
	respuesta=1;
      }
      else respuesta=2;
      }
    }
    else respuesta=3;
  }
  return respuesta;
}

void informarAtaque(coord *c, int exito)
{
  if (exito==0)
    tableroRival[c->c1][c->c2]=0;
  else {
    tableroRival[c->c1][c->c2]=1;
    totalBarcosEnemigos--;
  }
}

int obtenerTotalBarcosEnemigos (){ return totalBarcosEnemigos; }

int obtenerTotalBarcos (){ return totalBarcos; }

