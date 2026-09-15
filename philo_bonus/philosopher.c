#include "philo_bonus.h"

/* Every philosopher picks up left fork then right fork: with n forks
** represented one-per-semaphore, this needs the room gate below to rule
** out deadlock, same reasoning as the mandatory part's philosopher.c. */
static void	take_forks(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->id - 1;
	right = philo->id % philo->data->n_ph;
	sem_wait(philo->data->forks[left]);
	printing(philo, "has taken a fork");
	sem_wait(philo->data->forks[right]);
	printing(philo, "has taken a fork");
}

static void	put_forks(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->id - 1;
	right = philo->id % philo->data->n_ph;
	sem_post(philo->data->forks[right]);
	sem_post(philo->data->forks[left]);
}

/* The room seat is held for the whole cycle, not just while picking up
** forks: releasing it early would let another process start competing
** while this one is still eating, defeating the room's whole purpose. */
static void	eat_sleep_think(t_philo *philo)
{
	sem_wait(philo->data->room);
	take_forks(philo);
	philo->last_meal = get_time();
	printing(philo, "is eating");
	precise_sleep(philo->data->eat_time);
	philo->meals_done++;
	if (philo->data->must_eat > 0 && philo->meals_done >= philo->data->must_eat)
		philo->stop = 1;
	put_forks(philo);
	sem_post(philo->data->room);
	printing(philo, "is sleeping");
	precise_sleep(philo->data->sleep_time);
	printing(philo, "is thinking");
}

/* stop is set mid-cycle, right after the meal quota is met, rather than
** only rechecked at the top of the loop below: that keeps the window
** where monitor_life could still see a stale last_meal and fire a false
** death down to one poll interval instead of a whole extra cycle. */
void	run_philo(t_philo *philo)
{
	philo->last_meal = get_time();
	philo->meals_done = 0;
	philo->stop = 0;
	if (pthread_create(&philo->monitor, NULL, &monitor_life, philo))
		exit(1);
	while (!philo->stop)
		eat_sleep_think(philo);
	pthread_join(philo->monitor, NULL);
	exit(0);
}
