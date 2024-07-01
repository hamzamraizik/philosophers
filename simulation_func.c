#include "philo.h"

void	ft_sleeping(t_data *data, int i)
{
	print_status(data, i, "is sleeping");
	ft_usleep(data->time_to_sleep);
}

void	ft_eating(t_data *data, int i)
{
	data->philos[i].meals_counter += 1;
	print_status(data, i, "is eating");
	ft_usleep(data->time_to_eat);
	if (data->how_much_time_must_eat != -1 && data->philos[i].meals_counter == data->how_much_time_must_eat)
		data->is_full_counter += 1;
	data->philos[i].last_meal_time = current_time();
}

void	ft_thinking(t_data *data, long philo_id)
{
	print_status(data, philo_id, "is thinking");
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
	if (!data->end_simulation)
		printf("%ld %d %s\n", current_time() - data->start_time, id, status);
	pthread_mutex_unlock(&data->print_lock);
}
