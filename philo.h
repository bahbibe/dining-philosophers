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

# define RESET   "\033[0m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN    "\033[36m"
# define WHITE   "\033[37m"

typedef struct s_data{
	pthread_mutex_t	*fork;
	pthread_mutex_t	mutex;
	pthread_mutex_t	eat;
	pthread_mutex_t	meals;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				n_ph;
	unsigned long	t0;
}t_data;

typedef struct s_philo{
	t_data			*data;
	pthread_t		thread;
	int				id;
	unsigned long	last_eat;
	int				nb_meals;
	int				finish;
}t_philo;

int		ft_isdigit(int c);
int		ft_is_space(int c);
int		ft_atoi(const char *str);
int		get_arg(char *arg);
int		check_args(char **av);
void	init_data(t_philo *philo, char **av, t_data *data);
int		creat_philo(t_philo *philo);
void	error(void);
time_t	get_time(void);
void	timer(time_t time);
int		death(t_philo *philo);
void	spaghetti(t_philo *philo);

#endif