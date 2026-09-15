#include "philo.h"

static void	init_philo(t_philo *philo, int i, char *must_eat_arg)
{
	philo[i].id = i + 1;
	philo[i].last_meal = get_time();
	philo[i].finished = 0;
	philo[i].meals_left = -1;
	if (must_eat_arg)
		philo[i].meals_left = ft_atoi(must_eat_arg);
}

int	init_data(t_philo *philo, char **av, t_data *data)
{
	int	i;

	data->n_ph = ft_atoi(av[1]);
	data->die_time = ft_atoi(av[2]);
	data->eat_time = ft_atoi(av[3]);
	data->sleep_time = ft_atoi(av[4]);
	data->fork = (pthread_mutex_t *)malloc(data->n_ph * sizeof(*data->fork));
	if (!data->fork)
		return (1);
	data->list = philo;
	data->room = 0;
	data->running = 1;
	pthread_mutex_init(&data->meal_lock, NULL);
	pthread_mutex_init(&data->progress_lock, NULL);
	pthread_mutex_init(&data->room_lock, NULL);
	pthread_mutex_init(&data->print_lock, NULL);
	for (i = 0; i < data->n_ph; i++)
	{
		philo[i].data = data;
		pthread_mutex_init(&data->fork[i], NULL);
		init_philo(philo, i, av[5]);
	}
	data->t0 = get_time();
	return (0);
}

void	destroy_data(t_data *data)
{
	int	i;

	for (i = 0; i < data->n_ph; i++)
		pthread_mutex_destroy(&data->fork[i]);
	free(data->fork);
	pthread_mutex_destroy(&data->meal_lock);
	pthread_mutex_destroy(&data->progress_lock);
	pthread_mutex_destroy(&data->room_lock);
	pthread_mutex_destroy(&data->print_lock);
}
