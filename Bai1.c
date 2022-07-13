#include"stdio.h"
#include"stdint.h"
#include"pthread.h"
#include"time.h"
#include"string.h"
#include"stdlib.h"
#include "unistd.h"
struct timespec tp;
struct timespec tmp;

long freq;
long check_freq;
struct timespec time1, time2;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   
 long get_freq()
{
  long data;
  FILE *fp;
  fp = fopen("freq.txt","r");
  char buff[100];
  fgets(buff,sizeof(buff),fp);
  char *eptr;
  data = strtol(buff,&eptr,10);
  fclose(fp);
  return data;
 }
 void *getTime(void *args )
{   
    long x = *((long*)args);
    clock_gettime(CLOCK_REALTIME,&tp);  
    return NULL;
}
void *getFreq(void *args)
{
  pthread_mutex_lock(&mutex);
  long new_freq = get_freq();
  if(new_freq == freq)
  {  
    pthread_mutex_unlock(&mutex);
    return NULL;
  }
    else
    {
    freq = new_freq;
    time1.tv_sec = 0;
      time1.tv_nsec = freq;
    } 
      pthread_mutex_unlock(&mutex);
      return NULL;

}
void *save_time(void *args)
{
  struct timespec *tpx = (struct timespec *)args;
  //save time
  if(tmp.tv_sec != tpx->tv_sec | tmp.tv_nsec != tpx->tv_nsec)
  {
    FILE *file;
    FILE *file2;
    file = fopen("time_and_interval.txt","a+");    long diff_sec = ((long) tpx->tv_sec) - tmp.tv_sec ;
    file2 =fopen("interval.dat","a+");
    long diff_nsec;

    if(tpx->tv_nsec > tmp.tv_nsec)
      {
        diff_nsec = tpx->tv_nsec - tmp.tv_nsec;
      }
    else 
      {
        diff_nsec = tmp.tv_nsec - tpx->tv_nsec;
        diff_sec = diff_sec - 1;
      }
    fprintf(file,"\n%ld.%09ld\n%ld.%09ld",tpx->tv_sec,tpx->tv_nsec,diff_sec,diff_nsec);  
    fprintf(file2,"\n%ld.%09ld",diff_sec,diff_nsec);  
   
    fclose(file);
    fclose(file2);
    tmp.tv_sec = tpx->tv_sec;
    tmp.tv_nsec = tpx->tv_nsec;
    return NULL;
  }
  else
  {
    return NULL;
  }
}

int main(int argc, char const *argv[])
{
    FILE *fp;
    freq = get_freq();
        pthread_t SAMPLE;
        pthread_t INPUT;
        pthread_t LOGGING;
        int s,i,l;
      time1.tv_sec = 0;
      time1.tv_nsec = freq;

      tmp.tv_sec = 0;
      tmp.tv_nsec = 0;   
       pthread_mutex_init(&mutex, NULL);      
      while(1)
        {

        if(nanosleep(&time1 , &time2) < 0 )   
	      {
	        printf("Nano sleep system call failed \n");
	        return -1;
	      }
    	else
          	{ 
	            i = pthread_create(&INPUT,NULL,getFreq,&check_freq);
	            s = pthread_create(&SAMPLE, NULL, getTime,&freq);
	            l = pthread_create(&LOGGING,NULL,save_time,&tp);
	            pthread_join(INPUT,NULL);
	            pthread_join(SAMPLE,NULL);
	        	pthread_join(LOGGING,NULL);
	        	pthread_mutex_destroy(&mutex);
      		}
        } 
        return 0;
}
