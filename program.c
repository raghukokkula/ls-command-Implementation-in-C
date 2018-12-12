#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


char* file_permissions(char *file)
{
        struct stat sb1;
        char *modeval = malloc(sizeof(char) * 9 + 1);
        if(stat(file, &sb1) == 0)
        {
                mode_t perm = sb1.st_mode;
                modeval[0] = (perm & S_IRUSR) ? 'r' : '-';
                modeval[1] = (perm & S_IWUSR) ? 'w' : '-';
                modeval[2] = (perm & S_IXUSR) ? 'x' : '-';
                modeval[3] = (perm & S_IRGRP) ? 'r' : '-';
                modeval[4] = (perm & S_IWGRP) ? 'w' : '-';
                modeval[5] = (perm & S_IXGRP) ? 'x' : '-';
                modeval[6] = (perm & S_IROTH) ? 'r' : '-';
                modeval[7] = (perm & S_IWOTH) ? 'w' : '-';
                modeval[8] = (perm & S_IXOTH) ? 'x' : '-';
                modeval[9] = '\0';
                return modeval;     
        }
        else
        {
                return strerror(errno);
        }
}

static void file_types(mode_t mode)
{
    switch (mode & S_IFMT)
    {
        case S_IFREG: putchar('-'); break;
        case S_IFDIR: putchar('d'); break;
        case S_IFLNK: putchar('l'); break;
        case S_IFCHR: putchar('c'); break;
        case S_IFBLK: putchar('b'); break;
        case S_IFSOCK: putchar('s'); break;
        case S_IFIFO: putchar('f'); break;
    }
}


int main(int argc, char *argv[])
{
        DIR *dp;
        struct dirent *dire;
        struct stat sb;
        int max_width, value_to_print;
	max_width = 8;
        value_to_print = 1000;

        if( argc != 2 )
        {
                fprintf(stderr,"Usage: %s <pathname>\n", argv[0] );
                exit(EXIT_FAILURE);
        }
        if( (dp = opendir(argv[1])) == NULL )
        {
                fprintf(stderr,"Can't open %s\n", argv[1]);
                exit(EXIT_FAILURE);
        }
        while( (dire = readdir(dp)) != NULL )
        {
                stat(dire->d_name, &sb);
                file_types(sb.st_mode);
                printf("%s", file_permissions(dire->d_name));

                printf("%4ld",(long) sb.st_nlink);

                printf(" %7s", getpwuid(sb.st_uid)->pw_name);

                printf("  %7s", getgrgid(sb.st_gid)->gr_name);
                //printf("%d   ", sb.st_uid);
                //printf("%ld  ", sb.st_gid);
                printf("  %8d", sb.st_size);
                printf("  %.12s", 4+ctime(&sb.st_mtime));
                printf("  %s\n", dire->d_name);
        }
        return 0;
}
