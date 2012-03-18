/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2012 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "Device/Driver/LX/LXNAVV7/Internal.hpp"
#include "Device/Port/Port.hpp"
#include "Device/Internal.hpp"
#include "Atmosphere/Pressure.hpp"

#include <cstdio>

bool
LXNAVV7Device::PutBallast(gcc_unused fixed fraction, fixed overload,
                     OperationEnvironment &env)
{
  if (!EnableNMEA(env))
    return false;

  char tmp[32];
  sprintf(tmp, "PLXV0,BAL,W,%.2f*CS", (double)overload);

  PortWriteNMEA(port, tmp);
  return true;
}

bool
LXNAVV7Device::PutBugs(fixed bugs, OperationEnvironment &env)
{
  if (!EnableNMEA(env))
    return false;

  char tmp[32];
  int transformed_bugs_value = 100 - (int)(bugs*100);
  printf("PLXV0,BUGS,W,%d*CS\n", transformed_bugs_value);
  sprintf(tmp, "PLXV0,BUGS,W,%d*CS", transformed_bugs_value);
  PortWriteNMEA(port, tmp);
  return true;
}

bool
LXNAVV7Device::PutMacCready(fixed mac_cready, OperationEnvironment &env)
{
  if (!EnableNMEA(env))
    return false;

  printf("PLXV0,MC,W,%1.1f*CS\n", (double)mac_cready);
  char tmp[32];
  sprintf(tmp, "PLXV0,MC,W,%1.1f*CS", (double)mac_cready);
  PortWriteNMEA(port, tmp);
  return true;
}

bool
LXNAVV7Device::PutQNH(const AtmosphericPressure &pres, OperationEnvironment &env)
{
  /* The V7 apparent does not accept an altimeter setting. */
  return false;
}
