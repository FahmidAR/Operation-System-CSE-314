#include<cstdio>
#include<pthread.h>
#include<semaphore.h>
#include<queue>
#include <unistd.h>
#include<time.h>

#include<bits/stdc++.h>

using namespace std;

#define DEBUG 0
#define PERSON_NO 20
#define SLEEP_ON_OFF 1

#define FILE_INPUT 1
#define FILE_OUPUT 1

time_t startTime , endTime;

//sem_t kioskFull, secuirityFull ;
sem_t kioskEmpty, secuirityEmpty ;

pthread_mutex_t printLock , justLock;
pthread_mutex_t boardingLock , spKioLock ;
pthread_mutex_t leftCountMtx , rightCountMtx;
pthread_mutex_t  changeLeftDirMtx , changeRightDirMtx;
pthread_mutex_t kioskQueLock, secuirityQueLock ;

int m=5,n=2,pp=3;
int w=5,x=10,y=1,z=5;
int left_walking = 0,right_walking = 0;

int poison_lembda =3;

void init_semaphore()
{
    cout<<"1705087 - FAHMID - IPC \n"<<endl;

    cout<<"Total Kiosk no in Airpot = "<<m<<endl;
    cout<<"Total Belt no in Airpot = "<<n<<endl;
    cout<<"Belt Capacity in Airpot = "<<pp<<endl;\
    cout<<"Vip Walkway in Airpot = 1"<<endl;
    cout<<"Special Kiosk in Airpot = 1"<<endl;

    cout<<"\nKiosk Time in Airpot = "<<w<<endl;
    cout<<"Belt Time in Airpot ="<<x<<endl;
    cout<<"Boarding Time in Airpot ="<<y<<endl;\
    cout<<"Vip Walkway Time in Airpot = "<<z<<endl;
    cout<<endl;

    //sem_init(&kioskFull, 0, 0);
    sem_init(&kioskEmpty, 0, m);

    //sem_init(&secuirityFull, 0, 0);
    sem_init(&secuirityEmpty, 0, n*pp);

    pthread_mutex_init(&boardingLock, 0);
    pthread_mutex_init(&spKioLock, 0);

    pthread_mutex_init(&printLock, 0);
    pthread_mutex_init(&justLock, 0);

    pthread_mutex_init(&changeRightDirMtx, 0);
    pthread_mutex_init(&changeLeftDirMtx, 0);

    pthread_mutex_init(&leftCountMtx, 0);
    pthread_mutex_init(&rightCountMtx, 0);
   
}

void des_semaphore()
{


    int return_value = sem_destroy(&kioskEmpty);
	if(return_value) {
		printf("\nkioskEmpty semaphore destruction failed\n");
		fflush(stdout);
	}

    return_value = sem_destroy(&secuirityEmpty);
	if(return_value) {
		printf("\nsecuirityEmpty destruction failed\n");
		fflush(stdout);
	}

    return_value = pthread_mutex_destroy(&justLock);
    if(return_value ) {
        printf("\njustLock mutex destruction failed\n");
        fflush(stdout);
    }

    return_value = pthread_mutex_destroy(&printLock);
    if(return_value ) {
        printf("\nprintLock mutex destruction failed\n");
        fflush(stdout);
    }

    return_value = pthread_mutex_destroy(&boardingLock);
    if(return_value ) {
        printf("\nboardingLock mutex destruction failed\n");
        fflush(stdout);
    }

    return_value = pthread_mutex_destroy(&spKioLock);
    if(return_value ) {
        printf("\nboardingLock mutex destruction failed\n");
        fflush(stdout);
    }

    return_value = pthread_mutex_destroy(&changeRightDirMtx);
    if(return_value) {
        printf("\nchangeDirMtx mutex destruction failed\n");
        fflush(stdout);
    }

    return_value = pthread_mutex_destroy(&changeLeftDirMtx);
    if(return_value) {
        printf("\nchangeDirMtx mutex destruction failed\n");
        fflush(stdout);
    }

    return_value = pthread_mutex_destroy(&leftCountMtx);
    if(return_value ) {
        printf("\nleftCountMtx mutex destruction failed\n");
        fflush(stdout);
    }

    return_value = pthread_mutex_destroy(&rightCountMtx);
    if(return_value ) {
        printf("\nrightCountMtx mutex destruction failed\n");
        fflush(stdout);
    }

    return_value = pthread_mutex_destroy(&boardingLock);
    if(return_value ) {
        printf("\nboardingLock mutex destruction failed\n");
        fflush(stdout);
    }
}  

queue<int> kioskQ , secuirityQ;

class person
{
    public :
    int personId ;
    bool personType ;
    bool forgertBoarding ;

    person(int id )
    {
        personId=id;

        int r = rand()%100+1;

        if(r>70)
        {
            personType=true;
        }
        else
        {
            personType=false;
        }

         r = rand()%100+1;

        if(r>80)
        {
            forgertBoarding=true;
        }
        else
        {
            forgertBoarding=false;
        }

    }

    person(int id,bool vip,bool f)
    {
        personId=id;
        personType=vip;
        forgertBoarding=f;
    }

};

void* take_airport_boarding(void* arg) {
    
    //Arriving at airport 
    //STEP - 1

	pthread_mutex_lock(&printLock);

        endTime = time(&endTime);

        int id = *((int*)arg);
        delete (int*)arg;
        arg = NULL;
        //printf("%d\n",id );
        person p(id);
    
        if(p.personType)
        {
            cout<<"\n->A Passenger(VIP) "<<p.personId<<" ARRIVED at the AIRPORT at "<<endTime-startTime<<endl;
        }
        else
        {
            cout<<"\n->A Passenger "<<p.personId<<" ARRIVED at the AIRPORT at "<<endTime-startTime<<endl;
        }   
    pthread_mutex_unlock(&printLock);

    //All about KIOSK
    //STEP -2

    int kiosk_no ;

    sem_wait(&kioskEmpty);

        pthread_mutex_lock(&kioskQueLock); 

            if(kioskQ.empty())
            {
                kioskQ.push(0);
            }
            
            kiosk_no = kioskQ.back();     //0 1 2 3 4
            kiosk_no = (kiosk_no%m)+1;  //1 2 3 1 2 m=3
            kioskQ.push(kiosk_no);
        
        pthread_mutex_unlock(&kioskQueLock);

        endTime = time(&endTime);

        //sem_getvalue(&kioskEmpty,&kiosk_no);

        pthread_mutex_lock(&printLock);
        if(p.personType)
        {
            cout<<"\n->K Passenger(VIP) "<<p.personId<<" has STARTED SELF-CHECK  in at KIOSK "<<kiosk_no<<" at "<<endTime-startTime<<endl;
        }
        else
        {
            cout<<"\n->K Passenger "<<p.personId<<" has STARTED SELF-CHECK in at KIOSK "<<kiosk_no<<" at "<<endTime-startTime<<endl;   
        }
        pthread_mutex_unlock(&printLock);

        sleep(w);

        endTime = time(&endTime);

        pthread_mutex_lock(&printLock);
        if(p.personType)
        {
            cout<<"\nK-> Passenger(VIP) "<<p.personId<<" has FINISHED CHECK in at time "<<endTime-startTime<<endl;
        }
        else
        {
            cout<<"\nK-> Passenger "<<p.personId<<" has FINISHED CHECK in at time "<<endTime-startTime<<endl;   
        }
        pthread_mutex_unlock(&printLock);

        pthread_mutex_lock(&kioskQueLock); 

        kioskQ.pop();
        
        pthread_mutex_unlock(&kioskQueLock);

    sem_post(&kioskEmpty);

    //STEP -3

    if(p.personType)
    {
        //Vip passeenger in VIP left CHANNEL -First time

        pthread_mutex_lock(&printLock);
                
            endTime = time(&endTime);

            cout<<"\n->WVL Passenger(VIP) "<<p.personId<<" started WAITING in VIP channel (LEFT) at time "<<endTime-startTime<<endl;

        pthread_mutex_unlock(&printLock);

        pthread_mutex_lock(&leftCountMtx);

            left_walking=left_walking+1;
            endTime = time(&endTime);

            if(left_walking==1)
            {
                pthread_mutex_lock(&changeRightDirMtx);
            }

            pthread_mutex_lock(&changeLeftDirMtx);
            pthread_mutex_unlock(&changeLeftDirMtx); // giving walkikg right one to finish walking

            pthread_mutex_lock(&printLock);
            cout<<"\n->VL Passenger(VIP) "<<p.personId<<" started WALKING in VIP channel (LEFT) at time "<<endTime-startTime<<endl; 
            pthread_mutex_unlock(&printLock);

        pthread_mutex_unlock(&leftCountMtx);

        sleep(z);

        pthread_mutex_lock(&leftCountMtx);

            left_walking=left_walking-1;
            endTime = time(&endTime);

            pthread_mutex_lock(&printLock);
            cout<<"\nVL-> Passenger(VIP) "<<p.personId<<" has CROSSED the VIP channel (LEFT)  in at time "<<endTime-startTime<<endl; 
            pthread_mutex_unlock(&printLock);

            if(left_walking==0)
            {
                pthread_mutex_unlock(&changeRightDirMtx);
            }

        pthread_mutex_unlock(&leftCountMtx);

    }
    else
    { 
        //All about SECURITY BELT 

        int security_belt_no_np ,security_belt_no_n ;

        pthread_mutex_lock(&printLock);
            
            endTime = time(&endTime);
            
            cout<<"\n->WS Passenger "<<p.personId<<" started WAITING for SECURITY-CHECK from time "<<endTime-startTime<<endl;

        pthread_mutex_unlock(&printLock);

        sem_wait(&secuirityEmpty);

            pthread_mutex_lock(&secuirityQueLock); 

                if(secuirityQ.empty())
                {
                    secuirityQ.push(0);
                }
                
                security_belt_no_np = secuirityQ.back();     //0 1 2 3 4
                security_belt_no_np = (security_belt_no_np%(n*pp))+1;  //1 2 3 4 1 n*p=4
                secuirityQ.push(security_belt_no_np);

                security_belt_no_n = security_belt_no_np%n+1; //2 1 2 1 2
            
            pthread_mutex_unlock(&secuirityQueLock);

            endTime = time(&endTime);

            //sem_getvalue(&secuirityEmpty,&security_belt_no);

            pthread_mutex_lock(&printLock);
            cout<<"\n->S Passenger "<<p.personId<<" has STARTED SECURITY-CHECK  in at BELT "<<security_belt_no_n<<" at "<<endTime-startTime<<endl;
            //cout<<"\n->Ssssss Passenger "<<p.personId<<" has STARTED SECURITY-CHECK  in at BELT (NP) "<<security_belt_no_np<<" at "<<endTime-startTime<<endl;
            pthread_mutex_unlock(&printLock);

            sleep(x);

            endTime = time(&endTime);

            pthread_mutex_lock(&printLock);
            cout<<"\nS-> Passenger "<<p.personId<<" has CROSSED the SECURITY-CHECK  in at time "<<endTime-startTime<<endl; 
            pthread_mutex_unlock(&printLock);

            pthread_mutex_lock(&secuirityQueLock); 

            secuirityQ.pop();
            
            pthread_mutex_unlock(&secuirityQueLock);

        sem_post(&secuirityEmpty);

    }

    //STEP -4
    //if(p.forgertBoarding)
    while(p.forgertBoarding)
        {
            //BOARDING 2-Forget ticket 

            pthread_mutex_lock(&printLock);
                
                endTime = time(&endTime);

                if(p.personType)
                {
                    cout<<"\n->WVR Passenger(VIP) "<<p.personId<<" has FORGOTED boarding pass, WAITING in VIP channel (RIGHT) at time "<<endTime-startTime<<endl;
                }
                else
                {
                    cout<<"\n->WVR Passenger "<<p.personId<<" has FORGOTED boarding pass, WAITING in VIP channel (RIGHT) at time "<<endTime-startTime<<endl;  
                }

            pthread_mutex_unlock(&printLock);


            //Using Right Vip Channel

            pthread_mutex_lock(&changeRightDirMtx);
            pthread_mutex_unlock(&changeRightDirMtx); // checked no one in left waiting before start (Priority )

            pthread_mutex_lock(&rightCountMtx);

            right_walking=right_walking+1;
            endTime = time(&endTime);

            if(right_walking==1)
            {
                pthread_mutex_lock(&changeLeftDirMtx);
            }

            pthread_mutex_lock(&printLock);

                if(p.personType)
                {
                    cout<<"\n->VR Passenger(VIP) "<<p.personId<<" started WALKING in VIP channel (RIGHT) at time "<<endTime-startTime<<endl;
                }
                else
                {
                    cout<<"\n->VR Passenger "<<p.personId<<" started WALKING in VIP channel (RIGHT) at time "<<endTime-startTime<<endl;
                }
             
            pthread_mutex_unlock(&printLock);

            pthread_mutex_unlock(&rightCountMtx);

            sleep(z);

            pthread_mutex_lock(&rightCountMtx);

                right_walking=right_walking-1;
                endTime = time(&endTime);

                pthread_mutex_lock(&printLock);

                if(p.personType)
                {
                    cout<<"\nVR-> Passenger(VIP) "<<p.personId<<" has CROSSED the VIP channel (RIGHT)  in at time "<<endTime-startTime<<endl;
                }
                else
                {
                    cout<<"\nVR-> Passenger "<<p.personId<<" has CROSSED the VIP channel (RIGHT)  in at time "<<endTime-startTime<<endl;
                }
                 
                pthread_mutex_unlock(&printLock);

                if(right_walking==0)
                {
                    pthread_mutex_unlock(&changeLeftDirMtx);
                }

            pthread_mutex_unlock(&rightCountMtx);

            //Special one Kiosk - For re - borading pass

             pthread_mutex_lock(&printLock);
            
                endTime = time(&endTime);

                if(p.personType)
                {
                    cout<<"\n->WSK Passenger(VIP) "<<p.personId<<" started WAITING for RE-SELF-CHECK in SPECIAL KIOSK from time "<<endTime-startTime<<endl;
                }
                else
                {
                    cout<<"\n->WSK Passenger "<<p.personId<<" started WAITING for RE-SELF-CHECK in SPECIAL KIOSK from time "<<endTime-startTime<<endl;  
                }

            pthread_mutex_unlock(&printLock);

            pthread_mutex_lock(&spKioLock);

                endTime = time(&endTime);

                sem_getvalue(&kioskEmpty,&kiosk_no);

                pthread_mutex_lock(&printLock);
                if(p.personType)
                {
                cout<<"\n->SK Passenger(VIP) "<<p.personId<<" has STARTED RE-SELF-CHECK  in SPECIAL KIOSK at "<<endTime-startTime<<endl;
                }
                else
                {
                cout<<"\n->SK Passenger "<<p.personId<<" has STARTED RE-SELF-CHECK in SPECIAL KIOSK at "<<endTime-startTime<<endl;   
                }
                pthread_mutex_unlock(&printLock);

                sleep(w);

                endTime = time(&endTime);

                pthread_mutex_lock(&printLock);
                if(p.personType)
                {
                cout<<"\nSK-> Passenger(VIP) "<<p.personId<<" has FINISHED CHECK in at time "<<endTime-startTime<<endl;
                }
                else
                {
                cout<<"\nSK-> Passenger "<<p.personId<<" has FINISHED CHECK in at time "<<endTime-startTime<<endl;   
                }
                pthread_mutex_unlock(&printLock);

            pthread_mutex_unlock(&spKioLock);

            //ALL passeenger in VIP left CHANNEL -Second time

            pthread_mutex_lock(&printLock);
                    
                endTime = time(&endTime);

                if(p.personType)
                {
                cout<<"\n->WVLF Passenger(VIP) "<<p.personId<<" started Re-WAITING in VIP channel (LEFT) at time "<<endTime-startTime<<endl;
                }
                else
                {
                cout<<"\n->WVLF Passenger "<<p.personId<<" started Re-WAITING in VIP channel (LEFT) at time "<<endTime-startTime<<endl;   
                }

            pthread_mutex_unlock(&printLock);

            pthread_mutex_lock(&leftCountMtx);

                left_walking=left_walking+1;
                endTime = time(&endTime);

                if(left_walking==1)
                {
                    pthread_mutex_lock(&changeRightDirMtx);
                }

                pthread_mutex_lock(&printLock);

                if(p.personType)
                {
                cout<<"\n->VLF Passenger(VIP) "<<p.personId<<" started Re-WALKING in VIP channel (LEFT) at time "<<endTime-startTime<<endl;
                }
                else
                {
                cout<<"\n->VLF Passenger "<<p.personId<<" started Re-WALKING in VIP channel (LEFT) at time "<<endTime-startTime<<endl;   
                }
                
                pthread_mutex_unlock(&printLock);

            pthread_mutex_unlock(&leftCountMtx);

            pthread_mutex_lock(&changeLeftDirMtx);
            pthread_mutex_unlock(&changeLeftDirMtx); // giving walkikg right one to finish walking

            sleep(z);

            pthread_mutex_lock(&leftCountMtx);

                left_walking=left_walking-1;
                endTime = time(&endTime);

                pthread_mutex_lock(&printLock);

                if(p.personType)
                {
                cout<<"\nVLF-> Passenger(VIP) "<<p.personId<<" has Re-CROSSED the VIP channel (LEFT)  in at time "<<endTime-startTime<<endl;
                }
                else
                {
                cout<<"\nVLF-> Passenger "<<p.personId<<" has Re-CROSSED the VIP channel (LEFT)  in at time "<<endTime-startTime<<endl;   
                }
                  
                pthread_mutex_unlock(&printLock);

                if(left_walking==0)
                {
                    pthread_mutex_unlock(&changeRightDirMtx);
                }

            pthread_mutex_unlock(&leftCountMtx);

            pthread_mutex_lock(&justLock);

            int r = rand()%100+1;

            if(r>10)
            {
                p.forgertBoarding=false;
            } 

            //if(p.forgertBoarding==false) break;

            pthread_mutex_unlock(&justLock);

            
        }
        
        //BOARDING for ALL (Forget/Not Forget) ticket 
        //Step 5

        pthread_mutex_lock(&printLock);
            
            endTime = time(&endTime);

            if(p.personType)
            {
                cout<<"\n->WB Passenger(VIP) "<<p.personId<<" has started WAITING to be boarded at time "<<endTime-startTime<<endl;
            }
            else
            {
                cout<<"\n->WB Passenger "<<p.personId<<" has started WAITING to be boarded at time "<<endTime-startTime<<endl;  
            }

        pthread_mutex_unlock(&printLock);

        pthread_mutex_lock(&boardingLock);

            endTime = time(&endTime);

            pthread_mutex_lock(&printLock);

            if(p.personType)
            {
                cout<<"\n->B Passenger(VIP) "<<p.personId<<" has started BOARDING the plane at time "<<endTime-startTime<<endl;
            }
            else
            {
                cout<<"\n->B Passenger "<<p.personId<<" has started BOARDING the plane at time "<<endTime-startTime<<endl;  
            }
            
            pthread_mutex_unlock(&printLock);

            sleep(y);

            endTime = time(&endTime);

            pthread_mutex_lock(&printLock);

            if(p.personType)
            {
                cout<<"\nB-> Passenger(VIP) "<<p.personId<<" has BOARDED the plane at time "<<endTime-startTime<<endl;
            }
            else
            {
                cout<<"\nB-> Passenger "<<p.personId<<" has BOARDED the plane at time "<<endTime-startTime<<endl;  
            }
            
            pthread_mutex_unlock(&printLock);

        pthread_mutex_unlock(&boardingLock);

        

    return 0;

}

int main(void)
{   

    if(FILE_INPUT)
    {
        string filename("input.txt");

        ifstream input_file(filename);
        if (!input_file.is_open()) {
            cerr << "Could not open the file - '"
                << filename << "'" << endl;
            return EXIT_FAILURE;
        }

        input_file >> m >> n >> pp ;
        input_file >> w >> x >> y >> z ;

        if(DEBUG)
        {
            cout << m << " "<< n << " "<< pp <<endl;
            cout << w << " "<< x << " "<< y << " "<< z <<endl;
        }    
        
    }

    if(FILE_OUPUT)
    {
        freopen("output.txt", "w", stdout);
    }
    

    pthread_t person[PERSON_NO];

    srand(time(0));

	init_semaphore();

    int return_value;

    default_random_engine generator;
    int person_per_min = PERSON_NO;
    int poison_lembda= 60/person_per_min;
    poisson_distribution<int> distribution(poison_lembda);

    startTime = time(&startTime);
    

	for(int id=1; id<=person_per_min; id++) {

        int* idd = new int(id);
    
		return_value = pthread_create(&person[id], NULL, take_airport_boarding, (void*)idd);
	
    	if(return_value) {
			printf("person[%d] thread creation failed\n", id);
			fflush(stdout);
		}

        int arival_time_interval = distribution(generator);

        sleep(arival_time_interval);
	}


	for(int id=1; id<=person_per_min; id++) {

		return_value = pthread_join(person[id], NULL);

		if(return_value) {
			printf("person[%d] thread join failed\n", id);
			fflush(stdout);
		}
	}

	//while(1);
    des_semaphore();
	return 0;
}