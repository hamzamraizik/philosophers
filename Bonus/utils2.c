
#include "philo.h"

int check(t_data *data)
{
    int result;
    sem_wait(data->sim_lock);
    result = data->end_simulation;
    sem_post(data->sim_lock);
    return result;
}


// int	check_time_dead(t_data *data, int i)
// {
// 	pthread_mutex_lock(&data->simulation_lock);
// 	if ((current_time() - data->philos[i].last_meal_time) > data->time_to_die)
// 	{
// 		pthread_mutex_unlock(&data->simulation_lock);
// 		print_status(data, data->philos[i].id, "died");
// 		pthread_mutex_lock(&data->simulation_lock);
// 		data->end_simulation = true;
// 		return (pthread_mutex_unlock(&data->simulation_lock), 1);
// 	}
// 	pthread_mutex_unlock(&data->simulation_lock);
// 	return (0);
// }

// int	check_meals_full_dead(t_data *data, int count)
// {
// 	if (count == data->number_of_philos)
// 	{
// 		print_status(data, count, "are all full, end semulation");
// 		pthread_mutex_lock(&data->simulation_lock);
// 		data->end_simulation = true;
// 		pthread_mutex_unlock(&data->simulation_lock);
// 		return (1);
// 	}
// 	return (0);
// }

// void	ft_unlock(t_data *data)
// {
// 	int	i;

// 	i = -1;
// 	while (++i < data->number_of_philos)
// 	{
// 		pthread_mutex_unlock(&data->philos[i].first_fork->fork);
// 		pthread_mutex_unlock(&data->philos[i].second_fork->fork);
// 	}
// }

void update_infos_of_philo(t_data *data, t_philo *philo)
{
    sem_wait(data->sim_lock);
    philo->last_meal_time = current_time();
    philo->meals_counter += 1;
    sem_post(data->sim_lock);
}

