#define ERROR_MSG(msg) (fprintf(stderr, "error: %s (file: %s, line: %d)\n", msg, __FILE__, __LINE__));
#define IF_ERROR(cond, msg) \
	if (cond) \
	{ \
		fprintf(stderr, "error: %s (file: %s, line: %d)\n", msg, __FILE__, __LINE__); \
		return -1; \
	}

#define IF_FUNC_ERROR(cond, func, msg) \
	if (cond) \
	{ \
		fprintf(stderr, "error: %s (file: %s, line: %d)\n", msg, __FILE__, __LINE__); \
		func(); \
		return -1; \
	} 
