#include "class/FileMgr.hpp"

//构造，默认路径为用户根目录
FileMgr::FileMgr() {
    path = getHomePath();
}

//全局：获取用户根目录
string getHomePath() {
    passwd* pwd;
    pwd = getpwuid(getuid());
    string _path = "/home/";
    _path += pwd->pw_name;
    return _path;
}

//全局：删除目录及其包含的文件
void removeF(const string& _path) {
    DIR* dir = opendir(_path.c_str());
    while(dirent64* ls = readdir64(dir))
    {
        if((int)ls->d_type == 4 && (string)ls->d_name != "." && (string)ls->d_name != "..") {
            int iCount = 0;
            string tpath = _path + "/" + ls->d_name;
            DIR* dir2 = opendir(tpath.c_str());
            while(dirent64* ls2 = readdir64(dir2)) {
                iCount++;
            }
            closedir(dir2);
            if(iCount <= 2)
                rmdir(tpath.c_str());
            else
                removeF(tpath);
        }
        else if((int)ls->d_type == 8) {
            string filepath;
            filepath = _path + "/" + ls->d_name;
            remove(filepath.c_str());
        }
    }
    closedir(dir);
    rmdir(_path.c_str());
}

//全局：复制文件或目录
void cpFile(const string& cfile, const string& target) {
    string tg;
    int ic = cfile.length() - 1;
    while(cfile[ic] != '/')
        ic--;
    tg = target + cfile.substr(ic, cfile.length() - ic);
    mkdir(tg.c_str(), S_IRWXU);
    DIR* dir = opendir(cfile.c_str());
    while(dirent64* ls = readdir64(dir)) {
        if((int)ls->d_type == 4 && (string)ls->d_name != "." && (string)ls->d_name != "..") {
            int iCount = 0;
            string opath = cfile + "/" + ls->d_name;
            string tpath = tg;
            string tpathdir = tg + "/" + ls->d_name;
            DIR* dir2 = opendir(opath.c_str());
            while(dirent64* ls2 = readdir64(dir2)) {
                iCount++;
            }
            closedir(dir2);
            if(iCount <= 2) {
                mkdir(tpathdir.c_str(), S_IRWXU);
            }
            else {
                cpFile(opath, tpath);
            }
        }
        else if((int)ls->d_type != 4) {
            string ofpath = cfile + "/" + ls->d_name;
            string tfpath = tg + "/" + ls->d_name;
            ifstream infile;
            infile.open(ofpath.c_str(), ios::binary | ios::in);
            ofstream outfile;
            outfile.open(tfpath.c_str(), ios::binary | ios::out);
            if(!infile || !outfile)
                return;
            outfile << infile.rdbuf();
            infile.close();
            outfile.close();
        }
    }
    closedir(dir);
    return;
}

//新建目录
void FileMgr::newPath(const string& name) {
    try {
        string _path = path + '/' + name;
        if(access(_path.c_str(), 0) == 0)
            throw "目录创建失败!";
        else if(mkdir(_path.c_str(), S_IRWXU) == 0) {
            cout << "操作成功，请按任意键返回主菜单……\n\n";
            return;
        }
        else
            throw "目录创建失败!";
    }
    catch(const char* ch) {
        cout << ch << endl
             << "操作失败，请按任意键返回主菜单……\n\n";
        return;
    }
}

//删除目录或文件
void FileMgr::deleteF() {
    cout << "------------------------------\n";
    int iCount = 0;
    vector<string> temp;
    vector<int> tp;
    dir = opendir(path.c_str());
    while(dirent64* ls = readdir64(dir)) {
        if((int)ls->d_type == 4 || (int)ls->d_type == 8) {
            if(string(ls->d_name) != "." && string(ls->d_name) != "..") {
                iCount++;
                cout << iCount << ". " << ls->d_name << endl;
                temp.push_back(ls->d_name);
                tp.push_back(ls->d_type);
            }
        }
    }
    closedir(dir);
    if(iCount == 0) {
        cout << "------------------------------\n\n"
             << "当前目录为空!" << endl
             << "请按任意键返回主菜单……\n\n ";
        return;
    }
    else {
        cout << "------------------------------\n\n"
             << "请选择您要删除的文件夹或文件的编号：\n\n";
        int iChoice;
        string choice;
        while(true) {
            getline(cin, choice);
            bool isNumber = true;
            for(int i = 0; i < choice.length(); i++) {
                if(choice[i] > '9' || choice[i] < '0') {
                    isNumber = false;
                    break;
                }
            }
            if(isNumber == false) {
                cout << "无效输入，请重试！\n\n";
                continue;
            }
            else
                iChoice = atoi(choice.c_str());
            if(iChoice > iCount || iChoice <= 0) {
                cout << "无效输入，请重试！\n\n";
                continue;
            }
            else
                break;
        }
        if(tp[iChoice - 1] == 4) {
            string tpath;
            if(path == "/")
                tpath = path + temp[iChoice - 1];
            else
                tpath = path + "/" + temp[iChoice - 1];
            removeF(tpath);
        }
        if(tp[iChoice - 1] == 8) {
            string tpath;
            if(path == "/")
                tpath = path + temp[iChoice - 1];
            else
                tpath = path + "/" + temp[iChoice - 1];
            unlink(tpath.c_str());
        }
    }
    system("clear");
    cout << "操作完成，请按任意键返回主菜单……\n\n";
    return;
}

//进入选中目录
void FileMgr::enterPath() {
    cout << "------------------------------\n";
    int iCount = 0;
    vector<string> temp;
    dir = opendir(path.c_str());
    while(dirent64* ls = readdir64(dir)) {
        if((int)ls->d_type == 4) {
            if(string(ls->d_name) != "." && string(ls->d_name) != "..") {
                iCount++;
                cout << iCount << ". " << ls->d_name << endl;
                temp.push_back(ls->d_name);
            }
        }
    }
    if(iCount == 0) {
        cout << "------------------------------\n\n"
             << "当前目录不存在文件夹!" << endl
             << "请按任意键返回主菜单……\n\n ";
    }
    else {
        cout << "------------------------------\n\n"
             << "请选择您要进入的文件夹的编号：\n\n";
        int iChoice;
        string choice;
        while(true) {
            getline(cin, choice);
            bool isNumber = true;
            for(int i = 0; i < choice.length(); i++) {
                if(choice[i] > '9' || choice[i] < '0') {
                    isNumber = false;
                    break;
                }
            }
            if(isNumber == false) {
                cout << "无效输入，请重试！\n\n";
                continue;
            }
            else
                iChoice = atoi(choice.c_str());
            if(iChoice > iCount || iChoice <= 0) {
                cout << "无效输入，请重试！\n\n";
                continue;
            }
            else
                break;
        }
        if(path == "/")
            path += temp[iChoice - 1];
        else
            path += ('/' + temp[iChoice - 1]);
        system("clear");
        cout << "操作成功！当前目录为 " << path << endl
             << "请按任意键返回主菜单……\n\n";
    }
    closedir(dir);
    return;
}

//进入指定目录
void FileMgr::enterPath(string& _path) {
    try {
        //将"~"替换为用户根目录
        if(_path[0] == '~')
            _path.replace(0, 1, getHomePath());
        //将"."替换为当前目录
        if(_path[0] == '.' && _path[1] != '.') {
            if(path == "/") {
                if(_path.length() == 1)
                    _path = "/";
                else
                    _path = _path.substr(1, _path.length() - 1);
            }
            else
                _path.replace(0, 1, path);
        }
        //将".."替换为上级目录
        if(_path[0] == '.' && _path[1] == '.') {
            if(path == "/") {
                system("clear");
                throw "您指定的路径不存在或无法访问！";
            }
            else {
                string tmp = path;
                while(tmp[tmp.length() - 1] != '/')
                    tmp.pop_back();
                tmp.pop_back();
                if(tmp.empty() && _path == "..")
                    _path = "/";
                else
                    _path.replace(0, 2, tmp);
            }
        }
        system("clear");
        if((dir = opendir(_path.c_str())) == NULL) {
            closedir(dir);
            throw "您指定的路径不存在或无法访问！";
        }
        else
        {
            path = _path;
            //去除中间多余的"/"
            for(int i = 0; i < path.length() - 1; i++) {
                if(path[i] == '/' && path[i + 1] == '/') {
                    path.replace(i, path.length() - i, path.substr(i + 1, path.length() - i - 1));
                    i--;
                }
            }
            //去除末尾多余的"/"
            if(path[path.length() - 1] == '/' && path != "/")
                path.pop_back();
            cout << "操作成功！当前目录为 " << path << endl
                 << "请按任意键返回主菜单……\n\n";
            return;
        }
    }
    catch(const char* ch) {
        cout << ch << endl
             << "操作失败，请按任意键返回主菜单……\n\n";
        return;
    }
}

//列出内容
void FileMgr::list() {
    cout << "当前目录 " << path << " 存在的文件夹和文件如下：" << endl
         << endl
         << "------------------------------\n";
    dir = opendir(path.c_str());
    while(dirent64* ls = readdir64(dir))
        cout << ls->d_name << endl;
    cout << "------------------------------\n\n"
         << "操作成功！\n"
         << "查看完成后请按任意键返回主菜单……\n\n";
    closedir(dir);
    return;
}

//重命名
void FileMgr::renameF() {
    cout << "------------------------------\n";
    int iCount = 0;
    vector<string> temp;
    dir = opendir(path.c_str());
    while(dirent64* ls = readdir64(dir)) {
        if((int)ls->d_type == 4 || (int)ls->d_type == 8) {
            if(string(ls->d_name) != "." && string(ls->d_name) != "..") {
                iCount++;
                cout << iCount << ". " << ls->d_name << endl;
                temp.push_back(ls->d_name);
            }
        }
    }
    closedir(dir);
    if(iCount == 0) {
        cout << "------------------------------\n\n"
             << "当前目录为空!" << endl
             << "请按任意键返回主菜单……\n\n ";
        return;
    }
    else {
        cout << "------------------------------\n\n"
             << "请选择您要重命名的文件夹或文件的编号：\n\n";
        int iChoice;
        string choice;
        while(true) {
            getline(cin, choice);
            bool isNumber = true;
            for(int i = 0; i < choice.length(); i++) {
                if(choice[i] > '9' || choice[i] < '0') {
                    isNumber = false;
                    break;
                }
            }
            if(isNumber == false) {
                cout << "无效输入，请重试！\n\n";
                continue;
            }
            else
                iChoice = atoi(choice.c_str());
            if(iChoice > iCount || iChoice <= 0) {
                cout << "无效输入，请重试！\n\n";
                continue;
            }
            else
                break;
        }
        system("clear");
        cout << "请输入新的文件或目录名：\n\n";
        string ipt;
        getline(cin, ipt);
        string opath;
        if(path == "/")
            opath = path + temp[iChoice - 1];
        else
            opath = path + "/" + temp[iChoice - 1];
        string tpath;
        if(path == "/")
            tpath = path + ipt;
        else
            tpath = path + "/" + ipt;
        rename(opath.c_str(), tpath.c_str());
    }
    system("clear");
    cout << "操作完成，请按任意键返回主菜单……\n\n";
    return;
}

//复制内容
void FileMgr::copyF() {
    cout << "------------------------------\n";
    int iCount = 0;
    vector<string> temp;
    vector<int> tp;
    dir = opendir(path.c_str());
    while(dirent64* ls = readdir64(dir)) {
        if((int)ls->d_type == 4 || (int)ls->d_type == 8) {
            if(string(ls->d_name) != "." && string(ls->d_name) != "..") {
                iCount++;
                cout << iCount << ". " << ls->d_name << endl;
                temp.push_back(ls->d_name);
                tp.push_back(ls->d_type);
            }
        }
    }
    closedir(dir);
    if(iCount == 0) {
        cout << "------------------------------\n\n"
             << "当前目录为空!" << endl
             << "请按任意键返回主菜单……\n\n ";
        return;
    }
    else {
        cout << "------------------------------\n\n"
             << "请选择您要复制的文件夹或文件的编号：\n\n";
        int iChoice;
        string choice;
        while(true) {
            getline(cin, choice);
            bool isNumber = true;
            for(int i = 0; i < choice.length(); i++) {
                if(choice[i] > '9' || choice[i] < '0') {
                    isNumber = false;
                    break;
                }
            }
            if(isNumber == false) {
                cout << "无效输入，请重试！\n\n";
                continue;
            }
            else
                iChoice = atoi(choice.c_str());
            if(iChoice > iCount || iChoice <= 0) {
                cout << "无效输入，请重试！\n\n";
                continue;
            }
            else
                break;
        }
        system("clear");
        cout << "请输入目标路径：\n\n";
        string ipt;
        getline(cin, ipt);
        try {
            if(ipt[0] == '~')
                ipt.replace(0, 1, getHomePath());
            if(ipt[0] == '.' && ipt[1] != '.') {
                if(path == "/") {
                    if(ipt.length() == 1)
                        ipt = "/";
                    else
                        ipt = ipt.substr(1, ipt.length() - 1);
                }
                else
                    ipt.replace(0, 1, path);
            }
            if(ipt[0] == '.' && ipt[1] == '.') {
                if(path == "/") {
                    system("clear");
                    throw "您指定的路径不存在或无法访问！";
                }
                else {
                    string tmp = path;
                    while(tmp[tmp.length() - 1] != '/')
                        tmp.pop_back();
                    tmp.pop_back();
                    if(tmp.empty() && ipt == "..")
                        ipt = "/";
                    else
                        ipt.replace(0, 2, tmp);
                }
            }
            system("clear");
            if((dir = opendir(ipt.c_str())) == NULL) {
                closedir(dir);
                throw "您指定的路径不存在或无法访问！";
            }
            else
            {
                for(int i = 0; i < ipt.length() - 1; i++) {
                    if(ipt[i] == '/' && ipt[i + 1] == '/') {
                        ipt.replace(i, ipt.length() - i, ipt.substr(i + 1, ipt.length() - i - 1));
                        i--;
                    }
                }
                if(ipt[ipt.length() - 1] == '/' && ipt != "/")
                    ipt.pop_back();
                string cfile;
                if(path == "/")
                    cfile = path + temp[iChoice - 1];
                else
                    cfile = path + "/" + temp[iChoice - 1];
                if(tp[iChoice - 1] == 4) {
                    cpFile(cfile, ipt);
                    cout << "操作完成！" << endl
                         << "请按任意键返回主菜单……\n\n";
                    return;
                }
                else {
                    string ofpath = cfile;
                    string tfpath;
                    if(ipt == "/")
                        tfpath = ipt + temp[iChoice - 1];
                    else
                        tfpath = ipt + "/" + temp[iChoice - 1];
                    ifstream infile;
                    infile.open(ofpath.c_str(), ios::binary | ios::in);
                    ofstream outfile;
                    outfile.open(tfpath.c_str(), ios::binary | ios::out);
                    if(!infile || !outfile)
                        throw "复制过程出错";
                    outfile << infile.rdbuf();
                    infile.close();
                    outfile.close();
                    cout << "操作完成！" << endl
                         << "请按任意键返回主菜单……\n\n";
                    return;
                }
            }
        }
        catch(const char* ch) {
            cout << ch << endl
                 << "操作失败，请按任意键返回主菜单……\n\n";
            return;
        }
    }
}

//返回上级目录
void FileMgr::goBack() {
    try {
        if(path == "/")
            throw "当前已是根目录，无需返回！";
        else {
            while(path[path.length() - 1] != '/')
                path.pop_back();
            if(path != "/")
                path.pop_back();
            cout << "已返回上一级目录，当前所在目录为 " << path << endl
                 << "请按任意键返回主菜单……\n\n";
            return;
        }
    }
    catch(const char* ch) {
        cout << ch << endl
             << "请按任意键返回主菜单……\n\n";
        return;
    }
}

//获取当前目录
string FileMgr::getPath() {
    return path;
}

//析构
FileMgr::~FileMgr() {
}