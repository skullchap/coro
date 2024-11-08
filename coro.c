/*
   author:  skullchap (skullchap@proton.me)
   started: 2024.11.02
   license: non-commercial (full text in license file)
*/

#include "coro.h"
void	initcoro(Coro *c)            {c->bp = c->sp = c->stack; c->stage=0;}
int	corostage(Coro *c)           {return c->stage;}
void*	newcvar(Coro *c, int n)      {void *p = c->sp; c->sp+=n; return p;}
int 	yield(Coro *c)               {c->sp = c->bp; return c->stage++;}
int 	yieldloop(Coro *c)           {c->sp = c->bp; return c->stage;}
int	endcoro(Coro *c)             {return (c->stage = -1);}
int	corodone(Coro *c)            {return c->stage == -1;}
