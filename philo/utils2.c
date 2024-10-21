/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmraizik <hmraizik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 18:21:12 by hmraizik          #+#    #+#             */
/*   Updated: 2024/09/08 18:23:23 by hmraizik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	if ((current_time() - data->philos[i].last_meal_time)
		> data->time_to_die)
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
		pthread_mutex_lock(&data->simulation_lock);
		data->end_simulation = true;
		pthread_mutex_unlock(&data->simulation_lock);
		return (1);
	}
	return (0);
}

void	update_infos_of_philo(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&data->simulation_lock);
	philo->last_meal_time = current_time();
	philo->meals_counter += 1;
	pthread_mutex_unlock(&data->simulation_lock);
}

void	ft_free(void *ptr, void *ptr2)
{
	if (ptr)
		free(ptr);
	if (ptr2)
		free(ptr2);
	return ;
}
