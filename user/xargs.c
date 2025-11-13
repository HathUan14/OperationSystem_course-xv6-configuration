#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_ARGS 32
#define MAX_BUFFER 512
int main(int argc, char * argv[]){
    //Kiểm tra số lượng đối số đầu vào
    if (argc < 2){
        fprintf(2, "Usage: xargs <command> [list of args]\n"); //Ghi lỗi vào luồng 2 - stderr
        exit(1);
    }
    //Tạo các biến, mảng
    char buf[MAX_BUFFER]; //buffer đọc các dòng
    char *args[MAX_ARGS]; //Lưu chuỗi tham số
    int i;
    
    //Copy các tham số argv[i] ( i chạy từ 1)
    for(i = 1; i < argc && i < MAX_ARGS - 1; i++){
        args[i - 1] = argv[i];
    }
    int fixed = i - 1; //Lấy số lượng tham số

    int n = 0;
    //Vòng lặp đọc dữ liệu từ stdin 
    while(1){
        char c;
        int r = read(0, &c, 1); //Đọc từng kí tự
        if (r == 0) break; //Đọc được EOF
        if (c == '\n'){
            buf[n] = 0; //Ngắt chuỗi bằng \0

            args[fixed] = buf;
            args[fixed + 1] = 0;

            int pid = fork(); //Tạo tiến trình con 
            if (pid == 0){
                exec(args[0], args);
                fprintf(2, "exec failed.\n");
                exit(1);
            }
            wait(0); //Chờ process con chạy xong

            n = 0; //Reset buffer cho dòng tiếp theo
        } else {
            if (n < sizeof(buf) - 1){
                buf[n++] = c;
            }
        }
    }
    exit(0);
}