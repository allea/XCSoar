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

#include "WindArrowRenderer.hpp"
#include "Screen/Canvas.hpp"
#include "Screen/Graphics.hpp"
#include "Screen/Fonts.hpp"
#include "Screen/Layout.hpp"
#include "Math/Angle.hpp"
#include "Math/Screen.hpp"
#include "NMEA/Derived.hpp"
#include "Units/Units.hpp"
#include "SettingsMap.hpp"

#include <tchar.h>
#include <cstdio>

void
WindArrowRenderer::Draw(Canvas &canvas, const Angle screen_angle,
                        const SpeedVector wind, const RasterPoint pos,
                        const PixelRect rc, bool with_tail)
{
  canvas.select(Fonts::MapBold);
  unsigned tsize = canvas.text_size(_T("99")).cx / 2;

  canvas.select(Graphics::hpWind);
  canvas.select(Graphics::hbWind);

  int wmag = iround(4 * wind.norm);

  int kx = tsize / Layout::FastScale(1) / 2;

  RasterPoint Arrow[7] = {
      { 0, -20 },
      { -6, -26 },
      { 0, -20 },
      { 6, -26 },
      { 0, -20 },
      { 8 + kx, -24 },
      { -8 - kx, -24 }
  };

  for (int i = 1; i < 4; i++)
    Arrow[i].y -= wmag;

  PolygonRotateShift(Arrow, 7, pos.x, pos.y, wind.bearing - screen_angle);

  canvas.polygon(Arrow, 5);

  if (with_tail) {
    RasterPoint Tail[2] = {
      { 0, Layout::FastScale(-20) },
      { 0, Layout::FastScale(-26 - min(20, wmag) * 3) },
    };

    PolygonRotateShift(Tail, 2, pos.x, pos.y, wind.bearing - screen_angle);

    // optionally draw dashed line
    Pen dash_pen(Pen::DASH, 1, COLOR_BLACK);
    canvas.select(dash_pen);
    canvas.line(Tail[0], Tail[1]);
  }

  TCHAR sTmp[12];
  _stprintf(sTmp, _T("%i"), iround(Units::ToUserWindSpeed(wind.norm)));

  canvas.set_text_color(COLOR_BLACK);

  TextInBoxMode style;
  style.Align = Center;
  style.Mode = Outlined;

  if (Arrow[5].y >= Arrow[6].y)
    TextInBox(canvas, sTmp, Arrow[5].x - kx, Arrow[5].y, style, rc);
  else
    TextInBox(canvas, sTmp, Arrow[6].x - kx, Arrow[6].y, style, rc);
}

void
WindArrowRenderer::Draw(Canvas &canvas, const Angle screen_angle,
                        const RasterPoint pos, const PixelRect rc,
                        const DerivedInfo &calculated,
                        const SETTINGS_MAP &settings)
{
  if (!calculated.wind_available)
    return;

  // don't bother drawing it if not significant
  if (calculated.wind.norm < fixed_one)
    return;

  WindArrowRenderer::Draw(canvas, screen_angle, calculated.wind, pos, rc,
                          settings.WindArrowStyle == 1);
}