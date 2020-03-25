#include"Point.h"
#include"PrintHelper.h"
#include"Logic.h"
#include"Snake.h"
#include <thread>

int main()
{
    Logic log;
    log.Run();
    return 0;
}


/*сделать так, что бы враги на первом шаге не генерировались возле змеи(в окрестности 5 * 5)*/