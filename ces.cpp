#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
/*
mutex ��������һ����������Mutual Exclusion Lock��������ȷ������ѧ�ҳ����������ʱ�Ļ����ԡ�ÿ����ѧ���ڽ���������ӵĲ���֮ǰ��
������Ҫ��� mutex ����ͨ����ȡ mutex ����ȷ��ÿ��ֻ����һ����ѧ���ڳ���������ӣ������˾��������ķ�����

forks ��������һ�����飬����ÿ����ѧ�Ҷ�Ӧһ�� forks ����forks �����ڱ�ʾ���ӵ�״̬��ÿ�����Ӷ�Ӧһ������
��ѧ���ڳ����������ʱ����Ҫ���λ�ȡ���ֺ����ֲ��Ӷ�Ӧ���������ھͲͽ������ͷ���Щ����ͨ���� forks ������ȷ��ȡ���ͷţ�
ȷ����ÿ����ѧ���ܹ���ȷ��ȡ�������ӣ�����������������ķ���
*/
pthread_mutex_t mutex;
pthread_mutex_t forks[5];

void* philosopher(void* arg)
{
    int philosopher_id=*(int*)arg;
    int left_fork=philosopher_id;
    int right_fork=(philosopher_id+1)%5;

     // ģ��˼��
    printf("��ѧ�� %d ����˼��...\n", philosopher_id);
    sleep(rand() % 3 + 1);

    pthread_mutex_lock(&mutex);

    pthread_mutex_lock(&forks[left_fork]);
    pthread_mutex_lock(&forks[right_fork]);

    // �Ͳ�
    printf("��ѧ�� %d �����˲��� %d �� %d����ʼ�Ͳ�...\n", philosopher_id, left_fork, right_fork);
    pthread_mutex_unlock(&mutex);
    sleep(rand() % 3 + 1);
    

    //���²���
    pthread_mutex_unlock(&forks[left_fork]);
    pthread_mutex_unlock(&forks[right_fork]);

    printf("��ѧ�� %d �����˲��� %d �� %d���Ͳͽ�����\n", philosopher_id, left_fork, right_fork);

    
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
    // �ȴ���ѧ���߳̽���
    for (int i = 0; i < 5; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // ���ٻ�����
    for (int i = 0; i < 5; i++) {
        pthread_mutex_destroy(&forks[i]);
    }
    pthread_mutex_destroy(&mutex);

     return 0;

}

