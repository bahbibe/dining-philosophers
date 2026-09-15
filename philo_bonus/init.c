#include "philo_bonus.h"

static int	append_str(char *buf, int pos, const char *s)
{
	while (*s)
		buf[pos++] = *s++;
	return (pos);
}

static int	append_uint(char *buf, int pos, unsigned long n)
{
	char	digits[20];
	int		n_digits;

	n_digits = 0;
	if (n == 0)
		digits[n_digits++] = '0';
	while (n > 0)
	{
		digits[n_digits++] = '0' + (n % 10);
		n /= 10;
	}
	while (n_digits > 0)
		buf[pos++] = digits[--n_digits];
	return (pos);
}

/*
** Builds "<prefix><instance_id>[_<idx>]" into buf. Hand-rolled instead of
** strcpy/snprintf: neither is in the bonus's authorized function list.
*/
static void	sem_name(char *buf, const char *prefix, unsigned long id, int idx)
{
	int	pos;

	pos = append_str(buf, 0, prefix);
	pos = append_uint(buf, pos, id);
	if (idx >= 0)
	{
		buf[pos++] = '_';
		pos = append_uint(buf, pos, idx);
	}
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
		sem_name(name, SEM_FORK_PREFIX, data->instance_id, i);
		sem_unlink(name);
		data->forks[i] = sem_open(name, O_CREAT, 0644, 1);
		if (data->forks[i] == SEM_FAILED)
			return (1);
	}
	return (0);
}

int	init_data(t_data *data, char **av)
{
	char	name[64];
	int		room_capacity;

	data->instance_id = (unsigned long)unique_id() ^ (uintptr_t)data;
	data->n_ph = ft_atoi(av[1]);
	data->die_time = ft_atoi(av[2]);
	data->eat_time = ft_atoi(av[3]);
	data->sleep_time = ft_atoi(av[4]);
	data->must_eat = -1;
	if (av[5])
		data->must_eat = ft_atoi(av[5]);
	if (open_forks(data))
		return (1);
	room_capacity = (data->n_ph > 1) ? data->n_ph - 1 : 1;
	sem_name(name, SEM_ROOM_PREFIX, data->instance_id, -1);
	sem_unlink(name);
	data->room = sem_open(name, O_CREAT, 0644, room_capacity);
	sem_name(name, SEM_PRINT_PREFIX, data->instance_id, -1);
	sem_unlink(name);
	data->print_lock = sem_open(name, O_CREAT, 0644, 1);
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
		sem_name(name, SEM_FORK_PREFIX, data->instance_id, i);
		sem_close(data->forks[i]);
		sem_unlink(name);
	}
	free(data->forks);
	sem_close(data->room);
	sem_close(data->print_lock);
	sem_name(name, SEM_ROOM_PREFIX, data->instance_id, -1);
	sem_unlink(name);
	sem_name(name, SEM_PRINT_PREFIX, data->instance_id, -1);
	sem_unlink(name);
}
