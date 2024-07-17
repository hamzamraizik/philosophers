/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmraizik <hmraizik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:05:02 by hmraizik          #+#    #+#             */
/*   Updated: 2024/07/17 11:05:03 by hmraizik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if (pthread_create(&data->philos[i].thread_id, NULL
				, philos_routine, &data->philos[i]) != 0)
			return (-1);
	}
	if (pthread_create(&data->monitor_thread, NULL, routine_monitor, data) != 0)
		return (-1);
	i = -1;
	while (++i < data->number_of_philos)
		if (pthread_join(data->philos[i].thread_id, NULL) != 0)
			return (-1);
	if (pthread_join(data->monitor_thread, NULL) != 0)
		return (-1);
	return (0);
}

int	init_data(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->number_of_philos);
	if (!data->philos)
		return (-1);
	data->forks = malloc(data->number_of_philos * sizeof(t_fork));
	if (!data->forks)
		return (-1);
	if (pthread_mutex_init(&(data->print_lock), NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&data->simulation_lock, NULL) != 0)
		return (-1);
	i = -1;
	while (++i < data->number_of_philos)
	{
		if (pthread_mutex_init(&(data->forks[i].fork), NULL) != 0)
			return (-1);
		data->forks[i].fork_id = i + 1;
	}
	return (0);
}
