
int n1 = 0;
int n2 = 0;

void str_humanise_bytes2(off_t bytes)
{
	if (bytes > 1 << 30) {
		printf("%u.%2.2u GiB",
			    (int)(bytes >> 30),
			    (int)(bytes & ((1 << 30) - 1)) / 10737419);
	} else if (bytes > 1 << 20) {
		int x = bytes + 5243;  /* for rounding */
		printf("%u.%2.2u MiB",
			    x >> 20, ((x & ((1 << 20) - 1)) * 100) >> 20);
	} else if (bytes > 1 << 10) {
		int x = bytes + 5;  /* for rounding */
		printf("%u.%2.2u KiB",
			    x >> 10, ((x & ((1 << 10) - 1)) * 100) >> 10);
	} else {
		printf("%u bytes", (int)bytes);
	}
}


typedef struct progress_data {
	git_transfer_progress fetch_progress;
	size_t completed_steps;
	size_t total_steps;
	const char *path;
} progress_data;

#ifndef PRIuZ
/* Define the printf format specifer to use for size_t output */
#if defined(_MSC_VER) || defined(__MINGW32__)
#	define PRIuZ "Iu"
#else
#	define PRIuZ "zu"
#endif
#endif

static void print_progress(const progress_data *pd)
{
	int network_percent = pd->fetch_progress.total_objects > 0 ?
		(100*pd->fetch_progress.received_objects) / pd->fetch_progress.total_objects :
		0;
	int index_percent = pd->fetch_progress.total_objects > 0 ?
		(100*pd->fetch_progress.indexed_objects) / pd->fetch_progress.total_objects :
		0;

	int checkout_percent = pd->total_steps > 0
		? (100 * pd->completed_steps) / pd->total_steps
		: 0;

	if (pd->fetch_progress.total_objects && pd->fetch_progress.received_objects == pd->fetch_progress.total_objects) {
		if (!n1) {
			printf("Recieving objects: %3d%% (%d/%d)  | ", network_percent, pd->fetch_progress.received_objects, pd->fetch_progress.total_objects);
			str_humanise_bytes2(pd->fetch_progress.received_bytes);
			printf(", done.\n");
			n1 = 1;
		}
		if (pd->fetch_progress.total_deltas) {
			if (pd->fetch_progress.indexed_deltas == pd->fetch_progress.total_deltas && !n2) {
				printf("Resolving deltas %d/%d, done.\n", pd->fetch_progress.indexed_deltas, pd->fetch_progress.total_deltas);
				n2 = 1;
			}
			else if (pd->fetch_progress.indexed_deltas != pd->fetch_progress.total_deltas && n2) printf("Resolving deltas %d/%d\r", pd->fetch_progress.indexed_deltas, pd->fetch_progress.total_deltas);
		}
	} else {
		printf("Recieving objects: %3d%% (%d/%d)  | ", network_percent, pd->fetch_progress.received_objects, pd->fetch_progress.total_objects);
			str_humanise_bytes2(pd->fetch_progress.received_bytes);
			printf("   \r");
	}
}

static int sideband_progress(const char *str, int len, void *payload)
{
	(void)payload; // unused

	printf("remote: %.*s", len, str);
	fflush(stdout);
	return 0;
}

static int fetch_progress(const git_transfer_progress *stats, void *payload)
{
	progress_data *pd = (progress_data*)payload;
	pd->fetch_progress = *stats;
	print_progress(pd);
	return 0;
}
static void checkout_progress(const char *path, size_t cur, size_t tot, void *payload)
{
	progress_data *pd = (progress_data*)payload;
	pd->completed_steps = cur;
	pd->total_steps = tot;
	pd->path = path;
	print_progress(pd);
}


int gitprefix(clone)(int argc, char **argv)
{
	if (argc < 2) {
		printf ("USAGE: %s <url> <path> (path is optional)\n", argv[0]);
		return -1;
	}
	
	git_libgit2_init();
	
	progress_data pd = {{0}};
	git_repository *cloned_repo = NULL;
	git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
	git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;
	const char *url = argv[1];
	const char *path = !argv[2]?extractrepo(argv[1]):argv[2];
	ps(url)
	ps(path)
	int error;
	
	git_repository * repo;

	// Validate args

	// Set up options
	checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;
	checkout_opts.progress_cb = checkout_progress;
	checkout_opts.progress_payload = &pd;
	clone_opts.checkout_opts = checkout_opts;
	clone_opts.fetch_opts.callbacks.sideband_progress = sideband_progress;
	clone_opts.fetch_opts.callbacks.transfer_progress = &fetch_progress;
	clone_opts.fetch_opts.callbacks.credentials = cred_acquire_cb;
	clone_opts.fetch_opts.callbacks.payload = &pd;
	n1 = 0;
	n2 = 0;
	// Do the clone
	error = git_clone(&cloned_repo, url, path, &clone_opts);
	if (error != 0) {
		const git_error *err = giterr_last();
		if (err) printf("ERROR %d: %s\n", err->klass, err->message);
		else printf("ERROR %d: no detailed info\n", error);
	}
	else if (cloned_repo) git_repository_free(cloned_repo);
	git_libgit2_shutdown();
	return error;
}
