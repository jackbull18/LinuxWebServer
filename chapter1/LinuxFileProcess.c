/*
    模拟Linux命令 ls -l
    列出当前目录下文件的权限，创建日期
    -rw-rw-r-- 1 ubuntu ubuntu 33 Apr 13 10:39 LinuxFileProcess.c
*/

#include <stdio.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>


/*
    argc是命令行总的参数个数 
    argv[]是argc个参数，其中第0个参数是程序的全名，以后的参数是命令行后面跟的用户输入的参数
*/
int main(int argc, char* argv[]){
    //判断输入的参数是否正确
    if(argc < 2){
        printf("输入格式为: %s filename\n",argv[0]);
        return -1;
    }

    //通过stat函数获取用户传入的文件的信息
    struct stat st;
    /*
        stat结构体的属性
        struct stat {
            dev_t st_dev; // 文件的设备编号
            ino_t st_ino; // 节点
            mode_t st_mode; // 文件的类型和存取的权限
            nlink_t st_nlink; // 连到该文件的硬连接数目
            uid_t st_uid; // 用户ID
            gid_t st_gid; // 组ID
            dev_t st_rdev; // 设备文件的设备编号
            off_t st_size; // 文件字节数(文件大小)
            blksize_t st_blksize; // 块大小
            blkcnt_t st_blocks; // 块数
            time_t st_atime; // 最后一次访问时间
            time_t st_mtime; // 最后一次修改时间
            time_t st_ctime; // 最后一次改变时间(指属性)
        };
    */
    /*
        int stat(const char * file, struct stat * buf)
        Get file attributes for FILE and put them in BUF.
        获取文件file的信息并且将信息保存在传入的stat机构体对象buf中
    */
    int ret = stat(argv[1], &st);
    if(ret == -1){
        perror("stat");
        return -1;
    }

    //通过stat.st_mode成员获取文件类型和文件权限
    char perms[11] = {0}; //用于保存文件类型和文件权限的字符串
    //S_IFMT 用来获取前四位文件类型的掩码
    //判断文件类型
    switch(st.st_mode & S_IFMT){
        case S_IFLNK://是否为符号链接
            perms[0] = 'l';
            break;
        case S_IFDIR://是否为目录
            perms[0] = 'd';
            break;
        case S_IFREG://是否为一般文件
            perms[0] = '-';
            break;
        case S_IFBLK://是否为块设备文件
            perms[0] = 'b';
            break;
        case S_IFCHR://是否为字符装置文件
            perms[0] = 'c';
            break;
        case S_IFSOCK://是否为socket
            perms[0] = 's';
            break;
        case S_IFIFO://是否为FIFO或者管道
            perms[0] = 'p';
            break;
        default:
            perms[0] = '?';
            break;
    }

    //判断文件的访问权限
    //文件所有者的访问权限
    perms[1] = (st.st_mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (st.st_mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (st.st_mode & S_IXUSR) ? 'x' : '-';
    //文件所在组的访问权限
    perms[4] = (st.st_mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (st.st_mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (st.st_mode & S_IXGRP) ? 'x' : '-';
    //其他人的文件权限
    perms[7] = (st.st_mode & S_IROTH) ? 'r' : '-';
    perms[8] = (st.st_mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (st.st_mode & S_IXOTH) ? 'x' : '-';

    //硬连接数
    int linkNum = st.st_nlink;
    //文件所有者
    char* fileUser = getpwuid(st.st_uid)->pw_name;
    //文件所在组
    char* fileGrp = getgrgid(st.st_gid)->gr_name;
    //文件大小
    long int fileSize = st.st_size;
    //获取修改的时间
    char* time = ctime(&st.st_mtime);
    char mtime[512] = {0};
    strncpy(mtime, time, strlen(time) - 1);

    //制作输出
    //发送格式化输出到str所指向的字符串
    char buf[1024];
    sprintf(buf, "%s %d %s %s %ld %s %s", perms, linkNum, fileUser, fileGrp,
    fileSize, mtime, argv[1]);

    printf("%s\n", buf);

    return 0;


}