typedef enum { READY, RUNNING, WAITING } State;

typedef struct process{
    unsigned int    pid,
                    priority;
    char            name[256];
    State           state;
} Process;
