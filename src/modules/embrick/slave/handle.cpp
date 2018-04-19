/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "handle.h"
#include "slave.h"
#include <handler/bus.h>
#include <io/mapper/io_mapper.h>
#include <devlog.h>

EmbrickSlaveHandle::EmbrickSlaveHandle(IODeviceController *controller,
    IOMapper::Direction direction, CIEC_ANY::EDataTypeID type, uint8_t offset,
    EmbrickSlaveHandler *slave) :
    IOHandle(controller, direction, type), offset(offset), slave(slave), updateMutex(
        &slave->updateMutex) {
  if (direction == IOMapper::In)
    buffer = slave->updateReceiveImage;
  else if (direction == IOMapper::Out)
    buffer = slave->updateSendImage;
}

EmbrickSlaveHandle::~EmbrickSlaveHandle() {
}

void EmbrickSlaveHandle::set(const CIEC_ANY &) {
  slave->forceUpdate();
}

void EmbrickSlaveHandle::onObserver(IOObserver *observer) {
  reset();

  IOHandle::onObserver(observer);
}

void EmbrickSlaveHandle::dropObserver() {
  IOHandle::dropObserver();

  reset();
}

