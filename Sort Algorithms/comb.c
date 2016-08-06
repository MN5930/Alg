#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <time.h>

/***************************************************
  Comb sort implementation
***************************************************/


static void combsort(int *arr, int n)
{
    float shrink_factor = 1.247330950103979;
    int gap = n, swapped = 1, swap, i;
 
    while ((gap > 1) || swapped)
    {
        if (gap > 1)
            gap = gap / shrink_factor;
 
        swapped = 0;
        i = 0;
 
        while ((gap + i) < n)
        {
            if (arr[i] - arr[i + gap] > 0)
            {
                swap = arr[i];
                arr[i] = arr[i + gap];
                arr[i + gap] = swap;
                swapped = 1;
            }
            ++i;
        }
    }
}
/***************************************************
  Declarations of utility functions used in the benchmark. They are
  defined after the main function. You do not need to understand how
  they work, just use them in your benchmarks by copy-pasting.
***************************************************/

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
  int nstart  =      1000;
  int nmax    =   4000000;
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
	  "# comb sort runtime measurements\n"
	  "#\n"
	  "# column 1: N\n"
	  "# column 2: T [ms]\n"
	   "# comlumn 3: min for 10 times running [ms]\n"
      "# column 4: max for 10 times running [ms]\n"
       "# column 5 : avg for 10 times running [ms]\n"  
	  "#\n");
  
  /*
    Main benchmarking loop.
    
    Note that we use a floating point "length" so that we can easily
    create a geometric series for N. This spaces the sampled array
    sizes more densely for small N, and for big N (where things
    usually take much longer) we don't take so many samples (otherwise
    running the benchmark takes a really long time).
  */
  int u,tempnum; 
  for (nd = nstart; nd <= nmax; nd *= nfac) {
  	double temp=0, t_min=10000000 , t_max=0,temp_time_avg=0;
		  	for (u=1;u<=10;u++){
    int nn;
    double tstart, tstop;
    
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
    
    tstart = clockms ();
    combsort (dup, nn);
    tstop = clockms ();
     temp= tstop - tstart;
      temp_time_avg += temp;
     tempnum=nn;
      if (t_min>temp){
      	t_min=temp;
      }
      	if (t_max<temp){
      	t_max=temp;
      }
}
     printf (" %8d  %8g  %8g  %8g  %8g\n",tempnum,temp, t_min,t_max,(temp_time_avg / 10));
    
//    printf ("  %8g\n", tstop - tstart);
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













/*
#include<stdio.h>
#define MAX 100
void shell(int s[],int n)
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


void main()
{
	int i,n,s[MAX];
	printf("please enter elements(n <=%d): ",MAX);
	scanf("%d",&n);
	n = n<MAX?n:MAX;
	printf("please enter the %d number:\n",n);
	for(i=0;i<n;i++)
		scanf("%d",&s[i]);
	shell(s,n);
	printf("the shell sort:\n");
	for(i=0;i<n;i++)
		printf("%d ",s[i]);
	printf("\n");
}*/
