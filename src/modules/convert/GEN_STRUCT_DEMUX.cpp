/*******************************************************************************
 * Copyright (c) 2020 Johannes Kepler University
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "GEN_STRUCT_DEMUX.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GEN_STRUCT_DEMUX_gen.cpp"
#endif
#include "GEN_STRUCT_MUX.h"
#include <stdio.h>
#include "GEN_STRUCT_DEMUX.h"


DEFINE_GENERIC_FIRMWARE_FB(GEN_STRUCT_DEMUX, g_nStringIdGEN_STRUCT_DEMUX);

const CStringDictionary::TStringId GEN_STRUCT_DEMUX::scm_anEventInputNames[] = { g_nStringIdREQ };
const CStringDictionary::TStringId GEN_STRUCT_DEMUX::scm_anEventOutputNames[] = { g_nStringIdCNF };

const CStringDictionary::TStringId GEN_STRUCT_DEMUX::scm_anDataInputNames[] = { g_nStringIdIN };

const TForteInt16 GEN_STRUCT_DEMUX::scm_anEIWithIndexes[] = {0};
const TDataIOID GEN_STRUCT_DEMUX::scm_anEIWith[] = {0, 255};
const TForteInt16 GEN_STRUCT_DEMUX::scm_anEOWithIndexes[] = {0};


void GEN_STRUCT_DEMUX::executeEvent(int paEIID) {
  if(scm_nEventREQID == paEIID) {
    for (size_t i = 0; i < st_IN().getStructSize(); i++){
      getDO(static_cast<unsigned int>(i))->setValue(*st_IN().getMember(i));
    }
    sendOutputEvent(scm_nEventCNFID);
  }
}

GEN_STRUCT_DEMUX::GEN_STRUCT_DEMUX(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CGenFunctionBlock<CFunctionBlock>(paSrcRes, paInstanceNameId){
}

GEN_STRUCT_DEMUX::~GEN_STRUCT_DEMUX(){
  if(nullptr != m_pstInterfaceSpec){
    delete[](m_pstInterfaceSpec->m_anEOWith);
    delete[](m_pstInterfaceSpec->m_aunDIDataTypeNames);
    delete[](m_pstInterfaceSpec->m_aunDONames);
    delete[](m_pstInterfaceSpec->m_aunDODataTypeNames);
  }
}

bool GEN_STRUCT_DEMUX::createInterfaceSpec(const char *paConfigString, SFBInterfaceSpec &paInterfaceSpec) {
  bool retval = false;
  CStringDictionary::TStringId structTypeNameId = GEN_STRUCT_MUX::getStructNameId(paConfigString);

  CIEC_ANY *data = CTypeLib::createDataTypeInstance(structTypeNameId, nullptr);

  if(nullptr != data) {
    if(data->getDataTypeID() == CIEC_ANY::e_STRUCT) {
      // we could find the struct
      CIEC_STRUCT *structInstance = static_cast<CIEC_STRUCT*>(data);

      size_t structSize = structInstance->getStructSize();
      if(structSize < 1 || structSize > 254) { //the structure size must be non zero and less than 255 (maximum number of data outputs)
        DEVLOG_ERROR("[GEN_STRUCT_DEMUX]: The structure %s has a size is not within range > 0 and < 255\n",
          CStringDictionary::getInstance().get(structTypeNameId));
      } else {
        TDataIOID *eoWith = new TDataIOID[structSize + 1];
        CStringDictionary::TStringId *doDataTypeNames = new CStringDictionary::TStringId[GEN_STRUCT_MUX::calcStructTypeNameSize(*structInstance)];
        CStringDictionary::TStringId *doNames = new CStringDictionary::TStringId[structSize];
        CStringDictionary::TStringId *diDataTypeNames = new CStringDictionary::TStringId[1];

        paInterfaceSpec.m_nNumEIs = 1;
        paInterfaceSpec.m_aunEINames = scm_anEventInputNames;
        paInterfaceSpec.m_anEIWith = scm_anEIWith;
        paInterfaceSpec.m_anEIWithIndexes = scm_anEIWithIndexes;
        paInterfaceSpec.m_nNumEOs = 1;
        paInterfaceSpec.m_aunEONames = scm_anEventOutputNames;
        paInterfaceSpec.m_anEOWith = eoWith;
        paInterfaceSpec.m_anEOWithIndexes = scm_anEOWithIndexes;
        paInterfaceSpec.m_nNumDIs = 1;
        paInterfaceSpec.m_aunDINames = scm_anDataInputNames;
        paInterfaceSpec.m_aunDIDataTypeNames = diDataTypeNames;
        paInterfaceSpec.m_nNumDOs = static_cast<TForteUInt8>(structSize);
        paInterfaceSpec.m_aunDONames = doNames;
        paInterfaceSpec.m_aunDODataTypeNames = doDataTypeNames;
        diDataTypeNames[0] = structTypeNameId;

        for(size_t i = 0, typeNameIndex = 0; i < paInterfaceSpec.m_nNumDOs; i++, typeNameIndex++) {
          CIEC_ANY &member = *structInstance->getMember(i);
          eoWith[i] = static_cast<TForteUInt8>(i);
          doNames[i] = structInstance->elementNames()[i];
          doDataTypeNames[typeNameIndex] = member.getTypeNameID();
          if(member.getDataTypeID() == CIEC_ANY::e_ARRAY){
            CIEC_ARRAY_TYPELIB &array = static_cast<CIEC_ARRAY_TYPELIB&>(member);
            doDataTypeNames[typeNameIndex + 1] = static_cast<CStringDictionary::TStringId>(array.size());
            doDataTypeNames[typeNameIndex + 2] = array.getElementTypeNameID();
            typeNameIndex += 2;
          }
        }
        eoWith[paInterfaceSpec.m_nNumDOs] = scmWithListDelimiter;
        retval = true;
      }
    } else {
      DEVLOG_ERROR("[GEN_STRUCT_DEMUX]: data type is not a structure: %s\n", CStringDictionary::getInstance().get(structTypeNameId));
    }
    delete data;
  } else {
    DEVLOG_ERROR("[GEN_STRUCT_DEMUX]: Couldn't create structure of type: %s\n", CStringDictionary::getInstance().get(structTypeNameId));
  }
  return retval;
}




