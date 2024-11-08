# coro
No BS Coroutines in ANSI C. (No assembly, no setjmp, no platform specific code and any other bs in less than 100 locs)

```c
void	initcoro(Coro*);             /* used after Coro var declare, or coro reuse/reinit. */
int	corostage(Coro*);            /* returns current switch case step. */
void*	newcvar(Coro*, int n);       /* ptr to coro's own stack to persist variable between jumps. */
int	yield(Coro*);                /* resets coro's stack ptr and increments coro's stage. */
int	yieldloop(Coro*);            /* same as yield, except increment part. used only in loops. */
int	endcoro(Coro*);              /* sets coro's stage to -1, which makes corodone return true. */
int	corodone(Coro*);             /* is coro finished execution or not? */

/* helper macros */
COROBEGIN(C)
	YIELD(C)
	YIELDLOOP(C)
COROEND(C)
```
### Basic structure
```c
...
#include "coro.h"
...

int
coro(Coro *c)
{
	int *i = newcvar(c, sizeof(int));	/* <- declare all vars before stepping inside switch case
						*     newcvar() returns ptr from coro's own stack, to persist var between jumps.
						*/
	switch (corostage(c))
	{
	case 0:
		*i = 0;
		printf("1: i = %d\n", (*i)++);
		return yield(c);		/* <- reset coro's stack ptr, increment switch stage */
	case 1:					/* <- continuation point after calling same function again */
		printf("1: i = %d\n", (*i)++);
		return yield(c);
	}
	endcoro(c);				/* <- mark coroutine as finished */
	return 0;
}
```
### Initialization and switching
```c
int
main()
{
        Coro c1, c2;
        initcoro(&c1);
        initcoro(&c2);

        while (!corodone(&c1) || !corodone(&c2)){
                coro1(&c1);
                coro2(&c2);
        }

        return 0;
}
```
### Perks
Some may find writing switch steps manually tedious.
If your compiler supports `__COUNTER__` (which is pretty likely), you can use `"corodef.h"` helper macros.
```c
...
#include "coro.h"
#include "corodef.h"
...

int
coro(Coro *c)
{
	int *i = newcvar(c, sizeof(int));
        
	COROBEGIN(c)
		*i = 0;
		printf("1: i = %d\n", (*i)++);
		YIELD(c);
						/* <- continuation point after calling same function again */
		printf("1: i = %d\n", (*i)++);
		YIELD(c)

		printf("1: i = %d\n", (*i)++);
	COROEND(c)
        
	return 0;
}
```
### Loops
Loops are a little different. Use yieldloop or YIELDLOOP macro, to continue back from inside loop.
```c
int
coro(Coro *c)
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

/* without macros */

int
coro(Coro *c)
{
        int *i = newcvar(c, sizeof(int));

        switch (corostage(c))
        {
        case 0:
		*i = 0;
		return yield(c);
                for( ; *i < 5; (*i)++){
                        printf("2: i = %d\n", *i);
			return yieldloop(c);
                        case 1:;
                }
        return yield(c);
        case 2:
                for(*i = 0; *i < 10 ; (*i)++){
                        printf("2: (2) i = %d\n", *i);
                        if(*i == 0) 			/* corostage must be incremented once, to fall inside loop */
                                return yield(c);
                        else
                                return yieldloop(c);
                        case 3:;
                }
        }
        endcoro(c);
        return 0;
}

```
### Variable Stack
`CoroDef(N)`codegen macro defines Coro struct with N-sized stack.
for example, CoroDef(128) will codegen Coro128 with stack of 128 bytes in size.
makes sense to use smaller Coro, when nesting it to parent coro's stack with newcvar().
e.g.
```c
CoroDef(128); 
Coro128 c128; 
Coro *c = (Coro*)&c128;
initcoro(c);
```
#### Check more in example folder (m in file name stands for macros version)



