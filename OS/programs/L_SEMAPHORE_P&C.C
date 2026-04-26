#include <stdio.h>
#include <stdlib.h>
 
int mutex = 1, full = 0, empty = 3, x = 0;
 
int wait(int s){
    return(--s);
}
 
int signal(int s){
    return(++s);
}
 
void producer(){
    empty = wait(empty);
    mutex = wait(mutex);
 
    x++;
    printf("\nProducer produces item %d", x);
 
    mutex = signal(mutex);
    full = signal(full);
}
 
void consumer(){
    full = wait(full);
    mutex = wait(mutex);
 
    printf("\nConsumer consumes item %d", x);
    x--;
 
    mutex = signal(mutex);
    empty = signal(empty);
}
 
int main(){
    int n;
 
    printf("\n1.PRODUCER\n2.CONSUMER\n3.EXIT\n");
 
    while(1){
        printf("\n\nENTER YOUR CHOICE: ");
        scanf("%d", &n);
 
        switch(n){
            case 1:
                if((mutex == 1) && (empty != 0))
                    producer();
                else
                    printf("BUFFER IS FULL!");
                break;
 
            case 2:
                if((mutex == 1) && (full != 0))
                    consumer();
                else
                    printf("BUFFER IS EMPTY!");
                break;
 
            case 3:
                exit(0);
                break;
        }
    }
 
    return 0;
}
 
