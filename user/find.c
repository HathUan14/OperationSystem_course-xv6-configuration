#include "kernel/types.h" // use for dirent, stat
#include "kernel/stat.h"
#include "user/user.h" 
#include "kernel/fs.h" // filesystem constants
#include "kernel/param.h" // DIRSIZ, or 14 - buffer size

void find(char *path, char *filename) 
{
    char buf[512]; // use for recursive find
    char *p;
    int fd; // file descriptor, < 0 mean error
    struct dirent de; // struct use for getting file/folder names in a directory
    struct stat st; // struct use for getting detail info about a file/folder

    if ((fd = open(path, 0)) < 0) 
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) // get info about current file/folder
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) 
    {
        case T_FILE:
            if (strcmp(path, filename) == 0) 
            {
                printf("%s\n", path);
            }
        break;
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) 
            {
                fprintf(2, "find: path too long\n");
                break;
            }

            strcpy(buf, path);
            p = buf + strlen(buf); // p point toward the end of "string" ('\0')
            *p++ = '/'; // "string" becomes "string/", p point toward the char next to '/'
            
            // A folder is in fact, a file with many "dirent structs" each dirent decribes an entry of file/folder
            while (read(fd, &de, sizeof(de)) == sizeof(de)) 
            {
                if (de.inum == 0)
                    continue;

                memmove(p, de.name, DIRSIZ); // "string/" becomes "string/filename"
                p[DIRSIZ] = '\0'; // buf = "string/filename\0"

                if (stat(buf, &st) < 0) 
                {
                    fprintf(2, "find: cannot stat %s\n", buf);
                    continue;
                }
                if (st.type == T_DIR && strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) 
                {
                    find(buf, filename);
                }
                if (st.type == T_FILE && strcmp(de.name, filename) == 0) 
                {
                    printf("%s\n", buf);
                }
            }
        break;
    }

    close(fd);
}

int main(int argc, char *argv[]) 
{
    if (argc != 3) 
    {
        fprintf(2, "-->Usage: find <path> <filename>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}