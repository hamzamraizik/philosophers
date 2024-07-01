
#include "philo.h"

void *philos_routine(void *void_data)
{
	t_data *data;
	t_philo *philo;

	philo = (t_philo*)void_data;
	data = philo->data;
	if (philo->id % 2 == 0)
		ft_usleep(data->time_to_eat);
	while (1)
	{
		pthread_mutex_lock(&data->simulation_lock);
		if (data->end_simulation)
			return (NULL);
		pthread_mutex_unlock(&data->simulation_lock);
		ft_thinking(data, philo->id);
		pthread_mutex_lock(&philo->first_fork->fork);
		print_status(data, philo->id, "has taken a fork");
		pthread_mutex_lock(&philo->second_fork->fork);
		print_status(data, philo->id, "has taken a fork");
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

	data = (t_data *)void_data;
	while (!data->end_simulation)
	{
		i = -1;
		while (++i < data->number_of_philos)
		{
			pthread_mutex_lock(&data->simulation_lock);
			if (data->end_simulation)
				break ;
			if ((current_time()) - data->philos[i].last_meal_time > data->time_to_die)
			{
				print_status(data, data->philos[i].id, "died");
				data->end_simulation = true;
				pthread_mutex_unlock(&data->simulation_lock);
				break ;
			}
			if (data->is_full_counter >= data->number_of_philos)
			{
				print_status(data, data->number_of_philos, "are all full, end semulation");
				data->end_simulation = true;
				pthread_mutex_unlock(&data->simulation_lock);
				break ;
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
	philos->time_to_die = ft_atoi(av[2]) * 1000;
	philos->time_to_eat = ft_atoi(av[3]) * 1000;
	philos->time_to_sleep = ft_atoi(av[4]) * 1000;
	philos->is_full_counter = 0;
	philos->start_time = current_time();
	if (av[5])
		philos->how_much_time_must_eat = ft_atoi(av[5]);
	else
		philos->how_much_time_must_eat = -1;
	philos->end_simulation = false;
}

void	ft_end(t_data *philos, int j)
{
	int		i;

	i = -1;
	(void)j;
	while (++i < philos->number_of_philos)
		pthread_mutex_destroy(&philos->forks[i].fork);
	pthread_mutex_destroy(&philos->print_lock);
	// pthread_mutex_destroy(&philos->simulation_lock);
	free(philos->forks);
	free(philos->philos);
	return ;
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
