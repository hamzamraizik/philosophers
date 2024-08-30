#include "philo.h"

int	init_philos(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->number_of_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_counter = 0;
		data->philos[i].last_meal_time = current_time();
		data->philos[i].first_fork = &data->forks[i];
		data->philos[i].second_fork
			= &data->forks[(i + 1) % data->number_of_philos];
		data->philos[i].data = data;
	if (sem_open("forks", O_CREAT, 0644, data->number_of_philos) == SEM_FAILED)
		return (-1);
	if (sem_open("print", O_CREAT, 0644, 1) == SEM_FAILED)
		return (-1);
	}
	i = -1;
	return (0);
}

int	init_data(t_data *data)
{
	int	i;
	int	pid;

	data->philos = malloc(sizeof(t_philo) * data->number_of_philos);
	if (!data->philos)
		return (-1);
	data->forks = malloc(data->number_of_philos * sizeof(t_fork));
	if (!data->forks)
		return (-1);
	i = -1;
	while (++i < data->number_of_philos)
	{
		pid = fork();
		if (pid != -1)
		{
			philos_routine(&data->philos[i]);
		}
	}
	return (0);
}

void	*philos_routine(void *void_data)
{
	t_data	*data;
	t_philo	*philo;

	philo = (t_philo *)void_data;
	data = philo->data;
	if (philo->id % 2 == 0)
		ft_usleep(data->time_to_eat / 2, data);
	while (check(data) == 0)
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
	while (!check(data))
	{
		i = -1;
		count = 0;
		while (++i < data->number_of_philos)
		{
			if (check(data) == 1)
				return (NULL);
			if (check_time_dead(data, i) == 1)
				return (NULL);
			pthread_mutex_lock(&data->simulation_lock);
			if (data->must_eat != -1 && data->philos[i].meals_counter >= data->must_eat)
					count += 1;
			pthread_mutex_unlock(&data->simulation_lock);
			if (check_meals_full_dead(data, count) == 1)
				return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
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

	if (ac == 5 || ac == 6)
	{
		if (parsing(av, &philos) == -1)
			return (printf("wrong arguments!!\n"), 1);
	}
	else
		return (printf("the number of arguments not enough 🙅!\n"), 1);
	if (init_data(&philos) == -1)
		return (printf("mutexes initialization error\n"), 1);
	if (init_philos(&philos) == -1)
		return (free(philos.forks), free(philos.philos)
			, printf("errore in threads initialization\n"), 1);
	free(philos.forks);
	free(philos.philos);
	return (0);
}

void	ft_sleeping(t_data *data, int i)
{
	print_status(data, i, "is sleeping");
	ft_usleep(data->time_to_sleep, data);
}

void	ft_eating(t_data *data, int i)
{
	print_status(data, i, "is eating");
	ft_usleep(data->time_to_eat, data);
}

void	ft_thinking(t_data *data, long philo_id)
{
	print_status(data, philo_id, "is thinking");
}

long	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_status(t_data *data, int id, const char *status)
{
	pthread_mutex_lock(&data->print_lock);
	if (check(data) == 0)
		printf("%ld %d %s\n", current_time() - data->start_time, id, status);
	pthread_mutex_unlock(&data->print_lock);
}

int	check(t_data *data)
{
	pthread_mutex_lock(&data->simulation_lock);
	if (data->end_simulation)
	{
		pthread_mutex_unlock(&data->simulation_lock);
		return (1);
	}
	pthread_mutex_unlock(&data->simulation_lock);
	return (0);
}

int	check_time_dead(t_data *data, int i)
{
	pthread_mutex_lock(&data->simulation_lock);
	if ((current_time() - data->philos[i].last_meal_time) > data->time_to_die)
	{
		pthread_mutex_unlock(&data->simulation_lock);
		print_status(data, data->philos[i].id, "died");
		pthread_mutex_lock(&data->simulation_lock);
		data->end_simulation = true;
		return (pthread_mutex_unlock(&data->simulation_lock), 1);
	}
	pthread_mutex_unlock(&data->simulation_lock);
	return (0);
}

int	check_meals_full_dead(t_data *data, int count)
{
	if (count == data->number_of_philos)
	{
		print_status(data, count, "are all full, end semulation");
		pthread_mutex_lock(&data->simulation_lock);
		data->end_simulation = true;
		pthread_mutex_unlock(&data->simulation_lock);
		return (1);
	}
	return (0);
}

void	ft_unlock(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->number_of_philos)
	{
		pthread_mutex_unlock(&data->philos[i].first_fork->fork);
		pthread_mutex_unlock(&data->philos[i].second_fork->fork);
	}
}

void	update_infos_of_philo(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&data->simulation_lock);
	philo->last_meal_time = current_time();
	philo->meals_counter += 1;
	pthread_mutex_unlock(&data->simulation_lock);
}

int	ft_atoi(char *s)
{
	int		i;
	int		result;
	int		number;

	result = 0;
	number = 0;
	i = 0;
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '+')
		i++;
	while (s[i] >= '0' && s[i] <= '9')
	{
		result = result * 10 + s[i] - '0';
		if (result < number)
			return (-1);
		number = result;
		i++;
	}
	if (s[i])
		return (-1);
	return (result);
}

void	ft_usleep(long time, t_data *data)
{
	long	time_before;

	time_before = current_time();
	while (current_time() - time_before < time)
	{
		if (check(data) == 1)
			return ;
		usleep(100);
	}
}
