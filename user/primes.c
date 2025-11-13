#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX 280
#define FIRST_PRIME 2

// tiến trình phát sinh các số tự nhiên 2..280
int generate_natural()
{
    int out_pipe[2];
    pipe(out_pipe);

    if (fork() == 0) {
        // child: phát số
        close(out_pipe[0]); // không đọc
        for (int i = FIRST_PRIME; i <= MAX; i++) {
            write(out_pipe[1], &i, sizeof(int));
        }
        close(out_pipe[1]);
        exit(0);
    }

    // parent: trả về read-end
    close(out_pipe[1]);
    return out_pipe[0];
}

// tiến trình lọc bỏ các số chia hết cho prime
int prime_filter(int in_fd, int prime)
{
    int num;
    int out_pipe[2];
    pipe(out_pipe);

    if (fork() == 0) {
        // child: đọc in_fd, ghi out_pipe
        close(out_pipe[0]); // không đọc từ out_pipe
        while (read(in_fd, &num, sizeof(int)) == sizeof(int)) {
            if (num % prime != 0) {
                write(out_pipe[1], &num, sizeof(int));
            }
        }
        // đóng các đầu pipe không dùng
        close(in_fd);
        close(out_pipe[1]);
        exit(0);
    }

    // parent: trả về đầu đọc mới cho pipeline
    close(in_fd);
    close(out_pipe[1]); // không ghi
    return out_pipe[0];
}


int main(int argc, char *argv[])
{
    int prime;
    int in = generate_natural();
    int pid;

    // tiến trình chính sẽ đọc prime từ pipeline và mở rộng pipeline
    while (read(in, &prime, sizeof(int)) == sizeof(int)) {
        printf("prime %d\n", prime);
        in = prime_filter(in, prime);
    }

    // chờ tất cả con, cháu, chắt... kết thúc
    while ((pid = wait(0)) > 0) {
    }

    exit(0);
}


