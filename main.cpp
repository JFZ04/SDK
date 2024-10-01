#include "UI/Demo.h"
#include "MainRun.h"
int main(int argc, char* argv[])
{
    MainRun mainRun;
    bool F = mainRun.Init();
    if(!F){
        return 0;
    }
    CC::Println("Server Run OK!");
    return mainRun.Run();
}