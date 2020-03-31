#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#define NEW 0
#define READY 1
#define WAITING 2
#define RUNNING 3
#define EXIT 4
#define SRT 0
#define RR 1
int system_time=0,runningtime=0,AllEx=0,Context_Switch_Permitted=1,AlreadyRuns=0,n,Cur_Mode=RR,TymQuanta;
//allex is used for when all the program exited from both the queues Cur_mode tells which queue is running at the time
struct Proc_Det 
{	int pid;
	int state;
	int more_to_run;
	int at;
	int waitingtime,turnAroundTime,ct,exect,qno;
	struct Proc_Det *prev;
	struct Proc_Det *next;
} *process_initial;
struct Queue
{
struct Proc_Det *front ,*rear;
}*RQ1,*RQ2;
void enqueuep(struct Proc_Det *pros)if
(RQ1->front==NULL)
	{
	RQ1->front=pros;
	RQ1->rear=pros;
	pros->next=NULL;
	}
	else
	{
	if(pros->more_to_run<RQ1->front->more_to_run)
           {
	pros->next=RQ1->front;
	RQ1->front->prev=pros;
	RQ1->front=pros;
	}
               else if(pros->more_to_run==RQ1->front->more_to_run)
	{
	pros->next=RQ1->front->next;
	pros->prev=RQ1->front;
	RQ1->front->next=pros;
           if(pros->next!=NULL)
           {
            pros->next->prev=pro
            }
                }
	else if(pros->more_to_run>RQ1->rear->more_to_run)
	{
	pros->next=NULL;
	RQ1->rear->next=pros;
	pros->prev=RQ1->rear;
	RQ1->rear=pros;
             }
	else
	{
              struct Proc_Det *start=RQ1->front->next;
              while(start->more_to_run<pros->more_to_run)
            {
	   start=start->next;
  }
                   if(start!=NULL&& pros->more_to_run==start->more_to_run)
                 {
                 pros->next=start->next;
                          start->next=pros;
                          pros->prev=start;
 }
                  else
               {
		   (start->prev)->next=pros;
		   pros->next=start;
		   pros->prev=start->prev;
		   start->prev=pros;
                }
		}
	}
}
struct Proc_Det *  deQueuep()
{
     if(RQ1->front==NULL)
     {
	     return NULL;
     }
     struct Proc_Det * temp=RQ1->front;
     RQ1->front=RQ1->front->next;
     temp->next=NULL;
     if(RQ1->front==NULL)
     {
	     RQ1->rear=NULL;
     }
     return temp;
}
void enqueue(struct Proc_Det *pros)
{
      if(RQ2->front==NULL)
      { 
         pros->prev=NULL;
         pros->next=NULL;
         RQ2->front=pros;           
         RQ2->rear=pros;
         return;
      }
     pros->prev=RQ2->rear;
     RQ2->rear->next=pros;
     RQ2->rear=pros;
}
struct Proc_Det * deQueue()
{
if(RQ2->front==NULL)
     {
	     return NULL;
     }
     struct Proc_Det * temp=RQ2->front;
     RQ2->front=RQ2->front->next;
     temp->next=NULL;
     if(RQ2->front==NULL)
     {
	     RQ2->rear=NULL;
     }
     return temp;
}
int main()
{
        RQ1 =(struct Queue*) malloc(sizeof(struct Queue));
        RQ2 =(struct Queue*) malloc(sizeof(struct Queue));
	printf("Please enter No of processes to schedule **  \n");
	scanf("%d",&n);
	printf("\nPlease Enter Time Quantum ** \n");
	scanf("%d",&TymQuanta);
	process_initial=(struct Proc_Det *)malloc(sizeof(struct Proc_Det)*n);
	for(int i=0;i<n;i++)
	{
             printf("\n\nEnter Process Id For %d Process ** \n",(i+1));
	     scanf("%d",&(process_initial[i].pid));
	     printf("\nEnter arrival time For %d Process **\n",(i+1));
	     scanf("%d",&(process_initial[i].at));
	     printf("\nEnter Execution time For %d Process ** \n",(i+1));
	     scanf("%d",&(process_initial[i].more_to_run));
	     printf("\nEnter QueueNo For %d Process \n",(i+1));
	     scanf("%d",&(process_initial[i].qno)); 
             process_initial[i].exect=process_initial[i].more_to_run;
	    process_initial[i].state=NEW;        
	}

    struct Proc_Det key; 
    int i,j;
    for (i = 1; i < n; i++) { 
        key = process_initial[i]; 
        j = i - 1; 
        while (j >= 0 && process_initial[j].at > key.at) { 
            process_initial[j + 1] = process_initial[j]; 
            j = j - 1; 
        } 
        process_initial[j + 1] = key; 
    }  
        struct Proc_Det *runing_pros=NULL;
       while(1)
       {
            int cnt=0,i;
           for(i=0;i<n;i++)
	   {
		   if(process_initial[i].state == NEW && system_time>=process_initial[i].at)
		   {      
			   if(process_initial[i].qno==1)
                           {
			       enqueuep(&process_initial[i]);
			       process_initial[i].state=READY;
			   }
                           else if(process_initial[i].qno==2)
                          {
                                enqueue(&process_initial[i]);
                                process_initial[i].state=READY;
                          }
                    }
                  if(process_initial[i].state==EXIT)
                  {
                    cnt++;
			}
	   }
	   if(cnt==n)
	   {
		  AllEx=1; 
		  
	   }
            if(AllEx)
            {
                  break;
            }
           if(RQ1->front!=NULL &&  AlreadyRuns==0)
           {
                    if(runing_pros!=NULL && runing_pros->qno==2)
                    {
                        enqueue(runing_pros);
                        runing_pros->state=READY;
                        runing_pros=NULL;
                     }
                      
		     Cur_Mode=SRT;
                     runningtime=1;
	     	     runing_pros=deQueuep();
             	     runing_pros->state=RUNNING;
             	     runing_pros->more_to_run--;
	             system_time++;
                    if(runningtime==runing_pros->exect)
                    {
                  	Context_Switch_Permitted=1;
                        AlreadyRuns =0;
                  	runing_pros->state=EXIT;
                  	runing_pros->ct=system_time;
                  	runing_pros->turnAroundTime=system_time-runing_pros->at;
                        runing_pros->waitingtime=runing_pros->turnAroundTime-runing_pros->exect;
                        runing_pros=NULL;
           
                    }
                    else
                    {
                       Context_Switch_Permitted=0;
                       AlreadyRuns=1;
                     }
            }
          else if(RQ2->front!=NULL&& RQ1->front==NULL && runing_pros==NULL && Context_Switch_Permitted==1)
         {
                     Cur_Mode=RR;
                     runningtime=1;
	     	     runing_pros=deQueue();
             	     runing_pros->state=RUNNING;
             	     runing_pros->more_to_run--;
	             system_time++;
                    if(runningtime==runing_pros->exect)
                    {
                  	Context_Switch_Permitted=1;
                  	runing_pros->state=EXIT;
                  	runing_pros->ct=system_time;
                  	runing_pros->turnAroundTime=system_time-runing_pros->at;
                        runing_pros->waitingtime=runing_pros->turnAroundTime-runing_pros->exect;
                        runing_pros=NULL;
           
                    }
                    else
                    {
                       Context_Switch_Permitted=0;
                     }                 
         }
         else if(runing_pros!=NULL && runing_pros->qno==1 && Context_Switch_Permitted==0&&runing_pros->state==RUNNING)
         {
                  
                     if(runing_pros->more_to_run==0)
             {
                  Context_Switch_Permitted=1;
                  AlreadyRuns=0;
                  runing_pros->state=EXIT;
                  runing_pros->ct=system_time;
                  runing_pros->turnAroundTime=system_time-(runing_pros->at);
                  runing_pros->waitingtime=(runing_pros->turnAroundTime)-(runing_pros->exect);
                  runing_pros=NULL;
                  continue;
             }
	if(RQ1->front!=NULL)
             {
            if(runing_pros->more_to_run>RQ1->front->more_to_run)
               {
                runing_pros->state=READY;
                enqueuep(runing_pros);
                AlreadyRuns=0;
                runing_pros=NULL;
                Context_Switch_Permitted=1;
               continue;
               }
             }    
             runningtime++;
             runing_pros->more_to_run--;
            system_time++
             if(runing_pros->more_to_run==0)
             {
                  Context_Switch_Permitted=1;
                  AlreadyRuns=0;
                  runing_pros->state=EXIT;
                  runing_pros->ct=system_time;
                  runing_pros->turnAroundTime=system_time-(runing_pros->at);
                  runing_pros->waitingtime=(runing_pros->turnAroundTime)-(runing_pros->exect);
                  runing_pros=NULL;
             }            
             else if(RQ1->front!=NULL)
             {
               if(runing_pros->more_to_run>RQ1->front->more_to_run)
               {
                runing_pros->state=READY;
                enqueuep(runing_pros);
                AlreadyRuns=0;
                runing_pros=NULL;
                Context_Switch_Permitted=1;
               }
               else
               {
                  Context_Switch_Permitted=0;
                 AlreadyRuns=1;
               }
              }
            else
          { 
              Context_Switch_Permitted=0;
             AlreadyRuns=1;
	  }            
         }
         else if(runing_pros!=NULL && runing_pros->qno==2&& runing_pros->state==RUNNING && Context_Switch_Permitted==0)
         {
               if(runing_pros->more_to_run==0)
             {
                  Context_Switch_Permitted=1;
                  runing_pros->state=EXIT;
                  runing_pros->ct=system_time;
                  runing_pros->turnAroundTime=system_time-(runing_pros->at);
                  runing_pros->waitingtime=(runing_pros->turnAroundTime)-(runing_pros->exect);
                  continue;
             }
              else if(runningtime==TymQuanta)
             {
                runing_pros->state=READY;
                enqueue(runing_pros);
                runing_pros=NULL;
                Context_Switch_Permitted=1;
                continue;
              }      
             runningtime++;
             runing_pros->more_to_run--;
            system_time++;
             if(runing_pros->more_to_run==0)
             {
                  Context_Switch_Permitted=1;
                  runing_pros->state=EXIT;
                  runing_pros->ct=system_time;
                  runing_pros->turnAroundTime=system_time-(runing_pros->at);
                  runing_pros->waitingtime=(runing_pros->turnAroundTime)-(runing_pros->exect);
             }            
             else if(runningtime==TymQuanta)
             {
                runing_pros->state=READY;
                enqueue(runing_pros);
                runing_pros=NULL;
                Context_Switch_Permitted=1;
              }
            else
          { 
              Context_Switch_Permitted=0;
	  }
         }
        else
        {
        
          system_time++
         } }
      int sumwt=0,sumtat=0;
for(int i=0;i<n;i++)
	{
           printf("\tprocess id=%d\t\t\tat=%d\t\t\tbt=%d\t\t\tct=%d\t\t\tTAT=%d\t\t\tWT=%d\n",process_initial[i].pid,process_initial[i].at,process_initial[i].exect,process_initial[i].ct,process_initial[i].turnAroundTime,process_initial[i].waitingtime);
           sumwt+=process_initial[i].waitingtime;
           sumtat+=process_initial[i].turnAroundTime
	}
printf("\n\n Avergae TurnAroundTime=%f \t AverageWaitingTime=%f\n\n",(sumtat/(n*1.0)),(sumwt/(n*1.0)));
printf("\n\n\n *****************\n\n\n");	
}
