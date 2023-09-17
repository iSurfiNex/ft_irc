/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsterin <rsterin@student.42angouleme.fr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 18:49:37 by rsterin           #+#    #+#             */
/*   Updated: 2023/09/17 18:49:37 by rsterin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "IrcServer.hpp"
#include <algorithm> // for std::transform
#include <cctype>    // for std::tolower
#include <ctype.h>   // isalnum

bool str_alnum(std::string &str);
std::string itoa(int num);
std::string tolowerStr(const std::string &str);
