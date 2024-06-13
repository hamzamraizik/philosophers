#include "philo.h"

void	ft_sleeping(t_data *data, int i)
{
	print_status(data, i, "sleeping...");
	ft_usleep(data->time_to_sleep);
}
void	ft_eating(t_data *data, int i)
{
	data->philos[i].meals_counter += 1;
	data->philos[i].last_meal_time = current_time();
	print_status(data, i, "is eating...");
	ft_usleep(data->time_to_eat);
}

void	ft_thinking(t_data *data, long philo_id)
{
	print_status(data, philo_id, "is thinking...");
}

long current_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}

void print_status(t_data *data, int id, const char *status)
{
	pthread_mutex_lock(&data->print_lock);
	// pthread_mutex_lock(&data->simulation_lock);
	if (id)
		printf("%ld %d %s\n", current_time(), id, status);
	// if (!data->end_simulation)
	// pthread_mutex_unlock(&data->simulation_lock);
	pthread_mutex_unlock(&data->print_lock);
}
