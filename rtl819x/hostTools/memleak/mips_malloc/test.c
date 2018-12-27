#include <stdlib.h>
#include <signal.h>


void func4()
{
    char *ptr = malloc(100);
    free(ptr);
        
}


void func3()
{
        func4();
}

void func2()
{
}

void fun1(char *p_name1, char *p_name2, char *p_name3, char *p_name4, char *p_name5)
{
        
        printf("%p %p %p %p %p\n", p_name1, p_name2, p_name3, p_name4, p_name5);
        printf("%s %s %s %s %s\n", p_name1, p_name2, p_name3, p_name4, p_name5);
}

int main()
{
        int i = 0;
        
        printf("1\n");
        fun1("helloa", "hellob", "helloc", "hellod", "helloe");
        printf("2\n");
        
        func2();
        
        printf("3\n");
        func3();
        printf("4\n");
}
