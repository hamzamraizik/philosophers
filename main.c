
#include "philo.h"

void *philos_routine(void *void_data)
{
    t_data *data;
    t_philo *philo;

	philo = (t_philo*)void_data;
	data = philo->data;
    while (1)
    {
        pthread_mutex_lock(&data->simulation_lock);
        if (data->end_simulation)
        {
            pthread_mutex_unlock(&data->simulation_lock);
           	break;
        }
        pthread_mutex_unlock(&data->simulation_lock);
        ft_thinking(data, philo->id);
        if (philo->id % 2 != 0)
        {
            pthread_mutex_lock(&philo->first_fork->fork);
            print_status(data, philo->id, "has taken a fork");
            pthread_mutex_lock(&philo->second_fork->fork);
            print_status(data, philo->id, "has taken a fork");
        }
        else
        {
            pthread_mutex_lock(&philo->second_fork->fork);
            print_status(data, philo->id, "has taken a fork");
            pthread_mutex_lock(&philo->first_fork->fork);
            print_status(data, philo->id, "has taken a fork");
        }
		ft_eating(data, philo->id);
        pthread_mutex_unlock(&philo->first_fork->fork);
        pthread_mutex_unlock(&philo->second_fork->fork);
        ft_sleeping(data, philo->id);
    }
    return (NULL);
}

void *routine_monitor(void *void_data)
{
	t_data	*data;
	int		i;
	int		is_full_counter;

	is_full_counter = 0;
	data = (t_data *)void_data;
	while (1)
	{
		i = -1;
		while (++i < data->number_of_philos)
		{
			pthread_mutex_lock(&data->simulation_lock);
			if ((data->how_much_time_must_eat) != -1 && (data->philos[i].meals_counter) >= (data->how_much_time_must_eat))
					is_full_counter += 1;
			if (is_full_counter >= data->number_of_philos)
			{
				print_status(data, data->number_of_philos, "are all full, end semulation");
				data->end_simulation = true;
				pthread_mutex_unlock(&data->simulation_lock);
				// ft_end(data, 0);
				return (NULL);
			}
			if (current_time() - data->philos[i].last_meal_time > data->time_to_die)
			{
				data->end_simulation = true;
				print_status(data, data->philos[i].id, "died");
				pthread_mutex_unlock(&data->simulation_lock);
				ft_end(data, 1);
				return(NULL);
			}
			pthread_mutex_unlock(&data->simulation_lock);
		}
		ft_usleep(1000);
	}
	return (NULL);
}

void	parsing(char **av, t_data *philos)
{
	philos->number_of_philos = ft_atoi(av[1]);
	// from ms to microsecond.
	philos->time_to_die = ft_atoi(av[2]) * 1000;
	philos->time_to_eat = ft_atoi(av[3]) * 1000;
	philos->time_to_sleep = ft_atoi(av[4]) * 1000;
	if (av[5])
		philos->how_much_time_must_eat = ft_atoi(av[5]);
	else if (!av[5])
		philos->how_much_time_must_eat = -1;
	philos->end_simulation = false;
}
void	ft_end(t_data *philos, int j)
{
	int		i;

	i = 0;
	(void)j;
	while (i < philos->number_of_philos)
	{
		pthread_detach(philos->philos[i].thread_id);
		i++;
	}
	pthread_detach(philos->monitor_thread);
	i = 0;
	while (i < philos->number_of_philos)
	{
		pthread_mutex_destroy(&philos->forks[i].fork);
		i++;
	}
	pthread_mutex_destroy(&philos->print_lock);
	pthread_mutex_destroy(&philos->simulation_lock);
	free(philos->forks);
	free(philos->philos);
	return ;
	// exit(j);
}

int main(int ac, char *av[])
{
	t_data	philos;

	if (ac == 5 || ac == 6)
		parsing(av, &philos);
	else
		ft_exit("the number of arguments not enough 🙅!");
	init_data(&philos);
	init_philos(&philos);
	ft_end(&philos, 0);
	return 0;
}
