//
// Created by Administrator on 2021/2/11 0011.
//

#ifndef STUDYLIBVIPSAPP_TEST_REFLECT_H
#define STUDYLIBVIPSAPP_TEST_REFLECT_H

typedef struct{

    const char* name;
    int age;

    void (*SetAge)(int age);
}Person;

#endif //STUDYLIBVIPSAPP_TEST_REFLECT_H
