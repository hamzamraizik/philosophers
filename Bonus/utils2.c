/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmraizik <hmraizik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:05:28 by hmraizik          #+#    #+#             */
/*   Updated: 2024/08/27 10:04:10 by hmraizik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check(t_data *data)
{
	sem_wait(data->simulation_lock);
	if (data->end_simulation)
	{
		sem_post(data->simulation_lock);
		return (1);
	}
	sem_post(data->simulation_lock);
	return (0);
}

int	check_time_dead(t_data *data, int i)
{
	sem_wait(data->simulation_lock);
	if ((current_time() - data->philos[i].last_meal_time) > data->time_to_die)
	{
		sem_post(data->simulation_lock);
		print_status(data, data->philos[i].id, "died");
		sem_wait(data->simulation_lock);
		data->end_simulation = true;
		return (sem_post(data->simulation_lock), 1);
	}
	sem_post(data->simulation_lock);
	return (0);
}

int	check_meals_full_dead(t_data *data, int count)
{
	if (count == data->number_of_philos)
	{
		print_status(data, count, "are all full, end semulation");
		sem_wait(data->simulation_lock);
		data->end_simulation = true;
		sem_post(data->simulation_lock);
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
		sem_post(data->philos[i].first_fork->fork);
		sem_post(data->philos[i].second_fork->fork);
	}
}

void	update_infos_of_philo(t_data *data, t_philo *philo)
{
	sem_wait(data->simulation_lock);
	philo->last_meal_time = current_time();
	philo->meals_counter += 1;
	sem_post(data->simulation_lock);
}
