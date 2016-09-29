/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/


#include "mitkPlanarSubdivisionPolygon.h"
#include "mitkPlaneGeometry.h"
#include "mitkProperties.h"

// stl related includes
#include <algorithm>

mitk::PlanarSubdivisionPolygon::PlanarSubdivisionPolygon():
  m_TensionParameter(0.0625),
  m_SubdivisionRounds(5)
{
  // Polygon is subdivision (in contrast to parent class PlanarPolygon
  this->SetProperty( "closed", mitk::BoolProperty::New( true ) );
  this->SetProperty( "subdivision", mitk::BoolProperty::New( true ) );

  // Other properties are inherited / already initialized by parent class PlanarPolygon
}


void mitk::PlanarSubdivisionPolygon::GeneratePolyLine()
{
  this->ClearPolyLines();
  ControlPointListType subdivisionPoints;
  ControlPointListType newSubdivisionPoints;
  subdivisionPoints.clear();
  subdivisionPoints = m_ControlPoints;

  if( m_ControlPoints.size() >= GetMinimumNumberOfControlPoints() )
  {

    for( unsigned int i=0; i < GetSubdivisionRounds(); i++ )
    {
      // Indices
      unsigned int index, indexPrev, indexNext, indexNextNext;

      const unsigned int numberOfPoints = subdivisionPoints.size();

      Point2D newPoint;

      // Keep cycling our array indices forward until they wrap around at the end
      for ( index = 0; index < numberOfPoints; ++index )
      {
        // Create new subdivision point according to formula
        // p_new = (0.5 + tension) * (p_here + p_next) - tension * (p_prev + p_nextnext)
        indexPrev = (numberOfPoints + index - 1) % numberOfPoints;
        indexNext = (index + 1) % numberOfPoints;
        indexNextNext = (index + 2) % numberOfPoints;

        newPoint[0] = (0.5 + GetTensionParameter()) * (double)( subdivisionPoints[index][0] + subdivisionPoints[indexNext][0] )
            - GetTensionParameter() * (double)( subdivisionPoints[indexPrev][0] + subdivisionPoints[indexNextNext][0]);
        newPoint[1] = (0.5 + GetTensionParameter()) * (double)( subdivisionPoints[index][1] + subdivisionPoints[indexNext][1] )
            - GetTensionParameter() * (double)( subdivisionPoints[indexPrev][1] + subdivisionPoints[indexNextNext][1]);

        newSubdivisionPoints.push_back( newPoint );
      }

      ControlPointListType mergedSubdivisionPoints;
      ControlPointListType::const_iterator it, itNew;


      for ( it = subdivisionPoints.cbegin() , itNew = newSubdivisionPoints.cbegin();
            it != subdivisionPoints.cend();
            ++it, ++itNew )
      {
        mergedSubdivisionPoints.push_back( *it );
        mergedSubdivisionPoints.push_back( *itNew );
      }

      subdivisionPoints = mergedSubdivisionPoints;

      newSubdivisionPoints.clear();
    }
  }

  bool isInitiallyPlaced = this->IsFinalized();

  unsigned int prevIndex = 0;
  m_PolyLineSegmentInfo.clear();
  unsigned int i;
  ControlPointListType::const_iterator it;
  for ( it = subdivisionPoints.cbegin(), i = 0;
        it != subdivisionPoints.cend();
        ++it, ++i )
  {
    unsigned int nextIndex;
    if ( i == 0 )
    {
      // For the FIRST polyline point, use the index of the LAST control point
      // (it will used to check if the mouse is near the very last polyline element)
      nextIndex = 0;
    }
    else
    {
      // For all other polyline points, use the index of the control point succeeding it
      // (for polyline points lying on control points, the index of the previous control point
      // is used)
      nextIndex = (((i - 1) >> this->GetSubdivisionRounds()) + 1) % m_ControlPoints.size();
      if(!isInitiallyPlaced && nextIndex > m_ControlPoints.size()-2)
      {
        this->AppendPointToPolyLine( 0, m_ControlPoints[m_ControlPoints.size()-1] );
        break;
      }
    }
    if (nextIndex != prevIndex) {
        prevIndex = nextIndex;
        m_PolyLineSegmentInfo.push_back(i);
    }

    this->AppendPointToPolyLine( 0, *it );
  }
  subdivisionPoints.clear();
}

std::tuple<int, int, mitk::Point2D> mitk::PlanarSubdivisionPolygon::FindClosestPolyLinePoint(const mitk::Point2D& point, double maxDistance) const
{
    auto closestPointInfo = Superclass::FindClosestPolyLinePoint(point, maxDistance);

    auto& polyLineSegmentIndex = std::get<cpPolyLineSegmentIndex>(closestPointInfo);
    if (polyLineSegmentIndex >= 0) {
        polyLineSegmentIndex = std::distance(m_PolyLineSegmentInfo.begin(), 
            std::lower_bound(m_PolyLineSegmentInfo.begin(), m_PolyLineSegmentInfo.end(), static_cast<unsigned long>(polyLineSegmentIndex)));
    }

    return closestPointInfo;
}

bool mitk::PlanarSubdivisionPolygon::Equals(const mitk::PlanarFigure& other) const
 {
   const mitk::PlanarSubdivisionPolygon* otherSubDivPoly = dynamic_cast<const mitk::PlanarSubdivisionPolygon*>(&other);
   if ( otherSubDivPoly )
   {
     if ( this->m_SubdivisionRounds != otherSubDivPoly->m_SubdivisionRounds)
       return false;
     if ( std::abs(this->m_TensionParameter - otherSubDivPoly->m_TensionParameter) > mitk::eps)
       return false;
     return Superclass::Equals(other);
   }
   else
   {
     return false;
   }
 }

