/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 15:15:40 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/07/25 15:49:26 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile sig_atomic_t	g_ack_received = 0;

void	handle_ack(int sig)
{
	(void)sig;
	g_ack_received = 1;
}

void	wait_for_ack(void)
{
	int	attempts;

	attempts = 0;
	while (!g_ack_received && attempts++ < 1000000)
		usleep(1);
	if (!g_ack_received)
		ft_printf("Didnt receive ackg in time.\n");
}

void	send_char_as_bits(unsigned	char c, int pid)
{
	int	current_bit;

	current_bit = 7;
	while (current_bit >= 0)
	{
		if ((c >> current_bit) & 1)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		wait_for_ack();
		current_bit--;
		g_ack_received = 0;
	}
}

void	validate_pid(int pid)
{
	if (pid <= 0)
	{
		ft_printf("❌Invalid PID (%d). Must be > 0\n", pid);
		exit(EXIT_FAILURE);
	}
	if (kill(pid, 0) == -1)
	{
		ft_printf("❌No such process or no permission for PID %d\n", pid);
		exit(EXIT_FAILURE);
	}
}

int	main(int argc, char *argv[])
{
	int					pid;
	char				*message;
	struct sigaction	sa;

	if (argc != 3)
	{
		ft_printf("Usage: %s <PID> <message>\n", argv[0]);
		return (0);
	}
	pid = ft_atoi(argv[1]);
	validate_pid(pid);
	message = argv[2];
	sa.sa_handler = handle_ack;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, NULL);
	while (*message)
		send_char_as_bits(*message++, pid);
	send_char_as_bits('\0', pid);
	return (0);
}
