#include "philo.h"

/* Every philosopher picks up left fork then right fork, in that order:
** deadlock-freedom here comes entirely from the room gate (room.c), so
** there is no need for a special-cased pickup order to break the cycle. */
static void	take_forks(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->id - 1;
	right = philo->id % philo->data->n_ph;
	pthread_mutex_lock(&philo->data->fork[left]);
	printing(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->fork[right]);
	printing(philo, "has taken a fork");
}

static void	release_forks(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->id - 1;
	right = philo->id % philo->data->n_ph;
	pthread_mutex_unlock(&philo->data->fork[right]);
	pthread_mutex_unlock(&philo->data->fork[left]);
}

/* The room seat is held for the whole cycle, not just while picking up
** forks: releasing it early would let a new philosopher start competing
** while this one is still eating, defeating the room's whole purpose. */
static void	eat_sleep_think(t_philo *philo)
{
	enter_room(philo);
	take_forks(philo);
	pthread_mutex_lock(&philo->data->meal_lock);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->data->meal_lock);
	printing(philo, "is eating");
	precise_sleep(philo->data->eat_time);
	pthread_mutex_lock(&philo->data->progress_lock);
	philo->meals_left--;
	pthread_mutex_unlock(&philo->data->progress_lock);
	release_forks(philo);
	leave_room(philo);
	printing(philo, "is sleeping");
	precise_sleep(philo->data->sleep_time);
	printing(philo, "is thinking");
}

/* A lone philosopher has one fork and can never get a second: they pick
** it up (as the subject requires) and just wait to starve. Handled here
** rather than by feeding n_ph == 1 through take_forks(), which would lock
** the same (only) fork mutex twice and deadlock. */
static int	still_running(t_philo *philo)
{
	int	running;

	pthread_mutex_lock(&philo->data->print_lock);
	running = philo->data->running;
	pthread_mutex_unlock(&philo->data->print_lock);
	return (running);
}

static void	starve_alone(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->fork[0]);
	printing(philo, "has taken a fork");
	while (still_running(philo))
		usleep(1000);
	pthread_mutex_unlock(&philo->data->fork[0]);
}

void	*live(void *arg)
{
	t_philo	*philo;
	int		meals_left;

	philo = (t_philo *)arg;
	if (philo->data->n_ph == 1)
	{
		starve_alone(philo);
		return (NULL);
	}
	while (still_running(philo))
	{
		pthread_mutex_lock(&philo->data->progress_lock);
		meals_left = philo->meals_left;
		if (meals_left == 0)
			philo->finished = 1;
		pthread_mutex_unlock(&philo->data->progress_lock);
		if (meals_left == 0)
			break ;
		eat_sleep_think(philo);
	}
	return (NULL);
}
