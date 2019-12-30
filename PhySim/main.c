#include <stdio.h>
#include <phy.h>
#include "servicetest.h"


int main()
{

    struct PhyService phy;
    struct ServiceTest test;

    printf("PhySim\n");

    initPhy(&phy);

    initServiceTest(&test, &phy);

    test.operations.testDataSap(&test);
    test.operations.testManagementSap(&test);
    test.operations.testSocket(&test);



    while(1);

}
