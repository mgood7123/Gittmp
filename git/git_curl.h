#include <curl/curl.h>

// git curl create https://github.com/mgood7123/invalid.git

int gitprefix(curl_create)(char * url)
{
	char * user = extractuser(url);
	char * password = NULL;
	char * repo = extractrepo(url);
	char * description = "";
	
	int postlen = strlen("{\"name\":\"\",\"description\":\"\"}")+strlen(repo)+strlen(description);
	char * post = malloc(postlen);
	memset(post, 0, postlen);
	snprintf(post, postlen, "{\"name\":\"%s\",\"description\":\"%s\"}", repo, description);
	
	printf("password: ");
	getline_stdin(&password);
	pi(strlen(password))
	pc(password[strlen(password)])
	pc(password[strlen(password)-1])
	int USERPWDLEN = strlen(user)+strlen(":")+strlen(password)+1;
	char * USERPWD = malloc(USERPWDLEN);
	memset(USERPWD,0,USERPWDLEN);
	snprintf(USERPWD, USERPWDLEN, "%s:%s", user, password);
	ps(post);
	pi(USERPWDLEN)
	pi(USERPWD[USERPWDLEN])
	ps(USERPWD);
	ps(password);
	CURL *curl;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Dark Secret Ninja/1.0");
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/user/repos");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
		curl_easy_setopt(curl, CURLOPT_USERPWD, USERPWD);
		//curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		printf("\ncreated server repo at %s\n", url);
		curl_easy_cleanup(curl);
	}
	else puts("curl failed to initialize");
	free(post);
	free(password);
	free(USERPWD);
	curl_global_cleanup();
	return 0;
}

int gitprefix(curl)(int argc, char * argv[]) {
	if (argc < 2) giterrornomessage("%s requires an additional command", argv[0]);
	if(strcmp(argv[1], "create") == 0) {
		if (argc < 3) giterrornomessage("%s requires a url", argv[1]);
		return gitprefix(curl_create)(argv[2]);
	}
	else giterrornomessage("%s is not a valid sub command of %s", argv[1], argv[0]);
}
