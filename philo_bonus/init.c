/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 00:00:00 by bahbibe           #+#    #+#             */
/*   Updated: 2026/09/15 00:00:00 by bahbibe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/*
** Builds "/philo_bonus_fork_<i>" into buf. Hand-rolled instead of
** strcpy/snprintf: neither is in the bonus's authorized function list.
*/
static void	fork_name(char *buf, int i)
{
	char	*prefix;
	char	digits[16];
	int		n_digits;
	int	pos;

	prefix = SEM_FORK_PREFIX;
	for (pos = 0; prefix[pos]; pos++)
		buf[pos] = prefix[pos];
	n_digits = 0;
	if (i == 0)
		digits[n_digits++] = '0';
	while (i > 0)
	{
		digits[n_digits++] = '0' + (i % 10);
		i /= 10;
	}
	while (n_digits > 0)
		buf[pos++] = digits[--n_digits];
	buf[pos] = '\0';
}

static int	open_forks(t_data *data)
{
	char	name[64];
	int		i;

	data->forks = (sem_t **)malloc(sizeof(sem_t *) * data->n_ph);
	if (!data->forks)
		return (1);
	for (i = 0; i < data->n_ph; i++)
	{
		fork_name(name, i);
		sem_unlink(name);
		data->forks[i] = sem_open(name, O_CREAT, 0644, 1);
		if (data->forks[i] == SEM_FAILED)
			return (1);
	}
	return (0);
}

int	init_data(t_data *data, char **av)
{
	int	room_capacity;

	data->n_ph = ft_atoi(av[1]);
	data->die_time = ft_atoi(av[2]);
	data->eat_time = ft_atoi(av[3]);
	data->sleep_time = ft_atoi(av[4]);
	data->must_eat = -1;
	if (av[5])
		data->must_eat = ft_atoi(av[5]);
	if (open_forks(data))
		return (1);
	sem_unlink(SEM_ROOM);
	sem_unlink(SEM_PRINT);
	room_capacity = (data->n_ph > 1) ? data->n_ph - 1 : 1;
	data->room = sem_open(SEM_ROOM, O_CREAT, 0644, room_capacity);
	data->print_lock = sem_open(SEM_PRINT, O_CREAT, 0644, 1);
	if (data->room == SEM_FAILED || data->print_lock == SEM_FAILED)
		return (1);
	data->t0 = get_time();
	return (0);
}

void	destroy_data(t_data *data)
{
	char	name[64];
	int		i;

	for (i = 0; i < data->n_ph; i++)
	{
		fork_name(name, i);
		sem_close(data->forks[i]);
		sem_unlink(name);
	}
	free(data->forks);
	sem_close(data->room);
	sem_close(data->print_lock);
	sem_unlink(SEM_ROOM);
	sem_unlink(SEM_PRINT);
}
