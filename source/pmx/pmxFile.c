#include "pmxFile.h"

/* for more detail see
https://github.com/benikabocha/saba/blob/master/src/Saba/Model/MMD/PMXFile.cpp */

static void pmxRead(void* buffer, uint32_t size, FILE* fd) {
    read_file_bytes(buffer, size, fd);
}

static void pmxReadString(pmx_text_t* string, FILE* fd) {
    pmxRead(&(string->len), sizeof(string->len), fd);
    string->data_wide = (wchar_t *)malloc(sizeof(wchar_t) * (string->len / 2 + 1));
    pmxRead(string->data_wide, string->len, fd);
    string->data_wide[string->len / 2] = L'\0'; // Null-terminate the string
}

static void pmxReadHeader(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->header.sign[0]), sizeof(pmx->header.sign), fd);

    pmxRead(&(pmx->header.version), sizeof(pmx->header.version), fd);

    pmxRead(&(pmx->header.goalCount), sizeof(pmx->header.goalCount), fd);

    pmxRead(&(pmx->header.encode), sizeof(pmx->header.encode), fd);
    pmxRead(&(pmx->header.addUV4Num), sizeof(pmx->header.addUV4Num), fd);
    pmxRead(&(pmx->header.vertexIndexSize), sizeof(pmx->header.vertexIndexSize), fd);
    pmxRead(&(pmx->header.textureIndexSize), sizeof(pmx->header.textureIndexSize), fd);
    pmxRead(&(pmx->header.materialIndexSize), sizeof(pmx->header.materialIndexSize), fd);
    pmxRead(&(pmx->header.boneIndexSize), sizeof(pmx->header.boneIndexSize), fd);
    pmxRead(&(pmx->header.morphIndexSize), sizeof(pmx->header.morphIndexSize), fd);
    pmxRead(&(pmx->header.rigidbodyIndexSize), sizeof(pmx->header.rigidbodyIndexSize), fd);

    pmxReadString(&pmx->header.localModelName, fd);
    pmxReadString(&pmx->header.generalModelName, fd);
    pmxReadString(&pmx->header.localModelComment, fd);
    pmxReadString(&pmx->header.generalModelComment, fd);

}

static void pmxReadIndex(int32_t* index, uint8_t Type, FILE* fd) {
    switch (Type) {
        case 1: {
            uint8_t idx;
            pmxRead(&idx, sizeof(idx), fd);
            if (idx != 0xFF) {
                *index = (int32_t)idx;
            } else {
                *index = -1;
            }
        } 
        break;
        case 2: {
            uint16_t idx;
            pmxRead(&idx, sizeof(idx), fd);
            if (idx != 0xFFFF) {
                *index = (int32_t)idx;
            } else {
                *index = -1;
            }
        } 
        break;
        case 4: {
            uint32_t idx;
            pmxRead(&idx, sizeof(idx), fd);
            if (idx != 0xFFFFFFFF) {
                *index = (int32_t)idx;
            } else {
                *index = -1;
            }
        } 
        break;
        default:
        break;
    }
}

static void pmxReadVertex(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->vertex.count), sizeof(pmx->vertex.count), fd);
    pmx->vertex.data = (pmx_vertex_data_t *)malloc(sizeof(pmx_vertex_data_t) * (pmx->vertex.count));
    memset(pmx->vertex.data, 0, sizeof(pmx_vertex_data_t) * (pmx->vertex.count));

    uint32_t c = 0;
    for (uint32_t index = 0; index < pmx->vertex.count; index++) {
        pmx_vertex_data_t* vertex = &(pmx->vertex.data[index]);
        
        pmxRead(&(vertex->position), sizeof(vertex->position), fd);

        pmxRead(&(vertex->normal), sizeof(vertex->normal), fd);

        pmxRead(&(vertex->uv), sizeof(vertex->uv), fd);

        if (0 != pmx->header.addUV4Num) {
            vertex->addUV = (vector4d_t*)malloc(sizeof(vector4d_t) * (pmx->header.addUV4Num));
            for (uint32_t i = 0; i < pmx->header.addUV4Num; i++) {
              pmxRead(&(vertex->addUV[i]), sizeof(vertex->addUV[i]), fd);
            }
        }

        pmxRead(&(vertex->weightType), sizeof(vertex->weightType), fd);

        switch (vertex->weightType) {
          case BDEF1:
            pmxReadIndex(&vertex->boneIndices[0], pmx->header.boneIndexSize, fd);
            break;
          case BDEF2:            
            pmxReadIndex(&vertex->boneIndices[0], pmx->header.boneIndexSize, fd);
            pmxReadIndex(&vertex->boneIndices[1], pmx->header.boneIndexSize, fd);
            pmxRead(&vertex->boneWeights[0], sizeof(vertex->boneWeights[0]), fd);
            break;
          case BDEF4:
            pmxReadIndex(&vertex->boneIndices[0], pmx->header.boneIndexSize, fd);
            pmxReadIndex(&vertex->boneIndices[1], pmx->header.boneIndexSize, fd);            
            pmxReadIndex(&vertex->boneIndices[2], pmx->header.boneIndexSize, fd);
            pmxReadIndex(&vertex->boneIndices[3], pmx->header.boneIndexSize, fd);
            pmxRead(&vertex->boneWeights[0], sizeof(vertex->boneWeights[0]), fd);
            pmxRead(&vertex->boneWeights[1], sizeof(vertex->boneWeights[1]), fd);
            pmxRead(&vertex->boneWeights[2], sizeof(vertex->boneWeights[2]), fd);
            pmxRead(&vertex->boneWeights[3], sizeof(vertex->boneWeights[3]), fd);
            break;
          case SDEF:
            pmxReadIndex(&vertex->boneIndices[0], pmx->header.boneIndexSize, fd);
            pmxReadIndex(&vertex->boneIndices[1], pmx->header.boneIndexSize, fd);            
            pmxRead(&vertex->boneWeights[0], sizeof(vertex->boneWeights[0]), fd);
            pmxRead(&vertex->sdefC, sizeof(vertex->sdefC), fd);
            pmxRead(&vertex->sdefR0, sizeof(vertex->sdefR0), fd);
            pmxRead(&vertex->sdefR1, sizeof(vertex->sdefR1), fd);
            break;
          case QDEF:            
            pmxReadIndex(&vertex->boneIndices[0], pmx->header.boneIndexSize, fd);
            pmxReadIndex(&vertex->boneIndices[1], pmx->header.boneIndexSize, fd);            
            pmxReadIndex(&vertex->boneIndices[2], pmx->header.boneIndexSize, fd);
            pmxReadIndex(&vertex->boneIndices[3], pmx->header.boneIndexSize, fd);
            pmxRead(&vertex->boneWeights[0], sizeof(vertex->boneWeights[0]), fd);
            pmxRead(&vertex->boneWeights[1], sizeof(vertex->boneWeights[1]), fd);
            pmxRead(&vertex->boneWeights[2], sizeof(vertex->boneWeights[2]), fd);
            pmxRead(&vertex->boneWeights[3], sizeof(vertex->boneWeights[3]), fd);
            break;
        default:
            break;
        }

        pmxRead(&vertex->edgeMag, sizeof(vertex->edgeMag), fd);
    }
}

static void pmxReadFace(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->face.count), sizeof(pmx->face.count), fd);
    pmx->face.count /= 3; // Each face has 3 vertices
    
    pmx->face.data = (pmx_face_data_t *)malloc(sizeof(pmx_face_data_t) * pmx->face.count);
    switch (pmx->header.vertexIndexSize) {
        case 1: {
            //uint8_t indices[3 * pmx->face.count];
            //pmxRead(&indices[0], sizeof(indices), fd);
            uint8_t indices[100000] = { 0 };
            pmxRead(&indices[0], sizeof(uint8_t)* pmx->face.count * 3, fd);
            for (uint32_t i = 0; i < pmx->face.count; i++) {
                pmx_face_data_t* face = &(pmx->face.data[i]);
                face->indices[0] = (uint32_t)indices[i * 3];
                face->indices[1] = (uint32_t)indices[i * 3 + 1];
                face->indices[2] = (uint32_t)indices[i * 3 + 2];
            }
        }
        break;
        case 2: {
            //uint16_t indices[3 * pmx->face.count];
            //pmxRead(&indices[0], sizeof(indices), fd);            
            uint16_t indices[100000] = { 0 };
            pmxRead(&indices[0], sizeof(uint16_t) * pmx->face.count * 3, fd);
            for (uint32_t i = 0; i < pmx->face.count; i++) {
                pmx_face_data_t* face = &(pmx->face.data[i]);
                face->indices[0] = (uint32_t)indices[i * 3];
                face->indices[1] = (uint32_t)indices[i * 3 + 1];
                face->indices[2] = (uint32_t)indices[i * 3 + 2];
            }
        }
        break;
        case 4: {
            //uint32_t indices[3 * pmx->face.count];
            //pmxRead(&indices[0], sizeof(indices), fd);
            uint32_t indices[100000] = { 0 };
            pmxRead(&indices[0], sizeof(uint32_t) * pmx->face.count * 3, fd);
            for (uint32_t i = 0; i < pmx->face.count; i++) {
                pmx_face_data_t* face = &(pmx->face.data[i]);
                face->indices[0] = indices[i * 3];
                face->indices[1] = indices[i * 3 + 1];
                face->indices[2] = indices[i * 3 + 2];
            }
        }
        break;
        default:
            break;
    }

}

static void pmxReadTexture(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->texture.Number), sizeof(pmx->texture.Number), fd);

    pmx->texture.path = (pmx_text_t *)malloc(sizeof(pmx_text_t) * pmx->texture.Number);
    for (uint32_t i = 0; i < pmx->texture.Number; i++) {
        pmxReadString(&pmx->texture.path[i], fd);    
        //wprintf(L"texture%d:%ls\n", i, pmx->texture.path[i].data_wide);

    }

}

static void pmxReadMaterial(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->material.count), sizeof(pmx->material.count), fd);

    pmx->material.data = (pmx_material_data_t *)malloc(sizeof(pmx_material_data_t) * pmx->material.count);
    for (uint32_t index = 0; index < pmx->material.count; index++) {
      pmx_material_data_t* material = &pmx->material.data[index];
      pmxReadString(&material->localMaterialName, fd);
      pmxReadString(&material->generalMaterialName, fd);

      pmxRead(&material->diffuse, sizeof(material->diffuse), fd);
      pmxRead(&material->specular, sizeof(material->specular), fd);
      pmxRead(&material->specularPower, sizeof(material->specularPower), fd);

      pmxRead(&material->ambient, sizeof(material->ambient), fd);

      pmxRead(&material->drawMode, sizeof(material->drawMode), fd);

      pmxRead(&material->edgeColor, sizeof(material->edgeColor), fd);
      pmxRead(&material->edgeRatio, sizeof(material->edgeRatio), fd);

      pmxReadIndex(&material->textureIndex, pmx->header.textureIndexSize, fd);
      pmxReadIndex(&material->specularTextureIndex, pmx->header.textureIndexSize, fd);
      pmxRead(&material->blendMode, sizeof(material->blendMode), fd);

      pmxRead(&material->toonMode, sizeof(material->toonMode), fd);
      if (external == material->toonMode) {
        pmxReadIndex(&material->toonTextureIndex, pmx->header.textureIndexSize, fd);
      } else if (internal == material->toonMode) {
        uint8_t toonTextureIndex;
        pmxRead(&toonTextureIndex, sizeof(toonTextureIndex), fd);
        material->textureIndex = (int32_t)toonTextureIndex;
      }

      pmxReadString(&material->memo, fd);

      pmxRead(&material->numFace, sizeof(material->numFace), fd);
    }
}

static void pmxReadBone(pmx_t* pmx, FILE* fd) {
  pmxRead(&(pmx->bone.count), sizeof(pmx->bone.count), fd);

  pmx->bone.data = (pmx_bone_data_t*)malloc(sizeof(pmx_bone_data_t) * pmx->bone.count);
  for (uint32_t index = 0; index < pmx->bone.count; index++) {
      pmx_bone_data_t* bone = &pmx->bone.data[index];
      
      pmxReadString(&bone->localBoonName, fd);
      pmxReadString(&bone->generalBoonName, fd);

      pmxRead(&bone->position, sizeof(bone->position), fd);

      pmxReadIndex(&bone->parentBoneIndex, pmx->header.boneIndexSize, fd);

      pmxRead(&bone->deformDepth, sizeof(bone->deformDepth), fd);

      pmxRead(&bone->boneflag, sizeof(bone->boneflag), fd);
        
      if (false == (bone->boneflag & TargetShowMode)){
        pmxRead(&bone->tailPosition, sizeof(bone->tailPosition), fd);
      } else {
        pmxReadIndex(&bone->tailBoneIndex, pmx->header.boneIndexSize, fd);
      }

      if ((bone->boneflag & AppendRotate) || 
          (bone->boneflag & AppendTranslate)) {
        pmxReadIndex(&bone->inheritBoneIndex, pmx->header.boneIndexSize, fd);
        pmxRead(&bone->inheritWeight, sizeof(bone->inheritWeight), fd);
      }

      if (bone->boneflag & FixedAxis) {
        pmxRead(&bone->fixedAxis, sizeof(bone->fixedAxis), fd);
      }

      if (bone->boneflag & LocalAxis) {
        pmxRead(&bone->localAxisX, sizeof(bone->localAxisX), fd);
        pmxRead(&bone->localAxisY, sizeof(bone->localAxisY), fd);
      }

      if (bone->boneflag & DeformOuterParent) {
        pmxRead(&bone->externalParentKey, sizeof(bone->externalParentKey), fd);
      }

      if (bone->boneflag & IK) {
        pmxReadIndex(&bone->ikTargetBoneIndex, pmx->header.boneIndexSize, fd);
        pmxRead(&bone->ikIterationCount, sizeof(bone->ikIterationCount), fd);
        pmxRead(&bone->ikLimitAngle, sizeof(bone->ikLimitAngle), fd);

        pmxRead(&bone->ikLinkCount, sizeof(bone->ikLinkCount), fd);

        bone->ikLinks = (pmx_lk_link_t*)malloc(sizeof(pmx_lk_link_t) * bone->ikLinkCount);
        for (uint32_t i = 0; i < bone->ikLinkCount; i++) {
            pmx_lk_link_t* link = &bone->ikLinks[i];
            pmxReadIndex(&link->boneIndex, pmx->header.boneIndexSize, fd);
            pmxRead(&link->angleLimit, sizeof(link->angleLimit), fd);

            if (0 != link->angleLimit) {
                pmxRead(&link->minLimit, sizeof(link->minLimit), fd);
                pmxRead(&link->maxLimit, sizeof(link->maxLimit), fd);
            }
        }
      }
    }
}

static void pmxReadMorph(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->morph.count), sizeof(pmx->morph.count), fd);

    pmx->morph.data = (pmx_morph_data_t*)malloc(sizeof(pmx_morph_data_t) * pmx->morph.count);
    for (uint32_t index = 0; index < pmx->morph.count; index++) {
        pmx_morph_data_t* morph = &pmx->morph.data[index];

        pmxReadString(&morph->localMorphName, fd);
        pmxReadString(&morph->generalMorphName, fd);

        pmxRead(&morph->controlPanel, sizeof(morph->controlPanel), fd);
        pmxRead(&morph->morphType, sizeof(morph->morphType), fd);

        pmxRead(&morph->morphCount, sizeof(morph->morphCount), fd);
        if (Group == morph->morphType) {
            morph->groupMorphs = (pmx_group_morph_t*)malloc(sizeof(pmx_group_morph_t) * morph->morphCount);
            for (uint32_t i = 0; i < morph->morphCount; i++) {
                pmx_group_morph_t* groupMorph = &morph->groupMorphs[i];
                pmxReadIndex(&groupMorph->morphIndex, pmx->header.morphIndexSize, fd);
                pmxRead(&groupMorph->weight, sizeof(groupMorph->weight), fd);
            }
        } else if (Position == morph->morphType) {
            morph->positionMorphs = (pmx_vertex_morph_t*)malloc(sizeof(pmx_vertex_morph_t) * morph->morphCount);
            for (uint32_t i = 0; i < morph->morphCount; i++) {
                pmx_vertex_morph_t* vertexMorph = &morph->positionMorphs[i];
                pmxReadIndex(&vertexMorph->vertexIndex, pmx->header.vertexIndexSize, fd);
                pmxRead(&vertexMorph->position, sizeof(vertexMorph->position), fd);
            }
        } else if (Bone == morph->morphType) {
            morph->boneMorphs = (pmx_bone_morph_t*)malloc(sizeof(pmx_bone_morph_t) * morph->morphCount);
            for (uint32_t i = 0; i < morph->morphCount; i++) {
                pmx_bone_morph_t* boneMorph = &morph->boneMorphs[i];
                pmxReadIndex(&boneMorph->boneIndex, pmx->header.boneIndexSize, fd);
                pmxRead(&boneMorph->position, sizeof(boneMorph->position), fd);
                pmxRead(&boneMorph->quaternion, sizeof(boneMorph->quaternion), fd);
            }
        } else if (UV == morph->morphType || 
                   AddUV1 == morph->morphType || 
                   AddUV2 == morph->morphType || 
                   AddUV3 == morph->morphType || 
                   AddUV4 == morph->morphType) {
            morph->uvMorphs = (pmx_uv_morph_t*)malloc(sizeof(pmx_uv_morph_t) * morph->morphCount);
            for (uint32_t i = 0; i < morph->morphCount; i++) {
                pmx_uv_morph_t* uvMorph = &morph->uvMorphs[i];
                pmxReadIndex(&uvMorph->vertexIndex, pmx->header.vertexIndexSize, fd);
                pmxRead(&uvMorph->uv, sizeof(uvMorph->uv), fd);
            }
        } else if (Material == morph->morphType) {
            morph->materialMorphs = (pmx_material_morph_t*)malloc(sizeof(pmx_material_morph_t) * morph->morphCount);
            for (uint32_t i = 0; i < morph->morphCount; i++) {
                pmx_material_morph_t* materialMorph = &morph->materialMorphs[i];
                pmxReadIndex(&materialMorph->materialIndex, pmx->header.materialIndexSize, fd);
                pmxRead(&materialMorph->opType, sizeof(materialMorph->opType), fd);
                pmxRead(&materialMorph->diffuse, sizeof(materialMorph->diffuse), fd);
                pmxRead(&materialMorph->specular, sizeof(materialMorph->specular), fd);
                pmxRead(&materialMorph->specularPower, sizeof(materialMorph->specularPower), fd);
                pmxRead(&materialMorph->ambient, sizeof(materialMorph->ambient), fd);
                pmxRead(&materialMorph->edgeColor, sizeof(materialMorph->edgeColor), fd);
                pmxRead(&materialMorph->edgeSize, sizeof(materialMorph->edgeSize), fd);
                pmxRead(&materialMorph->textureFactor, sizeof(materialMorph->textureFactor), fd);
                pmxRead(&materialMorph->sphereTextureFactor, sizeof(materialMorph->sphereTextureFactor), fd);
                pmxRead(&materialMorph->toonTextureFactor, sizeof(materialMorph->toonTextureFactor), fd);
            }

        } else if (Flip == morph->morphType) {
            morph->flipMorphs = (pmx_flip_morph_t*)malloc(sizeof(pmx_flip_morph_t) * morph->morphCount);
            for (uint32_t i = 0; i < morph->morphCount; i++) {
                pmx_flip_morph_t* flipMorph = &morph->flipMorphs[i];
                pmxReadIndex(&flipMorph->morphIndex, pmx->header.morphIndexSize, fd);
                pmxRead(&flipMorph->m_weight, sizeof(flipMorph->m_weight), fd);
            }
        } else if (Impluse == morph->morphType) {
            morph->impulseMorphs = (pmx_impulse_morph_t*)malloc(sizeof(pmx_impulse_morph_t) * morph->morphCount);
            for (uint32_t i = 0; i < morph->morphCount; i++) {
                pmx_impulse_morph_t* impulseMorph = &morph->impulseMorphs[i];
                pmxReadIndex(&impulseMorph->rigidbodyIndex, pmx->header.rigidbodyIndexSize, fd);
                pmxRead(&impulseMorph->localFlag, sizeof(impulseMorph->localFlag), fd);
                pmxRead(&impulseMorph->translateVelocity, sizeof(impulseMorph->translateVelocity), fd);
                pmxRead(&impulseMorph->rotateTorque, sizeof(impulseMorph->rotateTorque), fd);
            }
        }
    }
}

static void pmxReadDisplayFrame(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->displayFrame.count), sizeof(pmx->displayFrame.count), fd);

    pmx->displayFrame.data = (pmx_displayFrame_data_t*)malloc(sizeof(pmx_displayFrame_data_t) * pmx->displayFrame.count);
    for (uint32_t index = 0; index < pmx->displayFrame.count; index++) {
        pmx_displayFrame_data_t* frame = &pmx->displayFrame.data[index];

        pmxReadString(&frame->localFrameName, fd);
        pmxReadString(&frame->generalFrameName, fd);

        pmxRead(&frame->frameType, sizeof(frame->frameType), fd);

        pmxRead(&frame->frameCount, sizeof(frame->frameCount), fd);
        frame->targets = (pmx_frame_target_t*)malloc(sizeof(pmx_frame_target_t) * frame->frameCount);
        for (uint32_t i = 0; i < frame->frameCount; i++) {
            pmx_frame_target_t* target = &frame->targets[i];
            pmxRead(&target->targetType, sizeof(target->targetType), fd);
            if (BoneIndex == target->targetType) {
                pmxReadIndex(&target->targetIndex, pmx->header.boneIndexSize, fd);
            } else if (MorphIndex == target->targetType) {
                pmxReadIndex(&target->targetIndex, pmx->header.morphIndexSize, fd);
            }
        }
    }
}

static void pmxReadRigidbody(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->rigidBody.count), sizeof(pmx->rigidBody.count), fd);

    pmx->rigidBody.data = (pmx_rigidBody_data_t*)malloc(sizeof(pmx_rigidBody_data_t) * pmx->rigidBody.count);
    for (uint32_t index = 0; index < pmx->rigidBody.count; index++) {
        pmx_rigidBody_data_t* rigidBody = &pmx->rigidBody.data[index];

        pmxReadString(&rigidBody->localName, fd);
        pmxReadString(&rigidBody->generalName, fd);

        pmxReadIndex(&rigidBody->boneIndex, pmx->header.boneIndexSize, fd);
        pmxRead(&rigidBody->groupID, sizeof(rigidBody->groupID), fd);
        pmxRead(&rigidBody->collisionGroup, sizeof(rigidBody->collisionGroup), fd);

        pmxRead(&rigidBody->shape, sizeof(rigidBody->shape), fd);
        pmxRead(&rigidBody->shapeSize, sizeof(rigidBody->shapeSize), fd);
        
        pmxRead(&rigidBody->translate, sizeof(rigidBody->translate), fd);
        pmxRead(&rigidBody->rotate, sizeof(rigidBody->rotate), fd);

        pmxRead(&rigidBody->mass, sizeof(rigidBody->mass), fd);
        pmxRead(&rigidBody->translateDimmer, sizeof(rigidBody->translateDimmer), fd);
        pmxRead(&rigidBody->rotateDimmer, sizeof(rigidBody->rotateDimmer), fd);
        pmxRead(&rigidBody->repulsion, sizeof(rigidBody->repulsion), fd);
        pmxRead(&rigidBody->friction, sizeof(rigidBody->friction), fd);
        
        pmxRead(&rigidBody->operation, sizeof(rigidBody->operation), fd);
    }
}

static void pmxReadJoint(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->joint.count), sizeof(pmx->joint.count), fd);

    pmx->joint.data = (pmx_joint_data_t*)malloc(sizeof(pmx_joint_data_t) * pmx->joint.count);
    for (uint32_t index = 0; index < pmx->joint.count; index++) {
        pmx_joint_data_t* joint = &pmx->joint.data[index];

        pmxReadString(&joint->localJointName, fd);
        pmxReadString(&joint->generalJointName, fd);

        pmxRead(&joint->type, sizeof(joint->type), fd);
        pmxReadIndex(&joint->rigidBodyAIndex, pmx->header.rigidbodyIndexSize, fd);
        pmxReadIndex(&joint->rigidBodyBIndex, pmx->header.rigidbodyIndexSize, fd);

        pmxRead(&joint->translate, sizeof(joint->translate), fd);
        pmxRead(&joint->rotate, sizeof(joint->rotate), fd);

        pmxRead(&joint->translateLowerLimit, sizeof(joint->translateLowerLimit), fd);
        pmxRead(&joint->translateUpperLimit, sizeof(joint->translateUpperLimit), fd);
        pmxRead(&joint->rotateLowerLimit, sizeof(joint->rotateLowerLimit), fd);
        pmxRead(&joint->rotateUpperLimit, sizeof(joint->rotateUpperLimit), fd);

        pmxRead(&joint->springTranslateFactor, sizeof(joint->springTranslateFactor), fd);
        pmxRead(&joint->springRotateFactor, sizeof(joint->springRotateFactor), fd);
    }
}

static void pmxReadSoftBody(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->softbody.count), sizeof(pmx->softbody.count), fd);

    pmx->softbody.data = (pmx_softbody_data_t*)malloc(sizeof(pmx_softbody_data_t) * pmx->softbody.count);
    for (uint32_t index = 0; index < pmx->softbody.count; index++) {
        pmx_softbody_data_t* softbody = &pmx->softbody.data[index];

        pmxReadString(&softbody->localSoftbodyName, fd);
        pmxReadString(&softbody->generalSoftbodyName, fd);

        pmxRead(&softbody->type, sizeof(softbody->type), fd);
        pmxReadIndex(&softbody->materialIndex, pmx->header.materialIndexSize, fd);

        pmxRead(&softbody->group, sizeof(softbody->group), fd);
        pmxRead(&softbody->collisionGroup, sizeof(softbody->collisionGroup), fd);

        pmxRead(&softbody->flag, sizeof(softbody->flag), fd);

        pmxRead(&softbody->bLinkLength, sizeof(softbody->bLinkLength), fd);
        pmxRead(&softbody->numClusters, sizeof(softbody->numClusters), fd);

        pmxRead(&softbody->totalMass, sizeof(softbody->totalMass), fd);
        pmxRead(&softbody->collisionMargin, sizeof(softbody->collisionMargin), fd);

        pmxRead(&softbody->AeroModel, sizeof(softbody->AeroModel), fd);

        pmxRead(&softbody->VCF, sizeof(softbody->VCF), fd);
        pmxRead(&softbody->DP, sizeof(softbody->DP), fd);
        pmxRead(&softbody->DG, sizeof(softbody->DG), fd);
        pmxRead(&softbody->LF, sizeof(softbody->LF), fd);
        pmxRead(&softbody->PR, sizeof(softbody->PR), fd);
        pmxRead(&softbody->VC, sizeof(softbody->VC), fd);
        pmxRead(&softbody->DF, sizeof(softbody->DF), fd);
        pmxRead(&softbody->MT, sizeof(softbody->MT), fd);
        pmxRead(&softbody->CHR, sizeof(softbody->CHR), fd);
        pmxRead(&softbody->KHR, sizeof(softbody->KHR), fd);
        pmxRead(&softbody->SHR, sizeof(softbody->SHR), fd);
        pmxRead(&softbody->AHR, sizeof(softbody->AHR), fd);

        pmxRead(&softbody->SRHR_CL, sizeof(softbody->SRHR_CL), fd);
        pmxRead(&softbody->SKHR_CL, sizeof(softbody->SKHR_CL), fd);
        pmxRead(&softbody->SSHR_CL, sizeof(softbody->SSHR_CL), fd);
        pmxRead(&softbody->SR_SPLT_CL, sizeof(softbody->SR_SPLT_CL), fd);
        pmxRead(&softbody->SK_SPLT_CL, sizeof(softbody->SK_SPLT_CL), fd);
        pmxRead(&softbody->SS_SPLT_CL, sizeof(softbody->SS_SPLT_CL), fd);

        pmxRead(&softbody->V_IT, sizeof(softbody->V_IT), fd);
        pmxRead(&softbody->P_IT, sizeof(softbody->P_IT), fd);
        pmxRead(&softbody->D_IT, sizeof(softbody->D_IT), fd);
        pmxRead(&softbody->C_IT, sizeof(softbody->C_IT), fd);

        pmxRead(&softbody->LST, sizeof(softbody->LST), fd);
        pmxRead(&softbody->AST, sizeof(softbody->AST), fd);
        pmxRead(&softbody->VST, sizeof(softbody->VST), fd);

        pmxRead(&softbody->AnchorRigidbodyCount, sizeof(softbody->AnchorRigidbodyCount), fd);
        softbody->anchorRigidbodies = (pmx_anchorRigidbody_t*)malloc(sizeof(pmx_anchorRigidbody_t) * softbody->AnchorRigidbodyCount);
        for (uint32_t i = 0; i < softbody->AnchorRigidbodyCount; i++) {
            pmx_anchorRigidbody_t* anchorRigidbody = &softbody->anchorRigidbodies[i];
            pmxReadIndex(&anchorRigidbody->rigidBodyIndex, pmx->header.rigidbodyIndexSize, fd);
            pmxRead(&anchorRigidbody->vertexIndex, sizeof(anchorRigidbody->vertexIndex), fd);
            pmxRead(&anchorRigidbody->nearMode, sizeof(anchorRigidbody->nearMode), fd);
        }

        pmxRead(&softbody->pinVertexCount, sizeof(softbody->pinVertexCount), fd);
        softbody->pinVertexIndices = (int32_t*)malloc(sizeof(int32_t) * softbody->pinVertexCount);
        for (uint32_t i = 0; i < softbody->pinVertexCount; i++) {
            pmxReadIndex(&softbody->pinVertexIndices[i], pmx->header.vertexIndexSize, fd);
        }
    }
}

pmx_t* pmxReadFile(char *filename) {
    setlocale(LC_ALL, "");// suggested print to windows terminal

    FILE *fd = fopen(filename,"rb"); 
    if(fd == NULL) {
        perror("open failed!");
        exit(1);        //error, exit the program
    }

    pmx_t* pmx = (pmx_t*)malloc(sizeof(pmx_t));
    pmxReadHeader(pmx, fd);
    pmxReadVertex(pmx, fd);
    pmxReadFace(pmx, fd);
    pmxReadTexture(pmx, fd);
    pmxReadMaterial(pmx, fd);
    pmxReadBone(pmx, fd);
    pmxReadMorph(pmx, fd);
    pmxReadDisplayFrame(pmx, fd);
    pmxReadRigidbody(pmx, fd);
    pmxReadJoint(pmx, fd);
    
    if (0 !=feof(fd)) {
        pmxReadSoftBody(pmx, fd);
    }
    fclose(fd);

	return pmx;
}