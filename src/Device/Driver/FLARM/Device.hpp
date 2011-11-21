/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2011 The XCSoar Project
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

#ifndef XCSOAR_FLARM_DEVICE_HPP
#define XCSOAR_FLARM_DEVICE_HPP

#include "tchar.h"

class Port;
struct Declaration;
class OperationEnvironment;

class FlarmDevice
{
  Port &port;

public:
  FlarmDevice(Port &_port):port(_port) {}

  bool Declare(const Declaration &declaration, OperationEnvironment &env);

  bool SetPilot(const TCHAR *pilot_name);
  bool SetCoPilot(const TCHAR *copilot_name);
  bool SetPlaneType(const TCHAR *plane_type);
  bool SetPlaneRegistration(const TCHAR *registration);
  bool SetCompetitionId(const TCHAR *competition_id);
  bool SetCompetitionClass(const TCHAR *competition_class);

  bool SetStealthMode(bool enabled);
  bool SetRange(unsigned range);

  void Restart();

private:
  /**
   * Sends the supplied sentence with a $ prepended and a line break appended
   */
  void Send(const char *sentence);

  bool SetConfig(const TCHAR *setting, const TCHAR *value);

  bool SetGet(char *buffer);
#ifdef _UNICODE
  bool SetGet(TCHAR *s);
#endif

  bool DeclareInternal(const Declaration &declaration,
                       OperationEnvironment &env);
};

#endif