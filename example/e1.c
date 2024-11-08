#include <stdio.h>
#include <assert.h>
#include "coro.h"

int
coro1(Coro *c)
{
	int *i = newcvar(c, sizeof(int));

	switch (corostage(c))
	{
	case 0:
		*i = 0;
		printf("1: i = %d\n", (*i)++);
		return yield(c);
	case 1:
		printf("1: i = %d\n", (*i)++);
		return yield(c);
	}
	endcoro(c);
	return 0;
}

int
coro2(Coro *c)
{
	int *i = newcvar(c, sizeof(int));

	switch (corostage(c))
	{
	case 0:
		*i = 0;
		printf("2: i = %d\n", (*i)++);
		return yield(c);
	case 1:
		printf("2: i = %d\n", (*i)++);
		return yield(c);
	}
	endcoro(c);
	return 0;
}

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
