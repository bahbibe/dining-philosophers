/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 23:56:09 by bahbibe           #+#    #+#             */
/*   Updated: 2026/09/15 00:00:00 by bahbibe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>

# define USAGE "Usage: ./philo number_of_philosophers time_to_die " \
	"time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n"

/*
** Shared simulation state.
**
** - fork:       one mutex per fork.
** - list:       back-pointer to the philosopher array, so any philosopher
**               can read a sibling's last_meal (room.c uses this to give
**               fork admission priority to whoever is closest to dying).
** - room/room_lock: mutex-only admission gate. At most n_ph - 1
**               philosophers may be trying to pick up forks at once; with
**               n forks that guarantees at least one of them can always
**               get both, which is what actually rules out deadlock
**               (see room.c for the reasoning).
** - meal_lock:  guards last_meal. Split out from progress_lock because
**               it's touched far more often (every meal, plus every
**               death/urgency check) and shouldn't contend with the
**               rarely-touched meals_left/finished bookkeeping.
** - progress_lock: guards meals_left/finished.
** - print_lock: guards `running` and serializes every printed line, so a
**               state message can never overlap another one, and nothing
**               prints once the simulation has ended.
*/
typedef struct s_data
{
	struct s_philo	*list;
	pthread_mutex_t	*fork;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	progress_lock;
	pthread_mutex_t	room_lock;
	pthread_mutex_t	print_lock;
	int				room;
	int				running;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				n_ph;
	unsigned long	t0;
}	t_data;

typedef struct s_philo
{
	t_data			*data;
	pthread_t		thread;
	int				id;
	unsigned long	last_meal;
	int				meals_left;
	int				finished;
}	t_philo;

/* parsing.c */
int		ft_isdigit(int c);
int		ft_is_space(int c);
int		ft_atoi(const char *str);
int		get_arg(char *arg);
int		check_args(char **av);

/* init.c */
int		init_data(t_philo *philo, char **av, t_data *data);
void	destroy_data(t_data *data);

/* time_utils.c */
unsigned long	get_time(void);
void			precise_sleep(int ms);

/* monitor.c */
void	printing(t_philo *philo, char *msg);
int		is_dead(t_philo *philo);
void	report_death(t_philo *philo);

/* room.c */
void	enter_room(t_philo *philo);
void	leave_room(t_philo *philo);

/* philosopher.c */
void	*live(void *arg);

/* main.c */
int		spawn_philos(t_philo *philo);
void	join_philos(t_philo *philo);

#endif
