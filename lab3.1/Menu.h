#ifndef __MENU_H
#define __MENU_H

void main_menu() {
    printf("****************************\n");
    printf("* 1. 创建一个新的数字         \n");
    printf("* 2. 对已输入的数字进行操作.   \n");
    printf("* 3. 计算多项式.\n");
    printf("* 0. 退出程序.              \n");
    printf("****************************\n");
    printf("在以上选项中选择.\n");
}

void operate_menu() {
    printf("*********************\n");
    printf("* 1. A + B.         \n");
    printf("* 2. A - B.         \n");
    printf("* 3. A 乘 B.        \n");
    printf("* 4. 十进制转二进制.  \n");
    printf("* 5. 十进制转 N 进制. \n");
    printf("* 6: 二进制转十进制.  \n");
    printf("* 0. Exit submenu.  \n");
    printf("*********************\n");
    printf("在以上选项中选择.\n");
}

#endif