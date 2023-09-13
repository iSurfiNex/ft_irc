/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:16:54 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/13 14:23:12 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void getCmdArgs(std::string buffer, std::string &cmd, std::set<std::string> &args)
{
	bool two_points = false;
	std::string tmp;

	char *ptr;
	ptr = strtok((char *)buffer.c_str(), " ");
	while (ptr != NULL)
	{
		if (cmd.empty())
			cmd = ptr;
		else if (!strcmp(ptr, ":"))
			two_points = true;

		ptr = strtok(NULL, " ");
	}
}
