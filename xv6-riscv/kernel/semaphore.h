//Advanced lock for processes to coordinate their activities
struct semaphore {
    struct spinlock lock;   //spinlock protecting the semaphore
    int count;  //number of processes that can enter critical section
    char *name;
};