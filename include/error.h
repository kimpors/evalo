#define ERROR_MSG(fmt, ...) (fprintf(stderr, "error: " fmt " (file: %s, line: %d)\n", ##__VA_ARGS__, __FILE__, __LINE__));
