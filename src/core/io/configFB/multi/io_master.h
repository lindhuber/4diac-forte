/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_CORE_IO_CONFIGFB_MULTI_MASTER_H_
#define SRC_CORE_IO_CONFIGFB_MULTI_MASTER_H_

#include "../../device/io_controller_multi.h"
#include "io_adapter.h"
#include <fortelist.h>
#include "../io_configFB_controller.h"

class IOConfigFBMultiMaster;

typedef CSinglyLinkedList<IOConfigFBMultiMaster*> TMasterList;

class IOConfigFBMultiMaster: public IOConfigFBController {
public:
  IOConfigFBMultiMaster(CResource *pa_poSrcRes, const SFBInterfaceSpec *pa_pstInterfaceSpec,
      const CStringDictionary::TStringId pa_nInstanceNameId,
      TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData);

  static IOConfigFBMultiMaster* getMasterById(TForteUInt16 id);

  using IOConfigFBController::initHandle;

protected:
  IOConfigFBMultiAdapter& BusAdapterOut() {
    return (*static_cast<IOConfigFBMultiAdapter*>(m_apoAdapters[0]));
  }
  static const int scm_nBusAdapterAdpNum = 0;

  virtual void onStartup();

  virtual void onStop();

  virtual void executeEvent(int pa_nEIID);

private:
  static TMasterList *instances;

  static TForteUInt16 instancesIncrement;

  TForteUInt16 id;

  static const char * const scmFailedToInitSlaves;
};

#endif /* SRC_CORE_IO_CONFIGFB_MULTI_MASTER_H_ */
