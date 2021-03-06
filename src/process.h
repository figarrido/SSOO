typedef enum { READY, RUNNING, WAITING, NON_READY, DEAD } State;

typedef struct process{
    unsigned int    pid,
                    priority,
                    initial_time,
                    n_processes,
                    *running_array,
                    *waiting_array,
                    index,
                    user_time,
                    idle_time,
                    selected,
                    blocked,
                    response_time,
                    responded;
    char            name[256];
    State           state;
} Process;

Process *create_process(int,
                        char *,
                        int,
                        int,
                        int,
                        unsigned int *,
                        unsigned int *);
Process *create_idle_process(int);
int get_pid(Process *);
int get_priority(Process *);
int get_intervals(Process *);
int get_index(Process *);
char *get_name(Process *);
int get_selected(Process *);
int get_blocked(Process *);
int get_turnaround(Process *);
int get_response_time(Process *);
int get_idle_time(Process *);
int get_n_processes(Process *);

int get_running_time(Process *);
void update_user_time(Process *);

int get_waiting_time(Process *);
void update_idle_time(Process *, int);

State get_state(Process *);
void update_state(Process *, State);

int last_execution(Process *);
void destroy_process(Process *);
void print_process(Process *);
