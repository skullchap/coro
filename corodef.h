/*
   author:  skullchap (skullchap@proton.me)
   started: 2024.11.02
   license: non-commercial (full text in license file)
*/

#ifdef __COUNTER__
	#define STAGE (__COUNTER__ - STAGE_BASE - 1)

	#define YIELD(C) \
		return yield(C); \
		case STAGE:;

	#define YIELDLOOP(C) \
		{ \
			static char once = 0; \
			if(once){ \
				return yieldloop(C); \
			}else{ \
				once = 1; \
				return yield(C); \
				case STAGE:; \
			} \
		}		

	#define COROBEGIN(C) \
		enum {STAGE_BASE = __COUNTER__ }; \
		switch (corostage(C)) \
		{ \
		case STAGE:

	#define COROEND(C) endcoro(C);} 

	/* helper to use in debug builds, to check if coro's stack overflowed. 
	   pass real coro type that is behind ptr as a second param.
           e.g. assert(COROSTOFLOW(c, Coro128) == 0);
	*/
	#define COROSTOFLOW(C, REALCOROT) \
		(((C)->sp - (C)->bp) > (sizeof(((REALCOROT*)0)->stack)))
#endif
