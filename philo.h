/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 23:56:09 by bahbibe           #+#    #+#             */
/*   Updated: 2023/05/12 00:53:28 by bahbibe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>

# define USAGE   "Usage: \033[31m./philo \033[33m[number_of_philosophers] \
[time_to_die] [time_to_eat] [time_to_sleep]\033[36m \
[number_of_times_each_philosopher_must_eat(optional)]\n\033[0m"

typedef struct s_data
{
	pthread_mutex_t	*fork;
	pthread_mutex_t	mutex;
	pthread_mutex_t	eat;
	pthread_mutex_t	meals;
	pthread_mutex_t	simulation;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				n_ph;
	unsigned long	t0;
	int				end_simulation;
	int				print;
}	t_data;

typedef struct s_philo
{
	t_data			*data;
	pthread_t		thread;
	int				id;
	unsigned long	last_eat;
	int				nb_meals;
	int				finish;
}	t_philo;

int		ft_isdigit(int c);
int		ft_is_space(int c);
int		ft_atoi(const char *str);
int		get_arg(char *arg);
int		check_args(char **av);
void	init_data(t_philo *philo, char **av, t_data *data);
time_t	get_time(void);
void	timer(time_t time);
int		death(t_philo *philo);
void	spaghetti(t_philo *philo);
void	assign_forks(t_philo *philo);
void	*simulate(void *arg);
void	assign_forks(t_philo *philo);
void	printing(t_philo *philo, char *msg, unsigned long time);
void	join_threads(t_philo *philo);
int		create_philo(t_philo *philo);
void	print_dead(t_philo *philo);

#endif