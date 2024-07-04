#include "philo.h"

void	*philos_routine(void *void_data)
{
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo *)void_data;
	data = philo->data;
	if (philo->id % 2 == 0)
		ft_usleep(data->time_to_eat / 2);
	while (!check(data))
	{
		pthread_mutex_lock(&philo->first_fork->fork);
		print_status(data, philo->id, "has taken a fork");
		if (data->number_of_philos == 1)
			return (pthread_mutex_unlock(&philo->first_fork->fork), NULL);
		pthread_mutex_lock(&philo->second_fork->fork);
		print_status(data, philo->id, "has taken a fork");
		ft_eating(data, philo->id);
		update_infos_of_philo(data, philo);
		pthread_mutex_unlock(&philo->first_fork->fork);
		pthread_mutex_unlock(&philo->second_fork->fork);
		ft_sleeping(data, philo->id);
		ft_thinking(data, philo->id);
	}
	return (NULL);
}

void	*routine_monitor(void *void_data)
{
	t_data	*data;
	int		i;
	int		count;

	data = (t_data *)void_data;
	while (1)
	{
		i = -1;
		count = 0;
		while (++i < data->number_of_philos)
		{
			if (check(data))
				return (NULL);
			if (check_time_dead(data, i) == 1)
				return (NULL);
			pthread_mutex_lock(&data->simulation_lock);
			if (data->must_eat != -1 && data->philos[i].meals_counter
				>= data->must_eat)
				count += 1;
			pthread_mutex_unlock(&data->simulation_lock);
			(check_meals_full_dead(data, count) == 1) && (return (NULL));
		}
		usleep(1000);
	}
	return (NULL);
}

void	parsing(char **av, t_data *philos)
{
	philos->number_of_philos = ft_atoi(av[1]);
	philos->time_to_die = ft_atoi(av[2]);
	philos->time_to_eat = ft_atoi(av[3]);
	philos->time_to_sleep = ft_atoi(av[4]);
	philos->is_full_counter = 0;
	philos->start_time = current_time();
	if (av[5])
		philos->must_eat = ft_atoi(av[5]);
	else
		philos->must_eat = -1;
	philos->end_simulation = false;
}

void	ft_end(t_data *philos, int j)
{
	int	i;

	i = -1;
	(void)j;
	while (++i < philos->number_of_philos)
		pthread_detach((philos->philos[i].thread_id));
	i = -1;
	while (++i < philos->number_of_philos)
		pthread_mutex_destroy(&philos->forks[i].fork);
	pthread_mutex_destroy(&philos->print_lock);
	pthread_mutex_destroy(&philos->simulation_lock);
	free(philos->forks);
	free(philos->philos);
	return ;
}

int	main(int ac, char *av[])
{
	t_data	philos;

	if (ac == 5 || ac == 6)
		parsing(av, &philos);
	else
		ft_exit("the number of arguments not enough 🙅!");
	init_data(&philos);
	init_philos(&philos);
	free(philos.forks);
	free(philos.philos);
	// ft_end(&philos, 0);
	return (0);
}
