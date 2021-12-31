#include "FileMgr.cpp"

int main() {
    FileMgr mydir;
    system("clear");
    while(true)
    {
        cout << "--------------------欢迎使用 Linux 文件管理器--------------------\n\n"
             << "当前目录：" << mydir.getPath() << "\n\n"
             << "-----------------------------------------------------------------\n\n"
             << "请选择一个功能（选择后按 Enter 继续）：\n"
             << "1. 列出并查看当前目录的所有文件夹和文件\n"
             << "2. 在当前目录新建文件夹\n"
             << "3. 选择一个文件夹并进入\n"
             << "4. 选择一个文件夹或文件并删除\n"
             << "5. 重命名一个文件或目录\n"
             << "6. 复制一个文件或文件夹到指定路径\n"
             << "7. 进入指定目录\n"
             << "8. 返回上一级目录\n"
             << "9. 退出程序\n\n";
        string Input;
        string FName;
        string enter;
        while(getline(cin, Input))
        {
            if(Input.length() != 1)
                Input[0] = '0';
            switch(Input[0])
            {
            case '1':
                system("clear");
                mydir.list();
                getchar();
                break;
            case '2':
                system("clear");
                cout << "请输入您要新建的文件夹名称：\n\n";
                getline(cin, FName);
                system("clear");
                mydir.newPath(FName);
                getchar();
                break;
            case '3':
                system("clear");
                mydir.enterPath();
                getchar();
                break;
            case '4':
                system("clear");
                mydir.deleteF();
                getchar();
                break;
            case '5':
                system("clear");
                mydir.renameF();
                getchar();
                break;
            case '6':
                system("clear");
                mydir.copyF();
                getchar();
                break;
            case '7':
                system("clear");
                cout << "请输入您要进入的路径：\n\n";
                getline(cin, enter);
                mydir.enterPath(enter);
                getchar();
                break;
            case '8':
                system("clear");
                mydir.goBack();
                getchar();
                break;
            case '9':
                system("clear");
                cout << "程序已退出！\n\n";
                return 0;
            default:
                cout << "输入错误，请重新输入！\n\n";
            }
            if(Input[0] >= '1' && Input[0] <= '8')
                break;
        }
        system("clear");
    }
}