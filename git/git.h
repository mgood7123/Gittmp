//Author: mgood7123 (Matthew James Good) http://github.com/mgood7123

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

//#include "../../shells/Built-Ins/argv.h"

#include "../../shells/Built-Ins/Built-Ins.h"

int git(char * str);

#include "git_macros.h"
/*

we provide two methods of including the libgit header

1. use the built-in header

#define git_libgit_version_2_standard

2. use the newest libgit headers

#define git_libgit_version_2_new

*/

// for compatibility we use the standard git header provided by the hosts libgit2 installation

#define git_libgit_version_2_standard

int using_defined = true;
char * defined_username = "mgood7123";
char * defined_password = "Myaccount22";

#include "common.h"


#include "git_init.h"
#include "git_add.h"
#include "git_config.h"
#include "git_clone.h"
#include "git_curl.h"
#include "git_commit.h"
#include "git_merge.h"
#include "git_push.h"
#include "git_remote.h"
#include "git_stat.h"
#include "git_tests.h"

#define print_usage(x) printf("%s\n", x);
#define print_usage_command(x) printf("\t%s\n", x);
#define print_usage_command_info(x) printf("\t\t\t\t%s\n", x);


int gitprefix(show_usage)(void) {
	print_usage("usage: git [command] [action]")
	print_usage_command("git init repo")
	print_usage_command_info("initializes an empty git repository")
	return -1;
}

int gitprefix(clean_init)(void) {
	int inits_remaining = git_libgit2_init();
	int shutdowns_remaining = git_libgit2_shutdown();
	inits_remaining--;
	if (!inits_remaining && !shutdowns_remaining) {
		puts("nothing to clean");
		return 0;
	}
	if (inits_remaining < 0) {
		pi(inits_remaining)
		while(inits_remaining < 1) {
			inits_remaining = git_libgit2_init();
			pi(inits_remaining)
		}
		pi(inits_remaining)
	}
	if (shutdowns_remaining > 0) {
		pi(shutdowns_remaining)
		while(shutdowns_remaining > 0) {
			shutdowns_remaining = git_libgit2_shutdown();
			pi(shutdowns_remaining)
		}
	}
	return 0;
}

int gitprefix(clean)(int argc, char * argv[]) {
	if (argc < 2) giterrornomessage("%s requires an additional command", argv[0]);
	if(strcmp(argv[1], "init") == 0) return gitprefix(clean_init)();
	else giterrornomessage("%s is not a valid sub command of %s", argv[1], argv[0]);
}

int git(char * str) {
	int argc;
	char ** argv;
	string_to_argv(str, &argc, &argv);
	//builtin__argv(argc, argv);
	if (argc == 0) {
		free(argv);
		return gitprefix(show_usage)();
	}
	
	     gitdo(init)
	else gitdo(add)
	else gitdo(clean)
	else gitdo(clone)
	else gitdo(config)
	else gitdo(commit)
	else gitdo(commit2)
	else gitdo(curl)
	else gitdo(merge)
	else gitdo(push)
	else gitdo(remote)
	else gitdo(stat)
	else gitdo(status)
	else gitdo(test)
	else giterrornomessage("%s is not a valid command", argv[0]);
	return 0;
}
