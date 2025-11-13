#include "kernel/types.h"
#include "user/user.h"

int main(void)
{
    int parent2child[2];
    int child2parent[2];
    pipe(parent2child);
    pipe(child2parent);

    int pid = fork();

    if (pid == 0) 
    {
        // --- Child ---
        char buf[1];

        // Đóng các đầu không dùng để tránh deadlock
        close(parent2child[1]); // đóng đầu ghi của parent2child
        close(child2parent[0]); // đóng đầu đọc của child2parent

        // Đọc byte từ cha
        read(parent2child[0], buf, 1);
        printf("%d: received ping\n", getpid());

        // Gửi lại byte cho cha
        write(child2parent[1], buf, 1);

        // Đóng phần còn lại và thoát
        close(parent2child[0]);
        close(child2parent[1]);
        exit(0);
    } else 
    {
        // --- Parent ---
        char buf[1] = {'P'};

        // Đóng các đầu không dùng để tránh deadlock
        close(parent2child[0]); // đóng đầu đọc của parent2child
        close(child2parent[1]); // đóng đầu ghi của child2parent

        // Gửi byte cho con
        write(parent2child[1], buf, 1);

        // Đọc lại byte từ con
        read(child2parent[0], buf, 1);
        printf("%d: received pong\n", getpid());

        // Đóng phần còn lại
        close(parent2child[1]);
        close(child2parent[0]);
        wait(0); // chờ và reap child process
        exit(0);
    }
}
