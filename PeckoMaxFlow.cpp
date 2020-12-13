/*
 *Matthew Pecko
 *Professor Brass
 *12/13/2020
 *CSc 220 Homework 4
 */
/* compiles with command line  g++ PeckoMaxFlow.cpp */

#include <stdio.h>
#include <stdlib.h>


#define DEBUG 0
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;

#define k 10
int maxflow(int n, int *cap, int *flow, int *residual);
void printSquareArray(int* g, int n);

int main()
{    int capacities[4*k+2][4*k+2];
     int flow[4*k+2][4*k+2];
     int residual[4*k+2][4*k+2];
     int i,j, improvement, stepcount, totalflow;
     int inflow, outflow;
     for(i=0; i<4*k+2; i++)
       for(j=0; j<4*k+2; j++)
	 {  capacities[i][j]=0; flow[i][j]=0; residual[i][j]=0;
	 }
     /* initialize capacities */
     for(i=1; i< k+1; i++)
       { capacities[0][i] = 20;
	 capacities[i][k+i] = 4;
	 capacities[i][k+(i%k)+1] = 4;
	 capacities[i][2*k+i] = 5;
	 capacities[k+i][3*k+i] = 5;
	 capacities[2*k+i][3*k+i] = 3;
	 capacities[2*k+i][3*k+(i%k)+1] = 3;
	 capacities[2*k+i][3*k+((i+1)%k)+1] = 3;
	 capacities[3*k+i][4*k+1] = 12;
       }
     printf("created capacity matrix. %d vertices, expected maximum flow %d.\n",
	    4*k+2, 10*k );
     improvement=1; stepcount = 0;
     while( improvement > 0)
     { improvement=maxflow( 4*k+2, &(capacities[0][0]), &(flow[0][0]),
				   &(residual[0][0]) );
       stepcount ++;
       /* check flow matrix: capacity constraint and nonnegativity */
       for(i=0; i<4*k+2; i++)
          for(j=0; j<4*k+2; j++)
	  {  if( flow[i][j] < 0 )
	     {	printf("step %d: negative flow value %d on edge %d -> %d\n",
		       stepcount, flow[i][j], i, j); fflush(stdout); exit(0);
	     }
	     if( flow[i][j] > capacities[i][j] )
	     {	printf("step %d: flow %d > capacity %d on edge %d -> %d\n",
		       stepcount, flow[i][j], capacities[i][j], i, j);
	               fflush(stdout); exit(0);
	     }
	  }
       /* check flow matrix: inflow = outflow */
       for( i=1; i< 4*k+1; i++ )
       {  inflow = 0; outflow = 0;
	  for( j=0; j<4*k+2; j++ )
	  {  inflow += flow[j][i];
	    outflow += flow[i][j];
	  }
	  if( inflow != outflow )
	    {  printf("step %d: flow conservation violated in vertex %d\n",
		      stepcount, i); fflush(stdout); exit(0);
	    }
       }
       printf("step %d completed; improvement %d\n", stepcount, improvement );
     }
     totalflow =0;
     for( i=1; i<k+1; i++)
        totalflow += flow[0][i];
     printf("finished after %d steps; total flow %d\n", stepcount, totalflow);
}

class MyNode{
	private:
	vector<MyNode> adj;
	int label;

	public:
	MyNode(vector<MyNode> adj, int label){
		this->adj = adj;
		this->label = label;
	}

	int getLabel(){
		return this->label;
	}

	vector<MyNode> getAdj(){
		return this->adj;
	}
};

int maxflow(int n, int *cap, int *flow, int *residual){
     
	//Fulkerson algorithm
	//1. update residual capacity for the current flow
	//2. Find a path from S to T in the residual capacity graph along edges with positive capacity
	//3. find the smallest aailable capacity on he edges of this path in the residual capacity graph
	//4. update the flow along that path by sending that amount of flow from S to T

//Update residual graph	
int x, y, i;
     for(x = 0; x < n; ++x){
	     for(y = 0; y < n; ++y){
		i = x + n*y;
		residual[i] = cap[i] - flow[i];	
	     }
     }

     //find a path from S to T using BFS in the residual graph along edges with +cap

     queue<int> q;
     unordered_map<int, int> p;
     char visited[n];
     for(int i = 0; i < n; ++i) visited[i] = 'f';
int min;
int c;
     q.push(0);


     while(!q.empty()){
		c = q.front();	     
	     q.pop();

	     if(visited[c] == 'f'){
		visited[c] = 't';
		
		for(int z = c*n; z < c*n+n; z++){
			if(residual[z] > 0){
				q.push(z%n);
				if(visited[z%n] == 'f') p[z%n] = c;
			}

		}
	     
	     }

     }


     //find bottleneck
     min = residual[p[n-1]*n+n-1];

	int start = p[n-1];
	int prev = n-1;
	while(prev > 0){
		int myc = residual[start*n + prev];
		if(myc < min) min = myc;
	prev = start;
	start = p[start];
	}

	//update flows
	start = p[n-1];
	prev = n-1;
	while(prev > 0){
		flow[start*n + prev] += min;
		
		prev = start;
		start = p[start];
	}

	if(DEBUG){
     printSquareArray(cap, n);
     
   printf("\n"); 

     printSquareArray(residual, n);

   printf("\n"); 
     
   printSquareArray(flow, n);

   printf("\n"); 

	for(int i = 0; i < n; ++i){
   		printf("%d\n", p[i]); 
	
	}
	}

     return min;
}

void printSquareArray(int* g, int n){
     int x, y, i;
     for(x = 0; x < n; ++x){
	     for(y = 0; y < n; ++y){
			i = x + n*y;
			printf("%d\t", g[i]);
		}
		printf("\n");
     }
}

