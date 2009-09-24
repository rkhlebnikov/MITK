/*=========================================================================

Program:   openCherry Platform
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef CHERRYVIEWDESCRIPTOR_H_
#define CHERRYVIEWDESCRIPTOR_H_

#include "../cherryIViewPart.h"
#include "../cherryIViewDescriptor.h"

#include "service/cherryIConfigurationElement.h"

#include <string>
#include <vector>

namespace cherry
{

struct IHandlerActivation;

/**
 * \ingroup org_opencherry_ui_internal
 *
 */
class ViewDescriptor : public IViewDescriptor
{
private:

  std::string id;
  mutable SmartPointer<ImageDescriptor> imageDescriptor;
  IConfigurationElement::Pointer configElement;
  std::vector<std::string> categoryPath;

  /**
   * The activation token returned when activating the show view handler with
   * the workbench.
   */
  SmartPointer<IHandlerActivation> handlerActivation;

public:

  cherryObjectMacro(ViewDescriptor)

  /**
   * Create a new <code>ViewDescriptor</code> for an extension.
   *
   * @param e the configuration element
   * @throws CoreException thrown if there are errors in the configuration
   */
  ViewDescriptor(IConfigurationElement::Pointer e);

  /* (non-Javadoc)
   * @see org.opencherry.ui.internal.registry.IViewDescriptor#createView()
   */
  IViewPart::Pointer CreateView();

  /* (non-Javadoc)
   * @see org.opencherry.ui.internal.registry.IViewDescriptor#getCategoryPath()
   */
  const std::vector<std::string>& GetCategoryPath() const;

  /**
   * Return the configuration element for this descriptor.
   *
   * @return the configuration element
   */
  IConfigurationElement::Pointer GetConfigurationElement() const;

  /* (non-Javadoc)
   * @see org.opencherry.ui.internal.registry.IViewDescriptor#getDescription()
   */
  std::string GetDescription() const;

  /* (non-Javadoc)
   * @see org.opencherry.ui.IWorkbenchPartDescriptor#getId()
   */
  std::string GetId() const;

  /* (non-Javadoc)
   * @see org.opencherry.ui.IWorkbenchPartDescriptor#getImageDescriptor()
   */
  SmartPointer<ImageDescriptor> GetImageDescriptor() const;

  /* (non-Javadoc)
   * @see org.opencherry.ui.IWorkbenchPartDescriptor#getLabel()
   */
  std::string GetLabel() const;

  /**
   * Return the accelerator attribute.
   *
   * @return the accelerator attribute
   */
  std::string GetAccelerator() const;

  /* (non-Javadoc)
   * @see org.opencherry.ui.internal.registry.IViewDescriptor#getAllowMultiple()
   */
  bool GetAllowMultiple() const;

  /* (non-Javadoc)
   * @see org.eclipse.ui.views.IViewDescriptor#getRestorable()
   */
  bool IsRestorable() const;

  bool operator==(const Object*) const;

  /**
   * Activates a show view handler for this descriptor. This handler can later
   * be deactivated by calling {@link ViewDescriptor#deactivateHandler()}.
   * This method will only activate the handler if it is not currently active.
   *
   */
  void ActivateHandler();

  /**
   * Deactivates the show view handler for this descriptor. This handler was
   * previously activated by calling {@link ViewDescriptor#activateHandler()}.
   * This method will only deactivative the handler if it is currently active.
   *
   */
  void DeactivateHandler();

protected:

  /* (non-Javadoc)
   * @see IAdaptable#GetAdapterImpl(const std::type_info&)
   */
  Poco::Any GetAdapter(const std::string& adapter);

private:
  /**
   * load a view descriptor from the registry.
   */
  void LoadFromExtension();
};

}

#endif /*CHERRYVIEWDESCRIPTOR_H_*/
