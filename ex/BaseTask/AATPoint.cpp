/* Generated by Together */

#include "AATPoint.hpp"
#include "Util.h"
#include "Math/Earth.hpp"
#include "Math/Geometry.hpp"
#include "AATIsolineSegment.hpp"
#include "AATIsolineIntercept.hpp"
#include <math.h>

GEOPOINT AATPoint::get_reference_scored() const
{
  if (getActiveState() == BEFORE_ACTIVE) {
    return getMaxLocation();
  } else {
    return getMinLocation();
  }
}

GEOPOINT AATPoint::get_reference_travelled() const
{
  if (state_entered.Time>=0) {
    return getMaxLocation();
  } else {
    return getMinLocation();
  }
}

GEOPOINT AATPoint::get_reference_remaining() const
{
  if (getActiveState() == BEFORE_ACTIVE) {
    return getMaxLocation();
  } else {
    return TargetLocation;
  }
}

double 
AATPoint::getElevation() const
{
  // TODO: look up elevation of target and use that instead
  return Elevation; // + SAFETYTERRAIN
}

bool 
AATPoint::update_sample(const AIRCRAFT_STATE& state) 
{
  bool retval = OrderedTaskPoint::update_sample(state);
  if (active_state == CURRENT_ACTIVE) {
    retval |= check_target(state);
  }

  return retval;
}

// target handling

bool
AATPoint::check_target(const AIRCRAFT_STATE& state) 
{
  bool moved = false;
  if (isInSector(state)) {
    moved = check_target_inside(state);
  } else {
    moved = check_target_outside(state);
  }
  return moved;
}

bool
AATPoint::check_target_inside(const AIRCRAFT_STATE& state) 
{
  // target must be moved if d(p_last,t)+d(t,p_next) 
  //    < d(p_last,state)+d(state,p_next)

  if (double_leg_distance(TargetLocation) 
      < double_leg_distance(state.Location)) 
  {
    const double d_in_front = 0.01;
    const double d_to_max = ::Distance(state.Location, 
                                       getMaxLocation());

    if (d_to_max<d_in_front) {
      // no improvement available
      return false;
    } else {
      const double p = d_in_front/d_to_max;
      TargetLocation = ::InterpolateLocation(state.Location, 
                                             getMaxLocation(), p); 
      return true;
    }
  } else {
    return false;
  }
}

bool
AATPoint::check_target_outside(const AIRCRAFT_STATE& state) 
{
  // this is optional

  if (!get_previous()->isInSector(state)) {
    double b0s = ::Bearing(get_previous()->get_reference_remaining(),
                           state.Location);
    double bst = ::Bearing(state.Location,
                           TargetLocation);
    double da = ::AngleLimit180(b0s-bst);
    if (fabs(da)>10.0) {
      
      AATIsolineIntercept ai(*this);
      GEOPOINT pi;
      if (ai.intercept(*this, state, 0.0, pi)) {
//        printf("intercept %g %g\n",pi.Longitude, pi.Latitude);
        TargetLocation = pi;
        return true;
      }
    }
  }
  return false;
}


void 
AATPoint::set_range(const double p)
{
  TargetLocation = ::InterpolateLocation(getMinLocation(), 
                                         getMaxLocation(), p);
}


void 
AATPoint::update_projection()
{
  OrderedTaskPoint::update_projection();  

// TODO: update ellipse if we have one saved
}

////////////////////////////////////////////



void AATPoint::print(std::ostream& f, const int item) const
{
  switch(item) {
  case 0:
    OrderedTaskPoint::print(f);
    f << "#   Target\n";
    f << "#     " << TargetLocation.Longitude << "," 
      << TargetLocation.Latitude << "\n";
    break;

  case 1:

    if (getActiveState() != BEFORE_ACTIVE) {

      // note in general this will only change if 
      // prev max or target changes

      AATIsolineSegment seg(*this);
      
      if (seg.valid()) {
        for (double t = 0.0; t<=1.0; t+= 1.0/20) {
          GEOPOINT ga = seg.parametric(t);
          f << ga.Longitude << " " << ga.Latitude << "\n";
        }
      } else {
        GEOPOINT ga = seg.parametric(0.0);
        f << ga.Longitude << " " << ga.Latitude << "\n";
      }
      f << "\n";
    }
    break;
  }
}

