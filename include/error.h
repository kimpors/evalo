#define ERROR_MSG(fmt, ...) (fprintf(stderr, "error: " fmt " (file: %s, line: %d)\n", ##__VA_ARGS__, __FILE__, __LINE__));
#define WARN_MSG(fmt, ...) (fprintf(stderr, "warning: " fmt " (file: %s, line: %d)\n", ##__VA_ARGS__, __FILE__, __LINE__));
