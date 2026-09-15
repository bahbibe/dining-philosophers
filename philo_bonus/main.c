/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bahbibe <bahbibe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 00:00:00 by bahbibe           #+#    #+#             */
/*   Updated: 2026/09/15 00:00:00 by bahbibe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	kill_processes(pid_t *pids, int n_ph, pid_t except)
{
	int	i;

	for (i = 0; i < n_ph; i++)
		if (pids[i] > 0 && pids[i] != except)
			kill(pids[i], SIGKILL);
	for (i = 0; i < n_ph; i++)
		if (pids[i] > 0)
			waitpid(pids[i], NULL, 0);
}

/* Reaps children one by one; the first to exit nonzero has starved (and
** already printed its own death line), so every other survivor is killed
** on the spot. If none ever does, this returns once all n_ph have
** exited cleanly on their own (meal quota reached). */
int	wait_processes(pid_t *pids, int n_ph)
{
	int		i;
	int		status;
	pid_t	dead;

	for (i = 0; i < n_ph; i++)
	{
		dead = waitpid(-1, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			kill_processes(pids, n_ph, dead);
			return (1);
		}
	}
	return (0);
}

/* On a fork() failure partway through, the loop stops immediately rather
** than pressing on to spawn the rest: the unset tail of pids[] is filled
** with -1 so cleanup never touches an uninitialized/leftover pid. */
pid_t	*create_processes(t_data *data, int *ok)
{
	pid_t	*pids;
	t_philo	philo;
	int		i;

	pids = (pid_t *)malloc(sizeof(pid_t) * data->n_ph);
	if (!pids)
		return (NULL);
	*ok = 1;
	i = 0;
	while (i < data->n_ph && *ok)
	{
		philo.data = data;
		philo.id = i + 1;
		pids[i] = fork();
		if (pids[i] < 0)
			*ok = 0;
		else if (pids[i] == 0)
			run_philo(&philo);
		i++;
	}
	while (i < data->n_ph)
		pids[i++] = -1;
	return (pids);
}

int	main(int ac, char **av)
{
	t_data	data;
	pid_t	*pids;
	int		ok;

	if (ac != 5 && ac != 6)
		return (printf(USAGE), 1);
	if (check_args(av))
		return (1);
	if (init_data(&data, av))
		return (destroy_data(&data), 1);
	pids = create_processes(&data, &ok);
	if (!pids || !ok)
	{
		if (pids)
			kill_processes(pids, data.n_ph, -1);
		return (free(pids), destroy_data(&data), 1);
	}
	wait_processes(pids, data.n_ph);
	return (free(pids), destroy_data(&data), 0);
}
