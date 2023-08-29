#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
/*
mutex 锁：它是一个互斥锁（Mutual Exclusion Lock），用于确保在哲学家尝试拿起叉子时的互斥性。每个哲学家在进行拿起叉子的操作之前，
首先需要获得 mutex 锁。通过获取 mutex 锁，确保每次只能有一个哲学家在尝试拿起叉子，避免了竞争条件的发生。

forks 锁：它是一个数组，并且每个哲学家对应一个 forks 锁。forks 锁用于表示叉子的状态，每根叉子对应一个锁。
哲学家在尝试拿起叉子时，需要依次获取左手和右手叉子对应的锁，并在就餐结束后释放这些锁。通过对 forks 锁的正确获取和释放，
确保了每个哲学家能够正确获取两根叉子，并避免了死锁问题的发生
*/
pthread_mutex_t mutex;
pthread_mutex_t forks[5];

void* philosopher(void* arg)
{
    int philosopher_id=*(int*)arg;
    int left_fork=philosopher_id;
    int right_fork=(philosopher_id+1)%5;

     // 模拟思考
    printf("哲学家 %d 正在思考...\n", philosopher_id);
    sleep(rand() % 3 + 1);

    pthread_mutex_lock(&mutex);

    pthread_mutex_lock(&forks[left_fork]);
    pthread_mutex_lock(&forks[right_fork]);

    // 就餐
    printf("哲学家 %d 拿起了叉子 %d 和 %d，开始就餐...\n", philosopher_id, left_fork, right_fork);
    pthread_mutex_unlock(&mutex);
    sleep(rand() % 3 + 1);
    

    //放下叉子
    pthread_mutex_unlock(&forks[left_fork]);
    pthread_mutex_unlock(&forks[right_fork]);

    printf("哲学家 %d 放下了叉子 %d 和 %d，就餐结束。\n", philosopher_id, left_fork, right_fork);

    
    pthread_exit(NULL);


}


int main()
{
    srand(time(NULL));
    pthread_t philosophers[5];
    int philosopher_ids[5];

    for(int i=0;i<5;i++)
    {
        pthread_mutex_init(&forks[i],NULL);
    }
    pthread_mutex_init(&mutex,NULL);

    for(int i=0;i<5;i++)
    {
        philosopher_ids[i]=i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }
    // 等待哲学家线程结束
    for (int i = 0; i < 5; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // 销毁互斥锁
    for (int i = 0; i < 5; i++) {
        pthread_mutex_destroy(&forks[i]);
    }
    pthread_mutex_destroy(&mutex);

     return 0;

}

