#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "mac.h"
#include "datasap.h"
#include "servicetest.h"


//first commit
int main()
{

    struct MacService mac;

    struct ServiceTest test;

    initMac(&mac);
    initServiceTest(&test, &mac);

    test.operations.testDataSap(&test);
    test.operations.testManagementSap(&test);
    test.operations.testSocket(&test);
    test.operations.testMCSPRequest(&test);



    while(1);



//    macInit(&Mac);


//    sleep(4);


//    Mac.operations.sendData(&Mac, (uint8_t *)data, length);

//    while(1)
//    {
//        printf("\n\nEnter the Character: ");

//        scanf("%c", &c);

//        if(c == 's')
//            Mac.operations.sendData(&Mac, (uint8_t *)data, length);
//    }


    return 0;

}
