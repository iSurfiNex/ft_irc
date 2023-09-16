/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseInput.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:16:54 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/16 20:00:20 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void getCmdArgs(std::string buffer, std::string &cmd, strVec_t &args)
{
	bool two_points = false;
	std::string tmp;

	char *ptr;
	ptr = strtok((char *)buffer.c_str(), " ");
	while (ptr != NULL)
	{
		if (cmd.empty())
			cmd = ptr;
		else if (ptr[0] == ':' && !two_points)
		{
			tmp.append(ptr);
			tmp.erase(0, 1);
			if (!tmp.empty())
				tmp += " ";
			two_points = true;
		}
		else if (two_points)
		{
			tmp.append(ptr);
			tmp += " ";
		}
		else
			args.push_back(ptr);
		ptr = strtok(NULL, " ");
	}
	if (two_points)
		args.push_back(tmp);
}
