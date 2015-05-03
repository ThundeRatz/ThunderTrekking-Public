#ifndef __HOOK_H__
#define __HOOK_H__

enum {
	HOOK_MOV,
	HOOK_MAX
};

void hook_init();
void hook_trigger(int hook);
void hook_install(int hook, void (*handler)(long));
void hook_free();

#endif
