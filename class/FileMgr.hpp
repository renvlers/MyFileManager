#ifndef FILEMGR_HPP
#define FILEMGR_HPP

#include <iostream>
#include <cstdio>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fstream>
#include <errno.h>
#include <cstdlib>
#include <exception>
#include <pwd.h>
#include <cstdlib>
#include <unistd.h>
#include <vector>

using namespace std;

string getHomePath();

class FileMgr {
private:
    string path;
    DIR* dir;
public:
    FileMgr();

    void newPath(const string& name);
    void deleteF();
    void enterPath();
    void enterPath(string& _path);
    void list();
    void renameF();

    void copyF();

    void goBack();

    string getPath();

    ~FileMgr();
};

#endif