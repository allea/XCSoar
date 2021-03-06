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

#include "TrackingConfigPanel.hpp"
#include "Profile/ProfileKeys.hpp"
#include "Form/Edit.hpp"
#include "DataField/Boolean.hpp"
#include "DataField/Listener.hpp"
#include "Language/Language.hpp"
#include "Tracking/TrackingSettings.hpp"
#include "DataField/Base.hpp"
#include "Form/RowFormWidget.hpp"
#include "Screen/Layout.hpp"
#include "Interface.hpp"
#include "UIGlobals.hpp"

enum ControlIndex {
  TrackingInterval,
  Spacer,
  LT24Enabled,
  LT24Username,
  LT24Password
};

class TrackingConfigPanel
  : public RowFormWidget, DataFieldListener {
public:
  TrackingConfigPanel()
    :RowFormWidget(UIGlobals::GetDialogLook()) {}

public:
  void SetEnabled(bool enabled);

  /* methods from Widget */
  virtual void Prepare(ContainerWindow &parent, const PixelRect &rc);
  virtual bool Save(bool &changed, bool &require_restart);

private:
  /* methods from DataFieldListener */
  virtual void OnModified(DataField &df);
};

void
TrackingConfigPanel::SetEnabled(bool enabled)
{
  GetControl(LT24Username).set_enabled(enabled);
  GetControl(LT24Password).set_enabled(enabled);
}

void
TrackingConfigPanel::OnModified(DataField &df)
{
  if (IsDataField(LT24Enabled, df)) {
    const DataFieldBoolean &dfb = (const DataFieldBoolean &)df;
    SetEnabled(dfb.GetAsBoolean());
  }
}

void
TrackingConfigPanel::Prepare(ContainerWindow &parent, const PixelRect &rc)
{
  const TrackingSettings &settings =
    CommonInterface::GetComputerSettings().tracking;

  RowFormWidget::Prepare(parent, rc);

  AddTime(_("Tracking Interval"), _T(""), 5, 3600, 5, settings.interval);

  AddSpacer();

  AddBoolean(_T("LiveTrack24"),  _T(""), settings.livetrack24.enabled, this);

  AddText(_("Username"), _T(""), settings.livetrack24.username);
  AddPassword(_("Password"), _T(""), settings.livetrack24.password);

  SetEnabled(settings.livetrack24.enabled);
}

bool
TrackingConfigPanel::Save(bool &_changed, bool &_require_restart)
{
  bool changed = false, require_restart = false;

  TrackingSettings &settings =
    CommonInterface::SetComputerSettings().tracking;

  changed |= SaveValue(TrackingInterval, ProfileTrackingInterval, settings.interval);

  changed |= SaveValue(LT24Enabled, ProfileLiveTrack24Enabled, settings.livetrack24.enabled);

  changed |= SaveValue(LT24Username, ProfileLiveTrack24Username,
                       settings.livetrack24.username.buffer(), settings.livetrack24.username.MAX_SIZE);

  changed |= SaveValue(LT24Password, ProfileLiveTrack24Password,
                       settings.livetrack24.password.buffer(), settings.livetrack24.password.MAX_SIZE);

  _changed |= changed;
  _require_restart |= require_restart;

  return true;
}

Widget *
CreateTrackingConfigPanel()
{
  return new TrackingConfigPanel();
}
