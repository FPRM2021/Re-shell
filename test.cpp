#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(void)
{
        FILE* p = popen("cat test.cpp", "r");
        if (!p) return 1;
        int i = 0;
        char buff[1024];
        vector<string> tmp;


        while (fgets(buff, sizeof(buff), p)) {
                printf("%s", buff);
                tmp.push_back(string(buff));
                i++;
        }
        cout<<endl;

        cout<<i<<endl;

        cout<<endl<<endl;

        for(i=0; i < tmp.size(); i++)
            cout<<tmp[i];

        pclose(p);

        return 0;
}