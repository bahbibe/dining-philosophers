#include "philo.h"

/*
** Admission alone (at most n_ph - 1 in the room) only proves no deadlock;
** it says nothing about who gets picked first. Left to plain first-come
** admission, one philosopher can repeatedly lose the race for a fork to
** neighbours who keep re-entering sooner, and starve well within
** die_time even though the room bound is respected the whole time.
**
** So admission also requires being among the n_ph - 1 who have gone
** longest without eating: whoever is currently the single freshest-fed
** philosopher waits one extra turn instead of racing in on equal footing.
** That turns "no one is locked out forever" into "the most urgent one is
** never the one left waiting".
*/
static int	is_most_urgent(t_philo *philo)
{
	int				i;
	int				more_urgent_than_me;
	unsigned long	mine;
	unsigned long	other;

	pthread_mutex_lock(&philo->data->meal_lock);
	mine = philo->last_meal;
	pthread_mutex_unlock(&philo->data->meal_lock);
	more_urgent_than_me = 0;
	for (i = 0; i < philo->data->n_ph; i++)
	{
		pthread_mutex_lock(&philo->data->meal_lock);
		other = philo->data->list[i].last_meal;
		pthread_mutex_unlock(&philo->data->meal_lock);
		if (philo->data->list[i].id != philo->id && other < mine)
			more_urgent_than_me++;
	}
	return (more_urgent_than_me < philo->data->n_ph - 1);
}

void	enter_room(t_philo *philo)
{
	int	admitted;

	admitted = 0;
	while (!admitted)
	{
		pthread_mutex_lock(&philo->data->room_lock);
		if (philo->data->room < philo->data->n_ph - 1 && is_most_urgent(philo))
		{
			philo->data->room++;
			admitted = 1;
		}
		pthread_mutex_unlock(&philo->data->room_lock);
		if (!admitted)
			usleep(200);
	}
}

void	leave_room(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->room_lock);
	philo->data->room--;
	pthread_mutex_unlock(&philo->data->room_lock);
}
