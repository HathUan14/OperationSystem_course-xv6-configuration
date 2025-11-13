#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: trace mask command [args...]\n");
    exit(1);
  }

  int mask = atoi(argv[1]);
  trace(mask); // Gọi syscall trace ở user.h

  exec(argv[2], &argv[2]); // Chạy lệnh cần theo dõi
  exit(0);
}
