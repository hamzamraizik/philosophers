/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizoo <mizoo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:05:08 by hmraizik          #+#    #+#             */
/*   Updated: 2024/08/05 15:11:57 by mizoo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		return (printf("the number of arguments not enough 🙅!"), 1);
	if (init_data(&philos) == -1)
		return (printf("mutexes initialization error"), 1);
	if (init_philos(&philos) == -1)
		return (free(philos.forks), free(philos.philos)
			, printf("errore in threads initialization"), 1);
	free(philos.forks);
	free(philos.philos);
	return (0);
}
