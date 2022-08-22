#include"stdio.h"
#include"stdint.h"
#include"pthread.h"
#include"time.h"
#include"string.h"
#include"stdbool.h"
#include"stdlib.h"
#include "unistd.h"

struct timespec tp;
struct timespec tmp,request;
//uint8_t check_loop = 1;

long freq = 100000;
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  

// pthread_cond_t cond = PTHREAD_COND_INITIALIZER;   

void *getFreq(void *args)
{

struct sched_param sp;
        sp.sched_priority = 10;
        pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp);

  while(1)

{
  //pthread_mutex_lock(&mutex);

  FILE *fp;

  fp = fopen("freq.txt","r");

   unsigned long new_freq = 0;

  fscanf(fp,"%lu",&new_freq);

  fclose(fp); 

  if(new_freq != freq)
  {  
        freq = new_freq;
  }

}



}

 void *getTime(void *args)

{ 
    struct sched_param sp;
    sp.sched_priority = 70;
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp);

    clock_gettime(CLOCK_MONOTONIC, &request);

    while (1)
    {  clock_gettime(CLOCK_REALTIME, &tp);
    	request.tv_nsec += freq;
       
        if(request.tv_nsec > 1000000000) {
                request.tv_nsec -= 1000*1000*1000;
                request.tv_sec++;
        }
           clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &request, NULL);
	}

   
}



void *save_time(void *args)

{
    struct sched_param sp;
        sp.sched_priority = 40;
        pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp);
 
    FILE *file ;
    file = fopen("time_and_interval.txt","a+");

  //save time
  while(1)
  {

    long diff_sec = tp.tv_sec - tmp.tv_sec ;

    long diff_nsec;

    if(tmp.tv_nsec != tp.tv_nsec || tmp.tv_sec != tp.tv_sec)

    {

      if(tp.tv_nsec > tmp.tv_nsec)

        {

          diff_nsec = tp.tv_nsec - tmp.tv_nsec;

        }

      else 

        {

          diff_nsec = 1000000000 + tp.tv_nsec - tmp.tv_nsec ;

          diff_sec = diff_sec - 1;

        }

       fprintf(file,"\n%ld",diff_nsec);  

       tmp.tv_nsec =tp.tv_nsec;

       tmp.tv_sec = tp.tv_sec;

    } 
    fclose(file);

  } 

}



int main(int argc, char const *argv[])

{
        pthread_t SAMPLE;
        pthread_t INPUT;
        pthread_t LOGGING;

        int s,i,l;

      tmp.tv_sec = 0;
      tmp.tv_nsec = 0;   

    //pthread_mutex_init(&mutex, NULL); 

       i = pthread_create(&INPUT,NULL,getFreq,NULL);

        s = pthread_create(&SAMPLE, NULL, getTime,NULL);

       l = pthread_create(&LOGGING,NULL,save_time,NULL);      

       pthread_join(INPUT,NULL);

        pthread_join(SAMPLE,NULL);

       pthread_join(LOGGING,NULL);

       //    pthread_mutex_destroy(&mutex);

    

}
