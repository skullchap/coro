#include <stdio.h>
#include "coro.h"
#include "corodef.h"
#include <assert.h>

int coro3(Coro *c);

CoroDef(128);

int
coro1(Coro *c)
{
        int *i = newcvar(c, sizeof(int));
        Coro *c3 = newcvar(c, sizeof(Coro128));

        COROBEGIN(c){
                assert(COROSTOFLOW(c, Coro) == 0);
                *i = 0;
                initcoro(c3);
                printf("coro1 i = %d\n", (*i)++);
        }
        YIELD(c){
                printf("coro1 i = %d\n", (*i)++);
                coro3(c3);
        }
        YIELD(c){
                printf("coro1 i = %d\n", (*i)++);
                coro3(c3);
        }
        YIELD(c){
                printf("coro1 i = %d\n", (*i)++);
                coro3(c3);
                YIELD(c);

                printf("coro1 finishing coro3\n");
                while (!corodone(c3)){
                        coro3(c3);
                }
        }COROEND(c)

        return 0;
}

int
coro2(Coro *c)
{
        int *i = newcvar(c, sizeof(int));
        int *j = newcvar(c, sizeof(int));

        COROBEGIN(c)

                for(*i = 0; *i < 5; (*i)++){
                        printf("coro2 i = %d\n", *i);
                        YIELDLOOP(c);
                }
                for(*j = 5; *j < 10 ; (*j)++){
                        printf("coro2 (2) j = %d\n", *j);
                        YIELDLOOP(c);
                }

        COROEND(c)

        return 0;
}

int
coro3(Coro *c)
{
        int *i = newcvar(c, sizeof(int));

        COROBEGIN(c)
                *i = 0;
                printf("        coro3 i = %d\n", (*i)++);
                YIELD(c);
                printf("        coro3 i = %d\n", (*i)++);
                YIELD(c)
                printf("        coro3 i = %d\n", (*i)++);
                YIELD(c)
                printf("        coro3 i = %d\n", (*i)++);
                YIELD(c)
                printf("        coro3 i = %d\n", (*i)++);
                YIELD(c)
                printf("        coro3 i = %d\n", (*i)++);
                YIELD(c)
        COROEND(c)

        return 0;
}

int
main()
{
        Coro c1, c2, c4;

        initcoro(&c1);
        initcoro(&c2);
        initcoro(&c4);

        while (!corodone(&c1) || !corodone(&c2)){
                coro1(&c1);
                coro2(&c2);
		if(corodone(&c1))
			coro3(&c4);
        }

        return 0;
}
