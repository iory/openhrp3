/*
 * Copyright (c) 2008, AIST, the University of Tokyo and General Robotix Inc.
 * All rights reserved. This program is made available under the terms of the
 * Eclipse Public License v1.0 which accompanies this distribution, and is
 * available at http://www.eclipse.org/legal/epl-v10.html
 * Contributors:
 * National Institute of Advanced Industrial Science and Technology (AIST)
 */

/*!
  @file BodyInfo_impl.h
  @author Shin'ichiro Nakaoka
  @author Y.TSUNODA
*/

#ifndef OPENHRP_MODEL_LOADER_BODYINFO_IMPL_H_INCLUDED
#define OPENHRP_MODEL_LOADER_BODYINFO_IMPL_H_INCLUDED

#include <string>
#include <vector>

#include <OpenHRP/Corba/ORBwrap.h>
#include <OpenHRP/Corba/ModelLoader.h>

#include <OpenHRP/Parser/ModelNodeSet.h>
#include <OpenHRP/Parser/TriangleMeshShaper.h>
#include <OpenHRP/Parser/VrmlNodes.h>

#include <OpenHRP/Util/Tvmet3d.h>
#include <OpenHRP/Util/Tvmet4d.h>


namespace OpenHRP
{
    class BodyInfo_impl : public POA_OpenHRP::BodyInfo
    {
    public:
		
        BodyInfo_impl(PortableServer::POA_ptr poa);
        virtual ~BodyInfo_impl();

        virtual PortableServer::POA_ptr _default_POA();
		
        virtual char* name();
        virtual char* url();
        virtual StringSequence* info();
        virtual LinkInfoSequence* links();
        virtual AllLinkShapeIndices* linkShapeIndices();
        virtual ShapeInfoSequence* shapes();
        virtual AppearanceInfoSequence* appearances();
        virtual MaterialInfoSequence* materials();
        virtual TextureInfoSequence* textures();

        void loadModelFile(const std::string& filename);

        void setLastUpdateTime(time_t time) { lastUpdate_ = time;};
        time_t getLastUpdateTime() { return lastUpdate_; }

    private:
        
        PortableServer::POA_var poa;
		
        time_t lastUpdate_;

        std::string name_;
        std::string url_;
        StringSequence info_;
        LinkInfoSequence links_;
        ShapeInfoSequence  shapes_;
        AppearanceInfoSequence appearances_;
        MaterialInfoSequence materials_;
        TextureInfoSequence textures_;
        AllLinkShapeIndices linkShapeIndices_;

        TriangleMeshShaper triangleMeshShaper;
        
        /// ShapeInfoのindexと，そのshapeを算出したtransformのペア
        struct ShapeObject
        {
            Matrix44 transform;
            short     index;
        };

        /**
          Map for sharing shapeInfo
          if it is node that has already stored in shape_, it has the corresponding index.
        */
        typedef std::map<OpenHRP::VrmlShapePtr, ShapeObject> SharedShapeInfoMap;

        SharedShapeInfoMap sharedShapeInfoMap;

        int readJointNodeSet(JointNodeSetPtr jointNodeSet, int& currentIndex, int motherIndex);
        void setJointParameters(int linkInfoIndex, VrmlProtoInstancePtr jointNode );
        void setSegmentParameters(int linkInfoIndex, VrmlProtoInstancePtr segmentNode );
        void setSensors(int linkInfoIndex, JointNodeSetPtr jointNodeSet );
        void readSensorNode(int linkInfoIndex, SensorInfo& sensorInfo, VrmlProtoInstancePtr sensorNode);

        void traverseShapeNodes(int linkInfoIndex, MFNode& childNodes, const Matrix44& transform);
        void calcTransform(VrmlTransformPtr transform, Matrix44& out_T);
        int createShapeInfo(VrmlShapePtr shapeNode, const Matrix44& transform);
        void setTriangleMesh(ShapeInfo_var& shapeInfo, VrmlIndexedFaceSet* triangleMesh, const Matrix44& transform);
        void setPrimitiveProperties(ShapeInfo_var& shapeInfo, VrmlShapePtr shapeNode);
        int createAppearanceInfo(ShapeInfo_var& shapeInfo, VrmlShapePtr& shapeNode,
                                 VrmlIndexedFaceSet* faceSet, const Matrix44& transform);
        void setColors(AppearanceInfo_var& appInfo, VrmlIndexedFaceSet* triangleMesh);
        void setNormals(AppearanceInfo_var& appInfo, VrmlIndexedFaceSet* triangleMesh, const Matrix44& transform);
        int createMaterialInfo(VrmlMaterialPtr materialNode);
        int createTextureInfo(VrmlTexturePtr textureNode);
        std::string getModelFileDirPath();
    };
};


#endif
