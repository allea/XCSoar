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

#include "Device/Driver/LX.hpp"
#include "Device/Driver/LX/LXNAVV7/Internal.hpp"
#include "Profile/DeviceConfig.hpp"

static Device *
LXNAVV7CreateOnPort(const DeviceConfig &config, Port &com_port)
{
  return new LXNAVV7Device(com_port, config.bulk_baud_rate);
}

const struct DeviceRegister lxNavV7Device = {
  _T("LXNAV"),
  _T("LXNAV V7"),
  DeviceRegister::PASS_THROUGH |
  DeviceRegister::BULK_BAUD_RATE |
  DeviceRegister::RECEIVE_SETTINGS | DeviceRegister::SEND_SETTINGS,
  LXNAVV7CreateOnPort,
};
