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

#include "mitkSurfaceCutterCGAL.h"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>
#include <CGAL/bounding_box.h>

#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCell.h>
#include <vtkCleanPolyData.h>
#include <vtkTriangleFilter.h>

// Definitions of CGAL entities
typedef CGAL::Simple_cartesian<double> CGAL_Kernel;
typedef CGAL_Kernel::Point_3 CGAL_Point;
typedef CGAL_Kernel::Vector_3 CGAL_Vector;
typedef CGAL_Kernel::Plane_3 CGAL_Plane;
typedef CGAL_Kernel::Segment_3 CGAL_Segment;
typedef CGAL_Kernel::Triangle_3 CGAL_Triangle;

// Custom iterator over triangles for vtk polydata objects
class vtkPolyDataTrianglesIterator 
    : public boost::iterator_facade<
          vtkPolyDataTrianglesIterator  // Self for CRTP       
        , CGAL_Triangle const           // Value
        , boost::forward_traversal_tag  // CategoryOrTraversal
        , CGAL_Triangle                 // Reference
        >
{
public:
    vtkPolyDataTrianglesIterator()
        : data(nullptr), cellId(-1) {}   

    static vtkPolyDataTrianglesIterator begin(vtkPolyData* data)
    {
        return vtkPolyDataTrianglesIterator(data, 0);
    }

    static vtkPolyDataTrianglesIterator end(vtkPolyData* data)
    {
        return vtkPolyDataTrianglesIterator(data, data->GetNumberOfCells());
    }

private:
    vtkPolyDataTrianglesIterator(vtkPolyData* data, int index)
        : data(data), cellId(index)
    {
    }


    friend class boost::iterator_core_access;
    void increment() {
        ++cellId;
    }

    // Convert triangle to CGAL from VTK
    CGAL_Triangle dereference() const 
    { 
        if (cellId < data->GetNumberOfCells()) {
            vtkCell* cell = data->GetCell(cellId);

            assert(cell->GetNumberOfPoints() == 3);

            CGAL_Point p[3];
            for (int i = 0; i < 3; ++i) {
                double* pt = data->GetPoints()->GetPoint(cell->GetPointId(i));
                p[i] = CGAL_Point(pt[0], pt[1], pt[2]);
            }
                
            return CGAL_Triangle(p[0], p[1], p[2]);
        }
        return CGAL_Triangle();
    }

    bool equal(vtkPolyDataTrianglesIterator const& other) const
    {
        return data == other.data && cellId == other.cellId;
    }

    int cellId;
    vtkPolyData* data;
};

// Definition of AABB tree types
typedef CGAL::AABB_triangle_primitive<CGAL_Kernel, vtkPolyDataTrianglesIterator> CGAL_AABBPrimitive;
typedef CGAL::AABB_traits<CGAL_Kernel, CGAL_AABBPrimitive> CGAL_AABBTraits;
typedef CGAL::AABB_tree<CGAL_AABBTraits> CGAL_AABBTree;

namespace mitk {

class SurfaceCutterCGALPrivate {
public:
    SurfaceCutterCGALPrivate(vtkPolyData* surface)
    {
        setPolyData(surface);
    }

    void setPolyData(vtkPolyData* surface)
    {
        if (_tree) {
            _tree.release();
        }

        // Triangulated data
        vtkSmartPointer<vtkTriangleFilter> triangleFilter =
            vtkSmartPointer<vtkTriangleFilter>::New();
        triangleFilter->SetInputData(surface);
        triangleFilter->Update();
        _triangulatedData = triangleFilter->GetOutput();

        if (surface) {
            _tree.reset(new CGAL_AABBTree(vtkPolyDataTrianglesIterator::begin(_triangulatedData), vtkPolyDataTrianglesIterator::end(_triangulatedData)));
            _tree->accelerate_distance_queries();
        }
    }

    vtkSmartPointer<vtkPolyData> cutWithPlane(const mitk::Point3D planePoints[4]) const
    {
        if (!_triangulatedData || !_tree) {
            return vtkSmartPointer<vtkPolyData>::New();
        }

        CGAL_Point points[] = {
            CGAL_Point(planePoints[0][0], planePoints[0][1], planePoints[0][2]),
            CGAL_Point(planePoints[1][0], planePoints[1][1], planePoints[1][2]),
            CGAL_Point(planePoints[2][0], planePoints[2][1], planePoints[2][2]),
            CGAL_Point(planePoints[3][0], planePoints[3][1], planePoints[3][2])
        };

        CGAL::Bbox_3 bbox = points[0].bbox() + points[1].bbox() + points[2].bbox() + points[3].bbox();
        
        std::vector<CGAL_AABBTree::Intersection_and_primitive_id<CGAL_Plane>::Type> segments;

        if (_tree->do_intersect(bbox)) {
            CGAL_Plane query_plane(points[0], points[1], points[2]);
            _tree->all_intersections(query_plane, std::back_inserter(segments));
        }

        vtkSmartPointer<vtkPolyData> pdOut = vtkPolyData::New();
        vtkSmartPointer<vtkPoints> pointsOut = vtkPoints::New();
        pdOut->SetPoints(pointsOut);
        pointsOut->Allocate(2 * segments.size());
        pdOut->Allocate(segments.size());

        for (int i = 0; i < segments.size(); ++i) {
            CGAL_Segment* s = boost::get<CGAL_Segment>(&segments[i].first);
            pointsOut->InsertNextPoint(s->start().x(), s->start().y(), s->start().z());
            pointsOut->InsertNextPoint(s->end().x(), s->end().y(), s->end().z());

            auto ids = vtkIdList::New();
            ids->InsertNextId(2 * i);
            ids->InsertNextId(2 * i + 1);
            pdOut->InsertNextCell(VTK_LINE, ids);
        }

        vtkSmartPointer<vtkCleanPolyData> cleaner = vtkCleanPolyData::New();
        cleaner->SetInputData(pdOut);
        cleaner->Update();

        return cleaner->GetOutput();
    }

private:
    vtkSmartPointer<vtkPolyData> _triangulatedData;
    std::unique_ptr<CGAL_AABBTree> _tree;
};


SurfaceCutterCGAL::SurfaceCutterCGAL()
: p(new SurfaceCutterCGALPrivate())
{
}

SurfaceCutterCGAL::~SurfaceCutterCGAL()
{
}

void SurfaceCutterCGAL::setPolyData(vtkPolyData* surface)
{
    p->setPolyData(surface);
}

vtkSmartPointer<vtkPolyData> SurfaceCutterCGAL::cutWithPlane(const mitk::Point3D planePoints[4]) const
{
    return p->cutWithPlane(planePoints);
}

} // namespace mitk