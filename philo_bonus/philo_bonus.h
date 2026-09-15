#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <pthread.h>
# include <semaphore.h>
# include <stdatomic.h>
# include <stdint.h>

# define USAGE "Usage: ./philo_bonus number_of_philosophers time_to_die " \
	"time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n"

# define SEM_FORK_PREFIX "/philo_bonus_fork_"
# define SEM_ROOM_PREFIX "/philo_bonus_room_"
# define SEM_PRINT_PREFIX "/philo_bonus_print_"

/*
** Each philosopher is its own process; forks are named semaphores so
** they're shared across processes without any actual shared memory
** ("no state in memory" per the subject). `room`, sized n_ph - 1, is the
** same deadlock-avoidance gate as the mandatory part's room.c: at most
** n_ph - 1 processes may hold forks at once, so at least one can always
** get both of theirs. `instance_id` is folded into every semaphore's
** name so two runs started at once on the same machine never collide on
** the same name. getpid() would be the obvious source of that
** uniqueness but isn't in the bonus's authorized function list, so it's
** built instead from a gettimeofday() microsecond timestamp mixed with
** the caller's own stack address (ASLR-randomized per process, and free
** since it's a language operation, not a function call) — belt and
** braces, since two runs launched back-to-back from the same script can
** land in the same microsecond on their own.
*/
typedef struct s_data
{
	sem_t			**forks;
	sem_t			*room;
	sem_t			*print_lock;
	int				n_ph;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				must_eat;
	long			t0;
	unsigned long	instance_id;
}	t_data;

/*
** last_meal/meals_done/stop are shared between a process's two threads
** (its main loop and its monitor_life watchdog) with no mutex, since
** pthread_mutex_* isn't in the bonus's authorized function list.
** _Atomic gives correct, defined cross-thread visibility for that instead
** of leaning on `volatile`, which only stops the compiler from caching
** the value in a register and says nothing about visibility across
** threads.
*/
typedef struct s_philo
{
	t_data		*data;
	pthread_t	monitor;
	int			id;
	atomic_long	last_meal;
	atomic_int	meals_done;
	atomic_int	stop;
}	t_philo;

/* parsing.c */
int		ft_isdigit(int c);
int		ft_is_space(int c);
int		ft_atoi(const char *str);
int		get_arg(char *arg);
int		check_args(char **av);

/* init.c */
int		init_data(t_data *data, char **av);
void	destroy_data(t_data *data);

/* time_utils.c */
long	get_time(void);
long	unique_id(void);
void	precise_sleep(int ms);

/* print.c */
void	printing(t_philo *philo, char *msg);

/* monitor.c */
void	*monitor_life(void *arg);

/* philosopher.c */
void	run_philo(t_philo *philo);

/* main.c */
pid_t	*create_processes(t_data *data, int *ok);
int		wait_processes(pid_t *pids, int n_ph);
void	kill_processes(pid_t *pids, int n_ph, pid_t except);

#endif
