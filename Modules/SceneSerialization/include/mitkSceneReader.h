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

#pragma once 

#include <MitkSceneSerializationExports.h>

#include <tinyxml.h>

#include <itkObjectFactory.h>

#include "mitkDataStorage.h"

namespace mitk
{

class MITKSCENESERIALIZATION_EXPORT SceneReader : public itk::Object
{
  public:

    mitkClassMacroItkParent( SceneReader, itk::Object );
    itkFactorylessNewMacro(Self)
    itkCloneMacro(Self)

    struct LoadedNodeFileNames {
        std::vector<std::string> baseDataFiles; // First is the file to load, the rest are additional files generated by serializer
        std::string baseDataPropertiesFile;
        std::map<std::string, std::string> nodePropertiesFiles; // Map from renderer name to renderer-specific properties file. Default properties are associated with empty string
    };

    typedef std::map<const mitk::DataNode*, LoadedNodeFileNames> LoadedNodeFileNamesMap;

    virtual bool LoadScene(TiXmlDocument& document, const std::string& workingDirectory, DataStorage* storage, LoadedNodeFileNamesMap* nodeDataFileNames = nullptr);
};

}

