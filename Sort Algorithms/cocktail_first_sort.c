#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/uio.h>
#include <sys/time.h>


/***************************************************
  Cocktail sort implementation
***************************************************/


static void cocktail( int c[], int n )
{
	int i,j,k,temp,tag=1; //the sign of exchanging
	for(i=1;tag;i++ )//if the tag=1,going on;if the tag=0,stopping
	{
		tag = 0;
		for(j=n-i;j>=i;j-- )//Find the smallest element from the back to front
		{
			if(c[j] < c[j-1] )
			{
				temp = c[j];
				c[j] = c[j-1];
				c[j-1] = temp;
				tag = 1;
			}
		}
		for(k=i;k<n-i;k++)//Find the biggest element from the front to back
		{
			if( c[k] > c[k+1] )
			{
				temp = c[k];
				c[k] = c[k+1];
				c[k+1] = temp;
				tag = 1;
			}
		}
	}
}

/***************************************************



/***********************************************
bubble sort  ascending
**********************************************/
void bubbAsc(int array[],int n)
{
	int c, d, swap;
 for (c = 0 ; c < ( n - 1 ); c++)
  {
    for (d = 0 ; d < n - c - 1; d++)
    {
      if (array[d] > array[d+1]) /* For decreasing order use < */
      {
        swap       = array[d];
        array[d]   = array[d+1];
        array[d+1] = swap;
      }
    }
  }
}
/**************************************************
bubble sort  descending
*********************************************/
void bubbDes(int array[],int n)
{
	int c, d, swap;
 for (c = 0 ; c < ( n - 1 ); c++)
  {
    for (d = 0 ; d < n - c - 1; d++)
    {
      if (array[d] < array[d+1]) /* For decreasing order use < */
      {
        swap       = array[d];
        array[d]   = array[d+1];
        array[d+1] = swap;
      }
    }
  }
}



/***************************************************
  shell sort implementation
***************************************************/


static void shell(int s[],int n)
{
	int gap,i,j,k,temp;
	for (gap=n/2;gap>0;gap/=2) //decide the step
	{
		for(i=0;i<gap;i++)
		{
			for(j=i+gap;j<n;j=j+gap)
				if(s[j]<s[j-gap])
			{
				temp = s[j];//find the smaller
				k=j-gap;
				while (k>=0&&s[k]>temp)//insertion sort
				{
					s[k+gap] = s[k];
					k-=gap;
				}
				s[k+gap] = temp;  //insert the excat location
                }
		}
	}
}


/***************************************************
  Declarations of utility functions used in the benchmark. They are
  defined after the main function. You do not need to understand how
  they work, just use them in your benchmarks by copy-pasting.
***************************************************/
/******/
void bubbAsc(int *,int );

/************/
void bubbDes(int *,int );


/* Returns a random integer in the specified range. */
static int random_int (int minval, int maxval);

/* Returns a freshly allocated array with len random integers from the
   specified range. The returned array has to be freed at the end of
   the program. */
static int * random_array (int minval, int maxval, size_t len);

/* Returns the current time as milliseconds in double format. The first
   time this function is called, it resets an internal reference and
   returns zero. From then on it returns the number of milliseconds
   since that reset. */
static double clockms (void);


/***************************************************
  The main benchmark code begins here.
***************************************************/

int main (int argc, char ** argv)
{
  int dmin    =       0;
  int dmax    =    1000;
  int nstart  =      20;
  int nmax    =   30000;
  double nfac =       1.2;
  double nd;
  int * data;
  int * dup;
  
  /*
    Allocate one big random input data array. For varying the and
    making sure we always start with random input, we will use a
    duplicate (with just the first N entries) at each iteration.
  */
  
  printf ("# generating input data (this can take a while...)\n");
  fflush (stdout);
  data = random_array (dmin, dmax, nmax);
  dup = malloc (nmax * sizeof(int));
  if (NULL == dup) {
    err (EXIT_FAILURE, __FILE__": %s: malloc", __func__);
  }
  
  printf ("################################################\n"
	  "#\n"
	  "# Cocktail sort runtime measurements\n"
	  "#\n"
	  "# column 1: N\n"
	  "# column 2: Random array T [ms]\n"
      "# column 3: ascended array as input  [ms]\n"
      "# column 4: Descended array as in put [ms]\n"
      "#\n");
  
  /*
    Main benchmarking loop.
    
    Note that we use a floating point "length" so that we can easily
    create a geometric series for N. This spaces the sampled array
    sizes more densely for small N, and for big N (where things
    usually take much longer) we don't take so many samples (otherwise
    running the benchmark takes a really long time).
  */
 
 //int tempnum; 
 
 	
 
 	 for (nd = nstart; nd <= nmax; nd *= nfac) {
 	 	double temp0 = 0, temp1 = 0, temp2 = 0;
		  	
 	 		
    int nn;
    double tstart2, tstop2, tstart1 , tstop1 , tstart0, tstop0;
    
    /*
      convert the floating point "length" to an integer
    */
    nn = nd;
    
//    printf ("%8d", nn);
    fflush (stdout);
    
    /*
      use a fresh duplicate from the random input data
    */
    memcpy (dup, data, nn * sizeof(int));
    //=========== random input ===================
    tstart0 = clockms ();
    cocktail (dup, nn);
    tstop0 = clockms ();
    temp0 = tstop0 - tstart0;
	
	//========== Asc sort as input ===============
	bubbAsc (dup, nn);
    tstart1 = clockms ();
    cocktail (dup, nn);
    tstop1 = clockms ();
    temp1 = tstop1 - tstart1;
    //=========== Des sort as input ==============
    bubbDes (dup, nn);
    tstart2 = clockms ();
    cocktail (dup, nn);
    tstop2 = clockms ();
    temp2 = tstop2 - tstart2;
    //	printf ("  %8g", tstop - tstart);
      
      

     printf (" %8d  %8g  %8g  %8g\n",nn,temp0, temp1 ,temp2);
 }
  /*
    End of the program.
  */
  
  free (data);
  free (dup);
  
  return 0;
}


/***************************************************
  Implementations of the utility functions to create random data and
  measure execution time.
  
  YOU DO NOT NEED TO UNDERSTAND THESE FUNCTIONS! Just copy-paste them
  as needed when you create new benchmark programs.
***************************************************/


int random_int (int minval, int maxval)
{
  static int fd = -1;
  unsigned int delta, tmp, mask;
  
  if (0 > fd && 0 > (fd = open ("/dev/urandom", O_RDONLY))) {
    err (EXIT_FAILURE, __FILE__": %s: open /dev/urandom", __func__);
  }
  
  if (maxval < minval) {
    tmp = minval;
    minval = maxval;
    maxval = tmp;
  }
  if (0 == (delta = maxval - minval)) {
    return minval;
  }
  
  for (tmp = 1 << (8 * sizeof(int) - 1); 0 == (tmp & delta); tmp >>= 1) {
    /* nop */;
  }
  for (mask = tmp; 0 != tmp; tmp >>= 1) {
    mask |= tmp;
  }
  
  for (;;) {
    if (sizeof(int) != read (fd, &tmp, sizeof(int))) {
      err (EXIT_FAILURE, __FILE__": %s: read /dev/urandom", __func__);
    }
    tmp &= mask;
    if (tmp <= delta) {
      break;
    }
  }
  
  return minval + tmp;
}


int * random_array (int minval, int maxval, size_t len)
{
  int * arr;
  int * ii;
  
  arr = malloc (len * sizeof(int));
  if (NULL == arr) {
    err (EXIT_FAILURE, __FILE__": %s: malloc", __func__);
  }
  
  for (ii = arr; len > 0; ++ii, --len) {
    *ii = random_int (minval, maxval);
  }
  
  return arr;
}


double clockms (void)
{
  static struct timeval t0 = { 0, 0 };
  struct timeval t1;
  
  if (0 == t0.tv_sec) {
    if (0 != gettimeofday (&t0, NULL)) {
      err (EXIT_FAILURE, __FILE__": %s: gettimeofday", __func__);
    }
  }
  if (0 != gettimeofday (&t1, NULL)) {
    err (EXIT_FAILURE, __FILE__": %s: gettimeofday", __func__);
  }
  
  return 1e3 * (t1.tv_sec - t0.tv_sec) + 1e-3 * (t1.tv_usec - t0.tv_usec);
}
