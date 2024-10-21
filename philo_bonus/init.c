/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmraizik <hmraizik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 16:14:07 by hmraizik          #+#    #+#             */
/*   Updated: 2024/09/19 03:54:50 by hmraizik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->number_of_philos);
	if (!data->philos)
		return (-1);
	i = -1;
	while (++i < data->number_of_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_counter = 0;
		data->philos[i].last_meal_time = current_time();
		data->philos[i].data = data;
	}
	return (0);
}

int	init_data(t_data *data)
{
	sem_unlink("forks");
	sem_unlink("print");
	sem_unlink("sim_lock");
	data->forks = sem_open("forks", O_CREAT, 0644, data->number_of_philos);
	data->print_lock = sem_open("print", O_CREAT, 0644, 1);
	data->sim_lock = sem_open("sim_lock", O_CREAT, 0644, 1);
	if (data->forks == SEM_FAILED || data->print_lock == SEM_FAILED
		|| data->sim_lock == SEM_FAILED)
		return (-1);
	if (init_philos(data) == -1)
		return (-1);
	return (0);
}

int	check_meals_dead(t_philo *philo)
{
	sem_wait(philo->data->sim_lock);
	if (philo->data->must_eat != -1 && philo->meals_counter
		>= philo->data->must_eat)
	{
		sem_post(philo->data->sim_lock);
		return (1);
	}
	sem_post(philo->data->sim_lock);
	return (0);
}

void	philosopher_process(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	while (philo->data->must_eat == -1 || (philo->data->must_eat != -1
			&& philo->meals_counter < philo->data->must_eat))
	{
		sem_wait(data->forks);
		print_status(data, philo->id, "has taken a fork");
		sem_wait(data->forks);
		print_status(data, philo->id, "has taken a fork");
		update_infos_of_philo(data, philo);
		ft_eating(data, philo->id);
		sem_post(data->forks);
		sem_post(data->forks);
		ft_sleeping(data, philo->id);
		print_status(data, philo->id, "is thinking");
	}
	exit(0);
}

void	cleanup_semaphores(t_data *data)
{
	sem_close(data->forks);
	sem_close(data->print_lock);
	sem_close(data->sim_lock);
	sem_unlink("forks");
	sem_unlink("print");
	sem_unlink("sim_lock");
}
