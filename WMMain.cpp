#include "WMMgr.h"

int main(int argc, char** argv)
{
    WMMgr mgr;
    if(mgr.ReadFile(argv[1], argv[2], argv[3], argv[4])) 
        cout << "hi" << endl;
    //mgr.CutAll();
    //mgr.LinkSeq();
    mgr.WriteFile();

}
