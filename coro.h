/*
   author:  skullchap (skullchap@proton.me)
   started: 2024.11.02
   license: non-commercial (full text in license file)
*/

typedef struct Coro Coro;
enum
{StackN=	2048};

void	initcoro(Coro*);
int	corostage(Coro*);
void*	newcvar(Coro*, int n);
int	yield(Coro*);
int	yieldloop(Coro*);
int	endcoro(Coro*);
int	corodone(Coro*);

struct Coro
{
	int stage;
	unsigned char *bp, *sp;
	unsigned char stack[StackN];
};

#define CoroDef(N) \
typedef struct Coro ##N Coro ##N; \
struct Coro ##N \
{ \
	int stage; \
	unsigned char *bp, *sp; \
	unsigned char stack[N]; \
}
