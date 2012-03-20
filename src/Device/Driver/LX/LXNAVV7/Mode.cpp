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

#include "Internal.hpp"
#include "Device/Port/Port.hpp"
#include "Device/Internal.hpp"


/**
 * Enable pass-through mode on the V7.  
 */

bool
LXNAVV7Device::EnablePassThrough(OperationEnvironment &env)
{
  /* remember baud rate */
  old_baud_rate = port.GetBaudrate();

  return PortWriteNMEA(port, "PLXV0,CONNECTION,W,DIRECT");
}


// EnableNMEA from base class.

bool
LXNAVV7Device::SetBaudRate(gcc_unused OperationEnvironment &env)
{
  /* Set baud rate of device to current baud rate of port.

     Currently, this seems non-sensical: if the V7 is reading at the
     wrong baud rate, it won't read the instructions.  If it is,
     it doesn't need them.  It may have a role when auto-bauding is enabled.
   */

  unsigned old_baud_rate = port.GetBaudrate();

  /* translate baud rate into index for V7 */
  int brc = 0;
  unsigned int br[] = {4800,9600,19200,38400,57600,115200,230400,256000,460800,512000,1024000,0};
  for (int i=0; br[i] > 0; i++)
      if (br[i] == old_baud_rate)
	{brc = i; break;}

  char tmp[32];
  sprintf(tmp, "PLXV0,BRPDA,W,%1d",brc);
  PortWriteNMEA(port, tmp);
  return true;
}

bool
LXNAVV7Device::EnableNMEA(gcc_unused OperationEnvironment &env)
{
  {
    ScopeLock protect(mutex);
    if (mode == Mode::NMEA)
      return true;

    mode = Mode::NMEA;
    busy = false;
  }

  if (old_baud_rate)
    port.SetBaudrate(old_baud_rate);

  /* in case the V7 is still in pass-through mode: */
  PortWriteNMEA(port, "PLXV0,CONNECTION,W,VSEVEN");

  SetBaudRate(env); /* inform V7 of the current baud rate (just in case) */

  /* set NMEAs rate 
     PLXVF rate in Hz (20/10/5/2/1)
     PLXVS rate
     LXWP0 - 1/sec (often)
     LXWP5 - 0 (what is this?)
   */
  
  PortWriteNMEA(port, "PLXV0,NMEARATE,W,5,10,1,2,10,0");

  port.Flush();

  return true;
}
