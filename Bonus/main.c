#include "philo.h"

void	philos_routine(void *void_data)
{
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo *)void_data;
	data = philo->data;
	if (philo->id % 2 == 0)
		ft_usleep(data->time_to_eat / 2, data);
	while (!check(data))
	{
		sem_wait(philo->first_fork->fork);
		print_status(data, philo->id, "has taken a fork");
		if (data->number_of_philos == 1)
		{
			sem_post(philo->first_fork->fork);
			return;
		}
		sem_wait(philo->second_fork->fork);
		print_status(data, philo->id, "has taken a fork");
		ft_eating(data, philo->id);
		update_infos_of_philo(data, philo);
		sem_post(philo->first_fork->fork);
		sem_post(philo->second_fork->fork);
		ft_sleeping(data, philo->id);
		ft_thinking(data, philo->id);
	}
}

void	routine_monitor(t_data *data)
{
	int		i;
	int		count;

	while (!check(data))
	{
		i = -1;
		count = 0;
		while (++i < data->number_of_philos)
		{
			if (check(data))
				return;
			if (check_time_dead(data, i))
				return;
			sem_wait(data->simulation_lock);
			if (data->must_eat != -1 && data->philos[i].meals_counter >= data->must_eat)
				count += 1;
			sem_post(data->simulation_lock);
			if (check_meals_full_dead(data, count))
				return;
		}
		usleep(1000);
	}
}

int	parsing(char **av, t_data *philos)
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
	if (philos->number_of_philos == -1 
		|| philos->number_of_philos == 0
		|| philos->time_to_die == -1
		|| philos->time_to_eat == -1 || philos->time_to_sleep == -1
		|| (av[5] && philos->must_eat == -1))
		return (-1);
	philos->end_simulation = false;
	return (0);
}

int	main(int ac, char *av[])
{
	t_data	philos;
	int		i;

	if (ac == 5 || ac == 6)
	{
		if (parsing(av, &philos) == -1)
			return (printf("wrong arguments!!\n"), 1);
	}
	else
		return (printf("the number of arguments not enough 🙅!"), 1);
	if (init_data(&philos) == -1)
		return (printf("semaphores initialization error"), 1);

	if (init_philos(&philos) == -1)
	{
		free(philos.forks);
		free(philos.philos);
		return (printf("error in processes initialization"), 1);
	}
	// Wait for all philosopher processes to finish
	for (i = 0; i < philos.number_of_philos; ++i)
		waitpid(philos.philos[i].pid, NULL, 0);

	free(philos.philos);
	return (0);
}

