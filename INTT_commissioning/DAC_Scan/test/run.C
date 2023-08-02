#include "testnamespace.h"

void run()
{
    for (int i = 0; i < 1000000; i++)
    {
        if(i % 10 == 0)
        {
            cout<<i<<endl;
        }

        Testnamespace::Get_XY_all("intt3",4,10,12,"ideal",5);

    }
}