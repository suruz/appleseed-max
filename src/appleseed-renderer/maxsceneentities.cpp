
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2015 Francois Beaune, The appleseedhq Organization
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

// Interface header.
#include "maxsceneentities.h"

// appleseed.foundation headers.
#include "foundation/platform/windows.h"    // include before 3ds Max headers
#include "foundation/utility/memory.h"

// 3ds Max headers.
#include <object.h>


//
// MaxSceneEntities class implementation.
//

void MaxSceneEntities::clear()
{
    foundation::clear_release_memory(m_objects);
    foundation::clear_release_memory(m_lights);
}


//
// MaxSceneEntityCollector class implementation.
//

MaxSceneEntityCollector::MaxSceneEntityCollector(MaxSceneEntities& entities)
  : m_entities(entities)
{
}

void MaxSceneEntityCollector::collect(INode* scene)
{
    for (int i = 0, e = scene->NumberOfChildren(); i < e; ++i)
        visit(scene->GetChildNode(i));
}

void MaxSceneEntityCollector::visit(INode* node)
{
    // Recurse into children.
    for (int i = 0, e = node->NumberOfChildren(); i < e; ++i)
        visit(node->GetChildNode(i));

    // Skip non-renderable nodes.
    if (!node->Renderable())
        return;

    // Retrieve the ObjectState structure of this node.
    ObjectState object_state = node->EvalWorldState(0);
    if (object_state.obj == 0)
        return;

    switch (object_state.obj->SuperClassID())
    {
      case LIGHT_CLASS_ID:
        {
            // Hidden lights still emit light.

            LightObject* light_object = static_cast<LightObject*>(object_state.obj);

            // Skip disabled lights.
            if (!light_object->GetUseLight())
                return;

            // Collect this light.
            m_entities.m_lights.push_back(node);
        }
        break;

      case SHAPE_CLASS_ID:
      case GEOMOBJECT_CLASS_ID:
        {
            // Skip hidden objects.
            if (node->IsNodeHidden(TRUE))
                return;

            // Skip non-renderable objects.
            if (!object_state.obj->IsRenderable())
                return;

            // Collect this instance.
            m_entities.m_objects.push_back(node);
        }
        break;
    }
}
