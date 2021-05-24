#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <vector>
#include <iostream>
#include <fstream>

#define PER_RANK 2 /* Nodes/Rank: How 'fat' the DAG should be.  */
#define RANKS 10  /* RANKS: How 'tall' the DAG should be.  */
#define PERCENT 20   /* Chance of having an Edge.  */

int arr[10000][10000] = {};

int main (void)
{
  
    
  int i, j, k,nodes = 0, big = 0;
  srand (time (NULL));
  for (i = 0; i < RANKS; i++)
    {
      for (j = 0; j < nodes; j++)
      {
        for (k = 0; k < PER_RANK; k++)
          if ( (rand () % 100) < PERCENT)
            {
                big = std::max(j, big);
                big = std::max(k+nodes, big);
                if(j != k+nodes)
                  arr[j][k+nodes] = 1;
            }

      }
      nodes += rand() % (PER_RANK-1) + 1;
    }
    std::ofstream fout("file.txt");
  for (int i = 0; i < big; i++)
  {
    for (int j = 0; j < big; j++)
      fout << arr[i][j] << ' ';
    fout << std::endl;
  }
  return 0;
}